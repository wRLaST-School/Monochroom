#pragma once
#include <ICollider.h>
#include <Color.h>
#include <SphereCollider.h>
class DLLExport OBBCollider : public ICollider
{
public:
	bool IsTriggerSphere(SphereCollider* other, Vec3* pushOut = nullptr);

public:
	bool IsTrigger(ICollider* other, Vec3* pushOut = nullptr);
	void DrawCollider() override;

public:
	void Setting(const Vec3 pos, const Quaternion rot, const Vec3 scale);

public:
	bool Collide(const OBBCollider& other);

	void DrawBB(Color color = Color::White);

public:
	OBBCollider();
	Vec3 pos;
	Quaternion rot;
	Vec3 scale;
};

