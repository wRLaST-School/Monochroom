#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <OBBCollider.h>

class BlockCollider :
	public IScriptObject
{
private:
	Object3D* mObj;
	OBBCollider mBodyCollider;

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src);

	DefDel;

public:
	// ゲッター
	OBBCollider GetBodyCollider();
};

#ifndef BlockCollider_RegisterDef
#define BlockCollider_RegisterDef
RegisterScript(BlockCollider);
#endif

