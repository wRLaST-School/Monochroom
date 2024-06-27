#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <OBBCollider.h>

class DoorCollider :
    public IScriptObject
{
private:
    Object3D* mObj;
    Object3D* mLeft;
    Object3D* mRight;
    OBBCollider mLeftCollider;
    OBBCollider mRightCollider;
    Vec3 leftColliderPos;
    Vec3 rightColliderPos;
    Vec3 colliderScale;

public:
    void Init();
    void Update();
    void Draw();
    void OnInspectorWindowDraw();
    void CopyComponent(IComponent* src) { src; }

    DefDel;
};

RegisterScript(DoorCollider);

