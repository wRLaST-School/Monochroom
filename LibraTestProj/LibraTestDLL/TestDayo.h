#pragma once
#include "IScriptObject.h"

class TestDayo :
    public IScriptObject
{
public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
};

RegisterScript(TestDayo);

