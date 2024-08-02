#include "BlockCollEffect.h"
#include <Util.h>
#include <SceneManager.h>
void BlockCollEffect::Init()
{
	mBlockEffectPlane = SceneManager::FindObject<Object3D>("BlockEffectPlane");
	mCamera= SceneManager::FindObject<Object3D>("Camera");

	mBlockAfterScale = { 10.5f,10.5f,1 };

	mBlockEffectEase.SetEaseTimer(180);
	mBlockEffectEase.SetPowNum(3);
}

void BlockCollEffect::Update()
{
	if (mIsHit)
	{
		Vec3 zero = { 0,0,0 };

		mBlockEffectEase.Update();
		mBlockEffectPlane->position = mBlockPos;

		mBlockEffectPlane->scale = mBlockEffectEase.In(zero, mBlockAfterScale);
		mBlockEffectPlane->brightnessCB.contents->w = mBlockEffectEase.In(1, 0);

		mRota.x = mBlockEffectPlane->position.x - mCamera->position.x;
		mRota.y = mBlockEffectPlane->position.y - mCamera->position.y;
		mRota.z = mBlockEffectPlane->position.z - mCamera->position.z;

		mRota.y = atan2f(mRota.x, mRota.z);
		mBlockEffectPlane->rotationE.y = mRota.y;

		mBlockEffectPlane->Update();

		if (mBlockEffectEase.GetisEnd())
		{
			mBlockEffectEase.Reset();
			mIsHit = false;
			mBlockEffectPlane->scale = zero;
			mBlockEffectPlane->brightnessCB.contents->w = 0;
		}
	}
}

void BlockCollEffect::Draw()
{
	
}

void BlockCollEffect::CopyComponent(IComponent* src)
{

}

void BlockCollEffect::SetHitBlockPos(Vec3 pos)
{
	mBlockPos = pos;
}

void BlockCollEffect::SetIsHit(bool IsHit)
{
	mIsHit = IsHit;
}

void BlockCollEffect::SetHitRota(Vec3 rota)
{

}

RegisterScriptBody(BlockCollEffect);