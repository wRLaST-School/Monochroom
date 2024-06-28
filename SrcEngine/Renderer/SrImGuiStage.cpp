#include "stdafx.h"
#include "SrImGuiStage.h"
#include <SpImGui.h>
#include <NoEffect.h>
#include <DockPanel.h>
void SrImGuiStage::Init()
{
}

void SrImGuiStage::PreDraw()
{
	NoEffect::Effect("RenderTexture", "CurrentBuffer");
	DockPanel::DrawViewPort();
}

void SrImGuiStage::Render()
{
	SpImGui::Draw();
}

void SrImGuiStage::PostDraw()
{
}

void SrImGuiStage::DrawCommands(std::function<void(void)> cmd)
{
	OutputDebugStringA("Draw Command Queued on Unintended Stage\n");
}
