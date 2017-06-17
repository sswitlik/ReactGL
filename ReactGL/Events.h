#pragma once
#include "stdafx.h"
#include "Game.h"

class Events
{
	int id;
public:
	Events();
	~Events();

	void ImprovePlayerAccuracy(int plusAccuracy);
	void addPoints(int point);
};

