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
	void modelInit(Model *model);
	void Draw(float m[16]);	//return translate matrix
	virtual void Draw();			//OpenGL drawing
	void setMaterial(float bounciness, float friction);
	virtual void setGravityEnable(bool arg);
	virtual void update();
	virtual void makeCollision(BodyObj *CollideWith);
	virtual void init(rp3d::Vector3 position, rp3d::Quaternion orientation);
	virtual void kill();

	rp3d::RigidBody * getBody();
	rp3d::ProxyShape * getProxyShape();
	bool getIsDeleted();
	void setIsDeleted(bool arg);
	bool getOneParticles();
	bool getOneSplash();
	void setOneParticles(bool arg);
	void setOneSplash(bool arg);
	void setCollisionCategory(Category cat);
	void setCollisionCategory(int cat);
	int getCollsionCategory();

	void setType(rp3d::BodyType type);
	rp3d::BodyType getType();
	rp3d::CollisionShape * getShape();
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
	bool OneSplash;
};