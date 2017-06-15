#include "stdafx.h"
#include "Rock.h"


Rock::Rock(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass)
	: BodyObj(world, initPosition, initOrientation, shapeData, mass)
{
}


Rock::~Rock()
{
}
