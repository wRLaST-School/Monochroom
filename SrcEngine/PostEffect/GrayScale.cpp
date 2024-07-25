#include "GrayScale.h"
#include <GPipeline.h>
#include <SceneManager.h>
#include <SpRootSignature.h>
#include <ConsoleWindow.h>

GrayScaleInfo GrayScale::info;
SpConstBuffer<GraySclaeCB> GrayScale::cb(true);

int32_t GrayScale::mGrayEffectTime = 0;
bool GrayScale::mIsEquip = false;
bool GrayScale::mIsGrayEffect = false;

void GrayScale::Init()
{
	std::string name = "GrayScale";

	SpRootSignature* rsSSAO = SpRootSignature::Register(name);

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

	rsSSAO->Create();


	//RegisterRS(name);
	RegisterPipeline(name);

	cb.Create();
}

void GrayScale::Effect(const TextureKey& baseTex, const TextureKey& targetTex)
{
	if (info.isDraw)
	{
		GraySclaeCB cbdata;
		cbdata.offset = info.kOffsetUV * info.offsetRatio;
		cbdata.grayEffectRatio = info.grayEffectRatio;
		cbdata.grayPoint1 = info.kGrayPoint1;
		cbdata.grayPoint2 = info.kGrayPoint2;
		cbdata.grayEffectRadius = info.kGrayWindowSize.x / 1.5f * info.grayEffectRatio;

		ConsoleWindow::LogVec3("GRAY_OFFSET:", cbdata.offset);

		*cb.contents = cbdata;

		IPostEffector::Effect(baseTex, targetTex, "GrayScale",
			[&]()
			{
				GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, cb.buffer->GetGPUVirtualAddress());
				GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(2, SpTextureManager::GetGPUDescHandle("Goggle_Mask"));
			});
	}
}


//-------------------------------------------------------------------------------------------------------
void GrayScale::BeginGrayEffect(bool isEquip)
{
	mIsEquip = isEquip;
	mIsGrayEffect = true;
	mGrayEffectTime = 0;
}

void GrayScale::GrayEffectUpdate()
{
	if (mIsGrayEffect)
	{
		int32_t time = 0;

		if (mIsEquip)
		{
			time = mGrayEffectTime;
		}
		else
		{
			time = kGrayEffectTimeMax - mGrayEffectTime;
		}

		info.grayEffectRatio = (float)time / (float)kGrayEffectTimeMax;


		if (mGrayEffectTime >= kGrayEffectTimeMax)
		{
			mIsGrayEffect = false;
		}

		mGrayEffectTime++;
	}
}
