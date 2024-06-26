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
    OBBCollider mFrameCollider;
    Vec3 bodyColliderPos;
    Vec3 FrameColliderPos;
    Vec3 bodyColliderScale;
    Vec3 FrameColliderScale;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }
    void OnInspectorWindowDraw();

    DefDel;

public:
    OBBCollider GetBodyCollider();
    OBBCollider GetFrameCollider();
};

RegisterScript(ButtonCollider);

