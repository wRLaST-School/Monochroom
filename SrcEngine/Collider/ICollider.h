#pragma once
#include <Color.h>

enum class DLLExport ColliderType
{
	OBB,
	Sphere,
	Ray,
};

class DLLExport ICollider
{
public:
	bool isActive;
	Color color;
	ColliderType type;

public:
	virtual ~ICollider() {}
	virtual void DrawCollider() = 0;
	bool IsTrigger(ICollider* other, Vec3* pushOut = nullptr);

public:/*
	virtual bool Collide(const ICollider& other) const = 0;*/
};
