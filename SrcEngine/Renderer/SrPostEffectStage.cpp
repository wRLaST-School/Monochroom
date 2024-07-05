#include "stdafx.h"
#include "SrPostEffectStage.h"
#include <RTVManager.h>
#include <SpWindow.h>
#include <Bloom.h>
#include <GrayScale.h>
#include <GaussianBlur.h>
#include <NoEffect.h>

void SrPostEffectStage::Init()
{
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "BloomBefore", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "BloomAfter", true);
	RTVManager::CreateRenderTargetTexture(.5f, 1.f, "Bloom2ndAfter", true);
	RTVManager::CreateRenderTargetTexture(.5f, .5f, "Bloom3rdAfter", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "RenderTexture", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "CurrentScene", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "GrayScale", true);
	RTVManager::CreateRenderTargetTexture(2.f, 2.f, "ShadowMap", true);
	RTVManager::CreateRenderTargetTexture(0.5f, 0.5f, "GaussianBlur", true);
	RTVManager::CreateRenderTargetTexture(1.0f, 1.0f, "UI", true);

	SpTextureManager::LoadTexture("Assets/Images/black.png", "Black");
	SpTextureManager::AddMasterTextureKey("BloomBefore");
	SpTextureManager::AddMasterTextureKey("BloomAfter");
	SpTextureManager::AddMasterTextureKey("Bloom2ndAfter");
	SpTextureManager::AddMasterTextureKey("Bloom3rdAfter");
	SpTextureManager::AddMasterTextureKey("RenderTexture");
	SpTextureManager::AddMasterTextureKey("CurrentScene");
	SpTextureManager::AddMasterTextureKey("GrayScale");
	SpTextureManager::AddMasterTextureKey("ShadowMap");
	SpTextureManager::AddMasterTextureKey("GaussianBlur");
	SpTextureManager::AddMasterTextureKey("UI");

	BloomP1::Init();
	BloomP2::Init();
	BloomP3::Init();
	BloomFin::Init();
	GrayScale::Init();
	GaussianBlur::Init();
}

void SrPostEffectStage::PreDraw() {};
void SrPostEffectStage::PostDraw() {};
void SrPostEffectStage::Render()
{
	GrayScale::Effect(RTVManager::defaultRT, "GrayScale");
	GaussianBlur::Effect(RTVManager::defaultRT, "GaussianBlur");

	BloomP1::Effect(RTVManager::defaultRT, "BloomAfter");
	BloomP2::Effect("BloomAfter", "Bloom2ndAfter");
	BloomP3::Effect("Bloom2ndAfter", "Bloom3rdAfter");
	BloomFin::Effect(RTVManager::defaultRT, "Bloom3rdAfter", "RenderTexture");

	NoEffect::Effect("UI", "RenderTexture");
}

void SrPostEffectStage::DrawCommands(std::function<void(void)> cmd, TextureKey rt)
{
	OutputDebugStringA("Draw Command Queued on Unintended Stage\n");
}
