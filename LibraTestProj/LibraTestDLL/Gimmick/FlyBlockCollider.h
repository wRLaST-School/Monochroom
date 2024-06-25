#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <SphereCollider.h>

class FlyBlockCollider :
    public IScriptObject
{
private:
    Object3D* mObj;
    SphereCollider mBodyCollider;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }

    DefDel;

public:
    SphereCollider GetBodyCollider();
};

RegisterScript(FlyBlockCollider);

