#pragma once
#include "IScriptObject.h"
#include "FlyBlock.h"


class FlyBlockWhite :
    public FlyBlock
{
private:


public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }

    DefDel;
};

RegisterScript(FlyBlockWhite);

