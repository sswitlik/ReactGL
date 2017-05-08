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
	proxyShape->setCollisionCategoryBits(ARROWcat);
	proxyShape->setCollideWithMaskBits(MAPcat);


	//COLOR
	color.setAllValues(1, 0.5, 0);

	time = 0;
	collided = false;
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

//void QuaternionO2IToEulerAngles(float *Yaw, float *Pitch, float *Roll, const rp3d::Quaternion &q)
//{
//	float sp = -2.0f * (q.y*q.z - q.w*q.x);
//
//	if (sp == 1.0f)
//	{
//		*Pitch = PI_2 * sp;
//		*Yaw = atan2f(-q.x*q.z + q.w*q.y, 0.5f - q.y*q.y - q.z*q.z);
//		*Roll = 0.0f;
//	}
//	else
//	{
//		*Pitch = asinf(sp);
//		*Yaw = atan2f(q.x*q.z + q.w*q.y, 0.5f - q.x*q.x - q.y*q.y);
//		*Roll = atan2f(q.x*q.y + q.w*q.z, 0.5f - q.x*q.x - q.z*q.z);
//	}
//}

void Arrow::update()
{
	if (time < 25 && !collided) 
	{
		time++;
		return;
	}

	if (collided)	
	{
		rp3d::Vector3 stop(0, 0, 0);
		body->setAngularVelocity(stop);
		body->setLinearVelocity(stop);
		body->enableGravity(false);
	}
	else
	{
		//static bool antivibr = false;

		rp3d::Transform t = body->getTransform();
		rp3d::Vector3 speed = body->getLinearVelocity();
		rp3d::Quaternion q = t.getOrientation();
		//rp3d::Matrix3x3 m = q.getMatrix();

		printf("%f %f %f\n", speed.x, speed.y, speed.z);

		double x;
		double y;
		double z;

		
		QuaternionO2IToEulerAngles(&x, &y, &z, q);

		if (antivibr)
		{
			x += PI;
			z += PI;
		}

		printf("%f %f %f\n", x, y, z);

		//y from speed
		speed.normalize();
		y = acos(speed.y);

		rp3d::Vector3 angsp = body->getAngularVelocity();

		//printf("%f %f %f\n", angsp.x, angsp.y, angsp.z);

		rp3d::Quaternion newq(x, y, z);

		printf("%f %f %f\n", x, y, z);
		printf("%f %f %f %f\n\n", newq.y, newq.x, newq.z, newq.w);
		newq.setAllValues(newq.y, newq.x, newq.z, newq.w);

		t.setOrientation(newq);
		body->setTransform(t);

		if (speed.y < 0)
			antivibr = true;
	}
}