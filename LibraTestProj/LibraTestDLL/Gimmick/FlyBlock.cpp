#include "FlyBlock.h"
#include <ScriptComponent.h>


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

	if (mIsAttracted)
	{
		mEasing->Update();

		mParent->position = mEasing->Out(mBeginPos, mEndPos);

		if (mEasing->GetTimeRate() >= 1.0f)
		{
			mIsAttracted = false;
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
}

void FlyBlock::EndAttracting()
{
	mIsAttracted = false;
}

RegisterScriptBody(FlyBlock);
