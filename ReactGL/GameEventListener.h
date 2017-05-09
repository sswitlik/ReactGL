#pragma once
#include "reactphysics3d.h"
#include "BodyObj.h"

class GameEventListener: public rp3d::EventListener
{
public:
	GameEventListener(rp3d::DynamicsWorld *World, std::vector <BodyObj *> &effects);
	~GameEventListener();

	void newContact(const rp3d::ContactPointInfo &contact);

private:
	rp3d::DynamicsWorld *world;
	std::vector <BodyObj *> *events;
};

