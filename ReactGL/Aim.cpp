#include "stdafx.h"
#include "Aim.h"
#include <algorithm>

extern  Game *game;


Aim::Aim(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass)
	: BodyObj(world, initPosition, initOrientation, shapeData, mass)
{
	lives = 2.9;
	setType(rp3d::STATIC);
}

Aim::Aim(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::CollisionShape *shapeData, rp3d::decimal mass)
	: BodyObj(world, initPosition, initOrientation, shapeData, mass)
{
	lives = 2.9;
	setType(rp3d::STATIC);
}


Aim::~Aim()
{
	for (auto it = drilledIn.begin(); it < drilledIn.end(); it++)
	{
		(*it)->setGravityEnable(true);
		(*it)->pushToGravity();
	}

	auto events = game->getEvents();
	events.ImprovePlayerAccuracy(100);
}

void Aim::makeCollision(BodyObj *CollideWith)
{
	int collideCategory = CollideWith->getProxyShape()->getCollisionCategoryBits();
	if (collideCategory & ARROWcat)
	{
		auto collider = (Arrow *)CollideWith;
		bool isDrilled = false;
		std::vector<Arrow *>::iterator it;
		it = find(drilledIn.begin(), drilledIn.end(), collider);
		if (it != drilledIn.end())	//element juz wystepuje
		{
			;
		}
		else						//element nie wystepuje
		{
			drilledIn.push_back(collider);
			lives -= collider->getDamage();
		}
	}
	else
	if (collideCategory & RUBBISHcat)
	{
		BodyObj::makeCollision(CollideWith);
	}
}

void Aim::update()
{
	
	if (lives <= 0)
		IsDeleted = true;
}

void Aim::Draw()
{
	rp3d::Transform transform = body->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);

	if (model)
	{
		glPushMatrix();
		glMultMatrixf(matrix);
		glScalef(1.5, 1.5, 1.5);
		glColor3f(0, 0.5, 0.5);
		model->Render();
		glColor3f(1, 1, 1);
		//glutWireCube(1);
		glPopMatrix();
	}
}