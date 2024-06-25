#include "FlyBlockCollider.h"
#include <ScriptComponent.h>

void FlyBlockCollider::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
}

void FlyBlockCollider::Update()
{
	// 押し戻し用
	float r = Vec3(mObj->scale).GetMaxElement();
	mBodyCollider.Setting(mObj->position, r * 2);
}

void FlyBlockCollider::Draw()
{
	mBodyCollider.DrawCollider();
}

SphereCollider FlyBlockCollider::GetBodyCollider()
{
	return mBodyCollider;
}

RegisterScriptBody(FlyBlockCollider);
