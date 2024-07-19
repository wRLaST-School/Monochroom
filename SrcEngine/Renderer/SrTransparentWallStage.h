#pragma once
#include "IRenderStage.h"
class SrTransparentWallStage :
    public IRenderStage
{
public:

public:
    void Init();
    void PreDraw();
    void PostDraw();

    void Render();
    void DrawCommands(std::function<void(void)> cmd, TextureKey rt);
};

