#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <SphereCollider.h>


class GoggleCollider :
    public IScriptObject
{
private:
    Object3D* mObj;
    SphereCollider mSphereCollider;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }

public:
    SphereCollider GetSphereCollider() { return mSphereCollider; }

    DefDel;
};

RegisterScript(GoggleCollider);

