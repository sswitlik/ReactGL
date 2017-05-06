#include "stdafx.h"
#include "Camera.h"
#include <cmath>
#include <GL\freeglut.h>
#define PI180 0.01745329

Camera::Camera()
{
	x = 0;
	y = 3.0;
	z = 5.0;

	lx = 0;
	ly = 0;
	lz = -1;

	angle_h = 0;
	angle_v = 0;

	//orient.setAllValues(lx, ly, lz, 1);
}


Camera::~Camera()
{
}

void Camera::move(Direction dir)
{
	float dz = 0;
	float dx = 0;

	float speed = 0.1;
	switch (dir)
	{
	case UP:
		y += speed;
		break;
	case DOWN:
		y -= speed;
		break;
	case RIGHT:
		dx = speed;
		break;
	case LEFT:
		dx = -speed;
		break;
	case FORWARD:
		dz = speed;
		break;
	case BACK:
		dz = -speed;
		break;
	}

	float proportion = 1 / sqrt(pow(lx, 2) + pow(lz, 2));

	//przod i tyl
	z += lz*dz*proportion;
	x += lx*dz*proportion;

	//lewo i prawo
	float beta = atan2(lz, lx);
	z += cos(-beta) * dx;
	x += sin(-beta) * dx;
}

void Camera::rotate(Direction dir, float angle)
{
	angle = abs(angle);
	switch (dir)
	{
	case UP:
		if (ly < 0.999)
			angle_v = 0.01 * angle;
		else
			angle_v = 0;
		break;
	case DOWN:
		if (ly > -0.999)
			angle_v = -0.01 * angle;
		else
			angle_v = 0;
		break;
	case RIGHT:
		angle_h = 0.01 * angle;
		break;
	case LEFT:
		angle_h = -0.01 * angle;
		break;
	case FORWARD:
		break;
	case BACK:
		break;
	}

	float dl = sqrt(pow(lx, 2) + pow(lz, 2));
	float alfa = atan2(ly, dl);
	ly = sin(alfa + angle_v);
	dl = cos(alfa + angle_v);


	//lz = cos(alfa + angle_v);

	float beta = atan2(lz, lx);
	lx = cos(beta + angle_h)*dl;
	lz = sin(beta + angle_h)*dl;
	angle_h = 0;
	angle_v = 0;
}

void Camera::set()
{
	//3per
	//gluLookAt(x-2*lx, y-ly, z-2*lz, x+lx, y+ly, z+lz, 0, 1, 0);
	//1per
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0, 1, 0);
}

void Camera::sync(rp3d::Vector3 pos, float yaw, float roll, float pitch)
{
	x = pos.x;
	y = pos.y;
	z = pos.z;

	float tx = sin(yaw),
		//ty = ly,
		tz = cos(yaw);

	float dlxz = sqrt(pow(tx, 2) + pow(tz, 2));
	float dly = sqrt(1 - pow(ly, 2));

	float prop = dly / dlxz;

	tx *= prop;
	tz *= prop;

	lx = tx;
	//ly = ty;
	lz = tz;
}