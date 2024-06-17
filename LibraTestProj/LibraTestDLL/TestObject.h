#pragma once
#include "IScriptObject.h"
#include "Bases.h"
class TestObject :
    public IScriptObject
{
public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);
    DefDel;
};

RegisterScript(TestObject);