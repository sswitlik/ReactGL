#include "stdafx.h"
#include "Arrow.h"
#include "GL/freeglut.h"
#include "maths.h"

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
	
	//COLLISION FILTERING
	BodyObj::setCollisionCategory(ARROWcat);
	//proxyShape->setCollisionCategoryBits(ARROWcat);
	//proxyShape->setCollideWithMaskBits(MAPcat);

	//COLOR
	color.setAllValues(1, 0.5, 0);

	time = 0;
	collided = false;
	drilled = false;
	antivibr = false;
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
		glColor3f(color.x, color.y, color.z);
		glScalef(0.02, 1, 0.02);
		glutSolidCube(1);
		glColor3f(0.5,0.5, 0.5);
		glutWireCube(1);
	glPopMatrix();
}

//void Arrow::update()
//{
//	if (time < 25 && !collided)
//	{
//		time++;		//allow collision after 25
//		return;
//	}
//	
//		rp3d::Vector3 stop(0, 0, 0);
//		body->setAngularVelocity(stop);
//		body->setLinearVelocity(stop);
//		body->enableGravity(false);
//
//}

void Arrow::update()
{
	// bylo potrzebne przed collision filtering
	//if (time < 25 && !collided) 
	//{
	//	time++;		//allow collision after 25
	//	return;
	//}

	if (drilled)	
	{
		rp3d::Vector3 stop(0, 0, 0);
		body->setAngularVelocity(stop);
		body->setLinearVelocity(stop);
		body->enableGravity(false);
	}
	else
	if (collided)
	{
		;
	}
	else
	{
		rp3d::Transform t = body->getTransform();
		rp3d::Vector3 speed = body->getLinearVelocity();
		rp3d::Quaternion q = t.getOrientation();
	
		//printf("%f %f %f\n", speed.x, speed.y, speed.z);

		double x;
		double y;
		double z;

		
		QuaternionO2IToEulerAngles(&x, &y, &z, q);

		if (antivibr)	//if falling
		{
			x += PI;
			z += PI;
		}

		//printf("%f %f %f\n", x, y, z);

		//y from speed
		speed.normalize();
		y = acos(speed.y);

		rp3d::Vector3 angsp = body->getAngularVelocity();	//angle speed

		rp3d::Quaternion newq(x, y, z);

		newq.setAllValues(newq.y, newq.x, newq.z, newq.w);

		t.setOrientation(newq);
		body->setTransform(t);		//obrot wg predkowsci

		if (speed.y < 0)
			antivibr = true;
	}
}

void Arrow::makeCollision(int collideWith)
{
	if (collideWith & MAPcat)
	{
		if (!collided)
			drilled = true;
	}
	else
		collided = true;
}