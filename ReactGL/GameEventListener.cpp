#include "stdafx.h"
#include "GameEventListener.h"
#include "BodyObj.h"

GameEventListener::GameEventListener(rp3d::DynamicsWorld *World)
{
	world = World;
}


GameEventListener::~GameEventListener()
{
}

void GameEventListener::beginContact(const rp3d::ContactPointInfo &contact)
{
	std::vector<const rp3d::ContactManifold*> manifolds;

	// Get all the contacts of the world 
	manifolds = world->getContactsList();
	std::vector<const rp3d::ContactManifold*>::iterator it;

	// For each contact manifold of the body 
	for (it = manifolds.begin(); it != manifolds.end(); ++it) {
		const rp3d::ContactManifold* manifold = *it;

		rp3d::ProxyShape *ps1 = manifold->getShape1();
		BodyObj *body1 = (BodyObj *) ps1->getUserData();

		rp3d::ProxyShape *ps2 = manifold->getShape2();
		BodyObj *body2 = (BodyObj *) ps2->getUserData();

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
