#include "ViewCollider.h"
#include <ScriptComponent.h>
#include <ConsoleWindow.h>

void ViewCollider::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
}

void ViewCollider::Update()
{
	// レイ
	Vec3 dir = mObj->matWorld.ExtractAxisZ();
	//dir = Vec3::front;
	ConsoleWindow::Log(std::format("Dir : ({},{},{})", dir.x, dir.y, dir.z));
	mRayCollider.Setting(mObj->position, dir);
}

void ViewCollider::Draw()
{
	mRayCollider.DrawCollider();
}

RayCollider ViewCollider::GetRayCollider()
{
	return mRayCollider;
}

Vec3 ViewCollider::GetPos()
{
	return mObj->position;
}

RegisterScriptBody(ViewCollider);
