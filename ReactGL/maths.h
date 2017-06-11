#pragma once
#include "reactphysics3d.h"
#include <ctime>
#include <cstdlib>

#define PI_2 1.57079632679
#define PI 3.14159265358979
#define RAD  0.01745329
//#define PI_2 1.57079632679
#define PIx2 6.28318530717


enum Category {
	PLAYERcat = 0x0001,
	MAPcat = 0x0002,
	ARROWcat = 0x0004,
	EFFECTcat = 0x008,
	FREEcat = 0x0010
};

void QuaternionO2IToEulerAngles(float *Yaw, float *Pitch, float *Roll, const rp3d::Quaternion &q);

void QuaternionO2IToEulerAngles(double *Yaw, double *Pitch, double *Roll, const rp3d::Quaternion &q);
