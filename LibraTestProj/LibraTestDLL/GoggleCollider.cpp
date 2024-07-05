#include "GoggleCollider.h"
#include <ScriptComponent.h>


void GoggleCollider::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
}

void GoggleCollider::Update()
{
	Vec3 pos = mObj->position;
	float r = 1.0f;
	mSphereCollider.Setting(pos, r);
}

void GoggleCollider::Draw()
{
	mSphereCollider.DrawCollider();
}

RegisterScriptBody(GoggleCollider);
