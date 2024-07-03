#pragma once
#include <IPostEffector.h>
#include <SpConstBuffer.h>

struct GrayScaleInfo
{
	bool isDraw = true;
	Vec2 offset;
};

struct GraySclaeCB
{
	Vec2 offset;
};

class GrayScale : public IPostEffector
{
public:
	static GrayScaleInfo info;
	static SpConstBuffer<GraySclaeCB> cb;

public:
	static void Init();
	static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);
};