#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <OBBCollider.h>

class BlockCollider :
    public IScriptObject
{
private:
    Object3D* obj;
    OBBCollider bodyCollider;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
};

RegisterScript(BlockCollider);

