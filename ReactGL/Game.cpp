#include "stdafx.h"
#include "Game.h"
#include <GL/freeglut.h>
#include "Arrow.h"
#include "maths.h"

//#define PI_2 1.57079632679

Game::Game()
{
	quantity = 0;
	//TIME
	previousFrameTime = 0;
	accumulator = 0;

	//START WORLD
	ExampleTestInit();

	//PLAYER
	rp3d::Vector3 initPosition(1.0, 2.0, 2.0);
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Vector3 shapeData(0.125, 0.5, 0.125);

	player = new Player(World, initPosition, initOrientation, shapeData);

	//CONTACT 
	listener = new GameEventListener(World,&effects);
	World->setEventListener(listener);
}


Game::~Game()
{
	delete World;
	//World->destroyRigidBody(floor);
}


void Game::ExampleTestInit()
{
	//WORLD
	rp3d::Vector3 gravity(0.0, -9.81, 0.0);
	World = new rp3d::DynamicsWorld(gravity);

	World->setNbIterationsVelocitySolver(15);
	World->setNbIterationsPositionSolver(8);


	//TEMPORARY DATA FOR INITIALIZATION
	rp3d::Vector3 initPosition;
	rp3d::Quaternion initOrientation;
	rp3d::Vector3 shapeData;

	//FLOOR
	initPosition.setAllValues(0.0, 1, 0.0);
	initOrientation = rp3d::Quaternion::identity();
	shapeData.setAllValues(10, 0.5, 10);
	floor = new BodyObj(this->World, initPosition, initOrientation, shapeData, 1);
	floor->setType(1);
	floor->setMaterial(0, 1);

	//OBJ
	initPosition.setAllValues(0.0, 2.0, 0.0);
	initOrientation = rp3d::Quaternion::identity();
	shapeData.setAllValues(0.5, 0.5, 0.5);
	obj1 = new BodyObj(this->World, initPosition, initOrientation, shapeData, 2);
	obj1->setMaterial(0.3, 0.8);
	obj1->setType(1);
	//obj1->testset();
}

void Game::Draw_floor(float m[16])
{
	floor->Draw(m);
}

void Game::Draw_1(float m[16])
{
	obj1->Draw(m);
}

void Game::Update()
{
	player->update();
	for (auto *i : objs)
	{
		i->update();
	}

	///TIME UPDATE
	if (previousFrameTime == 0)
		previousFrameTime = glutGet(GLUT_ELAPSED_TIME) - 1;
	// Constant physics time step 
	const float timeStep = 0.016;
	const int timeStep2 = 16;
	// Get the current system time 
	unsigned int currentFrameTime = glutGet(GLUT_ELAPSED_TIME);

	// Compute the time difference between the two frames 
	unsigned int deltaTime = currentFrameTime - previousFrameTime;

	// Update the previous time 
	previousFrameTime = currentFrameTime;

	// Add the time difference in the accumulator 
	accumulator += deltaTime;

	// While there is enough accumulated time to take 
	// one or several physics steps 
	int t1 = glutGet(GLUT_ELAPSED_TIME);
	while (accumulator >= timeStep2) {

		// Update the Dynamics world with a constant time step 
		World->update(timeStep);

		// Decrease the accumulated time 
		accumulator -= timeStep2;
	}
	int t2 = glutGet(GLUT_ELAPSED_TIME);
	int j = 0;

	/*for (int i=0;i<1;i++)
	World->update(0.016);*/
}

void Game::plus()
{
	quantity++;

	rp3d::Vector3 initPosition;
	rp3d::Quaternion initOrientation;
	rp3d::Vector3 shapeData;

	initPosition.setAllValues(0.0, 15.0, 0.0);
	initOrientation = rp3d::Quaternion::identity();
	shapeData.setAllValues(0.5, 0.5, 0.5);
	BodyObj *obj = new BodyObj(this->World, initPosition, initOrientation, shapeData, 2);
	obj->setMaterial(0.8, 0.2);

	objs.push_back(obj);
}

void Game::rd()
{
	obj1->rd();
}

Player * Game::getplayer()
{
	return player;
}

void Game::testshoot()
{
	quantity++;
	Arrow *b = player->test_shoot();
	objs.push_back(b);
}

//void QuaternionO2IToEulerAngles(float *Yaw, float *Pitch, float *Roll, const rp3d::Quaternion &q)
//{
//	float sp = -2.0f * (q.y*q.z - q.w*q.x);
//
//	if (sp == 1.0f)
//	{
//		*Pitch = PI_2 * sp;
//		*Yaw = atan2f(-q.x*q.z + q.w*q.y, 0.5f - q.y*q.y - q.z*q.z);
//		*Roll = 0.0f;
//	}
//	else
//	{
//		*Pitch = asinf(sp);
//		*Yaw = atan2f(q.x*q.z + q.w*q.y, 0.5f - q.x*q.x - q.y*q.y);
//		*Roll = atan2f(q.x*q.y + q.w*q.z, 0.5f - q.x*q.x - q.z*q.z);
//	}
//}

void Game::testarrowrotate()
{
	if (!objs.size())
		return;

	//rp3d::Vector3 pow(0, 3000, 0);
	//objs[0]->body->applyForceToCenterOfMass(pow);
	for (auto *i : objs)
	{
		i->body->enableGravity(false);
		i->body->setLinearDamping(0.9);

		rp3d::Transform t = i->body->getTransform();
		rp3d::Quaternion q = t.getOrientation();
		rp3d::Matrix3x3 m = q.getMatrix();

		float x;
		float y;
		float z;

		QuaternionO2IToEulerAngles(&x, &y, &z, q);
		rp3d::Quaternion *newq;
		if (y > 0)
			newq = new rp3d::Quaternion(x, y - 0.25, z);
		else
			newq = new rp3d::Quaternion(x, y + 0.25, z);

		newq->setAllValues(newq->y, newq->x, newq->z, newq->w);
		t.setOrientation(*newq);
		i->body->setTransform(t);

		delete newq;
	}
}