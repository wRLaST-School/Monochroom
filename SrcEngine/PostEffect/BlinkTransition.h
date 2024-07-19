#pragma once
#include <IPostEffector.h>
#include <SpConstBuffer.h>

struct BlinkTransitionCB
{
	float effectTime;
	float effectTimeMax;
};

struct BlinkTransitionInfo
{
	float speed;
	bool isInEnd;
	bool isOutEnd;
	bool isStart;
};

class BlinkTransition : public IPostEffector
{
public:
	DLLExport static BlinkTransitionInfo info;

public:
	static SpConstBuffer<BlinkTransitionCB> cb;
	static void Init();
	DLLExport static void Reset();
	static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);

public:
	DLLExport static void Start();
	DLLExport static void TransitionIn();
	DLLExport static void TransitionOut();

};

