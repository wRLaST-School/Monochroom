#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Util/Timer.h>


class FirstPersonCamera :
    public IScriptObject
{
private:
    Object3D* player;
    Object3D* parentObj;
    const Vec3 OFFSET = Vec3(0, 0, 1);

    float mMoveLength = 0;

    float mTimer = 0;

    const float kMoveWigglePosLength = 0.0393f;
    const float kMoveWigglePosSpeed = 0.41f;
    const float kMoveWiggleRotLength = PIf * 0.0009f;
    const float kMoveWiggleRotSpeed = 0.35f;

    bool isSet = false;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
};

RegisterScript(FirstPersonCamera);