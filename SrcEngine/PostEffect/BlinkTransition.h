#pragma once
#include <IPostEffector.h>
#include <SpConstBuffer.h>

struct BlinkTransitionCB
{
	float effectTime;
	float effectTimeMax;
};

class BlinkTransition : public IPostEffector
{
public:
	static SpConstBuffer<BlinkTransitionCB> cb;
	static void Init();
	static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);

};

