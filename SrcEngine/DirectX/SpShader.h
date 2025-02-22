#pragma once
#include "Essentials.h"
#include "SpDirectX.h"
class SpShader
{
public:
	DLLExport void InitVS(const std::string& vsPath);
	DLLExport void InitPS(const std::string& psPath);
	DLLExport void InitGS(const std::string& gsPath);

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob = nullptr; // ピクセルシェーダオブジェクト

	std::string id;
	std::string vspath;
	std::string pspath;
	std::string gspath;
};

DLLExport void RegisterShader(const std::string& id);
DLLExport void RegisterShader(SpShader* shader, const std::string& id);
DLLExport void InitVS(const std::string& id, const std::string& vsPath);
DLLExport void InitPS(const std::string& id, const std::string& psPath);
DLLExport void InitGS(const std::string& id, const std::string& gsPath);
DLLExport SpShader* GetShader(const std::string& id);
DLLExport bool ReCompile(const std::string& id);

