#pragma once
#include "IScriptObject.h"
#include <AttractParticleEmitter.h>
#include <AttractParticleTmp.h>
#include <FlyBlock.h>


class AttractParticleManager :
	public IScriptObject
{
private:
	//ブロックとそのエフェクトのセット
	std::unique_ptr<std::map<FlyBlock*, std::unique_ptr<AttractParticleEmitter>>> mFlyBlockAndAttractEffect = nullptr;


public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

public:
	void BeginAttractEffect(FlyBlock* fbc, const Vec3& sPos, const Vec3& ePos);

	DefDel;
};

RegisterScript(AttractParticleManager);

