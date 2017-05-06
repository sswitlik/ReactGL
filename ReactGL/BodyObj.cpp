#include "stdafx.h"
#include "GL/freeglut.h"
#include "BodyObj.h"
#include <cmath>
#define RAD  0.01745329

BodyObj::BodyObj()
{

}

BodyObj::BodyObj(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass)
{
	rp3d::Transform transform(initPosition, initOrientation);
	body = world->createRigidBody(transform);

	shape = new rp3d::BoxShape(shapeData, 0.1);

	rp3d::Transform transform2 = rp3d::Transform::identity();
	proxyShape = body->addCollisionShape(shape, transform2, mass);

	//test 0.2
	angle = 0;

	proxyShape->setUserData(this);
}


BodyObj::~BodyObj()
{
	//delete
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

	glPushMatrix();
	glMultMatrixf(matrix);
	glColor3f(0, 0.5, 0.5);
	glutSolidCube(1);
	glColor3f(0, 0, 0);
	glutWireCube(1);
	glPopMatrix();
}

void BodyObj::setType(int type)
{
	switch (type)
	{
	case 1:	body->setType(rp3d::STATIC); break;
	case 2:	body->setType(rp3d::DYNAMIC); break;
	case 3: body->setType(rp3d::KINEMATIC); break;
	}
}

void BodyObj::setMaterial(float bounce, float friction)
{
	rp3d::Material& material = body->getMaterial();
	material.setBounciness(rp3d::decimal(bounce));
	material.setFrictionCoefficient(rp3d::decimal(friction));
	body->setLinearDamping(0.8);
}

void BodyObj::rd()	//test 0.2
{
	angle += 10;
	rp3d::Transform trans = body->getTransform();
	rp3d::Quaternion orient = trans.getOrientation();

	rp3d::Matrix3x3 m = orient.getMatrix();

	m[0][0] = cos(angle * RAD);
	m[0][2] = sin(angle * RAD);
	m[2][0] = -sin(angle * RAD);
	m[2][2] = cos(angle * RAD);

	rp3d::Quaternion neworient(m);

	trans.setOrientation(neworient);

	body->setTransform(trans);
	// Apply a force to the center of the body 
	//body->applyForceToCenterOfMass(force);
	//body->applyTorque(force);
	//body->setAngularVelocity(force);
}
//
//void BodyObj::obrot()	//test 0.2
//{
//	rp3d::Vector3 force(0.0, 5.0, 0.0);
//	body->setAngularVelocity(force);
//}

void BodyObj::testset()	//test 0.2
{
	body->enableGravity(false);

	body->setAngularDamping(0.99);
}
