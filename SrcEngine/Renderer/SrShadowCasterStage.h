#pragma once
#include "IRenderStage.h"
#include <Camera.h>
#include <memory>
class SrShadowCasterStage :
    public IRenderStage
{
public:
    static std::unique_ptr<Camera> lightCamera;

public:
    void Init();
    void PreDraw();
    void PostDraw();

    void Render();
    void DrawCommands(std::function<void(void)> cmd, TextureKey rt);
};
