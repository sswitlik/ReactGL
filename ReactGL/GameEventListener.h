#pragma once
#include "reactphysics3d.h"

class GameEventListener: public rp3d::EventListener
{
public:
	GameEventListener(rp3d::DynamicsWorld *World);
	~GameEventListener();

	void newContact(const rp3d::ContactPointInfo &contact);

private:
	rp3d::DynamicsWorld *world;
};

