#include "stdafx.h"
#include "GL/freeglut.h"
#include "BodyObj.h"
#include <cmath>
#include "maths.h"
#include "Game.h"

#define RAD  0.01745329

extern Game *game;

BodyObj::BodyObj()
{

}

BodyObj::BodyObj(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass)
{
	rp3d::Transform transform(initPosition, initOrientation);
	body = world->createRigidBody(transform);

	shape = new rp3d::BoxShape(shapeData, rp3d::decimal(0.1));

	rp3d::Transform transform2 = rp3d::Transform::identity();
	proxyShape = body->addCollisionShape(shape, transform2, mass);

	//pointer to this - using in collision detection
	proxyShape->setUserData(this);

	//COLLISION FILTERING
	setCollisionCategory(FREEcat);
	
	//DRAWING
	model = NULL;
	modelll.setAllValues(shapeData.x*2, shapeData.y * 2, shapeData.z * 2);

	//SAVE DELETING
	IsDeleted = false;
	gameWorld = world;

	//PARTICLES
	OneParticles = true;
	OneSplash = true;

	//LEVEL INITIALIZATION
	body->setType(rp3d::STATIC);
}

BodyObj::BodyObj(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::CollisionShape *shapeData, rp3d::decimal mass)
{
	rp3d::Transform transform(initPosition, initOrientation);
	body = world->createRigidBody(transform);

	shape = shapeData;

	rp3d::Transform transform2 = rp3d::Transform::identity();
	proxyShape = body->addCollisionShape(shape, transform2, mass);

	//pointer to this - using in collision detection
	proxyShape->setUserData(this);

	//COLLISION FILTERING
	setCollisionCategory(FREEcat);

	//DRAWING
	model = NULL;
	modelll.setAllValues(1, 1, 1);

	//SAVE DELETING
	IsDeleted = false;
	gameWorld = world;
}

BodyObj::~BodyObj()
{
	delete shape;
	body->removeCollisionShape(proxyShape);
	gameWorld->destroyRigidBody(body);
}

void BodyObj::Draw(float m[16])
{
	rp3d::Transform transform = body->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);

	for (int i = 0; i < 16; i++)
		m[i] = matrix[i];
}

void BodyObj::Draw()
{
	rp3d::Transform transform = body->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);
	
	if (model)
	{
	glPushMatrix();
		glMultMatrixf(matrix);
		glScalef(modelll.x*0.6, modelll.y*0.6, modelll.z*0.6);
		//glScalef(0.5, 0.5, 0.5);
		glColor3f(0, 0.5, 0.5);
		model->Render();
		glColor3f(1, 1, 1);
		//glutWireCube(1);
	glPopMatrix();
	}
	else
	{
	glPushMatrix();
		glMultMatrixf(matrix);
		glScalef(modelll.x, modelll.y, modelll.z);
		glColor3f(0, 0.5, 0.5);
		glutSolidCube(1);
		glColor3f(0, 0, 0);
		glutWireCube(1);
	glPopMatrix();
	}
}

void BodyObj::setCollisionCategory(Category cat)
{
	proxyShape->setCollisionCategoryBits(cat);
	switch (cat)
	{
	case FREEcat:
		proxyShape->setCollideWithMaskBits(WATERcat | RUBBISHcat);
		break;
	case MAPcat:
		proxyShape->setCollideWithMaskBits(MAPcat | PLAYERcat | EFFECTcat | ARROWcat | FREEcat | WATERcat | RUBBISHcat);
		break;
	case ARROWcat:
		proxyShape->setCollideWithMaskBits(MAPcat | FREEcat | WATERcat | RUBBISHcat);
		break;
	case EFFECTcat:
		this->proxyShape->setCollideWithMaskBits( MAPcat );
		break;
	case RUBBISHcat:
		proxyShape->setCollideWithMaskBits(ALLcats);
		break;
	case WATERcat:
		proxyShape->setCollideWithMaskBits(MAPcat | FREEcat | ARROWcat);
		break;
	}
}
void BodyObj::setCollisionCategory(int cat)
{
	proxyShape->setCollisionCategoryBits(cat);
	switch (cat)
	{
	case FREEcat:
		proxyShape->setCollideWithMaskBits(WATERcat | RUBBISHcat);
		break;
	case MAPcat:
		proxyShape->setCollideWithMaskBits(MAPcat | PLAYERcat | EFFECTcat | ARROWcat | FREEcat | WATERcat | RUBBISHcat);
		break;
	case ARROWcat:
		proxyShape->setCollideWithMaskBits(MAPcat | FREEcat | WATERcat | RUBBISHcat);
		break;
	case EFFECTcat:
		this->proxyShape->setCollideWithMaskBits(MAPcat);
		break;
	case RUBBISHcat:
		proxyShape->setCollideWithMaskBits(ALLcats);
		break;
	case WATERcat:
		proxyShape->setCollideWithMaskBits(MAPcat | FREEcat | ARROWcat);
		break;
	}
}

int BodyObj::getCollsionCategory()
{
	return proxyShape->getCollisionCategoryBits();
}
void BodyObj::setMaterial(float bounce, float friction)
{
	rp3d::Material& material = body->getMaterial();
	material.setBounciness(rp3d::decimal(bounce));
	material.setFrictionCoefficient(rp3d::decimal(friction));
	body->setLinearDamping(0.8);
}

void BodyObj::update()
{

}

void BodyObj::setMaxTime(float max)
{
}

void BodyObj::makeCollision(BodyObj *CollideWith)
{
	int collideCategory = CollideWith->getProxyShape()->getCollisionCategoryBits();
	if ((collideCategory & RUBBISHcat) && !IsDeleted)
	{
		IsDeleted = true;
		//delete this;
		//game->GarbageCollector.push_back(this);
	}
}

void BodyObj::modelInit(char *mesh, char *texture) 
{
	model = new Model();
	model->Initialize(mesh, texture);
}

void BodyObj::modelInit(Model *model)
{
	this->model = model;
}

void BodyObj::kill()
{
	//body->setType(rp3d::KINEMATIC);
	
	rp3d::Transform t = body->getTransform();
	rp3d::Vector3 pos(0,-4,0);
	t.setPosition(pos);
	body->setTransform(t);

	rp3d::Vector3 stop(0, 0, 0);
	body->setLinearVelocity(stop);
	body->setAngularVelocity(stop);
	setCollisionCategory(EFFECTcat);
	int i = 0;
}

void BodyObj::init(rp3d::Vector3 position, rp3d::Quaternion orientation)
{
	rp3d::Transform t(position, orientation);
	body->setTransform(t);
	OneSplash = true;
	OneParticles = true;
}

void BodyObj::setGravityEnable(bool arg)
{
	body->enableGravity(arg);
}

rp3d::RigidBody * BodyObj::getBody()
{
	return body;
}

rp3d::ProxyShape * BodyObj::getProxyShape()
{
	return proxyShape;
}

bool BodyObj::getIsDeleted()
{
	return IsDeleted;
}

void BodyObj::setIsDeleted(bool arg)
{
	IsDeleted = arg;
}

bool BodyObj::getOneParticles()
{
	return OneParticles;
}

bool BodyObj::getOneSplash()
{
	return OneSplash;
}

void BodyObj::setOneParticles(bool arg)
{
	OneParticles = arg;
}

void BodyObj::setOneSplash(bool arg)
{
	OneSplash = arg;
}

void BodyObj::setType(rp3d::BodyType type)
{
	body->setType(type);
}

rp3d::BodyType BodyObj::getType()
{
	return body->getType();
}

rp3d::CollisionShape * BodyObj::getShape()
{
	return shape;
}

void BodyObj::getColor(rp3d::Vector3 color)
{
}