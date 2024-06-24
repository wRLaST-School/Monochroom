#pragma once
#include "IScriptObject.h"
#include <PlayerCollider.h>
#include <BlockCollider.h>

class CollisionManager :
	public IScriptObject
{
private:
	PlayerCollider* mPlayerCollider;
	std::vector<BlockCollider*> mBlockColliders;

private:
	void PlayerHitBlocks();

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

	DefDel;
};

RegisterScript(CollisionManager);

