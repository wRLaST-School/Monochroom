#include "BlockCollEffect.h"
#include <Util.h>
#include <SceneManager.h>
void BlockCollEffect::Init()
{
	mBlockEffectPlane = SceneManager::FindObject<Object3D>("BlockEffectPlane");

	mBlockAfterScale = { 1.5f,1.5f,1 };

	
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
		mBlockEffectPlane->Update();

		if (mBlockEffectEase.GetisEnd())
		{
			mBlockEffectEase.Reset();
			mIsHit = false;
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

RegisterScriptBody(BlockCollEffect);