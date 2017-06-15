#include "stdafx.h"
#include "Level.h"
#include "BodyObj.h"

Level::Level()
{
}


Level::~Level()
{
}

void Level::Update(float plusTime)
{
}

void Level::getData(std::vector<BodyObj *> *toServe)
{
}

void Level::getModels(Model *rock1, Model *rock2, Model *aimbonus, Model *arrow)
{
}

void Level::Initialize()
{
}

void Level::Prepare()
{
}

void Level::mapUpdate()
{
}

BodyObj * Level::Make(char *type, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation)
{
	return NULL;
}

void Level::Kill(BodyObj *obj, char *type)
{
}

void Level::Draw()
{
}