#include "stdafx.h"
#include "Ray.h"

Ray::Ray()
{
}

Ray::Ray(Vec3 ray, Vec3 origin)
	: ray(ray), origin(origin) 
{
}

Ray::Ray(Vec3 ray, Vec3 origin, float length)
	: ray(ray), origin(origin), length(length)
{
}
