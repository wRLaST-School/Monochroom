#pragma once
#include "IScriptObject.h"
#include <Object3D.h>

class SponePoint :
    public IScriptObject
{
private:
    Object3D* mParent = nullptr;

    const float kResponeYMin = -50.0f;

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

