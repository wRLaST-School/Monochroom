#include "GlassCollider.h"
#include <ScriptComponent.h>

void GlassCollider::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
}

void GlassCollider::Update()
{
	Vec3 pos = mObj->position;
	Quaternion rot = Quaternion::EulerToQuaternion(mObj->rotationE);
	Vec3 scale = mObj->scale;
	if (scale.z <= 0.25f)
	{
		scale.z *= 2;
	}
	mBodyCollider.Setting(pos, rot, scale);
}

void GlassCollider::Draw()
{
	mBodyCollider.DrawCollider();
}

OBBCollider GlassCollider::GetBodyCollider()
{
	return mBodyCollider;
}

RegisterScriptBody(GlassCollider);
