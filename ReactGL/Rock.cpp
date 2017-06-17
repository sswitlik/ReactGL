#include "stdafx.h"
#include "Rock.h"


Rock::Rock(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass)
	: BodyObj(world, initPosition, initOrientation, shapeData, mass)
{
	time = 0;
	timeable = false;
	isAfterTime = false;
	maxTime = 0;
}

Rock::~Rock()
{
}

void Rock::setMaxTime(float max)
{
	timeable = true;
	time = 0;
	//time in seconds = maxTime/timeStep (0,016)
	maxTime = max/timeStep;
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

		x = rand() % 40 - 20;
		y = rand() % 40 - 20;
		z = rand() % 40 - 20;
		force.setAllValues(x, y, z);
		body->applyTorque(force);
		isAfterTime = true;
	}
}

void Rock::init(rp3d::Vector3 position, rp3d::Quaternion orientation)
{
	BodyObj::init(position, orientation);
	isAfterTime = false;
}