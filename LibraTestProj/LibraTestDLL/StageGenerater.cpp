#include "StageGenerater.h"
#include <BlinkTransition.h>
#include <SceneManager.h>
#include <ScriptComponent.h>
#include <Input.h>
#include <GameManager.h>

void StageGenerater::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
	mPlaneAlpha = SceneManager::FindObject<Object3D>("PlaneAlpha");
	mPlane = SceneManager::FindObject<Object3D>("Plane");

	Vec3 scale = Vec3(100, 100, 1);
	mPlaneAlpha->scale = scale;
	mPlane->scale = scale;

	mIsStart = false;
	mIsStarted = false;
	mIsEnd = false;

	mObj->position.y = -10000;
	mMoveSpeed = 0.25f;
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

	mObj->position += mMoveVec.Norm() * mMoveSpeed;

	Vec3 end = mMoveVec * 100;
	float dis = Vec3::Distance(mObj->position, end);

	if (dis <= mMoveSpeed)
	{
		mObj->position = end;
		mIsStart = false;
		mIsEnd = true;
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
	mObj->position = Vec3(player->position) + mMoveVec.Norm() * 5;
	mObj->rotationE = player->rotationE;
}

bool StageGenerater::GetisEnd()
{
	return mIsEnd;
}

RegisterScriptBody(StageGenerater);
