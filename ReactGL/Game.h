#pragma once
#include "stdafx.h"
#include "Model.h"
//#include "reactphysics3d.h"
#include <vector>
#include "BodyObj.h"
#include "Player.h"
#include "GameEventListener.h"

class Game
{
public:
	Game();
	~Game();
	void Draw_floor();
	void Draw_1(float m[16]);
	void plus();
	void Update();

	//PLAYER
	Player * getplayer();
	void testshoot();
	void testarrowrotate();
	//OBIEKTY GRY
	int quantity;
	std::vector <BodyObj *> map;
	std::vector <BodyObj *> objs;
	std::vector <BodyObj *> effects;
	//MODELTEST
	Model *mod;
	Model * getMod();


	//test 0.2
	void rd();
	//private:
	rp3d::DynamicsWorld *World;

	//BodyObj *floor;
	//BodyObj *obj1;
	void ExampleTestInit();
	////TIME
	unsigned int previousFrameTime;
	unsigned int accumulator;

	//PLAYER
	Player *player;

	//CONTACT
	GameEventListener *listener;
};