#include "stdafx.h"
#include "SrHolographicCasterStage.h"
#include <Camera.h>
#include <RTVManager.h>

void SrHolographicCasterStage::Init()
{
}

void SrHolographicCasterStage::PreDraw()
{
	SpDirectX* dx = GetSpDX();
	dx->cmdList->SetPipelineState(GPipeline::GetState("HolographicCaster"));
	dx->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("HolographicCaster")->rootsignature.Get());

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

	//Camera::UseCurrent();

	Camera::sCurrent->UpdateViewProjMatrix();

	RTVManager::SetRenderTargetToTexture("HolographicCaster");
}

void SrHolographicCasterStage::PostDraw()
{
}

void SrHolographicCasterStage::Render()
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

void SrHolographicCasterStage::DrawCommands(std::function<void(void)> cmd, TextureKey rt)
{
	commands_[rt].push_back(cmd);
}
