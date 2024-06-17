#pragma once
#include <string.h>

namespace ShaderCreater
{
	void CreateDefaultShader(const std::string& path, const std::string& name);


	// 頂点シェーダーの作成
	void CreateShaderHeader(const std::string& path, const std::string& name, std::string* fileData = nullptr);
	void CreateVertexShader(const std::string& path, const std::string& name, std::string* fileData = nullptr);
	void CreatePixelShader(const std::string& path, const std::string& name, std::string* fileData = nullptr);

	// ファイルデータをstring型に格納する
	std::string CopyFileData(const std::string srcPath);

	// 文字列の指定した文字列Aを指定した文字列Bに置き換える
	void ConvertStringData(std::string* str, const std::string& search, const std::string& replace);

};