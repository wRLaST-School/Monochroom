#include "stdafx.h"
#include "TagWindow.h"
#include <SpImGui.h>
#include <format>
#include <fstream>
#include <InspectorWindow.h>

void TagWindow::Show()
{
	open = true;
}

void TagWindow::Hide()
{
	open = false;
}

void TagWindow::Draw()
{
	if (open && !prevOpen)
	{
		OnOpenWindow();
	}
	else if (!open && prevOpen)
	{
		OnCloseWindow();
	}
	prevOpen = open;

	if (open)
	{
		SpImGui::Command([&] {
			if (ImGui::Begin("TagWindow", &open))
			{
				int32_t tagWindowIndex = 0;
				for (auto itr = tags.begin(); itr != tags.end();)
				{
					SpImGui::InputText(std::format("##TagWindowID{:04}",tagWindowIndex).c_str(), *itr);
					ImGui::SameLine();
					std::string buttonName = std::format("-##TagWindowDeleteID{:04}", tagWindowIndex);

					bool button = ImGui::Button(buttonName.c_str());
					
					ImGui::SameLine();
					std::string buttonNameAdd = std::format("Attach##TagWindowAttachID{:04}", tagWindowIndex);
					if (ImGui::Button(buttonNameAdd.c_str())) {
						if(InspectorWindow::GetSelected<IComponent>())
							InspectorWindow::GetInstance()->selected_->AddTag(*itr);
					}


					if (button) {
						itr = tags.erase(itr);
					}
					else
					{
						itr++;
					}

					tagWindowIndex++;
				}
				if (ImGui::Button("+"))
				{
					tags.push_back("");
				}
			}
			ImGui::End();
		});
	}
}

void TagWindow::DDSource()
{
}

void TagWindow::SaveTags()
{
	nlohmann::json dat;

	for (auto& tag : tags)
	{
		dat.push_back(tag);
	}

	//ファイル書き込み
	std::ofstream file;

	file.open(savePath);

	file << dat.dump(4);

	file.close();
}

void TagWindow::LoadTags()
{
	//ファイル読み込み
	std::ifstream file;

	file.open(savePath);

	if (file.fail())
	{
		assert(0);
	}

	nlohmann::json deserialized;
	file >> deserialized;

	file.close();

	tags.clear();

	for (auto& tag : deserialized)
	{
		tags.push_back(tag);
	}
}

void TagWindow::OnOpenWindow()
{
	LoadTags();
}

void TagWindow::OnCloseWindow()
{
	SaveTags();
}
