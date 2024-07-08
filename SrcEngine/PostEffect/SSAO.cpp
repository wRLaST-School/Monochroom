#include "SSAO.h"
#include <SpRootSignature.h>
#include <GPipeline.h>
#include <Camera.h>
#include <SpTextureManager.h>

SpConstBuffer<SSAOData> SSAO::cb(true);
std::string SSAO::name1;
std::string SSAO::name2;

void SSAO::CreateRS()
{
#pragma region SSAO RS
	{
		SpRootSignature* rsSSAO = SpRootSignature::Register("SSAO");

		rsSSAO->UseDefaultSettings();

		D3D12_DESCRIPTOR_RANGE descRange{};
		descRange.NumDescriptors = 1;
		descRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange.BaseShaderRegister = 0;
		descRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_DESCRIPTOR_RANGE descRange2{};
		descRange2.NumDescriptors = 1;
		descRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange2.BaseShaderRegister = 1;
		descRange2.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_DESCRIPTOR_RANGE descRange3{};
		descRange3.NumDescriptors = 1;
		descRange3.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange3.BaseShaderRegister = 2;
		descRange3.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//定数バッファ0番スプライト用
		rsSSAO->params.emplace_back();
		rsSSAO->params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rsSSAO->params[0].Descriptor.ShaderRegister = 0;
		rsSSAO->params[0].Descriptor.RegisterSpace = 0;
		rsSSAO->params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//テクスチャレジスタ0番
		rsSSAO->params.emplace_back();
		rsSSAO->params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rsSSAO->params[1].DescriptorTable.pDescriptorRanges = &descRange;
		rsSSAO->params[1].DescriptorTable.NumDescriptorRanges = 1;
		rsSSAO->params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//テクスチャレジスタ0番
		rsSSAO->params.emplace_back();
		rsSSAO->params[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rsSSAO->params[2].DescriptorTable.pDescriptorRanges = &descRange2;
		rsSSAO->params[2].DescriptorTable.NumDescriptorRanges = 1;
		rsSSAO->params[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//テクスチャレジスタ0番
		rsSSAO->params.emplace_back();
		rsSSAO->params[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rsSSAO->params[3].DescriptorTable.pDescriptorRanges = &descRange3;
		rsSSAO->params[3].DescriptorTable.NumDescriptorRanges = 1;
		rsSSAO->params[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		rsSSAO->Create();
	}
#pragma endregion

#pragma region BilateralFilter RS
	{
		SpRootSignature* rsBilateralFilter = SpRootSignature::Register("BilateralFilter");

		rsBilateralFilter->UseDefaultSettings();

		D3D12_DESCRIPTOR_RANGE descRange{};
		descRange.NumDescriptors = 1;
		descRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange.BaseShaderRegister = 0;
		descRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_DESCRIPTOR_RANGE descRange2{};
		descRange2.NumDescriptors = 1;
		descRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange2.BaseShaderRegister = 1;
		descRange2.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_DESCRIPTOR_RANGE descRange3{};
		descRange3.NumDescriptors = 1;
		descRange3.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange3.BaseShaderRegister = 2;
		descRange3.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//定数バッファ0番スプライト用
		rsBilateralFilter->params.emplace_back();
		rsBilateralFilter->params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rsBilateralFilter->params[0].Descriptor.ShaderRegister = 0;
		rsBilateralFilter->params[0].Descriptor.RegisterSpace = 0;
		rsBilateralFilter->params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//テクスチャレジスタ0番
		rsBilateralFilter->params.emplace_back();
		rsBilateralFilter->params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rsBilateralFilter->params[1].DescriptorTable.pDescriptorRanges = &descRange;
		rsBilateralFilter->params[1].DescriptorTable.NumDescriptorRanges = 1;
		rsBilateralFilter->params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//テクスチャレジスタ0番
		rsBilateralFilter->params.emplace_back();
		rsBilateralFilter->params[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rsBilateralFilter->params[2].DescriptorTable.pDescriptorRanges = &descRange2;
		rsBilateralFilter->params[2].DescriptorTable.NumDescriptorRanges = 1;
		rsBilateralFilter->params[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//テクスチャレジスタ0番
		rsBilateralFilter->params.emplace_back();
		rsBilateralFilter->params[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rsBilateralFilter->params[3].DescriptorTable.pDescriptorRanges = &descRange3;
		rsBilateralFilter->params[3].DescriptorTable.NumDescriptorRanges = 1;
		rsBilateralFilter->params[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		rsBilateralFilter->Create();
	}
#pragma endregion
}

void SSAO::Init()
{
	CreateRS();

	name1 = "SSAO";
	RegisterPipeline(name1);
	cb.Create();

	name2 = "BilateralFilter";
	RegisterPipeline(name2);
}

void SSAO::EffectAO(const TextureKey& baseTex, const TextureKey& targetTex)
{
	Matrix projMat = Camera::sCurrent->GetProjMat();
	cb.contents->projMat = projMat;
	cb.contents->invProjMat = -projMat;

	IPostEffector::Effect(baseTex, targetTex, name1,
		[&]()
		{
			// CB
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, cb.buffer->GetGPUVirtualAddress());

			// SR
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(2, SpTextureManager::GetGPUDescHandle(baseTex + "_depth_"));
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(3, SpTextureManager::GetGPUDescHandle("NormalMap"));
		});
}

void SSAO::EffectBilateralFilter(const TextureKey& baseTex, const TextureKey& aoTex, const TextureKey& targetTex)
{
	IPostEffector::Effect(baseTex, targetTex, name2,
		[&]()
		{
			// SR
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(2, SpTextureManager::GetGPUDescHandle(aoTex));
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(3, SpTextureManager::GetGPUDescHandle("NormalMap"));
		});
}
