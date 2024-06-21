#include "stdafx.h"
#include "InspectorWindow.h"
#include <SpImGui.h>
#include <ComponentFactory.h>
#include <TagWindow.h>
#include <format>
void InspectorWindow::SelectObject(IComponent* component)
{
	GetInstance()->selected_ = component;
}

void InspectorWindow::DrawWindow()
{
	SpImGui::Command([&] {
		static bool open = true;

		ImGui::Begin("Inspector Window", &open, ImGuiWindowFlags_MenuBar);
		if (selected_)
		{
			SpImGui::InputText("Name", &selected_->name_, ImGuiInputTextFlags_::ImGuiInputTextFlags_None);

			ImGui::Separator();
			if (ImGui::CollapsingHeader("Tags"))
			{
				int32_t inspectorTagIndex = 0;
				for(auto itr = selected_->tags.begin(); itr != selected_->tags.end();)
				{
					ImGui::Text(itr->c_str());
					ImGui::SameLine();
					std::string buttonName = std::format("-##TagWindowDeleteID{:04}", inspectorTagIndex);
					if (ImGui::Button(buttonName.c_str())) {
						itr = selected_->tags.erase(itr);
					}
					else
					{
						itr++;
					}
					inspectorTagIndex++;
				}

				if (ImGui::Button("+"))
				{
					TagWindow::Show();
				}

				ImGui::Separator();
			}

			selected_->OnInspectorWindowDraw();
			selected_->CommonInspectorWindowDraw();

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("SelectComponentPopup");
			}

			//コンポーネント選択ポップアップの設定
			if (ImGui::BeginPopup("SelectComponentPopup"))
			{
				for (auto& item : ComponentFactory::GetInstance()->factory)
				{
					if (ImGui::MenuItem(item.first.c_str()))
					{
						ComponentFactory::AddChildComponent(selected_, item.first.c_str(), item.first.c_str());
					}
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();
	});
}

void InspectorWindow::SDraw()
{
	GetInstance()->DrawWindow();
}

InspectorWindow* InspectorWindow::GetInstance()
{
	static InspectorWindow ins;
	return &ins;
}
