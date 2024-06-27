#pragma once
#include <IPostEffector.h>

struct GrayScaleInfo
{
	bool isDraw = true;
};

class GrayScale : public IPostEffector
{
public:
	static GrayScaleInfo info;

public:
	static void Init();
	static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);
};