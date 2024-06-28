#pragma once
#include "Essentials.h"
class SpDepthForTex;

class SpDepth
{
public:
	void Init();
	void Resize();

	void CreateDSV(std::string key);

	D3D12_CPU_DESCRIPTOR_HANDLE GetHandleCPU(std::string key);
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandleGPU(std::string key);

	eastl::map<std::string, SpDepthForTex> depthes;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

	size_t index = 0;

	ID3D12Resource* GetDefault();
};

class SpDepthForTex
{
public:
	void Init();
	void Resize();

	ID3D12Resource* resource;
	size_t index = 0;
};

void InitSpDepth();
SpDepth* GetSpDepth();

