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

	//LEVEL
	level = new LevelStraightFall();
	
	//START WORLD
	//ExampleTestInit();
	InitLevel();

	//PLAYER
	rp3d::Vector3 initPosition(0, 4.0, 0);
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Vector3 shapeData(0.25, 1, 0.25);

	player = new Player(World, initPosition, initOrientation, shapeData);

	//CONTACT 
	listener = new GameEventListener(World,&effects);
	World->setEventListener(listener);

}


Game::~Game()
{
	delete level;
	for (auto *obj : objs)
	{
		delete obj;
	}
	for (auto *obj : map)
	{
		delete obj;
	}
	for (auto *obj : effects)
	{
		delete obj;
	}
}

void Game::InitLevel()
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

	//BOTTOM RUBBISH
	initPosition.setAllValues(0, -4, 200);
	initOrientation = rp3d::Quaternion::identity();
	shapeData.setAllValues(50, 1, 250);
	auto bottom = new Water(this->World, initPosition, initOrientation, shapeData, 1);
	bottom->setType(rp3d::STATIC);
	bottom->setMaterial(0, 1);
	bottom->setCollisionCategory(RUBBISHcat);
	map.push_back(bottom);

	//WATER
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			initPosition.setAllValues(i*4-4, 0, j*4-4);
			initOrientation = rp3d::Quaternion::identity();
			shapeData.setAllValues(2, 1, 2);
			auto water = new Water(this->World, initPosition, initOrientation, shapeData, 1);
			water->setType(rp3d::STATIC);
			water->setMaterial(0, 1);
			water->setCollisionCategory(RUBBISHcat);
			map.push_back(water);
		}
	}

	std::vector<BodyObj *> *bricks = new std::vector<BodyObj *>;
	//FLOOR
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (j%2)
				initPosition.setAllValues(j-1, 3, i-1);
			else
				initPosition.setAllValues(j - 1, 3, i-1+0.5);
			initOrientation = rp3d::Quaternion::identity();
			shapeData.setAllValues(0.45, 0.3, 0.45);
			auto brick = new BodyObj(this->World, initPosition, initOrientation, shapeData, 1);
			brick->setType(rp3d::STATIC);
			brick->setMaterial(0, 1);
			//brick->setCollisionCategory(MAPcat);
			brick->setCollisionCategory(FREEcat);
			if (rand() % 2)
				brick->modelInit("Models/MyRock2.obj", "Models/MyRockTex.bmp");
			else 
				brick->modelInit("Models/MyRock.obj", "Models/MyRockTex.bmp");
			bricks->push_back(brick);
			map.push_back(brick);
		}
	}
	level->getData(bricks);

	//AIMS
	initPosition.setAllValues(0, 4, 6);
	rp3d::Quaternion initOrientRotated(0, 0, PI_2);
	//shapeData.setAllValues(0.2, 1, 1);
	auto aim = new Aim(this->World, initPosition, initOrientRotated, new rp3d::CylinderShape(1,0.2,0.1), 1);
	aim->setType(rp3d::STATIC);
	aim->setMaterial(0, 1);
	aim->setCollisionCategory(MAPcat);
	aim->modelInit("Models/AimBonus.obj", "Models/AimBonus.bmp");
	map.push_back(aim);

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
	auto floor = new BodyObj(this->World, initPosition, initOrientation, shapeData, 1);
	floor->setType(rp3d::STATIC);
	floor->setMaterial(0, 1);
	floor->setCollisionCategory(MAPcat);
	map.push_back(floor);

	//OBJ
	initPosition.setAllValues(0.0, 2.0, 0.0);
	initOrientation = rp3d::Quaternion::identity();
	shapeData.setAllValues(0.5, 0.5, 0.5);
	auto obj1 = new BodyObj(this->World, initPosition, initOrientation, shapeData, 2);
	obj1->setMaterial(0.3, 0.8);
	obj1->setType(rp3d::STATIC);
	obj1->setCollisionCategory(MAPcat);
	obj1->modelInit("Models/kostka3.obj", "Models/kostka3.bmp");
	map.push_back(obj1);
}

void Game::Update()
{
	//Clear the garbage
	for (auto i = GarbageCollector.begin(); i<GarbageCollector.end(); i++)
	{
		rp3d::ProxyShape *toDeleteProxy = (*i)->getProxyShape();
		rp3d::RigidBody *toDeleteBody = (*i)->getBody();
		//toDeleteBody->removeCollisionShape(toDeleteProxy);
		World->destroyRigidBody(toDeleteBody);
		delete (*i);
		(*i) = NULL;
		int j = 0;
	}
	GarbageCollector.clear();


	///TIME UPDATE
	if (previousFrameTime == 0)
		previousFrameTime = glutGet(GLUT_ELAPSED_TIME) - 1;

	
	//const float timeStep = 0.016;				// Constant physics time step 
	const int timeStep2 = 16;
	
	unsigned int currentFrameTime = glutGet(GLUT_ELAPSED_TIME);			// Get the current system time 
	
	unsigned int deltaTime = currentFrameTime - previousFrameTime;		// Compute the time difference between the two frames 
	
	previousFrameTime = currentFrameTime;		// Update the previous time 
	
	accumulator += deltaTime;					// Add the time difference in the accumulator 

	int t1 = glutGet(GLUT_ELAPSED_TIME);		// While there is enough accumulated time to take one or several physics steps 
	while (accumulator >= timeStep2) {

		World->update(timeStep);				// Update the Dynamics world with a constant time step 

		player->update();

		float tmStp = timeStep;
		level->Update(tmStp);

		for (auto i = map.begin(); i < map.end(); )
		{
			if ((*i)->getIsDeleted())
			{
				delete (*i);
				i = map.erase(i);
			}
			else
			{
				(*i)->update();
				i++;
			}

		}
		for (auto i = objs.begin(); i < objs.end(); )
		{
			if ((*i)->getIsDeleted())
			{
				//delete (x);
				i = objs.erase(i);
			}
			else
			{
				(*i)->update();
				i++;
			}

		}
		for (auto i = effects.begin(); i < effects.end(); )
		{
			if ((*i)->getIsDeleted())
			{
				delete (*i);
				i = effects.erase(i);
			}
			else
			{
				(*i)->update();
				i++;
			}

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
	BodyObj *obj = new BodyObj(this->World, initPosition, initOrientation, shapeData, 10);
	obj->setMaterial(0.01, 0.2);
	obj->modelInit("Models/MyRock.obj", "Models/MyRockTex.bmp");
	objs.push_back(obj);
}

Player * Game::getplayer()
{
	return player;
}

void Game::testshoot()
{
	Arrow *b = player->test_shoot();
	objs.push_back(b);
}

Events Game::getEvents()
{
	return events;
}

rp3d::DynamicsWorld * Game::getWorld()
{
	return World;
}