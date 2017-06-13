#pragma once
#include "stdafx.h"
#include "reactphysics3d.h"
#include "maths.h"
#include "Model.h"

class BodyObj
{
public:
	BodyObj();
	BodyObj(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass);
	BodyObj(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::CollisionShape *shapeData, rp3d::decimal mass);

	virtual ~BodyObj();
	void modelInit(char *mesh, char *texture);
	void Draw(float m[16]);	//return translate matrix
	virtual void Draw();			//OpenGL drawing
	void setType(rp3d::BodyType type);
	void setMaterial(float bounciness, float friction);
	void setCollisionCategory(Category cat);
	virtual void setGravityEnable(bool arg);
	virtual void update();
	virtual void makeCollision(BodyObj *CollideWith);
	
	rp3d::RigidBody * getBody();
	rp3d::ProxyShape * getProxyShape();
	bool getIsDeleted();
	bool getOneParticles();
	void setOneParticles(bool arg);

protected:
	rp3d::RigidBody *body;
	rp3d::ProxyShape *proxyShape;
	rp3d::CollisionShape *shape;
	
	//DRAWING
	Model * model;
	rp3d::Vector3 modelll;
	
	//to avoid multiple deleteing the same body
	bool IsDeleted;
	rp3d::DynamicsWorld *gameWorld;

	bool OneParticles;
};