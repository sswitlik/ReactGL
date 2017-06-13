#pragma once
#include "stdafx.h"
#include "Model.h"
//#include "reactphysics3d.h"
#include <vector>
#include "BodyObj.h"
#include "Player.h"
#include "GameEventListener.h"
#include "Level.h"
#include "Events.h"

class Game
{
private:
	//WORLD
	rp3d::DynamicsWorld *World;
	
	////TIME
	unsigned int previousFrameTime;
	unsigned int accumulator;

	//PLAYER
	Player *player;

	//CONTACT
	GameEventListener *listener;

	//LEVEL
	Level *level;

	//EVENTS
	Events events;

public:
	Game();
	~Game();

	void plus();
	void Update();
	void ExampleTestInit();		//init map
	void InitLevel();

	//PUBLIC GARBAGE COLLECTOR FOR ALL THE GAME
	std::vector<BodyObj *> GarbageCollector;
	
	//PLAYER
	Player * getplayer();
	void testshoot();

	//OBIEKTY GRY
	std::vector <BodyObj *> map;
	std::vector <BodyObj *> objs;
	std::vector <BodyObj *> effects;

	Events getEvents();

	rp3d::DynamicsWorld * getWorld();
};