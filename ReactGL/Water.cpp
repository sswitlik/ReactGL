#include "stdafx.h"
#include "Water.h"


Water::Water(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass) 
	: BodyObj( world, initPosition, initOrientation, shapeData, mass)
{

}


Water::~Water()
{
}
