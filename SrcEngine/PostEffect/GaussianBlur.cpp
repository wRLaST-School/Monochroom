#include "stdafx.h"
#include "GaussianBlur.h"

GaussianBlurInfo GaussianBlur::info;

void GaussianBlur::Init()
{
	std::string name = "GaussianBlur";
	RegisterRS(name);
	RegisterPipeline(name);
}

void GaussianBlur::Effect(const TextureKey& baseTex, const TextureKey& targetTex)
{
	if (info.isDraw)
	{
		IPostEffector::Effect(baseTex, targetTex, "GaussianBlur");
	}
}
