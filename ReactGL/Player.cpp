#include "stdafx.h"
#include "Player.h"
#include <GL\freeglut.h>
#include "Game.h"
#include "Arrow.h"
#include "maths.h"

Player::Player(rp3d::DynamicsWorld *World, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData)
{
	world = World;

	//PHYSICS
	rp3d::Transform transform(initPosition, initOrientation);
	body = world->createRigidBody(transform);
	shape = new rp3d::BoxShape(shapeData, 0.1);

	rp3d::Transform transform2 = rp3d::Transform::identity();
	rp3d::decimal mass = rp3d::decimal(4.0);
	proxyShape = body->addCollisionShape(shape, transform2, mass);

	//body->enableGravity(false);

	rp3d::Material& material = body->getMaterial();
	material.setBounciness(rp3d::decimal(0));
	material.setFrictionCoefficient(rp3d::decimal(1));

	//COLLISION FILTERING
	proxyShape->setCollisionCategoryBits(PLAYERcat);
	proxyShape->setCollideWithMaskBits(MAPcat);

	//ROTATION
	Yaw = 0; Roll = 0; Pitch = 0;

	//CAMERA
	cam.sync(initPosition, Yaw, Roll, Pitch);

	//CONTROL
	w = false;
	s = false;
	a = false;
	d = false;
	jump = false;
	jump_border = 0.1;
	shoot = false;
	nextShotPower = 0;

	//STATS
	accuracy = 1;
	shootSpeed = 500;
}

Player::~Player()
{
	delete shape;
}

void Player::update()
{
	rp3d::Transform trans = body->getTransform();

	cam.sync(trans.getPosition(), Yaw, Roll, Pitch);
	//cam.set();
	unrotate();
	serve_controls();
}

void Player::set_control(int control)
{
	switch (control)
	{
	case 0:
		w = 1;
		break;
	case 1:
		s = 1;
		break;
	case 2:
		a = 1;
		break;
	case 3:
		d = 1;
		break;
	case 4:
		jump = 1;
		break;
	case 5:
		shoot = 1;
		break;
	}
}

void Player::unset_control(int control)
{
	switch (control)
	{
	case 0:
		w = 0;
		break;
	case 1:
		s = 0;
		break;
	case 2:
		a = 0;
		break;
	case 3:
		d = 0;
		break;
	case 4:
		jump = 0;
		break;
	case 5:
		shoot = 0;
		break;
	}
}

void Player::unrotate()
{
	rp3d::Vector3 angv(0, 0, 0);
	body->setAngularVelocity(angv);

	rp3d::Transform trans = body->getTransform();
	rp3d::Quaternion orient = trans.getOrientation();

	QuaternionO2IToEulerAngles(&Yaw, &Pitch, &Roll, orient);

	if (Pitch > 0.1 || Roll > 0.1 || Pitch < -0.1 || Roll < -0.1)
	{
		rp3d::Quaternion o(0, Yaw, 0);
		trans.setOrientation(o);
		body->setTransform(trans);
	}
}

void Player::stop()
{
	rp3d::Material& material = body->getMaterial();
	material.setFrictionCoefficient(rp3d::decimal(1));

	rp3d::Vector3 vel = body->getLinearVelocity();
	rp3d::Vector3 speed(0, vel.y, 0);
	body->setLinearVelocity(speed);
}

void Player::move(Direction dir)
{
	rp3d::Material& material = body->getMaterial();
	material.setFrictionCoefficient(rp3d::decimal(0));


	rp3d::Transform trans = body->getTransform();
	rp3d::Quaternion orient = trans.getOrientation();
	//float Yaw, Pitch, Roll;
	QuaternionO2IToEulerAngles(&Yaw, &Pitch, &Roll, orient);

	rp3d::Vector3 vel = body->getLinearVelocity();

	if (vel.y > 0.1 || vel.y < -0.1)		//can't move if fall
		return;

	float x = 0;
	float y = 0;
	float z = 0;

	float speed = 2.5;

	switch (dir)
	{
	case UP:
		//y += speed;
		break;
	case DOWN:
		//y -= speed;
		break;
	case RIGHT:
		x = -sin(Yaw + PI_2)*speed;
		z = -cos(Yaw + PI_2)*speed;
		break;
	case LEFT:
		x = sin(Yaw + PI_2)*speed;
		z = cos(Yaw + PI_2)*speed;
		break;
	case FORWARD:
		x = sin(Yaw)*speed;
		z = cos(Yaw)*speed;
		break;
	case BACK:
		x = -sin(Yaw)*speed;
		z = -cos(Yaw)*speed;
		break;
	}

	float new_x = x + vel.x;
	float new_z = z + vel.z;
	float new_y = y + vel.y;

	rp3d::Vector2 max_speed(new_x, new_z);
	if (max_speed.length() > 5)
	{
		max_speed.normalize();
		max_speed *= 5;

	}
	rp3d::Vector3 new_vel(max_speed.x, new_y, max_speed.y);
	body->setLinearVelocity(new_vel);
}

void Player::move2(Direction dir)
{
	//rp3d::Transform trans = body->getTransform();
	rp3d::Vector3 pos = body->getLinearVelocity();

	float x = 0;
	float y = 0;
	float z = 0;

	switch (dir)
	{
	case UP:
		//y += speed;
		break;
	case DOWN:
		//y -= speed;
		break;
	case RIGHT:
		z += 4;
		break;
	case LEFT:
		z -= 4;
		break;
	case FORWARD:
		x += 4;
		break;
	case BACK:
		x -= 4;
		break;
	}

	//trans.setPosition(pos);

	pos.setAllValues(x, pos.y, z);
	//trans.setPosition(pos);

	//body->setTransform(trans);
	body->setLinearVelocity(pos);
	//pos.setAllValues(x, y, z);
	//rp3d::Quaternion o = rp3d::Quaternion::identity();
	//rp3d::Transform t(pos, o);
	//body->applyForceToCenterOfMass(pos);
	////body->setLinearVelocity(pos);
}

void Player::rotate2(Direction dir, float angle)
{
	rp3d::Transform trans = body->getTransform();
	rp3d::Quaternion orient = trans.getOrientation();

	switch (dir)
	{
	case UP:
		break;
	case DOWN:
		break;
	case RIGHT:
		Yaw -= angle;
		break;
	case LEFT:
		Yaw += angle;
		break;
	case FORWARD:
		break;
	case BACK:
		break;
	}

	rp3d::Matrix3x3 m = orient.getMatrix();
	m[0][0] = cos(Yaw);
	m[0][2] = sin(Yaw);
	m[2][0] = -sin(Yaw);
	m[2][2] = cos(Yaw);

	rp3d::Quaternion neworient(m);
	trans.setOrientation(neworient);
	body->setTransform(trans);
}

void Player::serve_controls()
{
	//friction = 0
	rp3d::Material& material = body->getMaterial();
	material.setFrictionCoefficient(rp3d::decimal(0));

	//get velocity
	rp3d::Vector3 vel = body->getLinearVelocity();

	#pragma region JUMP
	{
		if (vel.y < -0.1)
			jump_border = 0.1;

		if (jump && abs(vel.y) < jump_border)
		{
			jump_border = 0.01;
			vel.y += 4;
			rp3d::Vector3 force(vel.x, vel.y, vel.z);
			body->setLinearVelocity(force);
		}
	}

	#pragma region MOVE
	{
		rp3d::Transform trans = body->getTransform();
		rp3d::Quaternion orient = trans.getOrientation();

		float x = 0;
		float y = 0;
		float z = 0;

		float speed = 5;

		if (w ^ s)	//XOR
		{
			if (w)
			{
				x = sin(Yaw)*speed;
				z = cos(Yaw)*speed;
			}
			if (s)
			{
				x = -sin(Yaw)*speed;
				z = -cos(Yaw)*speed;
			}
		}

		if (a ^ d)	//XOR
		{
			if (a)
			{
				x += sin(Yaw + PI_2)*speed;
				z += cos(Yaw + PI_2)*speed;
			}
			if (d)
			{
				x += -sin(Yaw + PI_2)*speed;
				z += -cos(Yaw + PI_2)*speed;
			}
		}

		float new_x = x;
		float new_z = z;
		float new_y = y + vel.y;

		rp3d::Vector2 max_speed(new_x, new_z);
		if (max_speed.length() > 5)
		{
			max_speed.normalize();
			max_speed *= 5;

		}
		rp3d::Vector3 new_vel(max_speed.x, new_y, max_speed.y);
		body->setLinearVelocity(new_vel);
	}
	
	#pragma region SHOOT
	{
		if (shoot)
		{
			if (nextShotPower < 20000)
				nextShotPower += shootSpeed;
		}
	}

}

void Player::Draw()
{
	rp3d::Transform transform = body->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);

	glPushMatrix();
		glMultMatrixf(matrix);
		glScalef(0.25, 1, 0.25);
		glColor3f(0, 0, 0.5);
		//glutSolidCube(1);
		glColor3f(1, 1, 1);
		glutWireCube(1);
	glPopMatrix();
}

void Player::Drawsight(int xcentr, int ycentr)
{
	float r = accuracy*2;
	glColor4f(0.8f, 0.8f, 0.8, 0.3);
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(xcentr, ycentr);
		int Np = 48;
		for (int i = 0; i <= Np; ++i) {
			float x = sin(2.0f * PI * ((float)i / Np));
			float y = cos(2.0f * PI * ((float)i / Np));
			glVertex2f(xcentr+x*r, ycentr+y*r);

		}
	glEnd();

	glColor4f(0.8f, 0.8f, 0.8f, 0.8);
	glBegin(GL_LINES);
		glVertex2f(xcentr - 50, ycentr);
		glVertex2f(xcentr + 50, ycentr);
		glVertex2f(xcentr, ycentr - 50);
		glVertex2f(xcentr, ycentr + 50);
	glEnd();
}

void Player::DrawShootPower(int xcentr, int ysize)
{
	glColor4f(0.8f, 0.8f, 0.8f, 0.8);
	glBegin(GL_QUAD_STRIP);
		glVertex2f(xcentr - 100, ysize - 50);
		glVertex2f(xcentr - 100, ysize - 60);

		glVertex2f(xcentr - 100 + nextShotPower/100, ysize - 50);
		glVertex2f(xcentr - 100 + nextShotPower/100, ysize - 60);
		
		//for (int i = 0; i < nextShotPower / 100; i+=10)
		//{
		//	glVertex2f(xcentr - 100 + i, ysize - 50);
		//	glVertex2f(xcentr - 100 + i, ysize - 60);
		//	
		//}
	glEnd();
}

void Player::look_vertical(float angle)
{
	if (angle > 0)
		cam.ly += 0.5;
	//cam.rotate(UP, angle);
	else
		cam.ly -= 0.5;
	//cam.rotate(DOWN, angle);
}

void Player::make_jump()
{
	rp3d::Vector3 vel = body->getLinearVelocity();

	if (vel.y > 0.01 || vel.y < -0.01)		//can't move if fall
		return;

	rp3d::Vector3 force(0, 1000, 0);
	body->applyForceToCenterOfMass(force);
}

Arrow * Player::test_shoot()
{
	rp3d::Transform t = body->getTransform();
	rp3d::Vector3 initPosition = t.getPosition();
	initPosition.setAllValues(initPosition.x, initPosition.y - 0.5, initPosition.z);

	rp3d::Vector3 newposition(initPosition.x, initPosition.y + 0.5, initPosition.z);

	Camera shootDir(cam);
	float randval = (float)((rand() % (2000 * (int)accuracy)) / 1000) - accuracy;
	shootDir.rotate(DOWN, randval);
	randval = (float)((rand() % (2000 * (int)accuracy)) / 1000) - accuracy;
	shootDir.rotate(UP, randval);
	randval = (float)((rand() % (2000 * (int)accuracy)) / 1000) - accuracy;
	shootDir.rotate(LEFT, randval);
	randval = (float)((rand() % (2000 * (int)accuracy)) / 1000) - accuracy;
	shootDir.rotate(RIGHT, randval);
	float angleX = shootDir.lx,
		  angleY = shootDir.ly,
		  angleZ = shootDir.lz;

	float roll, yaw, pitch;
	roll = acos(angleY);
	yaw = atan2(angleX,angleZ);
	pitch = Pitch;
	rp3d::Quaternion initOrientation(roll,yaw,pitch);


	//Capsule
	const rp3d::Vector2 shapeData(0.1, 0.5);

	Arrow *bullet = new Arrow(world, newposition, initOrientation, 0.05, 0.5, 10);

	rp3d::Vector3 CenterMass(0, 0.2, 0);;
	bullet->body->setCenterOfMassLocal(CenterMass);

	rp3d::Material& material = bullet->body->getMaterial();
	//material.setFrictionCoefficient(0.4);
	bullet->body->setAngularDamping(0.6);
	material.setRollingResistance(0.1);

	float power = nextShotPower;
	rp3d::Vector3 force(angleX * power, angleY * power, angleZ * power);
	bullet->body->applyForceToCenterOfMass(force);

	bullet->BodyObj::modelInit("Models/arrow.obj", "Models/Rock.bmp");
	//after shoot power to zero
	nextShotPower = 0;

	return bullet;
}

