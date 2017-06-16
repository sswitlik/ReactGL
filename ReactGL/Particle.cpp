#include "stdafx.h"
#include "Particle.h"

extern Game *game;

Particle::Particle(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, float radiusSize, rp3d::Vector3 color)
{
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(initPosition, initOrientation);
	body = world->createRigidBody(transform);

	rp3d::Vector3 shapeData(radiusSize, radiusSize, radiusSize);
	shape = new rp3d::SphereShape(radiusSize);

	rp3d::Transform transform2 = rp3d::Transform::identity();
	proxyShape = body->addCollisionShape(shape, transform2, 100);

	//pointer to this - using in collision detection
	proxyShape->setUserData(this);

	//COLLISION FILTERING
	//setCollisionCategory(EFFECTcat);

	//DRAWING
	model = NULL;
	modelll.setAllValues(shapeData.x * 2, shapeData.y * 2, shapeData.z * 2);

	//SAVE DELETING
	IsDeleted = false;
	gameWorld = world;		//in Destructor needed

	//LEVEL INITIALIZATION
	body->setType(rp3d::STATIC);

//-------------------------------------------
	this->color = color;

	maxTime = 100;		//maxTime*timeStep = timeInSeconds	//timeStep = 0.017 (13.06.2017)
	time = 0;
	timeInSec = 0;

	//TWORZENIE ODLAMKOW
	//queueNumber = 0;
		//makeShards(5, initPosition);

}


Particle::Particle(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, float radiusSize, rp3d::Vector3 color, int queueNumb)
{
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(initPosition, initOrientation);
	body = world->createRigidBody(transform);

	rp3d::Vector3 shapeData(radiusSize, radiusSize, radiusSize);
	shape = new rp3d::SphereShape(radiusSize);

	rp3d::Transform transform2 = rp3d::Transform::identity();
	proxyShape = body->addCollisionShape(shape, transform2, 100);

	//pointer to this - using in collision detection
	proxyShape->setUserData(this);

	//COLLISION FILTERING
	setCollisionCategory(EFFECTcat);

	//DRAWING
	model = NULL;
	modelll.setAllValues(shapeData.x * 2, shapeData.y * 2, shapeData.z * 2);

	//SAVE DELETING
	IsDeleted = false;
	gameWorld = world;		//in Destructor needed

//-------------------------------------------
	this->color = color;

	maxTime = 100;		//maxTime*timeStep = timeInSeconds	//timeStep = 0.017 (13.06.2017)
	time = 0;
	timeInSec = 0;

	//TWORZENIE ODLAMKOW
	queueNumber = queueNumb;
	//if (queueNumb < 2)
	//	makeShards(3, initPosition);
}

Particle::~Particle()
{
}

void Particle::update()
{
	time += 1;
	timeInSec += timeStep;

	if (time > maxTime)
		IsDeleted = true;
}

void Particle::Draw()
{
	rp3d::Transform transform = body->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);

	glPushMatrix();
		glMultMatrixf(matrix);
		glScalef(modelll.x, modelll.y, modelll.z);
		//glColor3f(1,1,1);
		glColor3ub (color.x,color.y,color.z);
		glutSolidCube(1);
	glPopMatrix();

}

void Particle::makeShards(int hmany, rp3d::Vector3 initPos)
{
	float radius = 0.01;
	std::vector<int> nonRepeat;
	for (int i = 0; i < hmany; i++)
	{
		float x = ((rand() % 20)-10)/100;
		float y = ((rand() % 20)-10)/100;
		float z = ((rand() % 20)-10)/100;

		rp3d::Vector3 pos(initPos.x+x, initPos.y+y, initPos.z+z);
		Particle *partic = new Particle(gameWorld, pos, modelll.x / 2, color, queueNumber);
		rp3d::Vector3 direction(x, y, z);
		partic->giveForce(direction);
		partic->setMaxTime(200);
		//partic->setQueueNumber(queueNumber + 1);

		game->effects.push_back(partic);
	}
}

void Particle::init(rp3d::Vector3 position, rp3d::Quaternion orientation)
{
	BodyObj::init(position, orientation);
	time = 0;
	maxTime = 100;
}

void Particle::giveForce(rp3d::Vector3 force)
{
	body->applyForceToCenterOfMass(force);
}

void Particle::setCollisionCategory(Category cat)
{
	proxyShape->setCollisionCategoryBits(cat);
	proxyShape->setCollideWithMaskBits(0);
}

void Particle::setMaxTime(int max)
{
	maxTime = max;
}

void Particle::setQueueNumber(int numb)
{
	queueNumber = numb;
}

void Particle::getColor(rp3d::Vector3 color)
{
	this->color = color;
}