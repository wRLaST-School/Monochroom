#pragma once
#include <functional>
#include <imgui.h>
#include <SrcImgui/imgui/ImGuizmo/ImGuizmo.h>
#include <SrcImgui/imgui/ImGuizmo/GraphEditor.h>

class SpImGui
{
public:
	static void Init();

	static void InitDirectXForImGui();

	static DLLExport void Command(std::function<void(void)> command);

	static void Draw();

	static void EndFrame();

	static void Shutdown();

private:
	static std::list<std::function<void(void)>> sCommands;

	//ImGui ItemのWrapper関数群
private:
	struct InputTextCallback_UserData
	{
		std::string* Str;
		ImGuiInputTextCallback  ChainCallback;
		void* ChainCallbackUserData;
	};

public:
	static DLLExport bool DoubleClickButton(const char* label, const ImVec2& size = ImVec2(0, 0));

	static DLLExport bool DoubleClickImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int32_t frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
	static DLLExport bool DoubleClickImageButton2(ImTextureID user_texture_id, const ImVec2& size, std::function<void(void)> singleClickEvent, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int32_t frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

	static DLLExport bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
	static DLLExport bool DragAngle(const char* label, float* prad);

	// ラッピング関数
	static DLLExport bool TreeNode(const char* str_id, const bool isOpen);
	static DLLExport bool InputText(const char* label, std::string& input, ImGuiInputTextFlags flags = 0);

private:
	static int InputTextCallback(ImGuiInputTextCallbackData* data);
};

