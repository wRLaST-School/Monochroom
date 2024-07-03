#include "StageButton.h"
#include <ScriptComponent.h>
#include <SceneManager.h>


void StageButton::Init()
{
	mIsPushedBody = false;
	mIsMovingBody = false;

	mEasing = std::make_unique<Easing>();
	mEasing->SetEaseTimer(kPushingFrame);
	mEasing->SetEaseType(EaseType::Out);

	mParent = This()->Parent();
}

void StageButton::Update()
{
	if (mIsMovingBody)
	{
		mEasing->Update();

		auto body = SceneManager::FindChildObject<Object3D>("Body", mParent);
		body->position = mEasing->Out(mStartBodyPos, mEndBodyPos);

		if (mEasing->GetisEnd())
		{
			mIsMovingBody = false;
		}
	}
	else if(mIsPushedBody)
	{
		mPushBackTimer++;

		if (mPushBackTimer >= kPushBackLimit)
		{
			EndPushingButton();
		}
	}
}

void StageButton::Draw()
{

}



//-----------------------------------------------------------------------------
void StageButton::BeginPushingButtonInter(const Vec3& sPos, const Vec3& ePos)
{
	if (mIsMovingBody)
	{
		return;
	}

	mIsMovingBody = true;

	mStartBodyPos = sPos;
	mEndBodyPos = ePos;

	mPushBackTimer = 0;

	mEasing->Reset();
}

void StageButton::BeginPushingButton()
{
	mPushBackTimer = 0;

	//凹んでる状態で凹ませ始めるのはおかしいので
	if (mIsPushedBody)
	{
		return;
	}

	mIsPushedBody = true;

	BeginPushingButtonInter(kNormalBodyPos, kPushedBodyPos);
}

void StageButton::EndPushingButton()
{
	//凹んでない状態で凹みが戻るのはおかしいので
	if (!mIsPushedBody)
	{
		return;
	}

	mIsPushedBody = false;

	BeginPushingButtonInter(kPushedBodyPos, kNormalBodyPos);
}

RegisterScriptBody(StageButton);
