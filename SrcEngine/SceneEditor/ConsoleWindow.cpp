#include "stdafx.h"
#include "ConsoleWindow.h"
#include <SpImGui.h>
#include <GPipeline.h>
#include <Model.h>
#include <Sprite.h>

ConsoleWindow::ConsoleWindow()
{
	mIsOpen = true;
}

void ConsoleWindow::DrawWindow()
{
	SpImGui::Command([&]
		{
			if (!mIsOpen)
			{
				return;
			}

			ImGui::Begin("Console Window", &mIsOpen, ImGuiWindowFlags_MenuBar);

			ImGui::BeginMenuBar();
			if (ImGui::Button("Clear", ImVec2(96, 24)))
			{
				logs.clear();
			}
			ImGui::EndMenuBar();

			for (const auto& log : logs)
			{
				ImGui::Text(log.c_str());
			}

			ImGui::End();

			logs.clear();
		});
}

void ConsoleWindow::SDraw()
{
	GetInstance()->DrawWindow();
}

void ConsoleWindow::Log(const std::string& log)
{
	GetInstance()->logs.push_back(log);
}

void ConsoleWindow::SetisOpen(bool isOpen)
{
	mIsOpen = isOpen;
}

bool ConsoleWindow::GetisOpen()
{
	return mIsOpen;
}

ConsoleWindow* ConsoleWindow::GetInstance()
{
	static ConsoleWindow ins;
	return &ins;
}


