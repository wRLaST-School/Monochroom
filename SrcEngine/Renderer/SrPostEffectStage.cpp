#include "stdafx.h"
#include "SrPostEffectStage.h"
#include <RTVManager.h>
#include <SpWindow.h>
#include <Bloom.h>
#include <GrayScale.h>

void SrPostEffectStage::Init()
{
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "BloomBefore", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "BloomAfter", true);
	RTVManager::CreateRenderTargetTexture(.5f, 1.f, "Bloom2ndAfter", true);
	RTVManager::CreateRenderTargetTexture(.5f, .5f, "Bloom3rdAfter", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "RenderTexture", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "CurrentScene", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "GrayScale", true);
	SpTextureManager::LoadTexture("Assets/Images/black.png", "Black");
	SpTextureManager::AddMasterTextureKey("BloomBefore");
	SpTextureManager::AddMasterTextureKey("BloomAfter");
	SpTextureManager::AddMasterTextureKey("Bloom2ndAfter");
	SpTextureManager::AddMasterTextureKey("Bloom3rdAfter");
	SpTextureManager::AddMasterTextureKey("RenderTexture");
	SpTextureManager::AddMasterTextureKey("CurrentScene");
	SpTextureManager::AddMasterTextureKey("GrayScale");

	BloomP1::Init();
	BloomP2::Init();
	BloomP3::Init();
	BloomFin::Init();
	GrayScale::Init();
}

void SrPostEffectStage::PreDraw() {};
void SrPostEffectStage::PostDraw() {};
void SrPostEffectStage::Render()
{
	GrayScale::Effect("CurrentScene", "GrayScale");

	BloomP1::Effect("CurrentScene", "BloomAfter");
	BloomP2::Effect("BloomAfter", "Bloom2ndAfter");
	BloomP3::Effect("Bloom2ndAfter", "Bloom3rdAfter");
	BloomFin::Effect("CurrentScene", "Bloom3rdAfter", "RenderTexture");
}

void SrPostEffectStage::DrawCommands(std::function<void(void)> cmd)
{
	OutputDebugStringA("Draw Command Queued on Unintended Stage\n");
}
