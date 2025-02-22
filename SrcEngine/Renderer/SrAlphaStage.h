#pragma once
#include "IRenderStage.h"
#include <Object3D.h>
class SrAlphaStage :
    public IRenderStage
{
public:
    void Init();
    void PreDraw();
    void PostDraw();

    void Render();
    void DrawCommands(std::function<void(void)> cmd, TextureKey rt = RTVManager::defaultRT);

    //Renderの最初に呼ぶ
    void SortObjects();
    void RegisterAlphaObject(Object3D* obj);

private:
    std::list<Object3D*> objects_;
    std::list<Object3D*> sortedObj_;
};

