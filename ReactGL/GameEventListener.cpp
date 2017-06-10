#include "stdafx.h"
#include "GameEventListener.h"
#include "BodyObj.h"
#include "maths.h"
#include "Arrow.h"

GameEventListener::GameEventListener(rp3d::DynamicsWorld *World, std::vector <BodyObj *> *effects)
{
	world = World;

	//test
	rp3d::Vector3 initPosition;
	rp3d::Quaternion initOrientation;
	rp3d::Vector3 shapeData;

	events = effects;
}


GameEventListener::~GameEventListener()
{
}

void GameEventListener::newContact(const rp3d::ContactPointInfo &contact)
{
	std::vector<const rp3d::ContactManifold*> manifolds;

	// Get all the contacts of the world 
	manifolds = world->getContactsList();
	std::vector<const rp3d::ContactManifold*>::iterator it;

	// For each contact manifold of the body 
	for (it = manifolds.begin(); it != manifolds.end(); ++it) {
		const rp3d::ContactManifold* manifold = *it;

		rp3d::ProxyShape *ps1 = manifold->getShape1();
		rp3d::ProxyShape *ps2 = manifold->getShape2();

		BodyObj *body1 = (BodyObj *)ps1->getUserData();
		BodyObj *body2 = (BodyObj *)ps2->getUserData();

		int arg = ps2->getCollideWithMaskBits();
		if (body1)
			body1->makeCollision(ps2->getCollideWithMaskBits());
		if (body2)
			body2->makeCollision(ps1->getCollideWithMaskBits());

		/*
		// For each contact point of the manifold 
		for (int i = 0; i<manifold->getNbContactPoints(); i++) {

			// Get the contact point 
			rp3d::ContactPoint* point = manifold->getContactPoint(i);

			// Get the world-space contact point on body 1 
			rp3d::Vector3 pos = point->getWorldPointOnBody1();

			// Get the world-space contact normal 
			rp3d::Vector3 normal = point->getNormal();
		}
		*/
	}
}
