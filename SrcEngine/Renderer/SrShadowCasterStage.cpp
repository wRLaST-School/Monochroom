#include "stdafx.h"
#include "SrShadowCasterStage.h"
#include <Camera.h>
#include <RTVManager.h>

std::unique_ptr<Camera> SrShadowCasterStage::lightCamera = nullptr;

void SrShadowCasterStage::Init()
{
	lightCamera = std::make_unique<Camera>();
	lightCamera->UseDefaultParams();
	lightCamera->renderWidth = 1920.f * 2.f;
	lightCamera->renderHeight = 1080.f * 2.f;
	lightCamera->projectionMode = ProjectionMode::Orthographic;

	lightCamera->position = Vec3(-100, 90, 80);
	lightCamera->rotationE = ConvertAngleToRadian(Vec3(40, 125, 0));
	lightCamera->left = -960;
	lightCamera->right = +960;
	lightCamera->top = +540;
	lightCamera->bottom = -540;
	lightCamera->rectRate = 0.3f;
}

void SrShadowCasterStage::PreDraw()
{
	SpDirectX* dx = GetSpDX();
	dx->cmdList->SetPipelineState(GPipeline::GetState("ShadowCaster"));
	dx->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("ShadowCaster")->rootsignature.Get());

	D3D12_VIEWPORT viewport{};

	viewport.Width = (FLOAT)GetSpWindow()->width * 2.f;
	viewport.Height = (FLOAT)GetSpWindow()->height * 2.f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	dx->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // 切り抜き座標左
	scissorrect.right = scissorrect.left + GetSpWindow()->width * 2;        // 切り抜き座標右
	scissorrect.top = 0;                                        // 切り抜き座標上
	scissorrect.bottom = scissorrect.top + GetSpWindow()->height * 2;       // 切り抜き座標下

	dx->cmdList->RSSetScissorRects(1, &scissorrect);

	dx->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	lightCamera->farZ = 1000.0f;

	//Camera::sCurrent->UpdateViewProjMatrix();
	lightCamera->UpdateMatrix();
	lightCamera->UpdateLightViewProjMatrix();

	RTVManager::SetRenderTargetToTexture("ShadowMap");

}

void SrShadowCasterStage::PostDraw()
{
}

void SrShadowCasterStage::Render()
{
	for (auto& rt : commands_)
	{
		//RTVManager::SetRenderTargetToTexture(rt.first);

		for (auto& cmd : rt.second)
		{
			cmd();
		}
	}
	commands_.clear();
}

void SrShadowCasterStage::DrawCommands(std::function<void(void)> cmd, TextureKey rt)
{
	commands_[rt].push_back(cmd);
}
