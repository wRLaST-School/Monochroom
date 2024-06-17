#include "SpMath.h"

//ラジアンから角度
float ConvertRadianToAngle(float radian) {
	float angle = 180 / PIf * radian;
	return angle;
}

Vec2 ConvertRadianToAngle(Vec2 radian)
{
	return Vec2(ConvertRadianToAngle(radian.x), ConvertRadianToAngle(radian.y));
}

Vec3  ConvertRadianToAngle(Vec3 radian)
{
	return Vec3(ConvertRadianToAngle(radian.x), ConvertRadianToAngle(radian.y), ConvertRadianToAngle(radian.z));
}

//角度からラジアン
float ConvertAngleToRadian(float angle) {
	float radian = PIf / 180 * angle;
	return radian;
}

Vec2 DLLExport ConvertAngleToRadian(Vec2 angle)
{
	return Vec2(ConvertAngleToRadian(angle.x), ConvertAngleToRadian(angle.y));
}

Vec3 DLLExport ConvertAngleToRadian(Vec3 angle)
{
	return Vec3(ConvertAngleToRadian(angle.x), ConvertAngleToRadian(angle.y), ConvertAngleToRadian(angle.z));
}
