#pragma once
#include "ICollider.h"
#include "SphereCollider.h"
#include "SpMath.h"
class DLLExport RayCollider :
	public ICollider
{
public:
	Ray r;

public:
	RayCollider();
	void Setting(const Vec3 pos, const Vec3 dir, const float length = -1);

public:
	void DrawCollider() override;

public:
	RayCollider(Vec3 r, Float3 origin = { 0, 0, 0 }) :r(r, origin) {};
	RayCollider(const Ray& r) : r(r) {};

	bool Collide(const SphereCollider& other)const;
	bool Collide(const PlaneCollider& other, Vec3* intersectionOut = nullptr) const;

public:
	bool checkLength = true;
};