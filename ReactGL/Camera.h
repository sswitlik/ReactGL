#pragma once
#include "reactphysics3d.h"

enum Direction
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
	FORWARD,
	BACK
};

class Camera
{
public:
	Camera();
	~Camera();

	void move(Direction dir);
	void rotate(Direction dir, float angle);
	void set();
	rp3d::Vector3 getDirectionVector();
	void sync(rp3d::Vector3 pos, float yaw, float roll, float pitch);
	//private:
	//rp3d::Quaternion orient;	//test 0.4
	float x;	//position
	float y;
	float z;

	float lx;	//look
	float ly;
	float lz;

	float angle_v;
	float angle_h;
};
