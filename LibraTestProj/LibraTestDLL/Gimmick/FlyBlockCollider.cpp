#include "FlyBlockCollider.h"
#include <ScriptComponent.h>
#include <SceneManager.h>
#include <ConsoleWindow.h>
#include <Timer.h>

void FlyBlockCollider::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
	mFlyBlock = SceneManager::FindChildObject<FlyBlock>("FlyBlock", mObj);

	const uint32_t maxTime = 60;
	mTimer.SetLimitTimer(maxTime);
	mTimer.SetTimer(maxTime);
	mTimer.Reset();
}

void FlyBlockCollider::Update()
{
	mTimer.Update();
	if (mTimer.GetisTimeOut())
	{
		mMoveCollider.isActive = true;
	}

	// 移動用
	Vec3 moveVec = mFlyBlock->GetAttractedDir() / 2;
	mMoveCollider.Setting(
		Vec3(mObj->position) + moveVec, Quaternion::EulerToQuaternion(mObj->rotationE), Vec3(mObj->scale)* 0.9f);

	// 押し戻し用
	mBodyCollider.Setting(mObj->position, Quaternion::EulerToQuaternion(mObj->rotationE), mObj->scale);

	// 重力判定用
	Vec3 scale = Vec3(mObj->scale);
	scale.y *= 0.25f;
	Vec3 pos = Vec3(mObj->position) + Vec3(0.f, -mObj->scale.y + scale.y, 0.f);
	Quaternion rot = Quaternion();
	mDownCollider.Setting(pos, rot, scale);

	// 重なり判定用
	pos = Vec3(mObj->position) + Vec3(0.f, mObj->scale.y - scale.y, 0.f);
	mTopCollider.Setting(pos, rot, scale);
}

void FlyBlockCollider::Draw()
{
	mMoveCollider.DrawCollider();
	//mBodyCollider.DrawCollider();
	mTopCollider.DrawCollider();
	mDownCollider.DrawCollider();
}

void FlyBlockCollider::IsMoveing()
{
	//mMoveCollider.isActive = false;
	mTimer.Reset();
}

OBBCollider FlyBlockCollider::GetMoveCollider()
{
	return mMoveCollider;
}

OBBCollider FlyBlockCollider::GetBodyCollider()
{
	return mBodyCollider;
}

OBBCollider FlyBlockCollider::GetDownCollider()
{
	return mDownCollider;
}

OBBCollider FlyBlockCollider::GetTopCollider()
{
	return mTopCollider;
}

RegisterScriptBody(FlyBlockCollider);
