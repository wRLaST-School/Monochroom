#include "KawaseBloom.h"
#include <GPipeline.h>
#include <SpRootSignature.h>

void KawaseBloom::CreateRS()
{
	{
		SpRootSignature* rs = SpRootSignature::Register("KawaseBloomP2");

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

		rs->Create();
	}
	{
		SpRootSignature* rs = SpRootSignature::Register("KawaseBloomP3");

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

		rs->Create();
	}
}

void KawaseBloom::Init()
{
	CreateRS();

	RegisterRS("KawaseBloomP1");

	RegisterPipeline("KawaseBloomP1");
	RegisterPipeline("KawaseBloomP2");
	RegisterPipeline("KawaseBloomP3");

	RTVManager::CreateRenderTargetTexture(0.5f, 0.5f, "KawaseBloomP1", true);
	RTVManager::CreateRenderTargetTexture(0.25f, 0.25f, "KawaseBloomP1_Half", true);
	RTVManager::CreateRenderTargetTexture(0.5f, 0.5f, "KawaseBloomP2", true);
	RTVManager::CreateRenderTargetTexture(1.0f, 1.0f, "KawaseBloomP3", true);

	SpTextureManager::AddMasterTextureKey("KawaseBloomP1");
	SpTextureManager::AddMasterTextureKey("KawaseBloomP1_Half");
	SpTextureManager::AddMasterTextureKey("KawaseBloomP2");
	SpTextureManager::AddMasterTextureKey("KawaseBloomP3");
}

void KawaseBloom::Effect(const TextureKey& baseTex, const TextureKey& targetTex)
{
	// 輝度抽出
	IPostEffector::Effect(baseTex, "KawaseBloomP1", "KawaseBloomP1");
	IPostEffector::Effect(baseTex, "KawaseBloomP1_Half", "KawaseBloomP1");

	// ブラー
	IPostEffector::Effect("KawaseBloomP1", "KawaseBloomP2", "KawaseBloomP2", [&]()
		{
			// SR
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(2, SpTextureManager::GetGPUDescHandle("KawaseBloomP1_Half"));
		});


	// 合成
	IPostEffector::Effect(baseTex, targetTex, "KawaseBloomP3", [&]()
		{
			// SR
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(2, SpTextureManager::GetGPUDescHandle("KawaseBloomP2"));
		});
}
