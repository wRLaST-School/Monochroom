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

}

void CollisionManager::Draw()
{

}

void CollisionManager::CopyComponent(IComponent* src)
{

}
