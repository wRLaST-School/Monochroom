#include "StageGenerating.h"
#include <GPipeline.h>
#include <SpRootSignature.h>

StageGeenratingInfo StageGenerating::info;

void StageGenerating::CreateRS()
{
	SpRootSignature* rs = SpRootSignature::Register("StageGenerating");

	rs->UseDefaultSettings();

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
	rs->params.emplace_back();
	rs->params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rs->params[0].Descriptor.ShaderRegister = 0;
	rs->params[0].Descriptor.RegisterSpace = 0;
	rs->params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//テクスチャレジスタ0番
	rs->params.emplace_back();
	rs->params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rs->params[1].DescriptorTable.pDescriptorRanges = &descRange;
	rs->params[1].DescriptorTable.NumDescriptorRanges = 1;
	rs->params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//テクスチャレジスタ0番
	rs->params.emplace_back();
	rs->params[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rs->params[2].DescriptorTable.pDescriptorRanges = &descRange2;
	rs->params[2].DescriptorTable.NumDescriptorRanges = 1;
	rs->params[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//テクスチャレジスタ0番
	rs->params.emplace_back();
	rs->params[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rs->params[3].DescriptorTable.pDescriptorRanges = &descRange3;
	rs->params[3].DescriptorTable.NumDescriptorRanges = 1;
	rs->params[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rs->Create();
}

void StageGenerating::Init()
{
	CreateRS();

	std::string name = "StageGenerating";
	RegisterPipeline(name);

	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "StageGenerating", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "Plane", true);

	SpTextureManager::AddMasterTextureKey("StageGenerating");
	SpTextureManager::AddMasterTextureKey("Plane");
}

void StageGenerating::Effect(const TextureKey& baseTex, const TextureKey& targetTex)
{
	if (!info.isDraw)
	{
		return;
	}

	IPostEffector::Effect(baseTex, targetTex, "StageGenerating", [&]()
		{
			// SR
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(2, SpTextureManager::GetGPUDescHandle(RTVManager::defaultRT + "_depth_"));
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(3, SpTextureManager::GetGPUDescHandle("Plane_depth_"));
		});
}
