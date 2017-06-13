#pragma once
class Level
{
public:
	Level();
	~Level();

	virtual void Update(float plusTime);
	virtual void getData(std::vector<BodyObj *> *toServe);
};

