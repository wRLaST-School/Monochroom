#include "stdafx.h"
#include "AssetBrowser.h"
#include <SpImGui.h>

#include <fstream>
#include <iostream>

#include <windows.system.h>

#include <SpTextureManager.h>
#include <SceneManager.h>
#include <BTEditorScene.h>
#include <regex>

#include <future>
#include <chrono>

#include <SceneRW.h>
#include <SceneFromFile.h>
#include <GameManager.h>

#include <format>

#include <FileCreater.h>

constexpr char* sAssetsDirectory = "Assets";
const static std::filesystem::path sAssetPath = "Assets";

void AssetBrowser::OnImGuiRender()
{
	//非同期系で投げるだけのモノをしまっておく場所
	static eastl::list<std::future<void>> ftrs;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
	windowFlags |= ImGuiWindowFlags_MenuBar;
	ImGui::Begin("Asset Browser", nullptr, windowFlags);

	PopWindow();
	PopModalWindow();

	//サムネイル画像サイズ調整用スライダー
	ImGui::BeginMenuBar();
	ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16.f, 512.f);
	ImGui::EndMenuBar();

	ImGui::Text("%s\\", currentDirectory_.string().c_str());

	//レイアウト調整
	float cellSize = thumbnailSize + padding;

	float panelW = ImGui::GetContentRegionAvail().x;

	int32_t columnCount = static_cast<int>(panelW / cellSize);

	//1つも入らないときも1つは入れる(0だとエラーになる)
	if (columnCount < 1)
	{
		columnCount = 1;
	}

	ImGui::Columns(columnCount, 0, false);

	//何かのフォルダ内にいるなら戻るボタンを表示
	if (currentDirectory_ != std::filesystem::path(sAssetsDirectory))
	{
		if (SpImGui::DoubleClickButton("<-", { thumbnailSize, thumbnailSize }))
		{
			currentDirectory_ = currentDirectory_.parent_path();
		};
		ImGui::Text("^");

		ImGui::NextColumn();
	}

	//現在のフォルダ内のすべてのファイルに対して
	for (auto& ditr : std::filesystem::directory_iterator(currentDirectory_))
	{
		//フォルダなら
		if (ditr.is_directory())
		{
			//const auto& path = ditr.path().string();
			std::string fileName = ditr.path().filename().string();
			if (SpImGui::DoubleClickImageButton((ImTextureID)SpTextureManager::GetGPUDescHandle("Engine_FolderIcon").ptr, { thumbnailSize, thumbnailSize }))
			{
				currentDirectory_ /= ditr.path().filename();
			};
			ImGui::Text(fileName.c_str());

			ImGui::NextColumn();
		}
	}

	//現在のフォルダ内のすべてのファイルに対して
	//フォルダ->ファイルの順でソートするため2周探索している
	for (auto& ditr : std::filesystem::directory_iterator(currentDirectory_))
	{
		//ファイルなら
		if (!ditr.is_directory())
		{
			std::string fileName = ditr.path().filename().string();
			std::string filePath = ditr.path().string();

			//画像ならプレビュー表示の読み込みを開始
			//画像ファイルの場合
			std::regex re(".(png|PNG|jpg|JPG|dds|DDS)");
			std::string ext = ditr.path().extension().string();
			if (std::regex_match(ext, re))
			{
				if (!SpTextureManager::IsMasterTexture(filePath))
				{
					SpTextureManager::AddMasterTextureKey(filePath);

					//非同期で画像読み込みを投げる
					ftrs.emplace_back(std::async(std::launch::async, [filePath, this] {
						std::lock_guard lock(mtx);
						SpTextureManager::LoadTexture(filePath, filePath);
						loadedPreviewTextures.Access([&](auto& map) { map.emplace(filePath, 1); });
						}));
				}

				bool hasTexLoaded = false;
				loadedPreviewTextures.Access([&](auto& map) { hasTexLoaded = map.count(filePath); });

				if (hasTexLoaded)
				{
					ImGui::ImageButton("buttonTag",
						(ImTextureID)SpTextureManager::GetGPUDescHandle(filePath).ptr,
						{ thumbnailSize, thumbnailSize });
				}
				else
				{
					ImGui::ImageButton("buttonTag",
						(ImTextureID)SpTextureManager::GetGPUDescHandle("Engine_FileIcon").ptr,
						{ thumbnailSize, thumbnailSize });
				}
			}//画像ファイルの場合の処理ここまで
			else
			{
				ImGui::ImageButton("buttonTag",
					(ImTextureID)SpTextureManager::GetGPUDescHandle("Engine_FileIcon").ptr,
					{ thumbnailSize, thumbnailSize });
			}

			if (ImGui::IsItemHovered() && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				selectedItemPath = filePath;
			};

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				FileOpenAction(ditr);
			};

			if (ImGui::BeginDragDropSource())
			{
				const char* relPath = selectedItemPath.c_str();
				ImGui::SetDragDropPayload("AST_BROWSER_ITEM", relPath, strlen(relPath) * sizeof(char));
				ImGui::EndDragDropSource();
			}

			ImGui::Text(fileName.c_str());

			ImGui::NextColumn();
		}
	}

	ImGui::Columns(1);

	ImGui::End();

	//非同期系で投げるだけのモノを片づける処理
	for (auto itr = ftrs.begin(); itr != ftrs.end();)
	{
		if (itr->wait_for(std::chrono::microseconds(0)) != std::future_status::timeout)
		{
			itr = ftrs.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void AssetBrowser::Draw()
{
	SpImGui::Command(std::bind(&AssetBrowser::OnImGuiRender, this));
}

void AssetBrowser::FileOpenAction(const std::filesystem::directory_entry& ditr)
{
	const auto& path = ditr.path().string();
	const auto& ext = ditr.path().extension();

	//拡張子ごとに特殊なプログラム呼び出しがあれば実行
	if (ext == ".bt")
	{
		SceneManager::LoadScene<BTEditorScene>(path);
		SceneManager::WaitForLoadAndTransition();
	}
	else if (ext == ".scene")
	{
		SceneManager::LoadScene<SceneFromFile>(path);
		SceneManager::WaitForLoadAndTransition();
	}
	else
	{
		ShellExecuteA(0, 0, path.c_str(), 0, 0, SW_SHOW);
	}

}

void AssetBrowser::PopWindow()
{
	// ウィンドウ内でのマウス右クリックの判定
	if (ImGui::IsWindowHovered() &&
		ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("Create AssetBrowser Pop");
	}

	if (ImGui::BeginPopup("Create AssetBrowser Pop"))
	{
		if (ImGui::MenuItem("Folder"))
		{
			popID = "Create Folder";
			openPopModal = true;
			inputName.clear();
		}
		if (ImGui::MenuItem("C++ Class"))
		{
			popID = "Create C++ Class";
			openPopModal = true;
			inputName.clear();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Default Scene"))
		{
			popID = "Create Scene";
			openPopModal = true;
			inputName.clear();
		}
		if (ImGui::MenuItem("Default Shader"))
		{
			popID = "Create Shader";
			openPopModal = true;
			inputName.clear();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Show in Explorer"))
		{
			std::string path = currentDirectory_.string();
			std::wstring wpath = std::wstring(path.begin(), path.end());
			ShellExecute(NULL, L"open", wpath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
		}

		ImGui::EndPopup();
	}
}

void AssetBrowser::PopModalWindow()
{
	if (openPopModal)
	{
		ImGui::OpenPopup(popID.c_str());
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_None, ImVec2(0.5f, 0.5f));
	ImVec2 size = ImVec2(384, 128);
	ImGui::SetNextWindowSize(size);

	int windowFlag = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	if (ImGui::BeginPopupModal(popID.c_str(), nullptr, windowFlag))
	{
		char* cstr = const_cast<char*>(inputName.c_str());
		ImGui::InputText("Name", cstr, 30);
		inputName = cstr;

		bool isConfirm = ImGui::Button("Confirm", ImVec2(128, 32));	ImGui::SameLine();
		bool isClicked = isConfirm || ImGui::Button("Cancel", ImVec2(128, 32));

		if (isClicked)
		{
			if (isConfirm)
			{
				// フォルダー作成なら
				if (popID == "Create Folder")
				{
					std::string path = currentDirectory_.string() + "/" + inputName;
					std::wstring wpath = std::wstring(path.begin(), path.end());
					CreateDirectory(wpath.c_str(), NULL);
				}

				// クラス作成なら
				if (popID == "Create C++ Class")
				{
					std::string projectPath = "LibraTestProj/LibraTestDLL/";
					FileCreater::CreateCppClass(projectPath, inputName);
				}

				// シーン作成なら
				if (popID == "Create Scene")
				{
					FileCreater::CreateDefautScene(currentDirectory_.string()+"/", inputName);
				}

				// シェーダー作成なら
				if (popID == "Create Shader")
				{
					std::string shadersFolderPath = "Assets/Shaders/";
					FileCreater::CreateDefaultShader(shadersFolderPath, inputName);
				}
			}

			inputName.clear();
			openPopModal = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void AssetBrowser::SDraw()
{
	GetInstance()->Draw();
}

void AssetBrowser::LoadResources()
{
	SpTextureManager::LoadTexture("Resources/icons/folder.png", "Engine_FolderIcon");
	SpTextureManager::LoadTexture("Resources/icons/file.png", "Engine_FileIcon");
	SpTextureManager::LoadTexture("Resources/icons/3d-file.png", "Engine_3DFileIcon");

	SpTextureManager::AddMasterTextureKey("Engine_FolderIcon");
	SpTextureManager::AddMasterTextureKey("Engine_FileIcon");
	SpTextureManager::AddMasterTextureKey("Engine_3DFileIcon");
}

void AssetBrowser::CopyFileByDD(std::wstring path)
{
	if (!GameManager::sShowDebug) return;

	std::filesystem::path oldFile = path;

	OutputDebugStringA(std::format("Copy {} to {}\n", oldFile.string(), (currentDirectory_.string() + "/" + oldFile.filename().string())).c_str());

	std::filesystem::copy(oldFile.c_str(), (currentDirectory_.string() + "/" + oldFile.filename().string()).c_str(),
		std::filesystem::copy_options::recursive | std::filesystem::copy_options::skip_existing);
}

AssetBrowser* AssetBrowser::GetInstance()
{
	static AssetBrowser ins;
	return &ins;
}

AssetBrowser::AssetBrowser() : currentDirectory_(sAssetsDirectory)
{
}
