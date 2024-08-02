#include "FlyBlock.h"
#include <ScriptComponent.h>
#include <ConsoleWindow.h>


const float FlyBlock::skAttractedHittingNotEndDot = 0.49f;

void FlyBlock::Init()
{
	mParent = This()->Parent()->CastTo<Object3D>();
	mEasing = std::make_unique<Easing>();
	mGravity = std::make_unique<Gravity>();

	mResponePos = mParent->position;
	mResponeRot = mParent->rotationE;

	mAttractParentVec = { 0,0,0 };

	mEasing->SetEaseTimer(kAttractedFrameMax);
	mEasing->SetPowNum(3.0f);
}

void FlyBlock::Update()
{
	if (!mParent)
	{
		return;
	}

	if (mIsAttracted)
	{
		mEasing->Update();

		mOldAttractPos = mParent->position;

		mParent->position = mEasing->InOut(mBeginPos, mEndPos);
		mAttractParentVec = Vec3{ mParent->position.x,mParent->position.y,mParent->position.z } - mOldAttractPos;

		if (mEasing->GetTimeRate() >= 1.0f)
		{
			mAttractParentVec = Vec3::zero;
			EndAttracting();
		}
	}
	else if (mAttractParentVec.GetLength())
	{
		mParent->position += mAttractParentVec;
	}
	
	if(!mIsAttracted)
	{
		mParent->position += mGravity->CalcGravity();
	}

	//一定以下に行ったらリスポーン
	if (mParent->position.y <= kResponePos.y)
	{
		mParent->position = mResponePos;
		mParent->rotationE = mResponeRot;
	}
}

void FlyBlock::Draw()
{

}

//-----------------------------------------------------------------
void FlyBlock::BeginAttracting(const Vec3& endPos)
{
	if (!mParent)
	{
		return;
	}

	mBeginPos = mParent->position;
	mEndPos = endPos;

	mIsAttracted = true;

	mEasing->Reset();
	mEasing->SetEaseTimer(kAttractedFrameMax);

	mGravity->ZeroVelocity();
}

void FlyBlock::EndAttracting()
{
	mIsAttracted = false;
	mBeginPos = { 0,0,0 };
	mEndPos = { 0,0,0 };
	mAttractParentVec = { 0,0,0 };
}

RegisterScriptBody(FlyBlock);
