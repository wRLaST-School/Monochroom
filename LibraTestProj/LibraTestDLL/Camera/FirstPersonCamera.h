#pragma once
#include "IScriptObject.h"
#include <Object3D.h>

class FirstPersonCamera :
    public IScriptObject
{
private:
    Object3D* player;
    Object3D* parentObj;
    const Vec3 OFFSET = Vec3(0, 0, 1);

    bool isSet = false;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
};

RegisterScript(FirstPersonCamera);