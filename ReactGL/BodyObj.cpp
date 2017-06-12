#include "stdafx.h"
#include "GL/freeglut.h"
#include "BodyObj.h"
#include <cmath>
#include "maths.h"

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

	//pointer to this - using in collision detection
	proxyShape->setUserData(this);

	//COLLISION FILTERING
	proxyShape->setCollisionCategoryBits(FREEcat);
	proxyShape->setCollideWithMaskBits(PLAYERcat | ARROWcat | MAPcat | FREEcat);

	//DRAWING
	model = NULL;
	modelll.setAllValues(shapeData.x*2, shapeData.y * 2, shapeData.z * 2);
}


BodyObj::~BodyObj()
{
	delete model;
	delete body;
	delete shape;
	delete proxyShape;
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
		glScalef(modelll.x, modelll.y, modelll.z);
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
	case MAPcat:
		proxyShape->setCollideWithMaskBits(MAPcat | PLAYERcat | EFFECTcat | ARROWcat | FREEcat);
		break;
	case ARROWcat:
		proxyShape->setCollideWithMaskBits(MAPcat | ARROWcat | FREEcat);
		break;

	}
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

void BodyObj::update()
{

}

void BodyObj::makeCollision(int collideWith)
{

}

void BodyObj::modelInit(char *mesh, char *texture) 
{
	model = new Model();
	model->Initialize(mesh, texture);
	int i = 0;
}