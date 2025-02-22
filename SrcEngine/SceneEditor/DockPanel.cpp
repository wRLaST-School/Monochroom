#include "stdafx.h"
#include "DockPanel.h"
#include <GameManager.h>
#include <SpTextureManager.h>
#include <Util.h>
#include <InspectorWindow.h>
#include <Object3D.h>
#include <SceneManager.h>
#include <SceneRW.h>
#include <SceneFromFile.h>
#include <SpriteObject.h>
#include <Input.h>
#include <PipelineWindow.h>
#include <ConsoleWindow.h>
#include <TagWindow.h>

void DockPanel::EnableScreenDock()
{
	SpImGui::Command([&] {
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;


		if (!GameManager::sShowDebug) {
			dockspace_flags |= ImGuiDockNodeFlags_::ImGuiDockNodeFlags_KeepAliveOnly;
			window_flags &= ~ImGuiWindowFlags_MenuBar;
		}
		static bool open = true;

		ImGui::Begin("Editor", &open, window_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save", "Ctrl + S"))
				{
					showSaveDialog = true;
				}

				if (ImGui::MenuItem("Load"))
				{
					showLoadDialog = true;
					//SceneRW::LoadScene(SceneManager::currentScene.get(), "Assets/Scene/TestSceneSave.scene");
				}

				if (ImGui::MenuItem("ImGui Demo"))
				{
					showImGuiDemo = true;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				bool isOpen = PipelineWindow::GetInstance()->GetisOpen();
				if (ImGui::MenuItem("Pipeline Window", nullptr, isOpen))
				{
					if (isOpen == false)
					{
						PipelineWindow::GetInstance()->SetisOpen(true);
					}
				}
				isOpen = ConsoleWindow::GetInstance()->GetisOpen();
				if (ImGui::MenuItem("Console Window", nullptr, isOpen))
				{
					if (isOpen == false)
					{
						ConsoleWindow::GetInstance()->SetisOpen(true);
					}
				}
				isOpen = TagWindow::open;
				if (ImGui::MenuItem("Tag Window", nullptr, isOpen))
				{
					if (isOpen == false)
					{
						TagWindow::Show();
					}
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (GameManager::sShowDebug)
		{
			if (Input::Key::Down(DIK_LCONTROL) || Input::Key::Down(DIK_RCONTROL))
			{
				bool pushLShift = Input::Key::Down(DIK_LSHIFT);

				if (pushLShift)
				{
					if (Input::Key::Triggered(DIK_S))
					{
						showSaveDialog = true;
					}
				}
				else
				{
					if (Input::Key::Triggered(DIK_S))
					{
						if (lastSavePath == "") showSaveDialog = true;
						else {

							SceneRW::SaveScene(SceneManager::currentScene.get(), lastSavePath);
						}
					}
				}
			}
		}

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		ImGui::End();

		ImGui::PopStyleVar(3);

		if (showSaveDialog)
		{
			DrawSaveDialog();
		}

		if (showLoadDialog)
		{
			DrawLoadDialog();
		}

		if (showImGuiDemo)
		{
			ImGui::ShowDemoWindow(&showImGuiDemo);
		}

		});
}

void DockPanel::DrawViewPort()
{
	if (GameManager::sShowDebug)
	{
		SpImGui::Command([] {
			static bool open = true;
			ImGui::Begin("Game", &open, ImGuiWindowFlags_NoCollapse);

			sViewPortWindow = ImGui::GetCurrentWindow();

			//タブ等を除いたウィンドウのサイズを取得(計算)
			ImVec2 cntRegionMax = ImGui::GetWindowContentRegionMax();
			ImVec2 cntRegionMin = ImGui::GetWindowContentRegionMin();
			ImVec2 wndSize = { cntRegionMax.x - cntRegionMin.x, cntRegionMax.y - cntRegionMin.y };

			//元のアス比とImGuiウィンドウのアス比を比較
			float outerWindowAspectRatio = static_cast<float>(Util::GetWinWidth()) / static_cast<float>(Util::GetWinHeight());
			float innerWindowAspectRatio = wndSize.x / wndSize.y;
			ImVec2 finalWndSize = wndSize;

			//横幅が大きかったら縦基準で画像サイズを決定
			if (outerWindowAspectRatio <= innerWindowAspectRatio)
			{
				finalWndSize.x *= outerWindowAspectRatio / innerWindowAspectRatio;
			}
			//縦幅が大きかったら横基準で画像サイズを決定
			else
			{
				finalWndSize.y *= innerWindowAspectRatio / outerWindowAspectRatio;
			}

			//画像を中央に持ってくる
			ImVec2 topLeft = { (wndSize.x - finalWndSize.x) * 0.5f + cntRegionMin.x,
								(wndSize.y - finalWndSize.y) * 0.5f + cntRegionMin.y };
			ImGui::SetCursorPos(topLeft);

			ImGui::Image((ImTextureID)SpTextureManager::GetGPUDescHandle("RenderTexture").ptr,
				finalWndSize);

			DragDropTarget();

			//Gizmo対応型だったらGizmoを表示
			Object3D* selectedObj = InspectorWindow::GetSelected<Object3D>();
			if (selectedObj)
			{
				selectedObj->DrawGizmo();
			}

			PointLight* selectedPtl = InspectorWindow::GetSelected<PointLight>();
			if (selectedPtl)
			{
				selectedPtl->DrawGizmo();
			}

			ImGui::End();
			});
	}
}

void DockPanel::DragDropTarget()
{
	if (ImGui::BeginDragDropTarget())
	{
		DDTargetTexture();

		ImGui::EndDragDropTarget();
	}
}

void DockPanel::DDTargetTexture()
{
	const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RES_WINDOW_ITEM_TEXTURE");

	if (payload) {
		const char* texKey = reinterpret_cast<const char*>(payload->Data);

		SpriteObject* obj = SceneManager::currentScene->AddComponent<SpriteObject>("SpriteObject");


		if (obj)
		{
			obj->tex = texKey;
		}
	}
}

void DockPanel::DrawSaveDialog()
{
	ImVec2 size = { 256, 128 };
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImVec2 topLeft = { center.x - size.x / 2.f, center.y - size.y / 2.f };

	ImGui::SetNextWindowPos(topLeft);
	ImGui::SetNextWindowSize(size);

	if (ImGui::Begin("SaveScenePopup", NULL, ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize))
	{
		const int32_t PATH_LENGTH = 256;
		static char path[PATH_LENGTH];

		ImGui::InputText("FileName", path, PATH_LENGTH);

		if (ImGui::Button("Save"))
		{
			lastSavePath = std::string("Assets/Scene/") + std::string(path) + std::string(".scene");
			SceneRW::SaveScene(SceneManager::currentScene.get(), std::string("Assets/Scene/") + std::string(path) + std::string(".scene"));
			showSaveDialog = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			showSaveDialog = false;
		}
	}
	ImGui::End();
}

void DockPanel::DrawLoadDialog()
{
	ImVec2 size = { 256, 128 };
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImVec2 topLeft = { center.x - size.x / 2.f, center.y - size.y / 2.f };

	ImGui::SetNextWindowPos(topLeft);
	ImGui::SetNextWindowSize(size);

	if (ImGui::Begin("SaveScenePopup", NULL, ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize))
	{
		ImGui::Text("This Feature is Not Fully Supported Yet");

		if (ImGui::Button("OK"))
		{
			showLoadDialog = false;

			SceneManager::LoadScene<SceneFromFile>("Assets/Scene/Game.scene");
			SceneManager::WaitForLoadAndTransition();
		}
	}
	ImGui::End();
}

ImGuiWindow* DockPanel::GetViewPortWindow()
{
	return sViewPortWindow;
}
