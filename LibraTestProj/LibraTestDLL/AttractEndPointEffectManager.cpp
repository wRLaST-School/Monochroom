#include "AttractEndPointEffectManager.h"
#include <SceneManager.h>



void AttractEndPointEffectManager::Init()
{
	mAttractEndPointEffects = std::make_unique<std::map<FlyBlock*, std::unique_ptr<AttractEndPointEffect>>>();

	auto blockObjs = SceneManager::FindObjectsWithTag<Object3D>("FlyBlock");

	//flyBlockに変更
	for (auto& bObj : blockObjs)
	{
		auto fbScr = SceneManager::FindChildObject<FlyBlock>("FlyBlock", bObj);

		//エフェクトのインスタンスはまだ空で登録
		mAttractEndPointEffects->insert(
			std::make_pair(fbScr, nullptr)
		);
	}
}

void AttractEndPointEffectManager::Update()
{

}

void AttractEndPointEffectManager::Draw()
{

}


//----------------------------------------------------------------------------------------
void AttractEndPointEffectManager::GenerateEffect(FlyBlock* fb, const Vec3& ePos)
{
	auto itr = mAttractEndPointEffects->find(fb);

	if (itr != mAttractEndPointEffects->end())
	{
		auto attractEndPointEffect = std::make_unique<AttractEndPointEffect>();

		itr->second.reset();
		itr->second = std::move(attractEndPointEffect);
	}
}

RegisterScriptBody(AttractEndPointEffectManager);
