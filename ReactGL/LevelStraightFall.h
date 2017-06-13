#pragma once
class LevelStraightFall : public Level
{
protected:
	float time;
	int toFall;
	std::vector<BodyObj *> *bricks;
public:
	LevelStraightFall();
	LevelStraightFall(std::vector<BodyObj *> *toServe);
	~LevelStraightFall();
	void Update(float plusTime);
	void getData(std::vector<BodyObj *> *toServe);
};

