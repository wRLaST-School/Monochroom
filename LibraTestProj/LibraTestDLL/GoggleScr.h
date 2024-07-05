#pragma once
#include "IScriptObject.h"
#include <Object3D.h>

class GoggleScr :
    public IScriptObject
{
private:
    Object3D* mParent = nullptr;
    Object3D* mOwner = nullptr;

    Vec3 mLocalPos = { 0,1.0f,0.3f };
    Vec3 mLocalEquipPos = { 0,0,0.2f };

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }

public:
    void GettedPlayer(Object3D* owner);

    DefDel;
};

RegisterScript(GoggleScr);

