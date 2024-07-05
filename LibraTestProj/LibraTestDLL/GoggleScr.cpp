#include "GoggleScr.h"
#include <ScriptComponent.h>
#include <ConsoleWindow.h>


void GoggleScr::Init()
{
	mParent = This()->Parent()->CastTo<Object3D>();

	mIsMoving = false;

	mEasing = std::make_unique<Easing>();

	mEasing->SetEaseTimer(kMoveTime);

	mLocalPos = kNormalPos;
}

void GoggleScr::Update()
{
	if (mOwner)
	{
		//着脱中
		if (mIsMoving)
		{
			ConsoleWindow::Log("MoveGoggle!!!!!!!!!!");

			mEasing->Update();

			mLocalPos = mEasing->Out(mStartPos, mEndPos);

			if (mEasing->GetTimeRate() >= 1.0f)
			{
				mIsMoving = false;
			}
		}

		//
		auto q = Quaternion::EulerToQuaternion(mOwner->rotationE);
		Vec3 frontVec = mLocalPos * q;


		Vec3 pos = Vec3{ mOwner->position.x,mOwner->position.y,mOwner->position.z } + Vec3{ frontVec.x, frontVec.y, frontVec.z };
		mParent->position = { pos.x,pos.y,pos.z };

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
	if (isEquip)
	{
		mStartPos = kNormalPos;
		mEndPos = kEquipPos;
	}
	else
	{
		mEndPos = kNormalPos;
		mStartPos = kEquipPos;
	}

	mIsMoving = true;

	mEasing->Reset();
}

RegisterScriptBody(GoggleScr);
