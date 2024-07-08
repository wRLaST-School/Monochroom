#include "SSAO.h"
#include <SpRootSignature.h>
#include <GPipeline.h>
#include <Camera.h>
#include <SpTextureManager.h>

SpConstBuffer<SSAOData> SSAO::cb(true);
std::string SSAO::name;

void SSAO::Init()
{
	name = "SSAO";

	// RSはManagerの方で作成した
	RegisterPipeline(name);

	cb.Create();
}

void SSAO::Effect(const TextureKey& baseTex, const TextureKey& targetTex)
{
	//Matrix viewMat = Camera::sCurrent->GetViewMat();
	Matrix projMat = Camera::sCurrent->GetProjMat();
	cb.contents->projMat = projMat;
	cb.contents->invProjMat = -projMat;

	IPostEffector::Effect(baseTex, targetTex, name,
		[&]()
		{
			// CB
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, cb.buffer->GetGPUVirtualAddress());

			// SR
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(2, SpTextureManager::GetGPUDescHandle(baseTex + "_depth_"));
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(3, SpTextureManager::GetGPUDescHandle("NormalMap"));
		});
}