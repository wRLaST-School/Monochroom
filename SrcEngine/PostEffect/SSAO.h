#pragma once
#include "IPostEffector.h"
#include <SpConstBuffer.h>

struct SSAOData
{
	Matrix projMat;
	Matrix invProjMat;
};

class SSAO :
	public IPostEffector
{
public:
	static std::string name1;
	static std::string name2;
	static void CreateRS();
	static void Init();
	static void EffectAO(const TextureKey& baseTex, const TextureKey& targetTex);
	static void EffectBilateralFilter(const TextureKey& baseTex, const TextureKey& aoTex, const TextureKey& targetTex);

	static SpConstBuffer<SSAOData> cb;
};