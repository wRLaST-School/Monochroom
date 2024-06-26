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
	mBodyCollider.Setting(mObj->position, r);

	// 重力判定用
	Vec3 scale = mObj->scale;
	scale.y = 0.5f;
	Vec3 pos = Vec3(mObj->position) + Vec3(0.f, -mObj->scale.y + scale.y, 0.f);
	Quaternion rot = Quaternion();
	mDownCollider.Setting(pos, rot, scale);
}

void FlyBlockCollider::Draw()
{
	mBodyCollider.DrawCollider();
	mDownCollider.DrawCollider();
}

SphereCollider FlyBlockCollider::GetBodyCollider()
{
	return mBodyCollider;
}

OBBCollider FlyBlockCollider::GetDownCollider()
{
	return mDownCollider;
}

RegisterScriptBody(FlyBlockCollider);
