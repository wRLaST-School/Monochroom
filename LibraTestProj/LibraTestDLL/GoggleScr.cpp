#include "GoggleScr.h"
#include <ScriptComponent.h>
#include <ConsoleWindow.h>


void GoggleScr::Init()
{
	mParent = This()->Parent()->CastTo<Object3D>();

	mIsMoving = false;
	mIsEquip = false;

	mEasing = std::make_unique<Easing>();
	mEasing->SetEaseType(EaseType::InOut);

	mLocalPos = kNormalPos;
}

void GoggleScr::Update()
{
	if (mOwner)
	{
		//プレイヤーの向いてる方向へのベクトル
		auto q = Quaternion::EulerToQuaternion(mOwner->rotationE);
		Vec3 frontVec = mLocalPos * q;
		Vec3 currentPos = Vec3{ mOwner->position.x,mOwner->position.y,mOwner->position.z } + Vec3{ frontVec.x, frontVec.y, frontVec.z };

		//着脱中
		if (mIsMoving)
		{
			ConsoleWindow::Log("MoveGoggle!!!!!!!!!!");

			mEasing->Update();

			mLocalPos = mEasing->Out(mStartPos, mEndPos);

			//徐々に透明に
			if (mIsEquip)
			{
				mAlpha = 1.0f - mEasing->GetTimeRate();
			}
			else
			{
				mAlpha = mEasing->GetTimeRate();
			}
			mParent->brightnessCB.contents->w = mAlpha;

			//終了
			if (mEasing->GetTimeRate() >= 1.0f)
			{
				mParent->brightnessCB.contents->w = 0;
				mIsMoving = false;
			}
		}

		mParent->position = { currentPos.x,currentPos.y,currentPos.z };

		mParent->rotationE = { -mOwner->rotationE.x,mOwner->rotationE.y + kRotOffsetY,mOwner->rotationE.z };
	}
}

void GoggleScr::Draw()
{

}

void GoggleScr::GettedPlayer(Object3D* owner)
{
	mOwner = owner;
}

void GoggleScr::TakeOnOff(bool isEquip)
{
	float easingPow = 1.0f;
	EaseType eType = EaseType::Lerp;
	int eTimer = 0;

	if (isEquip)
	{
		mStartPos = kNormalPos;
		mEndPos = kEquipPos;
		eTimer = (kOnMoveTime);
		eType = (EaseType::Out);
		easingPow = 3.0f;
	}
	else
	{
		mEndPos = kNormalPos;
		mStartPos = kEquipPos;
		eTimer = (kOffMoveTime);
		eType = (EaseType::InOut);
		easingPow = 2.0f;
	}

	mEasing->SetEaseTimer(eTimer);
	mEasing->SetEaseType(eType);
	mEasing->SetPowNum(easingPow);

	mParent->blendMode = Object3D::BlendMode::Alpha;

	mIsEquip = isEquip;

	mParent->Activate();

	mIsMoving = true;

	mEasing->Reset();
}

RegisterScriptBody(GoggleScr);
