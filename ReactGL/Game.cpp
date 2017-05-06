#include "stdafx.h"
#include "Game.h"
#include <GL/freeglut.h>
#include "Arrow.h"

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