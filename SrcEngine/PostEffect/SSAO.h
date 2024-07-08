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
	static std::string name;
	static void Init();
	static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);

	static SpConstBuffer<SSAOData> cb;
};