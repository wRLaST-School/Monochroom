#pragma once
#include "IScene.h"
#include <DLLDef.h>
class SceneFromFile :
    public IScene
{
public:
    DLLExport SceneFromFile(std::string filePath);
    DLLExport ~SceneFromFile() {};
    DLLExport void LoadResources();
    DLLExport void Init();
    void Update();
    void DrawBack();
    void Draw3D();
    void DrawSprite();

private:
    std::string filePath_;
};

