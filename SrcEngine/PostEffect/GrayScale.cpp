#include "GrayScale.h"

GrayScaleInfo GrayScale::info;

void GrayScale::Init()
{
	std::string name = "GrayScale";
	RegisterRS(name);
	RegisterPipeline(name);
}

void GrayScale::Effect(const TextureKey& baseTex, const TextureKey& targetTex)
{
	if (info.isDraw)
	{
		IPostEffector::Effect(baseTex, targetTex, "GrayScale");
	}
}
