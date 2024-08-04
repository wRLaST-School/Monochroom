#include "stdafx.h"
#include "SrPostEffectStage.h"
#include <RTVManager.h>
#include <SpWindow.h>
#include <Bloom.h>
#include <GrayScale.h>
#include <GaussianBlur.h>
#include <SSAO.h>
#include <NoEffect.h>
#include <BlinkTransition.h>
#include <RGBShift.h>
#include <StageGenerating.h>
#include <KawaseBloom.h>
#include <SceneManager.h>

void SrPostEffectStage::Init()
{
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "NormalMap", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "BloomBefore", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "BloomAfter", true);
	RTVManager::CreateRenderTargetTexture(.5f, 1.f, "Bloom2ndAfter", true);
	RTVManager::CreateRenderTargetTexture(.5f, .5f, "Bloom3rdAfter", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "BloomF", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "RenderTexture", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "CurrentScene", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "GrayScale", true);
	RTVManager::CreateRenderTargetTexture(2.f, 2.f, "ShadowMap", true);
	RTVManager::CreateRenderTargetTexture(0.5f, 0.5f, "GaussianBlur", true);
	RTVManager::CreateRenderTargetTexture(1.0f, 1.0f, "UI", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.0f, "LeftLens", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "SSAO", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "SSAOF", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "BlinkTransition", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "RGBShiftTarget", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "RGBShift", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "RGBShiftF", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "RGBShiftMask", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "HolographicCaster", true);
	SpTextureManager::LoadTexture("Assets/Images/goggle.png", "Goggle_Mask");
	SpTextureManager::LoadTexture("Assets/Images/Noice.png", "Noice.png");
	SpTextureManager::LoadTexture("Assets/Images/White.png", "White.png");
	SpTextureManager::LoadTexture("Assets/Models/Stage/Block/Block1.png", "Block1.png");
	SpTextureManager::LoadTexture("Assets/Models/Stage/Block/Block2.png", "Block2.png");
	//SpTextureManager::CreateNoiceTexture(128, 128, 16, "NoiceTexture");

	SpTextureManager::LoadTexture("Assets/Images/black.png", "Black");
	SpTextureManager::AddMasterTextureKey("NormalMap");
	SpTextureManager::AddMasterTextureKey("BloomBefore");
	SpTextureManager::AddMasterTextureKey("BloomAfter");
	SpTextureManager::AddMasterTextureKey("Bloom2ndAfter");
	SpTextureManager::AddMasterTextureKey("Bloom3rdAfter");
	SpTextureManager::AddMasterTextureKey("RenderTexture");
	SpTextureManager::AddMasterTextureKey("CurrentScene");
	SpTextureManager::AddMasterTextureKey("GrayScale");
	SpTextureManager::AddMasterTextureKey("ShadowMap");
	SpTextureManager::AddMasterTextureKey("GaussianBlur");
	SpTextureManager::AddMasterTextureKey("LeftLens");
	SpTextureManager::AddMasterTextureKey("UI");
	SpTextureManager::AddMasterTextureKey("SSAO");
	SpTextureManager::AddMasterTextureKey("SSAOF");
	SpTextureManager::AddMasterTextureKey("BlinkTransition");
	SpTextureManager::AddMasterTextureKey("RGBShift");
	SpTextureManager::AddMasterTextureKey("RGBShiftTarget");
	SpTextureManager::AddMasterTextureKey("RGBShiftF");
	SpTextureManager::AddMasterTextureKey("RGBShiftMask");
	SpTextureManager::AddMasterTextureKey("HolographicCaster");
	SpTextureManager::AddMasterTextureKey("Goggle_Mask");

	BloomP1::Init();
	BloomP2::Init();
	BloomP3::Init();
	BloomFin::Init();
	GrayScale::Init();
	GaussianBlur::Init();
	SSAO::Init();
	BlinkTransition::Init();
	RGBShift::Init();
	StageGenerating::Init();
	KawaseBloom::Init();
}

void SrPostEffectStage::PreDraw() {};
void SrPostEffectStage::PostDraw() {};
void SrPostEffectStage::Render()
{
	// SSAO
	SSAO::EffectAO(RTVManager::defaultRT, "SSAO");
	SSAO::EffectBilateralFilter(RTVManager::defaultRT, "SSAO", "SSAOF");

	// ステージ生成
	StageGenerating::Effect(RTVManager::defaultRT, "StageGenerating");

	// ブルーム
	TextureKey kawaseBloomBaseTex = StageGenerating::info.isDraw ? "StageGenerating" : RTVManager::defaultRT;
	KawaseBloom::Effect(kawaseBloomBaseTex, "KawaseBloomP3");

	//
	GaussianBlur::Effect(RTVManager::defaultRT, "GaussianBlur");
	RGBShift::Effect("RGBShiftTarget", "RGBShiftF");

	// グレースケール
	GrayScale::Effect("KawaseBloomP3", "GrayScale");
	//StageGenerating::Effect(RTVManager::defaultRT, "StageGenerater");
	//RGBShift::Effect("RGBShiftTex", "RGBShift");

	//NoEffect::Effect("UI", "RenderTexture");

}

void SrPostEffectStage::DrawCommands(std::function<void(void)> cmd, TextureKey rt)
{
	OutputDebugStringA("Draw Command Queued on Unintended Stage\n");
}
