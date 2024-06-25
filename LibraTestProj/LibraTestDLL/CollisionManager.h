#pragma once
#include "IScriptObject.h"
#include <ViewCollider.h>
#include <PlayerCollider.h>
#include <BlockCollider.h>
#include <FlyBlockCollider.h>

class CollisionManager :
	public IScriptObject
{
private:
	ViewCollider* mViewCollider;
	PlayerCollider* mPlayerCollider;
	std::vector<BlockCollider*> mBlockColliders;
	std::vector<FlyBlockCollider*> mFlyBlockColliders;

private:
	void RayHitFlyBlocks();
	void PlayerHitBlocks();

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

	DefDel;
};

RegisterScript(CollisionManager);

