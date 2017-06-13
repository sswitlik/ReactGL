#include "stdafx.h"
#include "GameEventListener.h"
#include "BodyObj.h"
#include "maths.h"
#include "Arrow.h"

extern Game *game;

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

		int collideMask1 = ps1->getCollisionCategoryBits();
		int collideMask2 = ps2->getCollisionCategoryBits();
		if (collideMask1 & ARROWcat || collideMask2 & ARROWcat)
			int i = 0;

		if (body1 && body2)
		{
			body1->makeCollision(body2);
			body2->makeCollision(body1);
		}


		//if (collideMask1 & ARROWcat && body1->getOneParticles())
		//{

		//	rp3d::Vector3 pos = body1->getBody()->getTransform().getPosition();
		//	rp3d::Vector3 vel = body1->getBody()->getLinearVelocity();
		//	vel.normalize();
		//	pos += vel / 4;
		//	for (int i = 0; i < 5; i++)
		//		{
		//			int x = rand() % 30000 - 15000,
		//				y = rand() % 30000 - 15000,
		//				z = rand() % 30000 - 15000;
		//			rp3d::Vector3 force(x, y, z);
		//			rp3d::Vector3 col(0.5, 0, 0);
		//			Particle *partic = new Particle(game->getWorld(), pos, 0.02, col);
		//			partic->setMaterial(0.5, 0.1);
		//			partic->giveForce(force);
		//			if (game->effects.size() < 5)
		//				game->effects.push_back(partic);
		//			body1->setOneParticles(false);
		//		}
		//}
		
		/*
		if (collideMask1 & ARROWcat && body1->getOneParticles())
		{
			// For each contact point of the manifold 
			for (int i = 0; i < manifold->getNbContactPoints(); i++) 
			{

				// Get the contact point 
				rp3d::ContactPoint* point = manifold->getContactPoint(i);

				// Get the world-space contact point on body 1 
				rp3d::Vector3 pos = point->getWorldPointOnBody1();

				// Get the world-space contact normal 
				rp3d::Vector3 normal = point->getNormal();

				for (int i = 0; i < 5; i++)
				{
					int x = rand() % 30000 - 15000,
						y = rand() % 30000 - 15000,
						z = rand() % 30000 - 15000;
					rp3d::Vector3 force(x, y, z);
					rp3d::Vector3 col(0.5, 0, 0);
					Particle *partic = new Particle(game->getWorld(), pos, 0.02, col);
					partic->setMaterial(0.5, 0.1);
					partic->giveForce(force);
					if (game->effects.size() < 5)
						game->effects.push_back(partic);
					body1->setOneParticles(false);
				}
			}
		}
		if (collideMask2 & ARROWcat && body2->getOneParticles())
		{
			// For each contact point of the manifold 
			for (int i = 0; i < manifold->getNbContactPoints(); i++) {

				// Get the contact point 
				rp3d::ContactPoint* point = manifold->getContactPoint(i);

				// Get the world-space contact point on body 2
				rp3d::Vector3 pos = point->getWorldPointOnBody2();

				// Get the world-space contact normal 
				rp3d::Vector3 normal = point->getNormal();

				//particle test
				for (int i = 0; i < 5; i++)
				{
					int x = rand() % 30000 - 15000,
						y = rand() % 30000 - 15000,
						z = rand() % 30000 - 15000;
					rp3d::Vector3 force(x, y, z);
					rp3d::Vector3 col(0.5, 0, 0);
					Particle *partic = new Particle(game->getWorld(), pos, 0.02, col);
					partic->setMaterial(0.5, 0.1);
					partic->giveForce(force);
					if (game->effects.size() < 5)
						game->effects.push_back(partic);
					body2->setOneParticles(false);
				}
			}
				
		}
		int i = 0;
		*/
	}
}
