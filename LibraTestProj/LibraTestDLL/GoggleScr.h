#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Util/Easing.h>

class GoggleScr :
    public IScriptObject
{
private:
    Object3D* mParent = nullptr;
    Object3D* mLensL = nullptr;
    Object3D* mLensR = nullptr;
    Object3D* mOwner = nullptr;

    const Vec3 kLensColor = { 0.3f,0.3f,0.3f };
    const float kLensAlpha = 0.7f;

    const Vec3 kNormalPos = { 0,1.1f,0.42f };
    const Vec3 kEquipPos = { 0,-0.3f,0.42f };

    const float kRotOffsetY = PIf;

    Vec3 mStartPos = { 0,0,0 };
    Vec3 mEndPos = { 0,0,0 };
    Vec3 mLocalPos = { 0,0,0 };

    Vec3 mOwnerFrontVec = { 0,0,0 };

    float mAlpha = 0;

    bool mIsMoving = false;
    bool mIsEquip = false;

    std::unique_ptr<Easing>mEasing = nullptr;

    const int kOnMoveTime = 40;
    const int kOffMoveTime = 18;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }

public:
    void GettedPlayer(Object3D* owner);

    void SetOwnerFrontVec(const Vec3& front) { mOwnerFrontVec = front; }

    void TakeOnOff(bool isEquip);
    void Equip();

    bool GetIsEquip() { return mIsEquip; }
    bool GetIsMoving() { return mIsMoving; }

    float GetPosRatio();

    DefDel;
};

RegisterScript(GoggleScr);

