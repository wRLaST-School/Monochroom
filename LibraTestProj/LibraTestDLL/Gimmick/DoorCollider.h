#pragma once
#include "IScriptObject.h"

class DoorCollider :
    public IScriptObject
{
public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
};

RegisterScript(DoorCollider);

