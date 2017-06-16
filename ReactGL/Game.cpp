#include "stdafx.h"
#include "Game.h"
#include <GL/freeglut.h>
#include "Arrow.h"
#include "maths.h"

extern int garbage;

Game::Game()
{
	srand(time(NULL));

	//quantity = 0;
	//TIME
	previousFrameTime = 0;
	accumulator = 0;
	
	//START WORLD
	//ExampleTestInit();
	InitLevel();

	//PLAYER
	rp3d::Vector3 initPosition(0, 4.0, 0);
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Vector3 shapeData(0.25, 1, 0.25);

	player = new Player(World, initPosition, initOrientation, shapeData);
	player->loadLevel(level);

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
	for (auto *obj : GarbageCollector)
	{
		delete obj;
	}

	delete AimBonusModel;
	delete Rock1Model;
	delete Rock2Model;
	delete ArrowModel;
	delete WaterModel;
	delete BirdModel;
}

void Game::InitLevel()
{
	//MODELS
	AimBonusModel = new Model();
	AimBonusModel->Initialize("Models/AimBonus.obj", "Models/AimBonus.bmp");
	Rock1Model = new Model();
	Rock1Model->Initialize("Models/Rock1.obj", "Models/RockTex2.bmp");
	Rock2Model = new Model();
	Rock2Model->Initialize("Models/Rock2.obj", "Models/RockTex2.bmp");
	ArrowModel = new Model();
	ArrowModel->Initialize("Models/Arrow3.obj", "Models/Arrow2.bmp");
	WaterModel = new Model();
	WaterModel->Initialize("Models/Water.obj", "Models/Water.bmp");
	SandModel = new Model();
	SandModel->Initialize("Models/Sand.obj", "Models/Sand.bmp");

	//WORLD
	rp3d::Vector3 gravity(0.0, -9.81, 0.0);
	World = new rp3d::DynamicsWorld(gravity);
	World->setNbIterationsVelocitySolver(15);
	World->setNbIterationsPositionSolver(8);

	//TEMPORARY DATA FOR INITIALIZATION
	rp3d::Vector3 initPosition;
	rp3d::Quaternion initOrientation;
	rp3d::Vector3 shapeData;

	int bottomSize = LevelLength + 200;
	//BOTTOM RUBBISH
	initPosition.setAllValues(0, -4, LevelLength/2);
	initOrientation = rp3d::Quaternion::identity();
	shapeData.setAllValues(50, 1, bottomSize/2);
	auto bottom = new Rubbish(this->World, initPosition, initOrientation, shapeData, 1);
	bottom->setType(rp3d::STATIC);
	bottom->setMaterial(0, 1);
	bottom->setCollisionCategory(RUBBISHcat);
	bottom->modelInit(SandModel);
	map.push_back(bottom);

	//WATER
	initPosition.setAllValues(0, 0, LevelLength/2);
	initOrientation = rp3d::Quaternion::identity();
	shapeData.setAllValues(50, 1, bottomSize/2);
	auto water = new Water(this->World, initPosition, initOrientation, shapeData, 1);
	water->setType(rp3d::STATIC);
	water->setMaterial(0, 1);
	water->setCollisionCategory(WATERcat);
	water->modelInit(WaterModel);
	map.push_back(water);

	//LEVEL
	level = new LevelStraightFall(World, &map, &objs);
	level->getModels(Rock1Model, Rock2Model, AimBonusModel, ArrowModel);
	level->Initialize();

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

void Game::Draw()
{
	level->Draw();
}

void Game::Update()
{
	static int loolTime = 0;
	//printf("%d\n", loolTime);
	garbage = GarbageCollector.size();
	//Clear the Garbage v2
/*
	if (GarbageCollector.size() > 5)
	{
		//COPYING WORLD
		rp3d::Vector3 gravity(0, -9.81, 0);
		rp3d::DynamicsWorld *newWorld = new rp3d::DynamicsWorld(gravity);
		
		rp3d::RigidBody *newBody;
		rp3d::Transform newTransform;
		rp3d::Transform identityTransform = rp3d::Transform::identity();
		rp3d::ProxyShape *newProxy;
		rp3d::CollisionShape *newShape;
		rp3d::decimal newMass;
		for (auto *i : objs)
		{
			rp3d::RigidBody *tmpBody = i->getBody();
			newTransform = tmpBody->getTransform();
			newBody = newWorld->createRigidBody(newTransform);

			newShape = i->getShape();
			newMass = tmpBody->getMass();
			if (newMass < 0.0000001)
				newMass = rp3d::decimal(1.0);
			newProxy = newBody->addCollisionShape(newShape, identityTransform, newMass);
			i->setType(tmpBody->getType());
			newProxy->setUserData(i);
			i->setCollisionCategory(i->getCollsionCategory());
			//---------------
			World->destroyRigidBody(tmpBody);
		}
		for (auto *i : map)
		{
			rp3d::RigidBody *tmpBody = i->getBody();
			newTransform = tmpBody->getTransform();
			newBody = newWorld->createRigidBody(newTransform);

			newShape = i->getShape();
			newMass = tmpBody->getMass();
			if (newMass < 0.0000001)
				newMass = rp3d::decimal(1.0);
			newProxy = newBody->addCollisionShape(newShape, identityTransform, newMass);
			i->setType(tmpBody->getType());
			newProxy->setUserData(i);
			i->setCollisionCategory(i->getCollsionCategory());
			//---------------
			World->destroyRigidBody(tmpBody);
		}
		for (auto *i : effects)
		{
			rp3d::RigidBody *tmpBody = i->getBody();
			newTransform = tmpBody->getTransform();
			newBody = newWorld->createRigidBody(newTransform);

			newShape = i->getShape();
			newMass = tmpBody->getMass();
			newProxy = newBody->addCollisionShape(newShape, identityTransform, newMass);
			newProxy->setUserData(i);
			i->setCollisionCategory(i->getCollsionCategory());
			//---------------
			World->destroyRigidBody(tmpBody);
		}
		//DELETING OLD WORLD
		for (auto *i : objs)
		{
			rp3d::RigidBody *tmpBody = i->getBody();
			World->destroyRigidBody(tmpBody);
		}
		for (auto *i : map)
		{
			rp3d::RigidBody *tmpBody = i->getBody();
			World->destroyRigidBody(tmpBody);
		}
		for (auto *i : effects)
		{
			rp3d::RigidBody *tmpBody = i->getBody();
			World->destroyRigidBody(tmpBody);
		}
		//CHANGE TO NEW
		World = newWorld;

	}
*/
	////Clear the garbage
/*if (GarbageCollector.size() > 10)
	{
		for (auto i = GarbageCollector.begin(); i < GarbageCollector.end(); i++)
		{
			delete (*i);
		}
		GarbageCollector.clear();
	}*/


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

		//test
		loolTime += 1;

		World->update(timeStep);				// Update the Dynamics world with a constant time step 

		player->update();

		float tmStp = timeStep;
		level->Update(tmStp);

/*
		for (auto i = map.begin(); i < map.end(); )
		{
			if ((*i)->getIsDeleted())
			{
				GarbageCollector.push_back(*i);
				(*i)->kill();
				auto x = (*i);
				i = map.erase(i);
				//delete x;
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
				GarbageCollector.push_back(*i);
				(*i)->kill();
				auto x = (*i);
				i = objs.erase(i);
				//i++;
				//delete x;
			}
			else
			{
				(*i)->update();
				i++;
			}

		}
		int k = 0;
		for (auto i = effects.begin(); i < effects.end(); )
		{
			if ((*i)->getIsDeleted())
			{
				//GarbageCollector.push_back(*i);
				//(*i)->kill();
				auto x = (*i);
				i = effects.erase(i);
				delete x;
			}
			else
			{
				(*i)->update();
				i++;
			}
				k++;

		}
*/
		
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
	obj->setMaterial(0.6, 0.2);
	//obj->modelInit("Models/MyRock.obj", "Models/MyRockTex.bmp");
	objs.push_back(obj);
}

Player * Game::getplayer()
{
	return player;
}

void Game::testshoot()
{
	player->test_shoot();
	//objs.push_back(b);
}

Events Game::getEvents()
{
	return events;
}

rp3d::DynamicsWorld * Game::getWorld()
{
	return World;
}

Level * Game::getLevel()
{
	return level;
}