#include "stdafx.h"
#include "SrImGuiStage.h"
#include <SpImGui.h>
#include <NoEffect.h>
#include <DockPanel.h>
#include <LineDrawer.h>
void SrImGuiStage::Init()
{
}

void SrImGuiStage::PreDraw()
{
	LineDrawer::DrawAllLines();
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

void SrImGuiStage::DrawCommands(std::function<void(void)> cmd, TextureKey rt)
{
	OutputDebugStringA("Draw Command Queued on Unintended Stage\n");
}
