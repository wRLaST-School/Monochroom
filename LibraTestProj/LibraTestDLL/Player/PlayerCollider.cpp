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
	mBodyCollider.Setting(mObj->position, r*2);

		// 重力判定用
	Vec3 pos = Vec3(mObj->position) + Vec3(0.f, -1.f, 0.f);
	Quaternion rot = Quaternion();
	Vec3 scale = mObj->scale;
	scale.y = 0.5f;
	mDownCollider.Setting(pos, rot, scale);

	ConsoleWindow::Log(std::format("Pos : ({},{},{})", pos.x, pos.y, pos.z));
	ConsoleWindow::Log(std::format("Rot : ({},{},{},{})", rot.v.x, rot.v.y, rot.v.z, rot.w));
	ConsoleWindow::Log(std::format("Scale : ({},{},{})", scale.x, scale.y, scale.z));
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