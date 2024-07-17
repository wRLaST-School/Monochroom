#pragma once
#include <IPostEffector.h>
#include <SpConstBuffer.h>

struct RGBShiftCB
{
	Vec2 shiftR;
	Vec2 shiftG;
	Vec2 shiftB;
	float timer;
};

class RGBShift : public IPostEffector
{
private:
	static float timeAccel;
	static float startTimer;
	static float maxTimer;

public:
	static SpConstBuffer<RGBShiftCB> cb;

	static void Init();
	static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);
};