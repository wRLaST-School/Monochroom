#pragma once
#include <Color.h>

enum class DLLExport ColliderType
{
	OBB,
	Sphere,
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

public:/*
	virtual bool Collide(const ICollider& other) const = 0;*/
};
