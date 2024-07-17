#pragma once
#include <IPostEffector.h>

class StageGenerating : public IPostEffector
{
public:
	static void CreateRS();
	static void Init();
	static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);
};

