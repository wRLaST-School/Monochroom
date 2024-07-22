#include "PlayerCollider.h"
#include <ScriptComponent.h>
#include <ConsoleWindow.h>
#include <Vec3.h>

void PlayerCollider::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
}

void PlayerCollider::Update()
{
	// 押し戻し用
	float r = Vec3(mObj->scale).GetMaxElement();
	mBodyCollider.Setting(mObj->position, r * 2);

	// 重力判定用
	Quaternion rot = Quaternion();
	Vec3 scale = mObj->scale;
	scale.y = 0.5f;
	Vec3 pos = Vec3(mObj->position) + Vec3(0.f, -1.75f - scale.y / 2.f, 0.f);
	mDownCollider.Setting(pos, rot, scale);
}

void PlayerCollider::Draw()
{
	mBodyCollider.DrawCollider();
	mDownCollider.DrawCollider();
}

SphereCollider PlayerCollider::GetBodyCollider()
{
	return mBodyCollider;
}

OBBCollider PlayerCollider::GetDownCollider()
{
	return mDownCollider;
}

RegisterScriptBody(PlayerCollider);