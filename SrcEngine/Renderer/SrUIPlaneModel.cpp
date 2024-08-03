#include "stdafx.h"
#include "SrUIPlaneModel.h"
#include <Light.h>
#include <Camera.h>
#include <RTVManager.h>
#include <LineDrawer.h>
#include <SceneManager.h>
#include <BlinkTransition.h>
#include <NoEffect.h>

void SrUIPlaneModel::Init()
{
}

void SrUIPlaneModel::PreDraw()
{
	SpDirectX* dx = GetSpDX();
	dx->cmdList->SetPipelineState(GPipeline::GetState("UIPlaneModel"));
	dx->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("NoLight3D")->rootsignature.Get());
	
	D3D12_VIEWPORT viewport{};

	viewport.Width = (FLOAT)GetSpWindow()->width;
	viewport.Height = (FLOAT)GetSpWindow()->height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	dx->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // 切り抜き座標左
	scissorrect.right = scissorrect.left + GetSpWindow()->width;        // 切り抜き座標右
	scissorrect.top = 0;                                        // 切り抜き座標上
	scissorrect.bottom = scissorrect.top + GetSpWindow()->height;       // 切り抜き座標下

	dx->cmdList->RSSetScissorRects(1, &scissorrect);

	dx->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Light::Use();
	Camera::UseCurrent();

	if (SceneManager::currentScene->GetName() == "Title" ||
		SceneManager::currentScene->GetName() == "StageSelect")
	{
		RTVManager::SetRenderTargetToTexture("RGBShiftF", false);

	}
	else
	{
		RTVManager::SetRenderTargetToTexture("KawaseBloomP3", false);

	}

	//RTVManager::SetRenderTargetToTexture("BloomBefore");

	//vector<TextureKey> rts = { "normalTest", "inverseTest" };
	//RTVManager::SetRenderTargets(rts);
}

void SrUIPlaneModel::PostDraw()
{
	if (SceneManager::currentScene->GetName() == "Title" ||
		SceneManager::currentScene->GetName() == "StageSelect")
	{
		BlinkTransition::Effect("RGBShiftF", "BlinkTransition");

	}
	else
	{
		// 最後
		BlinkTransition::Effect("KawaseBloomP3", "BlinkTransition");

	}

	NoEffect::Effect("BlinkTransition", "RenderTexture");
}

void SrUIPlaneModel::Render()
{
	for (auto& rt : commands_)
	{
		//RTVManager::SetRenderTargetToTexture(rt.first, false);

		for (auto& cmd : rt.second)
		{
			cmd();
		}
	}
	commands_.clear();
}

void SrUIPlaneModel::DrawCommands(std::function<void(void)> cmd, TextureKey rt)
{
	commands_[rt].push_back(cmd);
}
