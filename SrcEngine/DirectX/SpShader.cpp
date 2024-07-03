#include "SpShader.h"
#include "GPipeline.h"

using namespace std;

map<string, SpShader> shaderMap;

void SpShader::InitVS(const string& vsPath)
{
	if (vsPath.empty())
	{
		return;
	}

	string shaderFolder = "Assets/Shaders/";
	string vsPath_ = shaderFolder + vsPath;
	//シェーダーの読み込みとコンパイル
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	HRESULT result;
	result = D3DCompileFromFile(
		wstring(vsPath_.begin(), vsPath_.end()).c_str(),  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	vspath = vsPath;
}

void SpShader::InitPS(const string& psPath)
{
	if (psPath.empty())
	{
		return;
	}

	string shaderFolder = "Assets/Shaders/";
	string psPath_ = shaderFolder + psPath;
	//シェーダーの読み込みとコンパイル
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	HRESULT result;
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		wstring(psPath_.begin(), psPath_.end()).c_str(),   // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	pspath = psPath;
}

void SpShader::InitGS(const string& gsPath)
{
	if (gsPath.empty())
	{
		return;
	}

	string shaderFolder = "Assets/Shaders/";
	string gsPath_ = shaderFolder + gsPath;
	//シェーダーの読み込みとコンパイル
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	HRESULT result;
	result = D3DCompileFromFile(
		wstring(gsPath_.begin(), gsPath_.end()).c_str(),  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	gspath = gsPath;
}

void RegisterShader(const string& id)
{
	shaderMap[id] = SpShader();
	shaderMap[id].id = id;
}

void RegisterShader(SpShader* shader, const std::string& id)
{
	shaderMap[id] = *shader;
}

void InitGS(const string& id, const string& gsPath)
{
	shaderMap[id].InitGS(gsPath);
}

void InitVS(const string& id, const string& vsPath)
{
	shaderMap[id].InitVS(vsPath);
}

void InitPS(const string& id, const string& psPath)
{
	shaderMap[id].InitPS(psPath);
}

SpShader* GetShader(const string& id)
{
	auto it = shaderMap.find(id);
	if (it == shaderMap.end())
	{
		return nullptr;
	}
	return &shaderMap[id];
}

bool ReCompile(const std::string& id)
{
	if (shaderMap.find(id) == shaderMap.end())
	{
		return false;
	}

	shaderMap[id].InitVS(shaderMap[id].vspath);
	shaderMap[id].InitGS(shaderMap[id].gspath);
	shaderMap[id].InitPS(shaderMap[id].pspath);

	return true;
}
