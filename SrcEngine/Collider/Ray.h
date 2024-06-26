#pragma once
#include <Vec3.h>
class DLLExport Ray
{
public:
	Ray();
	Ray(Vec3 ray, Vec3 origin);
	Ray(Vec3 ray, Vec3 origin, float length);

	Vec3 ray;
	Vec3 origin;
	float length;
};

