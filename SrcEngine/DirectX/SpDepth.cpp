#include "SpDepth.h"
#include "SpWindow.h"
#include "SpDirectX.h"
#include <SpTextureManager.h>
#include <RTVManager.h>
#include <Util.h>
#include <format>

SpDepth wdp;

void SpDepth::Init()
{
	//cr HeapDesc
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 128;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	GetSpDX()->dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	CreateDSV("system_depth_default_bb", 1.f, 1.f);
}

void SpDepth::Resize()
{
	index = 0;
	for (auto& d : depthes) {
		auto dt = d.second;
		auto key = d.first;

		SpTextureManager::Release(d.first + "_depth_");

		dt.resource = SpTextureManager::GetTextureBuff(
			SpTextureManager::CreateResourceWithoutView((key + "_depth_"))
		);

		dt.index = index;

		SpTextureManager::TexData data;

		if (key != "system_depth_default_bb")
		{
			data = SpTextureManager::GetTextureData(key);
		}
		else
		{
			data.ratio.x = 1.f;
			data.ratio.y = 1.f;
		}

		float dx = data.ratio.x;
		float dy = data.ratio.y;

		SpTextureManager::CreateSRVOnResource((key + "_depth_"), DXGI_FORMAT_D32_FLOAT_S8X24_UINT, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, dx, dy);

		SpTextureManager::AddMasterTextureKey((key + "_depth_"));

		dt.resource = SpTextureManager::GetInstance().texBuffs[SpTextureManager::GetIndex(key + "_depth_")].Get();

		dt.resource->SetName(Util::StrToWStr(std::format("DEPTH:{}_depth_", key)).c_str());
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		GetSpDX()->dev->CreateDepthStencilView(dt.resource, &dsvDesc, GetHandleCPU(key));

		index++;
	}
}

void SpDepth::CreateDSV(std::string key, float dx, float dy)
{
	depthes.insert(std::pair<std::string, SpDepthForTex>(key, SpDepthForTex()));

	auto& dt = depthes[key];
	dt.resource = SpTextureManager::GetTextureBuff(
		SpTextureManager::CreateResourceWithoutView((key + "_depth_"))
	);

	dt.index = index;

	SpTextureManager::CreateSRVOnResource((key + "_depth_"), DXGI_FORMAT_D32_FLOAT_S8X24_UINT, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, dx, dy);

	SpTextureManager::AddMasterTextureKey((key + "_depth_"));

	dt.resource = SpTextureManager::GetInstance().texBuffs[SpTextureManager::GetIndex(key + "_depth_")].Get();

	dt.resource->SetName(Util::StrToWStr(std::format("DEPTH:{}_depth_", key)).c_str());
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	GetSpDX()->dev->CreateDepthStencilView(dt.resource, &dsvDesc, GetHandleCPU(key));
	index++;
}

D3D12_CPU_DESCRIPTOR_HANDLE SpDepth::GetHandleCPU(std::string key)
{
	auto handle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	handle.ptr += GetSpDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * depthes[key].index;

	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE SpDepth::GetHandleGPU(std::string key)
{
	auto handle = dsvHeap->GetGPUDescriptorHandleForHeapStart();

	handle.ptr += GetSpDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * depthes[key].index;

	return handle;
}

ID3D12Resource* SpDepth::GetDefault()
{
	return depthes[RTVManager::defaultRT].resource;
}

void InitSpDepth()
{
	wdp.Init();
}

SpDepth* GetSpDepth()
{
	return &wdp;
}
