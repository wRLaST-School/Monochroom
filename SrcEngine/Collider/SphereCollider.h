#pragma once
#include "ICollider.h"
#include "SpMath.h"
#include <PlaneCollider.h>
class DLLExport SphereCollider : public ICollider
{
public:
	SphereCollider();
	Vec3 pos;
	float r;

public:
	void Setting(const Vec3 pos, const float r);
	void DrawCollider() override;
};

