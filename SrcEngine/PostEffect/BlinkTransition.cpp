#include "BlinkTransition.h"
#include <GPipeline.h>

SpConstBuffer<BlinkTransitionCB> BlinkTransition::cb(true);

void BlinkTransition::Init()
{
	std::string name = "BlinkTransition";
	RegisterRS(name);
	RegisterPipeline(name);

	cb.Create();
	cb.contents->effectTime = 0.f;
	cb.contents->effectTimeMax = 2.61f;
}

void BlinkTransition::Effect(const TextureKey& baseTex, const TextureKey& targetTex)
{
	IPostEffector::Effect(baseTex, targetTex, "BlinkTransition",
		[&]()
		{
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, cb.buffer->GetGPUVirtualAddress());
		});
}
