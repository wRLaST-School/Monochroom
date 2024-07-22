#include "BlinkTransition.h"
#include <GPipeline.h>

SpConstBuffer<BlinkTransitionCB> BlinkTransition::cb(true);
BlinkTransitionInfo BlinkTransition::info;

void BlinkTransition::Init()
{
	std::string name = "BlinkTransition";
	RegisterRS(name);
	RegisterPipeline(name);

	cb.Create();
	cb.contents->effectTime = 0.f;
	cb.contents->effectTimeMax = 2.61f;

	info.speed = 0.01f;
	info.isInEnd = false;
	info.isOutEnd = false;
	info.isStart = false;
}

void BlinkTransition::Reset()
{
	cb.contents->effectTime = 0.f;
	cb.contents->effectTimeMax = 2.61f;
	info.speed = 0.025f;
	info.isInEnd = false;
	info.isOutEnd = false;
	info.isStart = false;
}

void BlinkTransition::Effect(const TextureKey& baseTex, const TextureKey& targetTex)
{
	IPostEffector::Effect(baseTex, targetTex, "BlinkTransition",
		[&]()
		{
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, cb.buffer->GetGPUVirtualAddress());
		});
}

void BlinkTransition::Start()
{
	cb.contents->effectTime = 0.f;
	info.isInEnd = false;
	info.isOutEnd = false;

	info.isStart = true;
}

void BlinkTransition::TransitionIn()
{
	if (info.isStart == false)
	{
		return;
	}

	if (info.isInEnd == false)
	{
		cb.contents->effectTime += info.speed;
	}

	if (cb.contents->effectTime >= cb.contents->effectTimeMax)
	{
		cb.contents->effectTime = cb.contents->effectTimeMax;
		info.isInEnd = true;
	}
}

void BlinkTransition::TransitionOut()
{
	if (info.isStart == false)
	{
		return;
	}

	if (info.isInEnd == false || info.isOutEnd == true)
	{
		return;
	}

	cb.contents->effectTime -= info.speed;
	if (cb.contents->effectTime <= 0.1f)
	{
		info.isInEnd = false;
		info.isOutEnd = true;
		info.isStart = false;
	}
	if (cb.contents->effectTime <= 0.0f)
	{
		cb.contents->effectTime = 0.f;
	}
}