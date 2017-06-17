#include "stdafx.h"
#include <cmath>
#include "reactphysics3d.h"
#include "maths.h"

void QuaternionO2IToEulerAngles(float *Yaw, float *Pitch, float *Roll, const rp3d::Quaternion &q)
{
	float sp = -2.0f * (q.y*q.z - q.w*q.x);

	if (sp == 1.0f)
	{
		*Pitch = PI_2 * sp;
		*Yaw = atan2f(-q.x*q.z + q.w*q.y, 0.5f - q.y*q.y - q.z*q.z);
		*Roll = 0.0f;
	}
	else
	{
		*Pitch = asinf(sp);
		*Yaw = atan2f(q.x*q.z + q.w*q.y, 0.5f - q.x*q.x - q.y*q.y);
		*Roll = atan2f(q.x*q.y + q.w*q.z, 0.5f - q.x*q.x - q.z*q.z);
	}
}

void QuaternionO2IToEulerAngles(double *Yaw, double *Pitch, double *Roll, const rp3d::Quaternion &q)
{
	double sp = -2.0f * (q.y*q.z - q.w*q.x);

	if (sp == 1.0f)
	{
		*Pitch = PI_2 * sp;
		*Yaw = atan2f(-q.x*q.z + q.w*q.y, 0.5f - q.y*q.y - q.z*q.z);
		*Roll = 0.0f;
	}
	else
	{
		*Pitch = asinf(sp);
		*Yaw = atan2f(q.x*q.z + q.w*q.y, 0.5f - q.x*q.x - q.y*q.y);
		*Roll = atan2f(q.x*q.y + q.w*q.z, 0.5f - q.x*q.x - q.z*q.z);
	}
}

void DrawString(GLfloat x, GLfloat y, char * string)
{
	// po�o�enie napisu
	glRasterPos2f(x, y);

	// wy�wietlenie napisu
	int len = 0;
	while (string[len] != ';')
		len++;
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);

}