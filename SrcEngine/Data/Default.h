#pragma once
#include "IScriptObject.h"

class Default :
    public IScriptObject
{
public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }

    DefDel;
};

RegisterScript(Default);
