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
	Vec3 pos = Vec3(mObj->position) + Vec3(0.f, -1.f, 0.f);
	Quaternion rot = Quaternion();
	Vec3 scale = mObj->scale;
	scale.y = 0.1f;
	mDownCollider.Setting(pos, rot, scale);
}

void PlayerCollider::Draw()
{
	mDownCollider.DrawCollider();
}

void PlayerCollider::CopyComponent(IComponent* src)
{

}
