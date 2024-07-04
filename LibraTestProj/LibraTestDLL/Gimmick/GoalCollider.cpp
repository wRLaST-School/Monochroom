#include "GoalCollider.h"
#include <ScriptComponent.h>

void GoalCollider::Init()
{
	mObj = This()->CastTo<Object3D>();
}

void GoalCollider::Update()
{
	Vec3 pos = mObj->position;
	float r = 1.0f;
	mClearCollider.Setting(pos, r);
}

void GoalCollider::Draw()
{
	mClearCollider.DrawCollider();
}

RegisterScriptBody(GoalCollider);
