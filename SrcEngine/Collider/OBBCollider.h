#pragma once
#include <ICollider.h>
#include <Color.h>
#include <SphereCollider.h>
#include <RayCollider.h>

class DLLExport OBBCollider : public ICollider
{
public:
	Vec3 pos;
	Quaternion rot;
	Vec3 scale;

public:
	OBBCollider();
	void Setting(const Vec3 pos, const Quaternion rot, const Vec3 scale);
	void DrawCollider() override;
	float CheckSeparationAxis(OBBCollider* other, Vec3 axis);

public:
	bool Collide(OBBCollider* other);
	void DrawBB(Color color = Color::White);
};

