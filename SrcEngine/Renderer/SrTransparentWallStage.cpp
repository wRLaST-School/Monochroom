#include "stdafx.h"
#include "SrTransparentWallStage.h"
#include <Camera.h>
#include <RTVManager.h>

void SrTransparentWallStage::Init()
{
}

void SrTransparentWallStage::PreDraw()
{
	SpDirectX* dx = GetSpDX();
	dx->cmdList->SetPipelineState(GPipeline::GetState("TransparentWall"));
	dx->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("3D")->rootsignature.Get());

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

	Camera::sCurrent->UpdateViewProjMatrix();

	//RTVManager::SetRenderTargetToTexture("CurrentScene");
}

void SrTransparentWallStage::PostDraw()
{
}

void SrTransparentWallStage::Render()
{
	for (auto& rt : commands_)
	{
		RTVManager::SetRenderTargetToTexture(rt.first, false);

		for (auto& cmd : rt.second)
		{
			cmd();
		}
	}
	commands_.clear();
}

void SrTransparentWallStage::DrawCommands(std::function<void(void)> cmd, TextureKey rt)
{
	commands_[rt].push_back(cmd);
}
