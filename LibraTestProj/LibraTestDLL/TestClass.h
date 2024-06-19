#pragma once
#include "IScriptObject.h"

class TestClass :
    public IScriptObject
{
public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
};

RegisterScript(TestClass);

