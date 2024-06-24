#include "CollisionManager.h"
#include <SceneManager.h>

void CollisionManager::Init()
{
	mPlayerCollider = SceneManager::FindObject<Object3D>("Player")->GetComponent<PlayerCollider>("PlayerCollider");

	auto objs = SceneManager::FindObjectsWithTag<Object3D>("Block");
	for (const auto& obj : objs)
	{
		mBlockColliders.push_back(obj->GetComponent<BlockCollider>("BlockCollider"));
	}
}

void CollisionManager::Update()
{
	PlayerHitBlocks();
}

void CollisionManager::Draw()
{

}

void CollisionManager::PlayerHitBlocks()
{
	auto collider = mPlayerCollider->GetBodyCollider();
	for (const auto& bc : mBlockColliders)
	{
		Vec3 pushOut = Vec3::zero;
		if (bc->GetBodyCollider().IsTriggerSphere(&collider, &pushOut))
		{
			mPlayerCollider->Parent()->CastTo<Object3D>()->position += pushOut;
		}
	}
}