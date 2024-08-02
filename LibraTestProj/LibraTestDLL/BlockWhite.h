#pragma once
#include "IScriptObject.h"

class BlockWhite :
    public IScriptObject
{
public:
    enum Type
    {
        NORMAL,
        WHITE,
    };

    Type kType = WHITE;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }

    DefDel;
};

RegisterScript(BlockWhite);

