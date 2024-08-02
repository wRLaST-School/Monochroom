#include "StageDoor.h"
#include <ScriptComponent.h>
#include <SceneManager.h>


void StageDoor::Init()
{
	mEasing = std::make_unique<Easing>();

	mEasing->SetEaseTimer(kMoveTime);
	mEasing->SetEaseType(EaseType::Out);

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
		(!mIsMoving && mIsClosing == isClosing)||
		mIsMoving)
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

	OpClDoorInternal(kClosingPosLTmp, kClosingPosLTmp + kClosingPosLTmp * kOpeningPosExtendL
		, kClosingPosRTmp, kClosingPosRTmp + kClosingPosRTmp * kOpeningPosExtendR, false);
}

void StageDoor::CloseDoor()
{
	OpClDoorInternal(kClosingPosLTmp + kClosingPosLTmp * kOpeningPosExtendL, kClosingPosLTmp
		, kClosingPosRTmp + kClosingPosRTmp * kOpeningPosExtendR, kClosingPosRTmp, true);
}



RegisterScriptBody(StageDoor);
