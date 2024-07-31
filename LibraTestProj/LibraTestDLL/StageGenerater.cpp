#include "StageGenerater.h"
#include <BlinkTransition.h>
#include <SceneManager.h>
#include <ScriptComponent.h>
#include <Input.h>
#include <GameManager.h>
#include <Util.h>

void StageGenerater::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
	mPlaneAlpha = SceneManager::FindObject<Object3D>("PlaneAlpha");
	mPlane = SceneManager::FindObject<Object3D>("Plane");
	mStageObj = SceneManager::FindObject<Object3D>("StageObj");

	Vec3 scale = Vec3(100, 100, 1);
	mPlaneAlpha->scale = scale;
	mPlane->scale = scale;
	mPlane->miscCB.contents->dissolveStrength = 0.0f;
	mPlaneAlpha->offset.y = 0.0f;

	mIsStart = false;
	mIsStarted = false;
	mIsEnd = false;

	mObj->position.y = -10000;
	mMoveSpeed = 0.05f;

	mEase.SetEaseTimer(120);
}

void StageGenerater::Update()
{
	if (!mIsStart)
	{
		return;
	}
	mMoveSpeed += 0.0025f;
	if (mMoveSpeed >= 0.5f)
	{
		mMoveSpeed = 0.5f;
	}
	mPlaneAlpha->offset.y -= 0.01f;

	mObj->position += mMoveVec.Norm() * mMoveSpeed;

	float dis = Vec3::Distance(mObj->position, mEnd);

	if (dis <= 100.f)
	{
		mIsEnd = true;
	}

	if (dis <= mMoveSpeed * 1.1f)
	{
		mObj->position = mEnd;
		mIsStart = false;
	}

	mEase.Update();
	if (!mEase.GetisEnd())
	{
		Object3D::dissolveStrength = mEase.Lerp(0.0f, 1.25f);
	}
}

void StageGenerater::Draw()
{

}

void StageGenerater::Start()
{
	if (mIsStarted)
	{
		return;
	}

	mIsStart = true;
	mIsStarted = true;

	Object3D* player = GameManager::GetInstance()->GetPlayer();

	mMoveVec = Vec3(sinf(player->rotationE.y), 0.5f, cosf(player->rotationE.y));
	mObj->position = Vec3(player->position) + mMoveVec.Norm() * 0.25f;
	mObj->rotationE = player->rotationE;

	mEnd = mMoveVec * 200 + mObj->position;
}

bool StageGenerater::GetisEnd()
{
	if (this == nullptr) return false;
	return mIsEnd;
}

RegisterScriptBody(StageGenerater);
