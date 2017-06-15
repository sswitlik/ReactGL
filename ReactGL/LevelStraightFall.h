#pragma once
#include "BodyObj.h"

class LevelStraightFall : public Level
{
protected:
	float time;		//from start lavel
	float timer;	//0 - timeHz
	int toFall;
	int phase;		//hmany timeHz passed

	std::vector<BodyObj *> *map;
	std::vector<BodyObj *> *objs;

	//Conteners
	std::vector<BodyObj *> effectCont;
	std::vector<BodyObj *> arrowCont;
	std::vector<BodyObj *> rockCont;
	std::vector<BodyObj *> aimCont;

	std::deque<BodyObj *> effectVec;
	std::deque<BodyObj *> arrowVec;
	std::deque<BodyObj *> rockVec;
	std::deque<BodyObj *> aimVec;

	//MODELS
	Model *Rock1Model;
	Model *Rock2Model;
	Model *AimBonusModel;
	Model *ArrowModel;

	rp3d::DynamicsWorld *world;
	void Prepare();			//fillConteners
	void mapUpdate();

public:
	LevelStraightFall();
	LevelStraightFall(rp3d::DynamicsWorld *_world, std::vector<BodyObj *> *_map, std::vector<BodyObj *> *_objs);
	~LevelStraightFall();
	void Update(float plusTime);
	void getData(std::vector<BodyObj *> *toServe);
	void getModels(Model *rock1, Model *rock2, Model *aimbonus, Model *arrow);
	void Initialize();
	BodyObj * Make(char *type, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation);
	void Kill(BodyObj *obj, char *type);
	void Draw();
};

