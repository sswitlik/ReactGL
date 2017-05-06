#include "stdafx.h"
#include "Arrow.h"
#include "GL/freeglut.h"

Arrow::Arrow()
{

}

Arrow::Arrow(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, float radius, float length, rp3d::decimal mass)
{
	rp3d::Transform transform(initPosition, initOrientation);
	body = world->createRigidBody(transform);

	shape = new rp3d::CapsuleShape(radius, length);

	rp3d::Transform transform2 = rp3d::Transform::identity();
	proxyShape = body->addCollisionShape(shape, transform2, mass);

	proxyShape->setUserData(this);
}


Arrow::~Arrow()
{
}

void Arrow::Draw()
{
	rp3d::Transform transform = body->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);

	glPushMatrix();
		glMultMatrixf(matrix);
		glColor3f(1, 0.5, 0);
		glScalef(0.1, 1, 0.1);
		glutSolidCube(1);
		glColor3f(0, 0, 0);
		glutWireCube(1);
	glPopMatrix();
}