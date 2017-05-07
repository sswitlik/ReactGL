#pragma once
#include "reactphysics3d.h"

#define PI_2 1.57079632679

enum Category {
	PLAYERcat = 0x0001,
	MAPcat = 0x0002,
	ARROWcat = 0x0004
};

void QuaternionO2IToEulerAngles(float *Yaw, float *Pitch, float *Roll, const rp3d::Quaternion &q);