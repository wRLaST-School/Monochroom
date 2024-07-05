#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Util/Easing.h>

class GoggleScr :
    public IScriptObject
{
private:
    Object3D* mParent = nullptr;
    Object3D* mOwner = nullptr;

    const Vec3 kNormalPos = { 0,1.5f,0.0f };
    const Vec3 kEquipPos = { 0,0.01f,-0.22f };

    const float kRotOffsetY = PIf;

    Vec3 mStartPos = { 0,0,0 };
    Vec3 mEndPos = { 0,0,0 };
    Vec3 mLocalPos = { 0,0,0 };

    Vec3 mOwnerFrontVec = { 0,0,0 };

    bool mIsMoving = false;

    std::unique_ptr<Easing>mEasing = nullptr;

    const int kMoveTime = 40;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }

public:
    void GettedPlayer(Object3D* owner);

    void SetOwnerFrontVec(const Vec3& front) { mOwnerFrontVec = front; }

    void TakeOnOff(bool isEquip);

    DefDel;
};

RegisterScript(GoggleScr);

