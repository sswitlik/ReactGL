#pragma once
#include "reactphysics3d.h"

#define PI_2 1.57079632679
#define PI 3.14159265358979

enum Category {
	PLAYERcat = 0x0001,
	MAPcat = 0x0002,
	ARROWcat = 0x0004,
	EFFECTcat = 0x008
};

void QuaternionO2IToEulerAngles(float *Yaw, float *Pitch, float *Roll, const rp3d::Quaternion &q);

void QuaternionO2IToEulerAngles(double *Yaw, double *Pitch, double *Roll, const rp3d::Quaternion &q);
