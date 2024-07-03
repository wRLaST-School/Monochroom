#include "GrayScale.h"
#include <GPipeline.h>

GrayScaleInfo GrayScale::info;
SpConstBuffer<GraySclaeCB> GrayScale::cb(true);

void GrayScale::Init()
{
	std::string name = "GrayScale";
	RegisterRS(name);
	RegisterPipeline(name);

	cb.Create();
}

void GrayScale::Effect(const TextureKey& baseTex, const TextureKey& targetTex)
{
	if (info.isDraw)
	{
		GraySclaeCB cbdata;
		cbdata.offset = info.offset;

		*cb.contents = cbdata;
		 
		IPostEffector::Effect(baseTex, targetTex, "GrayScale",
			[&]()
			{
				GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, cb.buffer->GetGPUVirtualAddress());
			});
	}
}
