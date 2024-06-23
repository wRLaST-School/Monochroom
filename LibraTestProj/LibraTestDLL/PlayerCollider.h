#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <OBBCollider.h>

class PlayerCollider :
    public IScriptObject
{
private:
    Object3D* mObj;
    OBBCollider mDownCollider;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
};

RegisterScript(PlayerCollider);

