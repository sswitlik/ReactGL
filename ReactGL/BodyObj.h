#pragma once
#include "stdafx.h"
#include "reactphysics3d.h"
#include "maths.h"

class BodyObj
{
public:
	BodyObj();
	BodyObj(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass);
	~BodyObj();
	void InitializeModel(std::string mesh, std::string texture);
	void Draw(float m[16]);	//return translate matrix
	virtual void Draw();			//OpenGL drawing
	void setType(int type);
	void setMaterial(float, float);
	void setCollisionCategory(Category cat);
	virtual void update();
	virtual void makeCollision(int collideWith);
	
	
	//test 0.2
	//float angle;
	
	//protected:
	rp3d::RigidBody *body;
	rp3d::ProxyShape *proxyShape;
	rp3d::CollisionShape *shape;
	//rp3d::Material& material;

	rp3d::Vector3 modelll;
	//rotation angles
	float x_yaw;
	float y_pitch;
	float z_roll;
};