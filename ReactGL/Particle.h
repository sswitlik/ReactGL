#pragma once
#include "BodyObj.h"
class Particle :
	public BodyObj
{
	int maxTime;
	int time;
	float timeInSec;
	int queueNumber;
	rp3d::Vector3 color;
public:
	Particle(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, float radiusSize, rp3d::Vector3 color);
	Particle(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, float radiusSize, rp3d::Vector3 color, int queueNumb);
	~Particle();

	void update();
	void Draw();
	void setMaxTime(int max);
	void makeShards(int hmany, rp3d::Vector3 initPos);
	void giveForce(rp3d::Vector3 force);
	void setQueueNumber(int numb);
	void setCollisionCategory(Category cat);
};

