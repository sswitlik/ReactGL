#pragma once
#include "reactphysics3d.h"
#include <vector>
#include "BodyObj.h"
#include "Player.h"

class Game
{
public:
	Game();
	~Game();
	void Draw_floor(float m[16]);
	void Draw_1(float m[16]);
	void plus();
	void Update();

	//PLAYER
	Player * getplayer();
	void testshoot();

	//nowe szesciany
	int quantity;
	std::vector <BodyObj *> objs;
	//test 0.2
	void rd();
	//private:
	rp3d::DynamicsWorld *World;

	BodyObj *floor;
	BodyObj *obj1;
	void ExampleTestInit();
	////TIME
	unsigned int previousFrameTime;
	unsigned int accumulator;

	//PLAYER
	Player *player;

};