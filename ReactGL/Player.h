#pragma once
#include "reactphysics3d.h"
#include "Camera.h"
#include "BodyObj.h"
#include "Arrow.h"
//#include "Game.h"

class Player
{
public:
	Player(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData);
	~Player();

	//CONTROLING
	void serve_controls();
	void set_control(int control);
	void unset_control(int control);
	void move(Direction dir);
	void make_jump();
	void move2(Direction dir);
	void rotate2(Direction dir, float angle);
	void look_vertical(float angle);
	Arrow * test_shoot();
	void Draw(float m[16]);

	//void set();
	Camera cam;

	void unrotate();
	void stop();
	void update();
//private:
	//ROTATION
	float Yaw;
	float Pitch;
	float Roll;

	//CONTROL
	bool w;
	bool s;
	bool a;
	bool d;
	bool jump;
	float jump_border;
	//PHYSICS
	rp3d::RigidBody *body;
	rp3d::ProxyShape *proxyShape;
	rp3d::BoxShape *shape;

	rp3d::DynamicsWorld *world;
};


