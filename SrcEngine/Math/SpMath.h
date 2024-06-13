#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include "Float2.h"
#include "Float3.h"
#include "Float4.h"
#include "Matrix.h"
#include <Quaternion.h>
#include "Ray.h"

#ifndef PI
#define PI 3.14159265359
#endif

#ifndef PIf
#define PIf 3.14159265359f
#endif

#ifndef EPSILON
#define EPSILON 0.000001f
#endif

//ラジアンから角度
float DLLExport ConvertRadianToAngle(float radian);
Vec2 DLLExport ConvertRadianToAngle(Vec2 radian);
Vec3 DLLExport ConvertRadianToAngle(Vec3 radian);

//角度からラジアン
float DLLExport ConvertAngleToRadian(float angle);
Vec2 DLLExport ConvertAngleToRadian(Vec2 angle);
Vec3 DLLExport ConvertAngleToRadian(Vec3 angle);