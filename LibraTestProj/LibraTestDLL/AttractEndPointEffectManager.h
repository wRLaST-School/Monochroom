#pragma once
#include "IScriptObject.h"
#include "AttractEndPointEffect.h"
#include "FlyBlock.h"


class AttractEndPointEffectManager :
    public IScriptObject
{
private:
    std::unique_ptr<std::map<FlyBlock*, std::unique_ptr<AttractEndPointEffect>>> mAttractEndPointEffects = nullptr;

public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src) { src; }

public:
    void GenerateEffect(FlyBlock* fb, const Vec3& ePos);


    DefDel;
};

RegisterScript(AttractEndPointEffectManager);

