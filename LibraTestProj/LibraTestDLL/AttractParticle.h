#pragma once
#include "IScriptObject.h"
#include <AttractParticleEmitter.h>
#include <AttractParticleTmp.h>


class AttractParticleManager :
	public IScriptObject
{
private:
	std::unique_ptr<AttractParticleEmitter> mAttractParticleEmitter = nullptr;


public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

	DefDel;
};

RegisterScript(AttractParticleManager);

