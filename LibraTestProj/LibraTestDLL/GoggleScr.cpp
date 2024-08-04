#include "GoggleScr.h"
#include <ScriptComponent.h>
#include <ConsoleWindow.h>
#include <SceneManager.h>


void GoggleScr::Init()
{
	if (mEasing != nullptr)
	{
		return;
	}

	mParent = This()->Parent()->CastTo<Object3D>();
	mLensL = SceneManager::FindChildObject<Object3D>("LensL", mParent);
	mLensR = SceneManager::FindChildObject<Object3D>("LensR", mParent);

	if (mLensL && mLensR)
	{
		mLensL->blendMode = Object3D::BlendMode::Alpha;
		mLensR->blendMode = Object3D::BlendMode::Alpha;

		mLensR->brightnessCB.contents->x = kLensColor.x;
		mLensR->brightnessCB.contents->y = kLensColor.y;
		mLensR->brightnessCB.contents->z = kLensColor.z;
		mLensR->brightnessCB.contents->w = kLensAlpha;

		mLensL->brightnessCB.contents->x = kLensColor.x;
		mLensL->brightnessCB.contents->y = kLensColor.y;
		mLensL->brightnessCB.contents->z = kLensColor.z;
		mLensL->brightnessCB.contents->w = kLensAlpha;

		mLensL->brightnessCB.contents->w = 1.0f;
		mLensR->brightnessCB.contents->w = 1.0f;
	}

	mIsMoving = false;
	mIsEquip = false;

	mParent->brightnessCB.contents->w = 1.0f;

	mParent->blendMode = Object3D::BlendMode::Opaque;

	mEasing = std::make_unique<Easing>();
	mEasing->SetEaseType(EaseType::InOut);

	mLocalPos = kNormalPos;
}

void GoggleScr::Update()
{
	if (mIsEquip)
	{
		ConsoleWindow::Log("Equip!!!!!!!!!!!!!");
	}


	if (mOwner)
	{
		mParent->brightnessCB.contents->w = 0;
		if (mLensL && mLensR)
		{
			mLensL->brightnessCB.contents->w = 0;
			mLensR->brightnessCB.contents->w = 0;
		}
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
	if (mEasing == nullptr)
	{
		Init();
	}

	mOwner = owner;

	mEasing->Reset();
	mIsEquip = true;
}

void GoggleScr::TakeOnOff(bool isEquip)
{
	float easingPow = 1.0f;
	EaseType eType = EaseType::Lerp;
	int eTimer = 0;

	SoundManager::Play("Goggle");

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

float GoggleScr::GetPosRatio()
{
	float ratio = mEasing->GetTimeRate();

	if (mIsEquip)
	{
		ratio = 1.0f - ratio;
	}


	return ratio;
}

RegisterScriptBody(GoggleScr);
