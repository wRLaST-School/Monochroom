#include "RGBShift.h"
#include <GPipeline.h>
#include <random>
#include <ConsoleWindow.h>
#include <SpRootSignature.h>

SpConstBuffer<RGBShiftCB> RGBShift::cb(true);
float RGBShift::timeAccel = 0;
float RGBShift::startTimer = 0;
float RGBShift::maxTimer = 0;

float Random(float min, float max)
{
	std::random_device rd;  // ハードウェア乱数生成器を初期化
	std::mt19937 gen(rd()); // メルセンヌ・ツイスタ乱数生成器を初期化
	std::uniform_real_distribution<float> dis(min, max); // minからmaxの範囲で一様分布を設定

	return dis(gen); // 乱数を生成して返す
}

void RGBShift::CreateRS()
{
	SpRootSignature* rs = SpRootSignature::Register("Composite");

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

void RGBShift::Init()
{
	CreateRS();

	std::string name = "RGBShift";
	RegisterRS(name);
	RegisterPipeline(name);

	name = "Composite";
	RegisterPipeline(name);

	cb.Create();

	cb.contents->shiftR = Vec2(0.006f, 0.0f);
	cb.contents->shiftB = Vec2(-0.008f, 0.0f);

	timeAccel = Random(0.01f, 0.1f);
	startTimer = Random(0, 5);
	maxTimer = std::floorf(Random(10, 100));
}

void RGBShift::Effect(const TextureKey& baseTex, const TextureKey& targetTex)
{
	cb.contents->timer += timeAccel;
	if (cb.contents->timer >= maxTimer)
	{
		timeAccel = Random(0.01f, 0.1f);
		startTimer = Random(0.0f, 0.9f);
		maxTimer = std::floorf(Random(1, 10));
		cb.contents->timer = startTimer;
	}

	cb.contents->shiftR.x = Random(0.001f, 0.01f);
	cb.contents->shiftR.y = Random(-0.005f, 0.005f);
	cb.contents->shiftB.x = -Random(0.001f, 0.01f);
	cb.contents->shiftB.y = -Random(-0.005f, 0.005f);

	IPostEffector::Effect(baseTex, "RGBShift", "RGBShift",
		[&]()
		{
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, cb.buffer->GetGPUVirtualAddress());
		});

	IPostEffector::Effect("RGBShift", targetTex, "Composite",
		[&]()
		{
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, cb.buffer->GetGPUVirtualAddress());
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(2, SpTextureManager::GetGPUDescHandle(RTVManager::defaultRT));
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(3, SpTextureManager::GetGPUDescHandle("RGBShiftMask"));
		});

	//RTVManager::defaultRT

	//ConsoleWindow::Log(std::format("cb.contents->timer : {}", cb.contents->timer).c_str());
}
