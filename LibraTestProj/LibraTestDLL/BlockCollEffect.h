#pragma once
#include "IScriptObject.h"
#include <SpDS.h>
#include <SpTextureManager.h>
#include <Object3D.h>
#include "Easing.h"

class BlockCollEffect :
    public IScriptObject
{
public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    void SetHitBlockPos(Vec3 pos);

    void SetIsHit(bool IsHit);

    void SetHitRota(Vec3 rota);

    DefDel;

private:

    bool mIsHit;

    Object3D* mBlockEffectPlane; 
    
    Object3D* mCamera;

    Easing mBlockEffectEase;

    TextureKey mSquareTex;

    Vec3 mBlockPos;

    Vec3 mRota;

    Vec3 mBlockNowScale;

    Vec3 mBlockBeforeScale;

    Vec3 mBlockAfterScale;


};
RegisterScript(BlockCollEffect);
