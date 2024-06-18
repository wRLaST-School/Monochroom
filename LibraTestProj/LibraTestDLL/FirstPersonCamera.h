#pragma once
#include "IScriptObject.h"
#include <Object3D.h>

class FirstPersonCamera :
    public IScriptObject
{
private:
    Object3D* player;
    Object3D* obj;
    Vec3 offset;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
};

RegisterScript(FirstPersonCamera);