#include "stdafx.h"
#include "Events.h"

extern Game *game;

Events::Events()
{
}


Events::~Events()
{
}

void Events::ImprovePlayerAccuracy(int plusAccuracy)
{
	Player *player = game->getplayer();

	player->ImproveAccuracy(plusAccuracy);
}

void Events::addPoints(int point)
{
	Player *player = game->getplayer();

	player->addPoints(point);
}



