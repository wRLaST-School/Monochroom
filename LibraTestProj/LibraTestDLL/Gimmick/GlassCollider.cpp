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
	scale.z *= 2;
	mBodyCollider.Setting(pos, rot, scale);
}

void GlassCollider::Draw()
{
	mBodyCollider.DrawCollider();
}

RegisterScriptBody(GlassCollider);
