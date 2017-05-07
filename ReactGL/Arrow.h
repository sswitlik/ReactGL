#pragma once
#include "BodyObj.h"

class Arrow : public BodyObj
{
protected:
	float damage;
	rp3d::Vector3 size;
	int time;
	rp3d::Vector3 color;
public:
	Arrow();
	Arrow(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, float radius, float length, rp3d::decimal mass);
	~Arrow();

	void Draw();
	void update();
};

