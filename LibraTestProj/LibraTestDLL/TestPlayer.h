#pragma once
#include "IScriptObject.h"
#include <string>
#include "Bases.h"

class TestPlayer :
    public IScriptObject
{
public:
    void Awake()override;
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
private:
    int count = 0;
};

RegisterScript(TestPlayer);
