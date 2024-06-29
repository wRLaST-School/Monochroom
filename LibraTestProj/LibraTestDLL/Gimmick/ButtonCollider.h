#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <OBBCollider.h>

class ButtonCollider :
    public IScriptObject
{
private:
    Object3D* mObj;
    OBBCollider mBodyCollider;
    OBBCollider mFlameCollider;
    Vec3 bodyColliderPos;
    Vec3 FlameColliderPos;
    Vec3 bodyColliderScale;
    Vec3 FlameColliderScale;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }
    void OnInspectorWindowDraw();

    DefDel;

public:
    OBBCollider GetBodyCollider();
    OBBCollider GetFlameCollider();
};

RegisterScript(ButtonCollider);

