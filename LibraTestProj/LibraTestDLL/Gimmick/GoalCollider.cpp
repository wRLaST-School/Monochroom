#include "GoalCollider.h"
#include <ScriptComponent.h>

void GoalCollider::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
}

void GoalCollider::Update()
{
	if (!mObj)
	{
		return;
	}

	Vec3 pos = mObj->position;
	float r = 1.0f;
	mClearCollider.Setting(pos, r);
}

void GoalCollider::Draw()
{
	mClearCollider.DrawCollider();
}

SphereCollider GoalCollider::GetClearCollider()
{
	return mClearCollider;
}

OBBCollider GoalCollider::GetBodyCollider()
{
	return mBodyCollider;
}

RegisterScriptBody(GoalCollider);
