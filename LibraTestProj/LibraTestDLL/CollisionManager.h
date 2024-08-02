#pragma once
#include "IScriptObject.h"
#include <SceneManager.h>
#include <ViewCollider.h>
#include <PlayerCollider.h>
#include <BlockCollider.h>
#include <FlyBlockCollider.h>
#include <ButtonCollider.h>
#include <GlassCollider.h>
#include <GoalCollider.h>
#include <GoggleCollider.h>
#include <TransparentCollider.h>
#include <PlayerGoggle.h>
#include <DoorCollider.h>

#include "BlockCollEffect.h"

class CollisionManager :
	public IScriptObject
{
private:
	PlayerGoggle* mPlayerGoggle;

private:
	ViewCollider* mViewCollider;
	PlayerCollider* mPlayerCollider;
	std::vector<DoorCollider*> mDoorColliders;
	std::vector<BlockCollider*> mBlockColliders;
	std::vector<FlyBlockCollider*> mFlyBlockColliders;
	std::vector<ButtonCollider*> mButtonColliders;
	std::vector<GlassCollider*> mGlassColliders;
	std::vector<GoalCollider*> mGoalColliders;
	std::vector<GoggleCollider*> mGoggleColliders;
	std::vector<TransparentCollider*> mTransparentColliders;
	SphereCollider coll;

	BlockCollEffect* mBlockCollEffect;

private:
	template<typename T>
	inline std::vector<T*> FindColliderList(const std::string& objectTag, const std::string& scriptTag);

private:
	void CameraInsideFlyBlocks();
	void RayHitFlyBlocks();
	void RayHitGoggle();
	void PlayerHitBlocks();
	void PlayerHitButtons();
	void PlayerHitGlasses();
	void PlayerHitGoals();
	void PlayerHitTransparents();
	void PlayerHitDoors();

	void FlyBlocksHitBlocks();
	void FlyBlocksHitButtons();
	void FlyBlocksHitGlasses();
	void FlyBlocksHitFlyBlocks();
	void FlyBlocksHitGoals();
	void FlyBlocksHitDoors();


public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

	float CheckRayHitOtherDis(FlyBlockCollider* current);
	void RecursiveAttracting(FlyBlockCollider* current, const  std::vector<FlyBlockCollider*>& colliders);
	bool CheckHitOtherFlyBlock(FlyBlockCollider* current);

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
