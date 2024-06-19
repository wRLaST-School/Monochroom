#include "stdafx.h"
#include "PipelineWindow.h"
#include <SpImGui.h>

PipelineWindow::PipelineWindow()
{
	mIsOpen = false;
}

void PipelineWindow::DrawWindow()
{
	SpImGui::Command([&] {
		if (!mIsOpen)
		{
			return;
		}

		ImGui::Begin("Pipeline Window", &mIsOpen);

		ImGui::End();
		});
}

void PipelineWindow::SDraw()
{
	GetInstance()->DrawWindow();
}

void PipelineWindow::SetisOpen(bool isOpen)
{
	mIsOpen = isOpen;
}

bool PipelineWindow::GetisOpen()
{
	return mIsOpen;
}

PipelineWindow* PipelineWindow::GetInstance()
{
	static PipelineWindow ins;
	return &ins;
}
