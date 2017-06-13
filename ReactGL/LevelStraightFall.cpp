#include "stdafx.h"
#include "LevelStraightFall.h"

LevelStraightFall::LevelStraightFall()
{
	time = 0;
	toFall = -3;
	bricks = NULL;
}

LevelStraightFall::LevelStraightFall(std::vector<BodyObj *> *toServe)
{
	time = 0;
	toFall = 0;
	bricks = toServe;
}

LevelStraightFall::~LevelStraightFall()
{
}

void LevelStraightFall::Update(float plusTime)
{
	float fallHz = 0.6;
	time += plusTime;

	if (time > fallHz)
	{
		time -= fallHz;
		
		if (toFall >= 0) 
		{
			int brickSize = bricks->size();
			if (toFall < brickSize)
				bricks->at(toFall)->setType(rp3d::DYNAMIC);
			if (toFall+1 < brickSize)
				bricks->at(toFall+1)->setType(rp3d::DYNAMIC);
			if (toFall+2 < brickSize)
				bricks->at(toFall+2)->setType(rp3d::DYNAMIC);
		}
		toFall += 3;
	}
}

void LevelStraightFall::getData(std::vector<BodyObj *> *toServe)
{
	//forget old
	bricks = toServe;
}