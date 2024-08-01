#include <AttractParticleManager.h>
#include <SceneManager.h>
#include <ConsoleWindow.h>


void AttractParticleManager::Init()
{
	mFlyBlockAndAttractEffect = std::make_unique<std::map<FlyBlock*, std::unique_ptr<AttractParticleEmitter>>>();

	auto blockObjs = SceneManager::FindObjectsWithTag<Object3D>("FlyBlock");

	//flyBlockに変更
	for (auto& bObj : blockObjs)
	{
		auto fbScr = SceneManager::FindChildObject<FlyBlock>("FlyBlock", bObj);

		//エミッターはまだ空で登録
		mFlyBlockAndAttractEffect->insert(
			std::make_pair(fbScr, nullptr)
		);
	}
}

void AttractParticleManager::Update()
{
	for (auto itr = mFlyBlockAndAttractEffect->begin();
		itr != mFlyBlockAndAttractEffect->end(); itr++)
	{
		FlyBlock* fbc = itr->first;
		AttractParticleEmitter* emitter = nullptr;
		if (itr->second)
		{
			emitter = itr->second.get();
		}

		//引き寄せられてなかったらエミッター無効に
		if(emitter)
		{
			if (fbc->GetIsAttracting())
			{
			}
			else
			{
				emitter->Deactivate();
			}

			emitter->Update();
			emitter->ParticlesUpdate();
		}
	}
}

void AttractParticleManager::Draw()
{
	for (auto itr = mFlyBlockAndAttractEffect->begin();
		itr != mFlyBlockAndAttractEffect->end(); itr++)
	{
		FlyBlock* fbc = itr->first;
		AttractParticleEmitter* emitter = nullptr;
		if (itr->second)
		{
			emitter = itr->second.get();
			emitter->Draw();
		}
	}
}

//--------------------------------------------------------------------------------------------------
void AttractParticleManager::BeginAttractEffect(FlyBlock* fbc, const Vec3& sPos, const Vec3& ePos)
{
	auto itr = mFlyBlockAndAttractEffect->find(fbc);

	if (itr != mFlyBlockAndAttractEffect->end())
	{
		auto attractEffectEmitter = std::make_unique<AttractParticleEmitter>();

		itr->second.reset();
		itr->second = std::move(attractEffectEmitter);
		itr->second->Initialize(sPos, ePos);
		itr->second->Activate();
	}
}

RegisterScriptBody(AttractParticleManager);
