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
	// 移動用
	float r = Vec3(mObj->scale).GetMaxElement();
	Vec3 moveVec = mFlyBlock->GetMoveVec();
	mMoveCollider.Setting(Vec3(mObj->position) + moveVec, r);

	// 押し戻し用
	mBodyCollider.Setting(mObj->position, Quaternion::EulerToQuaternion(mObj->rotationE), mObj->scale);

	// 重力判定用
	Vec3 scale = Vec3(mObj->scale);
	scale.y *= 0.5f;
	Vec3 pos = Vec3(mObj->position) + Vec3(0.f, -mObj->scale.y + scale.y, 0.f);
	Quaternion rot = Quaternion();
	mDownCollider.Setting(pos, rot, scale);
}

void FlyBlockCollider::Draw()
{
	mMoveCollider.DrawCollider();
	mBodyCollider.DrawCollider();
	mDownCollider.DrawCollider();
}

SphereCollider FlyBlockCollider::GetMoveCollider()
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

RegisterScriptBody(FlyBlockCollider);
