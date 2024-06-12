#include "stdafx.h"
#include "ShaderCreater.h"
#include <fstream>
#include <sstream>
#include <format>

const std::string sourceExt = ".hlsl";
const std::string headerExt = ".hlsli";
const std::string defaultShaderFolderPath = "Assets/Shaders/";

void ShaderCreater::CreateDefaultShader(const std::string& path, const std::string& name)
{
	std::string fileData;
	std::string srcPath;

	// ShaderHeader作成
	{
		// デフォルトのhlsliファイルのデータをstring型に格納
		srcPath = defaultShaderFolderPath + "Basic.hlsli";
		fileData = CopyFileData(srcPath);

		// 作成
		CreateShaderHeader(path, name, &fileData);
	}

	// VertexShader作成
	{
		// デフォルトのhlsliファイルのデータをstring型に格納
		srcPath = defaultShaderFolderPath + "BasicVS.hlsli";
		fileData = CopyFileData(srcPath);

		// 作成
		CreateVertexShader(path, name, &fileData);
	}

	// PixelShader作成
	{
		// デフォルトのhlsliファイルのデータをstring型に格納
		srcPath = defaultShaderFolderPath + "BasicPS.hlsli";
		fileData = CopyFileData(srcPath);

		// 作成
		CreatePixelShader(path, name, &fileData);
	}
}

void ShaderCreater::CreateShaderHeader(const std::string& path, const std::string& name, std::string* fileData)
{
	const std::string fullPath = path + name + sourceExt;
	std::ofstream file(fullPath);

	// ファイルが正しく開かれたかを確認します
	if (!file.is_open())
	{
		OutputDebugStringA(std::format("Failed to open file : {}", fullPath).c_str());
		return;
	}

	if (!fileData)
	{
		OutputDebugStringA(std::format("fileData is null").c_str());
		return;
	}

	file << *fileData << std::endl;
	file.close();
}

void ShaderCreater::CreateVertexShader(const std::string& path, const std::string& name, std::string* fileData)
{
	const std::string fullPath = path + name + "VS" + sourceExt;

	std::ofstream file(fullPath);

	// ファイルが正しく開かれたかを確認します
	if (!file.is_open())
	{
		OutputDebugStringA(std::format("Failed to open file : {}", fullPath).c_str());
		return;
	}

	if (!fileData)
	{
		OutputDebugStringA(std::format("fileData is null").c_str());
		return;
	}

	ConvertStringData(fileData, "Basic", name);
	file << *fileData << std::endl;
	file.close();
}

void ShaderCreater::CreatePixelShader(const std::string& path, const std::string& name, std::string* fileData)
{
	const std::string fullPath = path + name + "PS" + headerExt;

	std::ofstream file(fullPath);

	// ファイルが正しく開かれたかを確認します
	if (!file.is_open())
	{
		OutputDebugStringA(std::format("Failed to open file : {}", fullPath).c_str());
		return;
	}

	if (!fileData)
	{
		OutputDebugStringA(std::format("fileData is null").c_str());
		return;
	}

	ConvertStringData(fileData, "Basic", name);
	file << *fileData << std::endl;
	file.close();
}

std::string ShaderCreater::CopyFileData(const std::string srcPath)
{
	std::ifstream file(srcPath);
	if (file.is_open())
	{
		// ファイルの内容をすべて読み込む
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	return std::string();
}

void ShaderCreater::ConvertStringData(std::string* str, const std::string& search, const std::string& replace)
{
	// 文字列Aを探し、見つかった位置を記録する
	size_t pos = str->find(search);

	// 文字列Aが見つかった場合
	while (pos != std::string::npos)
	{
		// 文字列Aを文字列Bに置換する
		str->replace(pos, search.length(), replace);
		// 次の位置を探す
		pos = str->find(search, pos + replace.length());
	}
}
