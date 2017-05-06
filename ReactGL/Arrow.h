#pragma once
#include "BodyObj.h"

class Arrow : public BodyObj
{
protected:
	float damage;
	rp3d::Vector3 size;
public:
	Arrow();
	Arrow(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, float radius, float length, rp3d::decimal mass);
	~Arrow();

	void Draw();

};

