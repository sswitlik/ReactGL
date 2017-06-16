#include "stdafx.h"
#include "LevelStraightFall.h"
#include "BodyObj.h"
#include <algorithm>

LevelStraightFall::LevelStraightFall()
{
	time = 0;
	timer = 0;
	toFall = -3;
	phase = 0;

	map = NULL;
	objs = NULL;

	Prepare();
}

LevelStraightFall::LevelStraightFall(rp3d::DynamicsWorld *_world, std::vector<BodyObj *> *_map, std::vector<BodyObj *> *_objs)
{
	time = 0;
	timer = 0;
	toFall = 0;
	phase = 0;

	map = _map;
	objs = _objs;

	world = _world;
}

LevelStraightFall::~LevelStraightFall()
{
}

void LevelStraightFall::Update(float plusTime)
{
	//rp3d::Vector3 v(10, 2, 0);
	//Make("effect", v, rp3d::Quaternion::identity());
	printf("%d %d\n", effectVec.size(), effectCont.size());

	mapUpdate();

	float timeHz = 0.4;
	time += plusTime;
	timer += plusTime;

	
	if (time < 2)		//3s na rozgrzewke
		return;

	if (timer < timeHz)		//akcja wykonywana co timeHz
		return;

	time -= timeHz;
	phase++;


	auto it = objs->begin();
	if (objs->size() > 0)
	{
		while ((*it)->getType() == rp3d::DYNAMIC)
			it++;
		for (int i = 0; i < 3; i++)
		{
			(*it)->setType(rp3d::DYNAMIC);
			it++;
		}
	}

	//initialization temp data
	rp3d::Vector3 initPosition;
	rp3d::Quaternion initOrientation;
	rp3d::Vector3 shapeData;

	//add stones
	//int frontier = phase + 14;
	//for (int j = 0; j < 3; j++)
	//{
	//	if (j % 2)
	//		initPosition.setAllValues(j - 1, 3, frontier);
	//	else
	//		initPosition.setAllValues(j - 1, 3, frontier + 0.5);
	//	initOrientation = rp3d::Quaternion::identity();
	//	shapeData.setAllValues(0.45, 0.3, 0.45);
	//	auto brick = new BodyObj(world, initPosition, initOrientation, shapeData, 1);
	//	brick->setType(rp3d::STATIC);
	//	brick->setMaterial(0, 1);
	//	brick->setCollisionCategory(FREEcat);
	//	if (rand() % 2)
	//		brick->modelInit(Rock1Model);
	//	else
	//		brick->modelInit(Rock2Model);
	//	objs->push_back(brick);

	//}
		



	if ((phase + 5) % 10 == 0)	//add aim
		;
	//if (toFall >= 0) 
	//{
	//	int brickSize = bricks->size();
	//	if (toFall < brickSize)
	//		bricks->at(toFall)->setType(rp3d::DYNAMIC);
	//	if (toFall+1 < brickSize)
	//		bricks->at(toFall+1)->setType(rp3d::DYNAMIC);
	//	if (toFall+2 < brickSize)
	//		bricks->at(toFall+2)->setType(rp3d::DYNAMIC);
	//}
	//toFall += 3;
	
}

void LevelStraightFall::getData(std::vector<BodyObj *> *toServe)
{
	//forget old
	objs = toServe;
}

void LevelStraightFall::Initialize()
{
	Prepare();
	rp3d::Vector3 initPosition;
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Quaternion cylinderOrientation(PI_2, 0, 0);
	rp3d::Vector3 shapeData;

	//START
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			initPosition.setAllValues(j, 2, i);
			auto obj = Make("rock", initPosition, initOrientation);
			obj->setMaxTime(j*2+3);
		}
	}

	//AIM
	{
		initPosition.setAllValues(8, 3, 4);
		auto obj = Make("aim", initPosition, cylinderOrientation);
		obj->setMaxTime(50);
	}

	//META
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			initPosition.setAllValues(j + LevelLength, 2, i -1);
			BodyObj * obj = Make("rock", initPosition, initOrientation);
		}
	}

	/*
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (j % 2)
				initPosition.setAllValues(j - 1, 3, i - 1);
			else
				initPosition.setAllValues(j - 1, 3, i - 1 + 0.5);
			initOrientation = rp3d::Quaternion::identity();
			shapeData.setAllValues(0.45, 0.3, 0.45);
			auto brick = new BodyObj(world, initPosition, initOrientation, shapeData, 1);
			brick->setType(rp3d::STATIC);
			brick->setMaterial(0, 1);
			brick->setCollisionCategory(FREEcat);
			if (rand() % 2)
				brick->modelInit(Rock1Model);
			else
				brick->modelInit(Rock2Model);
			objs->push_back(brick);
		}
	}

	initPosition.setAllValues(2, 4, 6);
	rp3d::Quaternion initOrientRotated(0, 0, PI_2);
	//shapeData.setAllValues(0.2, 1, 1);
	auto aim = new Aim(world, initPosition, initOrientRotated, new rp3d::CylinderShape(1, 0.2, 0.1), 1);
	aim->setType(rp3d::STATIC);
	aim->setMaterial(0, 1);
	aim->setCollisionCategory(MAPcat);
	aim->modelInit(AimBonusModel);
	map->push_back(aim);

	//META
	initPosition.setAllValues(0, 3, LevelLength);
	initOrientation = rp3d::Quaternion::identity();
	shapeData.setAllValues(2, 0.3, 2);
	auto meta = new BodyObj(world, initPosition, initOrientation, shapeData, 1);
	meta->setType(rp3d::STATIC);
	meta->setMaterial(0, 1);
	//brick->setCollisionCategory(MAPcat);
	meta->setCollisionCategory(FREEcat);
	if (rand() % 2)
		meta->modelInit(Rock1Model);
	else
		meta->modelInit(Rock2Model);
	objs->push_back(meta);
	*/
}

void LevelStraightFall::getModels(Model *rock1, Model *rock2, Model *aimbonus, Model *arrow)
{
	Rock1Model = rock1;
	Rock2Model = rock2;
	AimBonusModel = aimbonus;
	ArrowModel = arrow;
}

void LevelStraightFall::Prepare()
{
	rp3d::Vector3 zeroVector(0, 0, 0);
	rp3d::Quaternion zeroQuaternion(0, 0, 0);
	
	//EFFECTS
	for (int i = 0; i < 30; i++)
	{
		auto obj = new Particle(world, zeroVector, 0.02, zeroVector);
		effectCont.push_back(obj);
	}

	//ARROWS
	for (int i = 0; i < 5; i++)
	{
		auto obj = new Arrow(world, zeroVector, zeroQuaternion, 0.05, 0.5, 2);
		arrowCont.push_back(obj);
		obj->modelInit(ArrowModel);
	}

	//ROCKS
	rp3d::Vector3 rockShape(0.45, 0.3, 0.45);
	for (int i = 0; i < 200; i++)
	{
		auto obj = new Rock(world, zeroVector, zeroQuaternion, rockShape, 10);
		rockCont.push_back(obj);
		if (rand() % 2)
			obj->modelInit(Rock1Model);
		else
			obj->modelInit(Rock2Model);
	}

	//AIMS
	for (int i = 0; i < 20; i++)
	{
		auto obj = new Aim(world, zeroVector, zeroQuaternion, new rp3d::CylinderShape(1, 0.2, 0.1), 30);
		aimCont.push_back(obj);
		obj->modelInit(AimBonusModel);
	}
}

BodyObj * LevelStraightFall::Make(char *type, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation)
{
	BodyObj *obj;
	if (type == "arrow")
	{
		if (arrowCont.size() > 0)
		{
			obj = arrowCont.back();
			obj->setType(rp3d::DYNAMIC);
			obj->init(initPosition, initOrientation);
			arrowVec.push_back(obj);
			arrowCont.pop_back();

		}
		else
		{
			obj = arrowVec.front();
			obj->init(initPosition, initOrientation);
			arrowVec.pop_front();
			arrowVec.push_back(obj);
		}
	}
	else
	if (type == "effect")
	{
		if (effectCont.size() > 0)
		{
			obj = effectCont.back();
			obj->setType(rp3d::DYNAMIC);
			obj->init(initPosition, initOrientation);
			effectVec.push_back(obj);
			effectCont.pop_back();
		}
		else
		{
			obj = effectVec.front();
			obj->setType(rp3d::DYNAMIC);
			obj->init(initPosition, initOrientation);
			effectVec.pop_front();
			effectVec.push_back(obj);
		}
	}
	else
	if (type == "rock")
	{
		if (rockCont.size() > 0)
		{
			obj = rockCont.back();
			obj->init(initPosition, initOrientation);
			obj->setCollisionCategory(MAPcat);
			rockVec.push_back(obj);
			rockCont.pop_back();
		}
		else
		{
			obj = rockVec.front();
			obj->init(initPosition, initOrientation);
			obj->setCollisionCategory(MAPcat);
			rockVec.pop_front();
			rockVec.push_back(obj);
		}
	}
	else
	if (type == "aim")
	{
		if (aimCont.size() > 0)
		{
			obj = aimCont.back();
			obj->init(initPosition, initOrientation);
			obj->setCollisionCategory(MAPcat);
			aimVec.push_back(obj);
			aimCont.pop_back();
		}
		else
		{
			obj = aimVec.front();
			obj->init(initPosition, initOrientation);
			obj->setCollisionCategory(MAPcat);
			aimVec.pop_front();
			aimVec.push_back(obj);
		}
	}

	return obj;
}

void LevelStraightFall::Kill(BodyObj *obj, char *type)
{
	rp3d::Vector3 zeroPosition(0, 0, 0);
	rp3d::Quaternion zeroOrientation = rp3d::Quaternion::identity();
	if (type == "arrow")
	{
		auto it = arrowVec.begin();
		while ((*it) != obj)

			it++;

		arrowVec.erase(it);
		obj->setType(rp3d::STATIC);
		obj->init(zeroPosition, zeroOrientation);
		arrowCont.push_back(obj);
		obj->setIsDeleted(false);
	}
	else
	if (type == "effect")
	{
		auto it = effectVec.begin();
		while ((*it) != obj)
			it++;

		effectVec.erase(it);
		obj->setType(rp3d::STATIC);
		obj->init(zeroPosition, zeroOrientation);
		effectCont.push_back(obj);
		obj->setIsDeleted(false);
	}
	else
	if (type == "rock")
	{
		auto it = rockVec.begin();
		while ((*it ) != obj)
			it++;
		
		rockVec.erase(it);
		obj->setType(rp3d::STATIC);
		//obj->setCollisionCategory(EFFECTcat);
		obj->init(zeroPosition, zeroOrientation);
		rockCont.push_back(obj);
		obj->setIsDeleted(false);
	}
	else
	if (type == "aim")
	{
		auto it = aimVec.begin();
		while ((*it ) != obj)
			it++;
		
		aimVec.erase(it);
		obj->setType(rp3d::STATIC);
		//obj->setCollisionCategory(EFFECTcat);
		obj->init(zeroPosition, zeroOrientation);
		aimCont.push_back(obj);
		obj->kill();
		obj->setIsDeleted(false);
	}
}

void LevelStraightFall::mapUpdate()
{
	for (int i = 0; i<effectVec.size(); i++)
	{
		if (effectVec[i]->getIsDeleted())
			Kill(effectVec[i], "effect");
		else
		{
			effectVec[i]->update();
		}
	}

	for (int i=0; i<arrowVec.size();i++)
	{
		if (arrowVec[i]->getIsDeleted())
			Kill(arrowVec[i], "arrow");
		else
		{
			arrowVec[i]->update();
		}
	}

	for (int i = 0; i<rockVec.size(); i++)
	{
		if (rockVec[i]->getIsDeleted())
			Kill(rockVec[i], "rock");
		else
		{
			rockVec[i]->update();
		}
	}

	for (int i = 0; i<aimVec.size(); i++)
	{
		if (aimVec[i]->getIsDeleted())
			Kill(aimVec[i], "aim");
		else
		{
			aimVec[i]->update();
		}
	}
}

void LevelStraightFall::Draw()
{
	for (auto *i : effectVec)
		i->Draw();

	for (auto *i : arrowVec)
		i->Draw();

	for (auto *i : rockVec)
		i->Draw();

	for (auto *i : aimVec)
		i->Draw();
}