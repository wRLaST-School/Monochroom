#pragma once
#include "IScriptObject.h"

class ButtonCollider :
    public IScriptObject
{
public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
};

RegisterScript(ButtonCollider);

