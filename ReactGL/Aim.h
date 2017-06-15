#pragma once
#include "BodyObj.h"


class Aim :
	public BodyObj
{
	float lives;
	std::vector<Arrow *> drilledIn;

public:
	Aim(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass);
	Aim(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::CollisionShape *shapeData, rp3d::decimal mass);

	~Aim();

	void Draw();
	void makeCollision(BodyObj *CollideWith);
	void update();
	void kill();
};
