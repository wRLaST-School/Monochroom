#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <OBBCollider.h>
#include <SphereCollider.h>

class PlayerCollider :
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

};

#ifndef PlayerCollider_RegisterDef
#define PlayerCollider_RegisterDef
RegisterScript(PlayerCollider);

#endif