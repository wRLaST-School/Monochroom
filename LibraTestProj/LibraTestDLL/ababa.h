#pragma once
#include "IScriptObject.h"

class ababa :
    public IScriptObject
{
public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
};

RegisterScript(ababa);

