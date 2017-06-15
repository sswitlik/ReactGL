#pragma once
#include "BodyObj.h"
class Rock :
	public BodyObj
{
public:
	Rock(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass);
	~Rock();
};

