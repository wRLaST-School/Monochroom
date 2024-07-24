#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Util/Timer.h>
#include <Camera.h>


class FirstPersonCamera :
    public IScriptObject
{
private:
    Object3D* player;
    Camera* mParentObj;
    const Vec3 OFFSET = Vec3(0, 0, 1);

    float mMoveLength = 0;

    float mTimer = 0;

    const float kMoveWigglePosLength = 0.052f;
    const float kMoveWigglePosSpeed = 0.365f;
    const float kMoveWiggleRotLength = PIf * 0.0009f;
    const float kMoveWiggleRotSpeed = 0.35f;

    bool isSet = false;

public:
    void Init();
    void Update();
    void CameraUpdate();
    void CameraPosRotUpdateOnly();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;
};

RegisterScript(FirstPersonCamera);