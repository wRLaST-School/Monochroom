#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <SphereCollider.h>
#include <OBBCollider.h>

class FlyBlockCollider :
    public IScriptObject
{
private:
    Object3D* mObj;
    SphereCollider mBodyCollider;
    OBBCollider mDownCollider;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }

    DefDel;

public:
    SphereCollider GetBodyCollider();
    OBBCollider GetDownCollider();

    Object3D* GetObj() { return mObj; }
};

RegisterScript(FlyBlockCollider);

