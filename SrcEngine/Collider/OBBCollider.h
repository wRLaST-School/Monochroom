#pragma once
#include <ICollider.h>
#include <Color.h>
#include <SphereCollider.h>
#include <RayCollider.h>

class DLLExport OBBCollider : public ICollider
{
private:
	bool CheckSeparationAxis(OBBCollider* other, Vec3 axis);

public:
	bool IsTriggerOBB(OBBCollider* other);
	bool IsTriggerRay(RayCollider* other);
	bool IsTriggerSphere(SphereCollider* other, Vec3* pushOut = nullptr);
	void DrawCollider() override;

public:
	void Setting(const Vec3 pos, const Quaternion rot, const Vec3 scale);

public:

	void DrawBB(Color color = Color::White);

public:
	OBBCollider();
	Vec3 pos;
	Quaternion rot;
	Vec3 scale;
};

