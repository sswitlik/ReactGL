#pragma once
#include "stdafx.h"
#include "reactphysics3d.h"
#include "Camera.h"
#include "BodyObj.h"
#include "Arrow.h"
//#include "Game.h"
#include "Skydome.h"
#include "Level.h"

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
	BodyObj * test_shoot();

	//DRAWING
	void Draw();
	void Drawsight(int xcentr, int ycentr);
	void DrawShootPower(int xcentr, int ysize);
	void DrawPoints();

	//ATTRIBUTE SETTING
	void ImproveAccuracy(int plusAccuracy);

	//LEVEL
	void loadLevel(Level *level);
	void addPoints(int point);

	//void set();
	Camera cam;

	void unrotate();
	void stop();
	void update();
private:
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
	bool shoot;
	int nextShotPower;

	//PHYSICS
	rp3d::RigidBody *body;
	rp3d::ProxyShape *proxyShape;
	rp3d::CollisionShape *shape;

	rp3d::DynamicsWorld *world;

	//STATS
	float accuracy;
	int shootSpeed;

	//SKYDOME
	CSkydome *sky;

	//LEVEL
	Level *level;
	int points;
};


