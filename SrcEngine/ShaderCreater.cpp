#include "stdafx.h"
#include "ShaderCreater.h"
#include <fstream>
#include <sstream>
#include <format>

const std::string cppSourceExt = ".cpp";
const std::string cppHeaderExt = ".h";
const std::string defaultCppFileFolderPath = "SrcEngine/Data/";
const std::string vcxprojPath = "LibraTestProj/LibraTestDLL/LibraTestDLL.vcxproj";

const std::string shaderSourceExt = ".hlsl";
const std::string shaderHeaderExt = ".hlsli";
const std::string defaultShaderFolderPath = "Assets/Shaders/";

void ShaderCreater::CreateCppClass(const std::string& path, const std::string& name)
{
	std::string fileData;
	std::string srcPath;

	// .cppファイル作成
	{
		// デフォルトのhlsliファイルのデータをstring型に格納
		srcPath = defaultCppFileFolderPath + "Default.cpp";
		fileData = CopyFileData(srcPath);

		// 作成
		CreateCppSourceFile(path, name, &fileData);
	}

	// .hファイル作成
	{
		// デフォルトのhlsliファイルのデータをstring型に格納
		srcPath = defaultCppFileFolderPath + "Default.h";
		fileData = CopyFileData(srcPath);

		// 作成
		CreateCppHeaderFile(path, name, &fileData);
	}

	//EditVCXProj(name);
}

void ShaderCreater::CreateCppSourceFile(const std::string& path, const std::string& name, std::string* fileData)
{
	const std::string fullPath = path + name + cppSourceExt;
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

	ConvertStringData(fileData, "Default", name);
	file << *fileData << std::endl;
	file.close();
}

void ShaderCreater::CreateCppHeaderFile(const std::string& path, const std::string& name, std::string* fileData)
{
	const std::string fullPath = path + name + cppHeaderExt;
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

	ConvertStringData(fileData, "Default", name);
	file << *fileData << std::endl;
	file.close();
}

void ShaderCreater::EditVCXProj(const std::string& name)
{
	// ファイルを読み込みString型に保存
	std::string fileData = CopyFileData(vcxprojPath);

	// ClInclude の位置を見つけて追加
	std::string clIncludeTag = "<ClInclude Include=\"";
	size_t includePos = fileData.find(clIncludeTag);
	if (includePos != std::string::npos) 
	{
		size_t pos = fileData.find("\n", includePos);
		fileData.insert(pos + 1, "    <ClInclude Include=\"" + name + cppHeaderExt + "\" />\n");
	}
	else 
	{
		OutputDebugStringA("No ClInclude group found in the vcxproj file.");
	}

	// ClCompile の位置を見つけて追加
	std::string clCompileTag = "<ClCompile Include=\"";
	size_t compilePos = fileData.find(clCompileTag);
	if (compilePos != std::string::npos) 
	{
		size_t pos = fileData.find("\n", compilePos);
		fileData.insert(pos + 1, "    <ClCompile Include=\"" + name + cppSourceExt + "\" />\n");
	}
	else 
	{
		OutputDebugStringA("No ClCompile group found in the vcxproj file.");
	}

	// 変更を保存
	std::ofstream outFile(vcxprojPath);
	if (!outFile.is_open()) 
	{
		OutputDebugStringA(std::format("Failed to open file : {}", vcxprojPath).c_str());
		return;
	}
	outFile << fileData;
	outFile.close();
}

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
		srcPath = defaultShaderFolderPath + "BasicVS.hlsl";
		fileData = CopyFileData(srcPath);

		// 作成
		CreateVertexShader(path, name, &fileData);
	}

	// PixelShader作成
	{
		// デフォルトのhlsliファイルのデータをstring型に格納
		srcPath = defaultShaderFolderPath + "BasicPS.hlsl";
		fileData = CopyFileData(srcPath);

		// 作成
		CreatePixelShader(path, name, &fileData);
	}
}

void ShaderCreater::CreateShaderHeader(const std::string& path, const std::string& name, std::string* fileData)
{
	const std::string fullPath = path + name + shaderHeaderExt;
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
	const std::string fullPath = path + name + "VS" + shaderSourceExt;

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
	const std::string fullPath = path + name + "PS" + shaderSourceExt;

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
