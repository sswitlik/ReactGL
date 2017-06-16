#include "stdafx.h"
#include "Water.h"

extern Game *game;

Water::Water(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass) 
	: BodyObj( world, initPosition, initOrientation, shapeData, mass)
{

}


Water::~Water()
{
}

void Water::Draw()
{
	rp3d::Transform transform = body->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);

	if (model)
	{
		int jLimit = (LevelLength + 40) / 4;
		glPushMatrix();
		glScalef(2, 1, 2);
		//model->Render();
		for (int i = 0; i < 25; i++)
		{
			for (int j = 0; j < jLimit; j++)
			{
				glPushMatrix();
				glTranslatef(i *2-24, 0, j * 2-20);
				model->Render();
				glPopMatrix();
			}
		}
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		glMultMatrixf(matrix);
		glScalef(modelll.x, modelll.y, modelll.z);
		glColor3f(0, 0.5, 0.5);
		glutSolidCube(1);
		glColor3f(0, 0, 0);
		glutWireCube(1);
		glPopMatrix();
	}
}

void Water::makeCollision(BodyObj *CollideWith)
{
	rp3d::ProxyShape *pro = CollideWith->getProxyShape();
	int actual = pro->getCollideWithMaskBits();
	pro->setCollideWithMaskBits(actual & 0xFFBF);	//all except Water

	//plusk
	if (CollideWith->getOneSplash())
	{
		auto collBody = CollideWith->getBody();
		auto pos = collBody->getTransform().getPosition();
		for (int i = 0; i < 7; i++)
		{
			int x = rand() % 20000 - 10000,
				y = rand() % 20000 + 60000,
				z = rand() % 20000 - 10000;
			rp3d::Vector3 force(x, y, z);
			rp3d::Vector3 col(0, 0, 0);
			int randColor = rand() % 6;
			switch (randColor)
			{
			case 0:
				col.setAllValues(23, 171, 218);
				break;
			case 1:
				col.setAllValues(29, 180, 223);
				break;
			case 2:
				col.setAllValues(54, 198, 232);
				break;
			case 3:
				col.setAllValues(78, 209, 237);
				break;
			default:
				col.setAllValues(217, 244, 251);
				break;
			}
			//Particle *partic = new Particle(gameWorld, pos, 0.02, col);
			auto partic = game->getLevel()->Make("effect", pos, rp3d::Quaternion::identity());

			partic->getColor(col);
			partic->setMaxTime(80);
			partic->setMaterial(0.5, 0.1);
			auto tmpBody = partic->getBody();
			tmpBody->applyForceToCenterOfMass(force);
			partic->setCollisionCategory(EFFECTcat);

			x = rand() % 20 - 10;
			y = rand() % 20 - 10;
			z = rand() % 20 - 10;
			force.setAllValues(x, y, z);
			tmpBody->applyTorque(force);
			//if (game->effects.size() < 60)
			//	game->effects.push_back(partic);

			CollideWith->setOneSplash(false);
			int asfa = 0;
		}
	}
}