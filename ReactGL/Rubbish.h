#pragma once
#include "BodyObj.h"
class Rubbish :
	public BodyObj
{
public:
	Rubbish(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass);
	~Rubbish();

	void makeCollision(BodyObj *CollideWith);
	void Draw();
};

