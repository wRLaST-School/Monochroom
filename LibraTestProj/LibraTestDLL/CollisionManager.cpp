#include "CollisionManager.h"
#include <SceneManager.h>
#include <ConsoleWindow.h>
#include <GameManager.h>

void CollisionManager::Init()
{
	auto player = SceneManager::FindObject<Object3D>("Player");
	mPlayerCollider = SceneManager::FindChildObject<PlayerCollider>("PlayerCollider", player);
	//mPlayerCollider = GameManager::GetInstance()->GetPlayer()->CastToScript<PlayerCollider>();

	auto objs = SceneManager::FindObjectsWithTag<Object3D>("Block");
	for (const auto& obj : objs)
	{
		auto collider = SceneManager::FindChildObject<BlockCollider>("BlockCollider", obj);
		mBlockColliders.push_back(collider);
	}
}

void CollisionManager::Update()
{
	if (!ConsoleWindow::NullCheck<PlayerCollider>(mPlayerCollider))
	{
		//ConsoleWindow::Log("mPlayerCollider is Null");
		return;
	}

	PlayerHitBlocks();
}

void CollisionManager::Draw()
{

}

void CollisionManager::PlayerHitBlocks()
{
	auto playerBodyCollider = mPlayerCollider->GetBodyCollider();
	for (const auto& bc : mBlockColliders)
	{
		Vec3 pushOut = Vec3::zero;
		if (bc->GetBodyCollider().IsTriggerSphere(&playerBodyCollider, &pushOut))
		{
			SceneManager::FindObject<Object3D>("Player")->position += pushOut;
			//mPlayerCollider->Parent()->CastTo<Object3D>()->position += pushOut;
		}
	}
}

RegisterScriptBody(CollisionManager);
