#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <SphereCollider.h>
#include <OBBCollider.h>
#include <FlyBlock.h>

class FlyBlockCollider :
    public IScriptObject
{
private:
    Object3D* mObj;
    FlyBlock* mFlyBlock;
    SphereCollider mMoveCollider;
    OBBCollider mBodyCollider;
    OBBCollider mDownCollider;
    OBBCollider mTopCollider;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }

    DefDel;

public:
    SphereCollider GetMoveCollider();
    OBBCollider GetBodyCollider();
    OBBCollider GetDownCollider();
    OBBCollider GetTopCollider();

    Object3D* GetObj() { return mObj; }
};

RegisterScript(FlyBlockCollider);

