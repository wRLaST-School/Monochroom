#include "FlyBlock.h"
#include <ScriptComponent.h>


const float FlyBlock::skAttractedHittingNotEndDot = 0.49f;

void FlyBlock::Init()
{
	mParent = This()->Parent()->CastTo<Object3D>();
	mEasing = std::make_unique<Easing>();
	mGravity = std::make_unique<Gravity>();

	mEasing->SetEaseTimer(kAttractedFrameMax);
}

void FlyBlock::Update()
{
	if (!mParent)
	{
		return;
	}

	mMoveVec = Vec3::zero;
	if (mIsAttracted)
	{
		mEasing->Update();

		mParent->position = mEasing->Out(mBeginPos, mEndPos);
		mMoveVec = (mEndPos - mBeginPos).GetNorm();

		if (mEasing->GetTimeRate() >= 1.0f)
		{
			EndAttracting();
		}
	}
	else
	{
		mParent->position += mGravity->CalcGravity();
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
}

RegisterScriptBody(FlyBlock);
