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





