#include "stdafx.h"
#include "Rubbish.h"


Rubbish::Rubbish(rp3d::DynamicsWorld *world, rp3d::Vector3 initPosition, rp3d::Quaternion initOrientation, rp3d::Vector3 shapeData, rp3d::decimal mass)
	: BodyObj(world, initPosition, initOrientation, shapeData, mass)
{
}

Rubbish::~Rubbish()
{
}

void Rubbish::makeCollision(BodyObj *CollideWith)
{

}

void Rubbish::Draw()
{
	
	rp3d::Transform transform = body->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);

	if (model)
	{
		glPushMatrix();
		glScalef(2, 1, 2);
		//model->Render();
		for (int i = 0; i < 25; i++)
		{
			for (int j = 0; j < 125; j++)
			{
				glPushMatrix();
				glTranslatef(i * 2 - 24, 0-4, j * 2 - 20);
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