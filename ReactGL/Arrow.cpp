#include "stdafx.h"
#include "Arrow.h"
#include "GL/freeglut.h"
#include "maths.h"

extern Game *game;

Arrow::Arrow()
{

}

Arrow::Arrow(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, float radius, float length, rp3d::decimal mass)
{
	rp3d::Transform transform(initPosition, initOrientation);
	body = world->createRigidBody(transform);

	shape = new rp3d::CapsuleShape(radius, length);

	rp3d::Transform transform2 = rp3d::Transform::identity();
	proxyShape = body->addCollisionShape(shape, transform2, mass);

	proxyShape->setUserData(this);
	
	//COLLISION FILTERING
	BodyObj::setCollisionCategory(ARROWcat);
	//proxyShape->setCollisionCategoryBits(ARROWcat);
	//proxyShape->setCollideWithMaskBits(MAPcat);

	//COLOR
	color.setAllValues(1, 0.5, 0);
	damage = 1;
	//DRAWING
	model = NULL;

	//LEVEL INITIALIZATION
	body->setType(rp3d::STATIC);

	time = 0;
	collided = false;
	drilled = false;
	antivibr = false;

	//SAVE DELETING
	IsDeleted = false;

	gameWorld = world;

	time = 0;
}


Arrow::~Arrow()
{
}

void Arrow::Draw()
{
	rp3d::Transform transform = body->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);

	if (model)
	{
	glPushMatrix();
		glMultMatrixf(matrix);
		//glScalef(modelll.x, modelll.y, modelll.z);
		glColor3f(0, 0.5, 0.5);
		model->Render();
	glPopMatrix();
	}
	else
	{
		glPushMatrix();
		glMultMatrixf(matrix);
		glColor3f(color.x, color.y, color.z);
		glScalef(0.02, 1, 0.02);
		glutSolidCube(1);
		glColor3f(0.5, 0.5, 0.5);
		glutWireCube(1);
	glPopMatrix();
	}
}

//void Arrow::update()
//{
//	if (time < 25 && !collided)
//	{
//		time++;		//allow collision after 25
//		return;
//	}
//	
//		rp3d::Vector3 stop(0, 0, 0);
//		body->setAngularVelocity(stop);
//		body->setLinearVelocity(stop);
//		body->enableGravity(false);
//
//}

void Arrow::update()
{
	time++;
	////bylo potrzebne przed collision filtering
	//if (time < 3 && !collided) 
	//{
	//	time++;		//allow collision after 25
	//	return;
	//}

	if (drilled)	
	{
		rp3d::Vector3 stop(0, 0, 0);
		body->setAngularVelocity(stop);
		body->setLinearVelocity(stop);
		
	}
	else
	if (collided)
	{
		int i=0;
	}
	else
	{
		rp3d::Transform t = body->getTransform();
		rp3d::Vector3 speed = body->getLinearVelocity();
		rp3d::Quaternion q = t.getOrientation();
	
		//printf("%f %f %f\n", speed.x, speed.y, speed.z);

		double x;
		double y;
		double z;

		float pi2 = 180/PI;
		QuaternionO2IToEulerAngles(&x, &y, &z, q);

		if (antivibr)	//if falling
		{
			x += PI;
			z += PI;
		}

		//printf("%f %f %f\n", x, y, z);

		//y from speed
		float XZLength = sqrt(1 - speed.y*speed.y);

		speed.normalize();

		y = acos(speed.y);

		//rp3d::Vector3 angsp = body->getAngularVelocity();	//angle speed

		rp3d::Quaternion newq(x, y, z);

		newq.setAllValues(newq.y, newq.x, newq.z, newq.w);

		t.setOrientation(newq);
		body->setTransform(t);		//obrot wg predkowsci

		if (speed.y < 0)
			antivibr = true;

		auto pos = t.getPosition();
		
		if (time % 4 == 0 && time >10)
		{
			int x = rand() % 90,
				y = rand() % 90,
				z = rand() % 90;

			rp3d::Quaternion orient(x, y, z);
			rp3d::Vector3 col(180, 180, 180);
			auto partic = game->getLevel()->Make("effect", pos, orient);
			partic->setCollisionCategory(EFFECTcat);
			partic->setType(rp3d::STATIC);
			partic->setMaxTime(30);
			partic->getColor(col);
		}

		//camera follow arrow
		//game->getplayer()->cam.sync(pos, 0, 1, 0);
	}
}

void Arrow::makeCollision(BodyObj *CollideWith)
{
	int collideCategory = CollideWith->getProxyShape()->getCollisionCategoryBits();


	if (getOneParticles())
	{
		rp3d::Vector3 pos = body->getTransform().getPosition();
		rp3d::Vector3 vel = body->getLinearVelocity();
		vel.normalize();
		pos += vel / 4;
		rp3d::Vector3 col[5];
		col[0].setAllValues(180, 0, 0);
		col[1].setAllValues(180, 0, 0);
		col[2].setAllValues(110, 61, 29);
		col[3].setAllValues(110, 61, 29);
		col[4].setAllValues(78, 86, 99);
		for (int i = 0; i < 5; i++)
		{
			int x = rand() % 30000 - 15000,
				y = rand() % 10000 + 30000,
				z = rand() % 30000 - 15000;
			rp3d::Vector3 force(x, y, z);
			//Particle *partic = new Particle(gameWorld, pos, 0.02, col);
			auto partic = game->getLevel()->Make("effect", pos, rp3d::Quaternion::identity());
			
			partic->setMaterial(0.5, 0.1);
			auto tmpBody = partic->getBody();
			tmpBody->applyForceToCenterOfMass(force);
			partic->setCollisionCategory(EFFECTcat);
			partic->getColor(col[i]);
			
			x = rand() % 20 - 10;
			y = rand() % 20 - 10;
			z = rand() % 20 - 10;
			force.setAllValues(x, y, z);
			tmpBody->applyTorque(force);
			setOneParticles(false);
		}
	}

	if (collideCategory & MAPcat)
	{
		if (!collided)		//je¿eli jeszcze nie kolidowal czyli leci prosto
		{
			drilled = true;
			body->enableGravity(false);
		}
	}
	else
	if (collideCategory & RUBBISHcat)
	{
		BodyObj::makeCollision(CollideWith);
	}
	else
		collided = true;
}

void Arrow::init(rp3d::Vector3 position, rp3d::Quaternion orientation)
{
	BodyObj::init(position, orientation);
	body->enableGravity(true);
	collided = false;
	drilled = false;
	antivibr = false;
	setCollisionCategory(ARROWcat);
	time = 0;
}

void Arrow::setGravityEnable(bool arg)
{
	BodyObj::setGravityEnable(arg);
	if (arg)
	{
		drilled = false;
		collided = true;
	}
	else
		drilled = true;
}

void Arrow::pushToGravity()
{
	rp3d::Vector3 push(0, -0.01, 0);
	body->applyForceToCenterOfMass(push);
}

float Arrow::getDamage()
{
	return damage;
}