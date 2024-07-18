#include "StageGenerater.h"
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
}

void StageGenerater::Update()
{
	if (!mIsStart)
	{
		return;
	}
	const float moveSpeed = 1.5f;
	mObj->position += mMoveVec.Norm() * moveSpeed;

	Vec3 end = mMoveVec * 330;
	float dis = Vec3::Distance(mObj->position, end);

	if (dis <= moveSpeed)
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

	mMoveVec = Vec3(sinf(player->rotationE.y), 0, cosf(player->rotationE.y));
	mObj->position = Vec3(player->position) + mMoveVec.Norm() * 5;
	mObj->rotationE = player->rotationE;
}

bool StageGenerater::GetisEnd()
{
	return mIsEnd;
}

RegisterScriptBody(StageGenerater);
