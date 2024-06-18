#pragma once
#include "IScriptObject.h"
#include <Object3D.h>

class PlayerControl :
    public IScriptObject
{
private:
    Object3D* obj;
    Vec3 frontVec;
    float moveSpeed;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
};

RegisterScript(PlayerControl);

