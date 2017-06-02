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
		if (ps1->getCollisionCategoryBits() == ARROWcat)
		{
			Arrow *body1 = (Arrow *)ps1->getUserData();
			body1->collided = true;
			body1->color.setAllValues(0.5, 1, 0);
		}

		rp3d::ProxyShape *ps2 = manifold->getShape2();
		if (ps2->getCollisionCategoryBits() == ARROWcat)
		{
			Arrow *body2 = (Arrow *)ps2->getUserData();
			body2->collided = true; 
			body2->color.setAllValues(0.5, 1, 0);
		}

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
