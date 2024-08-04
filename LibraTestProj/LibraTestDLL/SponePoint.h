#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Easing.h>

class SponePoint :
    public IScriptObject
{
private:
    Object3D* mParent = nullptr;

    const float kResponeYMin = -70.0f;

    const int16_t kErasedTimeMax = 240;
    std::unique_ptr<Easing> mEasing = nullptr;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }

public:
    void SponePlayer();

    DefDel;
};

RegisterScript(SponePoint);

