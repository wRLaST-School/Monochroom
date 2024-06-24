#pragma once
#include "IScriptObject.h"

class FlyBlockCollider :
    public IScriptObject
{
public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
};

RegisterScript(FlyBlockCollider);

