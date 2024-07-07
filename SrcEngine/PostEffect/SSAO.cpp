#include "SSAO.h"
#include <SpRootSignature.h>
#include <GPipeline.h>
#include <Camera.h>

SpConstBuffer<SSAOData> SSAO::cb(true);
std::string SSAO::name;

void SSAO::Init()
{
	name = "SSAO";

	RegisterRS(name);
	RegisterPipeline(name);

	cb.Create();
}

void SSAO::Effect(const TextureKey& baseTex, const TextureKey& targetTex)
{
	Matrix viewProj = Camera::sCurrent->GetViewMat() * Camera::sCurrent->GetProjMat();
	cb.contents->invViewProjMat = -viewProj;

	IPostEffector::Effect(baseTex, targetTex, name, [&]() {GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, cb.buffer->GetGPUVirtualAddress()); });
}