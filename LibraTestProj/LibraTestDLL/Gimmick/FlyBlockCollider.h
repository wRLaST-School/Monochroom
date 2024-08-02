#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <SphereCollider.h>
#include <OBBCollider.h>
#include <OBBCollider.h>
#include <FlyBlock.h>
#include <Timer.h>

class FlyBlockCollider :
	public IScriptObject
{
private:
	Object3D* mObj;
	FlyBlock* mFlyBlock;
	OBBCollider mMoveCollider;
	OBBCollider mBodyCollider;
	OBBCollider mDownCollider;
	OBBCollider mTopCollider;
	Timer mTimer;

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

	void IsMoveing();

	DefDel;

public:
	OBBCollider GetMoveCollider();
	OBBCollider GetBodyCollider();
	OBBCollider GetDownCollider();
	OBBCollider GetTopCollider();

	Object3D* GetObj() { return mObj; }
};

RegisterScript(FlyBlockCollider);

