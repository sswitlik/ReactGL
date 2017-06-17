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

	frontier = 12;

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
	
	frontier = 12;

	map = _map;
	objs = _objs;

	world = _world;
}

LevelStraightFall::~LevelStraightFall()
{
}

void LevelStraightFall::Update(float plusTime)
{
	mapUpdate();

	float timeHz = 0.5;
	time += plusTime;
	timer += plusTime;

	if (timer < timeHz)		//akcja wykonywana co timeHz
		return;

	timer -= timeHz;
	phase++;
//---------------------------------------------------------------------------------

	//initialization temp data
	rp3d::Vector3 initPosition;
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Vector3 shapeData;

	//add stones
	if (phase % 1 == 0 && frontier < LevelLength)
	{
		frontier++;
		for (int j = 0; j < 3; j++)
		{
			if (j % 2)
				initPosition.setAllValues(j, 2, frontier);
			else
				initPosition.setAllValues(j, 2, frontier + 0.5);

			auto brick = Make("rock", initPosition, initOrientation);
			brick->setMaxTime(18);
			int i = 0;
		}
	}
	//add aims
	if ((phase + 5) %  4 == 0)	//add aim
	{
		int side;
		if (rand() % 2)
			side = 1;		//right
		else
			side = -1;		//left
		int distance = rand() % 5 + 8;
		int height = rand() % 5 + 4;
		float location = ((float)(rand() % 61) / 10) - 3.0 + phase*2;
		initPosition.setAllValues(distance * side, height, location);
		float rotation = rand() % 91;
		rotation = rotation * PI / 180;
		BodyObj *obj;
		if (side == 1)
		{
			rp3d::Quaternion aimOrientation(PI_2 + rotation, PI_2, 0);
			obj = Make("aim", initPosition, aimOrientation);
		}
		else
		{
			rp3d::Quaternion aimOrientation(PI_2 - rotation, PI_2, 0);
			obj = Make("aim", initPosition, aimOrientation);
		}
		obj->setPoint(distance + height + rotation/5);
	}
	
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
	rp3d::Quaternion cylinderOrientation(PI_2, PI_2, 0);
	rp3d::Vector3 shapeData;

	//START
	for (int j = 0; j < 11; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (i % 2)
				initPosition.setAllValues(i, 2, j-1);
			else
				initPosition.setAllValues(i, 2, j-1 + 0.5);
			auto obj = Make("rock", initPosition, initOrientation);
			if (j < 3)
				obj->setMaxTime(j * 2 + 3);
			else
				obj->setMaxTime(j*0.4 + 6);
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
			initPosition.setAllValues( i - 1, 2, j + LevelLength);
			BodyObj * obj = Make("rock", initPosition, initOrientation);
		}
	}
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