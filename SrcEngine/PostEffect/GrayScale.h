#pragma once
#include <IPostEffector.h>
#include <SpConstBuffer.h>
#include <Easing.h>

struct GrayScaleInfo
{
	bool isDraw = false;
	const Vec2 kOffsetUV = { 0,-1.0f };
	float offsetRatio = 1.0f;
	float grayEffectRatio = 0;
	const Vec2 kGrayWindowSize = { 1280.0f,720.0f };
	const Vec2 kGrayPoint1 = { 230 / kGrayWindowSize.x,340 / kGrayWindowSize.y };
	const Vec2 kGrayPoint2 = { 1040 / kGrayWindowSize.x,340 / kGrayWindowSize.y };
	const float kGrayEffectRadius = 200.0f / kGrayWindowSize.x;
	float time = 0;

public:
	void Init()
	{
		offsetRatio = 1.0f;
		grayEffectRatio = 0;
	}
};

struct GraySclaeCB
{
	Vec2 offset;
	Vec2 grayPoint1;
	Vec2 grayPoint2;
	float grayEffectRadius;
	float time;
};

class GrayScale : public IPostEffector
{
private:
	static int32_t mGrayEffectTime;
	static int32_t mGrayEffectTimeMax;
	static bool mIsEquip;
	static bool mIsGrayEffect;
	static bool mIsGrayScale;

public:
	DLLExport static GrayScaleInfo info;
	static SpConstBuffer<GraySclaeCB> cb;
	DLLExport static bool mIsHavingGoggle;

public:
	static void Init();
	static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);

	static void EffectInit() { info.grayEffectRatio = 0; info.offsetRatio = 1.0f; info.isDraw = true; }

public:
	DLLExport static void BeginGrayEffect(bool isEquip, int32_t effectTime);
	DLLExport static bool GetIsEffect() { return mIsGrayEffect; }
	DLLExport static bool GetIsGrayScale() { return mIsGrayScale; }

private:
	static void GrayEffectUpdate();
};