#pragma once
#include "IScriptObject.h"
#include <SceneManager.h>
#include <ViewCollider.h>
#include <PlayerCollider.h>
#include <BlockCollider.h>
#include <FlyBlockCollider.h>
#include <ButtonCollider.h>
#include <GlassCollider.h>

class CollisionManager :
	public IScriptObject
{
private:
	ViewCollider* mViewCollider;
	PlayerCollider* mPlayerCollider;
	std::vector<BlockCollider*> mBlockColliders;
	std::vector<FlyBlockCollider*> mFlyBlockColliders;
	std::vector<ButtonCollider*> mButtonColliders;
	std::vector<GlassCollider*> mGlassColliders;

private:
	bool isPlayerDownHit;
	bool isPlayerTriggerButton;

private:
	template<typename T>
	inline std::vector<T*> FindColliderList(const std::string& objectTag, const std::string& scriptTag);

private:
	void RayHitFlyBlocks();
	void PlayerHitBlocks();
	void PlayerHitButtons();
	void PlayerHitGlasses();
	void FlyBlocksHitBlocks();
	void FlyBlocksHitButtons();
	void FlyBlocksHitGlasses();

public:
	void Init();
	void Update();
	void Draw() {}
	void CopyComponent(IComponent* src) { src; }

	DefDel;
};

RegisterScript(CollisionManager);

template<typename T>
inline std::vector<T*> CollisionManager::FindColliderList(const std::string& objectTag, const std::string& scriptTag)
{
	std::vector<T*> result;

	auto objs = SceneManager::FindObjectsWithTag<Object3D>(objectTag);
	for (const auto& obj : objs)
	{
		auto collider = SceneManager::FindChildObject<T>(scriptTag, obj);
		result.push_back(collider);
	}

	return result;
}
