#pragma once
#include "ICollider.h"
#include "SpMath.h"
#include <PlaneCollider.h>
class DLLExport SphereCollider : public ICollider
{
public:
	Vec3 pos;
	float r;

public:
	SphereCollider();
	void Setting(const Vec3 pos, const float r);
	void DrawCollider() override;
};

