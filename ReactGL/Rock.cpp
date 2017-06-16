#include "stdafx.h"
#include "Rock.h"


Rock::Rock(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass)
	: BodyObj(world, initPosition, initOrientation, shapeData, mass)
{
	time = 0;
	timeable = false;
	isAfterTime = false;
}

Rock::~Rock()
{
}

void Rock::setMaxTime(float max)
{
	timeable = true;
	//time in seconds = maxTime/timeStep (0,016)
	maxTime = max/timeStep;
	int i = 0;
}

void Rock::update()
{
	time++;
	if (!isAfterTime && timeable && time > maxTime)
	{
		setType(rp3d::DYNAMIC);
		setCollisionCategory(MAPcat);
		int x = rand() % 4000 - 1500,
			y = rand() % 2000 + 4000,
			z = rand() % 4000 - 2000;
		rp3d::Vector3 force(x, y, z);
		body->applyForceToCenterOfMass(force);
		isAfterTime = true;
	}
}