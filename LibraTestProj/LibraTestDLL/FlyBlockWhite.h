#pragma once
#include "IScriptObject.h"
#include "FlyBlock.h"


class FlyBlockWhite :
    public FlyBlock
{
private:


public:
    void Init() override;
    void Update() override;
    void Draw();
    void CopyComponent(IComponent* src) { src; }

    DefDel;
};

RegisterScript(FlyBlockWhite);

