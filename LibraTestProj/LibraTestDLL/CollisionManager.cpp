#include "CollisionManager.h"
#include <SceneManager.h>
#include <ConsoleWindow.h>

void CollisionManager::Init()
{
	mPlayerCollider = SceneManager::FindObject<PlayerCollider>("PlayerCollider");

	auto objs = SceneManager::FindObjectsWithTag<Object3D>("Block");
	for (const auto& obj : objs)
	{
		mBlockColliders.push_back(obj->GetComponent<BlockCollider>("BlockCollider"));
	}
}

void CollisionManager::Update()
{
	if (!ConsoleWindow::NullCheck<PlayerCollider>(mPlayerCollider))
	{
		ConsoleWindow::Log("mPlayerCollider is Null");
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
			mPlayerCollider->Parent()->CastTo<Object3D>()->position += pushOut;
		}
	}
}

RegisterScriptBody(CollisionManager);
