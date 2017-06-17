#pragma once
#include "BodyObj.h"
class Rock :
	public BodyObj
{
	int time;
	float maxTime;
	bool timeable;	//is destroyed because of time
	bool isAfterTime;
public:
	Rock(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass);
	~Rock();
	void setMaxTime(float max);
	void update();
	void init(rp3d::Vector3 position, rp3d::Quaternion orientation);
};

