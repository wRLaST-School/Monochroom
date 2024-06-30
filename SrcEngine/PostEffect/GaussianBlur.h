#pragma once
#include <IPostEffector.h>

struct GaussianBlurInfo
{
	bool isDraw = true;
};

class GaussianBlur : public IPostEffector
{
public:
	static GaussianBlurInfo info;

public:
	static void Init();
	static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);
};