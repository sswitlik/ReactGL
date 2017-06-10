#include "stdafx.h"
#include "Game.h"
#include <GL/freeglut.h>
#include "Arrow.h"
#include "maths.h"

//#define PI_2 1.57079632679

Game::Game()
{

	srand(time(NULL));

	//quantity = 0;
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
	delete floor;
	for (auto *obj : objs)
	{
		delete obj;
	}
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

	///TIME UPDATE
	if (previousFrameTime == 0)
		previousFrameTime = glutGet(GLUT_ELAPSED_TIME) - 1;

	
	const float timeStep = 0.016;				// Constant physics time step 
	const int timeStep2 = 16;
	
	unsigned int currentFrameTime = glutGet(GLUT_ELAPSED_TIME);			// Get the current system time 
	
	unsigned int deltaTime = currentFrameTime - previousFrameTime;		// Compute the time difference between the two frames 
	
	previousFrameTime = currentFrameTime;		// Update the previous time 
	
	accumulator += deltaTime;					// Add the time difference in the accumulator 

	int t1 = glutGet(GLUT_ELAPSED_TIME);		// While there is enough accumulated time to take one or several physics steps 
	while (accumulator >= timeStep2) {

		World->update(timeStep);				// Update the Dynamics world with a constant time step 

		player->update();

		for (auto *i : objs)
		{
			i->update();
		}
		
		accumulator -= timeStep2;				// Decrease the accumulated time 
	}

	int t2 = glutGet(GLUT_ELAPSED_TIME);
}

void Game::plus()
{
	rp3d::Vector3 initPosition;
	rp3d::Quaternion initOrientation;
	rp3d::Vector3 shapeData;

	initPosition.setAllValues(0.0, 15.5, 0.0);
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