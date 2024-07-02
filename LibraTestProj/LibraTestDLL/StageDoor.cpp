#include "StageDoor.h"
#include <ScriptComponent.h>
#include <SceneManager.h>


void StageDoor::Init()
{
	mEasing = std::make_unique<Easing>();

	mEasing->SetEaseTimer(kMoveTime);
	mEasing->SetEaseType(EaseType::Out);

	mClosingPosLTmp = SceneManager::FindChildObject<Object3D>("Left", Parent())->position;
	mClosingPosRTmp = SceneManager::FindChildObject<Object3D>("Right", Parent())->position;

	mIsMoving = false;
	mIsClosing = true;
}

void StageDoor::Update()
{
	if (mIsMoving)
	{
		mEasing->Update();

		auto leftD = SceneManager::FindChildObject<Object3D>("Left", This()->Parent());
		auto rightD = SceneManager::FindChildObject<Object3D>("Right", This()->Parent());

		leftD->position = mEasing->Out(mStartPosL, mEndPosL);
		rightD->position = mEasing->Out(mStartPosR, mEndPosR);

		if (mEasing->GetTimeRate() >= 1.0f)
		{
			mIsMoving = false;
		}
	}
	else if (!mIsClosing)
	{
		if (mCloseLimitTime >= kCloseLimitTime)
		{
			CloseDoor();
		}

		mCloseLimitTime++;
	}
}

void StageDoor::Draw()
{

}

//---------------------------------------------------------------------
void StageDoor::OpClDoorInternal(const Vec3& sPosL, const Vec3& ePosL,
	const Vec3& sPosR, const Vec3& ePosR, bool isClosing)
{
	if ((mIsClosing == isClosing && mIsMoving) ||
		(!mIsMoving && mIsClosing == isClosing))
	{
		return;
	}

	//途中で逆の動きを始メタ時以外
	if (!mIsMoving)
	{
		mEasing->Reset();
	}

	mStartPosL = sPosL;
	mStartPosR = sPosR;
	mEndPosL = ePosL;
	mEndPosR = ePosR;

	mIsMoving = true;

	mIsClosing = isClosing;
}

void StageDoor::OpenDoor()
{
	mCloseLimitTime = 0;

	OpClDoorInternal(mClosingPosLTmp, mClosingPosLTmp + mClosingPosLTmp * kOpeningPosExtendL
		, mClosingPosRTmp, mClosingPosRTmp + mClosingPosRTmp * kOpeningPosExtendR, false);
}

void StageDoor::CloseDoor()
{
	OpClDoorInternal(mClosingPosLTmp + mClosingPosLTmp * kOpeningPosExtendL, mClosingPosLTmp
		, mClosingPosRTmp + mClosingPosRTmp * kOpeningPosExtendR, mClosingPosRTmp, true);
}



RegisterScriptBody(StageDoor);
