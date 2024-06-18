#pragma once
#include <string.h>

namespace FileCreater
{
#pragma region C++関連

	void CreateCppClass(const std::string& path, const std::string& name);
	void CreateCppSourceFile(const std::string& path, const std::string& name, std::string* fileData = nullptr);
	void CreateCppHeaderFile(const std::string& path, const std::string& name, std::string* fileData = nullptr);
	void EditVCXProj(const std::string& name);

#pragma endregion

#pragma region シェーダー関連

	void CreateDefaultShader(const std::string& path, const std::string& name);

	void CreateShaderHeader(const std::string& path, const std::string& name, std::string* fileData = nullptr);
	void CreateVertexShader(const std::string& path, const std::string& name, std::string* fileData = nullptr);
	void CreatePixelShader(const std::string& path, const std::string& name, std::string* fileData = nullptr);

#pragma endregion

	// ファイルデータをstring型に格納する
	std::string CopyFileData(const std::string srcPath);

	// 文字列の指定した文字列Aを指定した文字列Bに置き換える
	void ConvertStringData(std::string* str, const std::string& search, const std::string& replace);

};