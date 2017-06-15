#pragma once
#include "BodyObj.h"

class Level
{
	virtual void Prepare();
	virtual void mapUpdate();
public:
	Level();
	~Level();

	virtual void Update(float plusTime);
	virtual void getData(std::vector<BodyObj *> *toServe);
	virtual void getModels(Model *rock1, Model *rock2, Model *aimbonus, Model *arrow);
	virtual void Initialize();
	virtual BodyObj * Make(char *type, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation);
	virtual void Kill(BodyObj *obj, char *type);
	virtual void Draw();
};

