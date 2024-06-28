#include "FlyBlockCollider.h"
#include <ScriptComponent.h>
#include <SceneManager.h>
#include <ConsoleWindow.h>

void FlyBlockCollider::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
	mFlyBlock = SceneManager::FindChildObject<FlyBlock>("FlyBlock", mObj);
}

void FlyBlockCollider::Update()
{
		float r = Vec3(mObj->scale).GetMaxElement();
	if (!mFlyBlock)
	{
		ConsoleWindow::Log("FlyBlock Null");
	}
	else
	{
		// 移動用
		Vec3 moveVec = mFlyBlock->GetMoveVec();
		mMoveCollider.Setting(Vec3(mObj->position) + moveVec, r);
	}

	// 押し戻し用
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
	mMoveCollider.DrawCollider();
	mDownCollider.DrawCollider();
}

SphereCollider FlyBlockCollider::GetBodyCollider()
{
	return mBodyCollider;
}

SphereCollider FlyBlockCollider::GetMoveCollider()
{
	return mMoveCollider;
}

OBBCollider FlyBlockCollider::GetDownCollider()
{
	return mDownCollider;
}

RegisterScriptBody(FlyBlockCollider);
