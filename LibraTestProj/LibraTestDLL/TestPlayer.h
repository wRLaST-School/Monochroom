#pragma once
#include "IScriptObject.h"
#include <string>
#include "Bases.h"

class TestPlayer :
    public IScriptObject
{
public:
    void Init();
    void Update();
    void Draw();

private:
    int count = 0;
};

RegisterScript(TestPlayer);
