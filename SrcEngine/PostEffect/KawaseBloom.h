#pragma once
#include <IPostEffector.h>
#include <SpConstBuffer.h>

class KawaseBloom : public IPostEffector
{
public:
	static void CreateRS();
	static void Init();
	static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);
};