#include "GPipelineManager.h"
#include "SpShader.h"
#include "GPipeline.h"
#include "SpRootSignature.h"
#include "Model.h"
#include "Sprite.h"
#include "ShadowCaster.h"
#include "NormalCaster.h"
#include <Particle.h>
#include <LineDrawer.h>

void GPipelineManager::CreateAll()
{
#pragma region ShadowCaster用
	RegisterShader("ShadowCaster");
	InitVS("ShadowCaster", "ShadowCasterVS.hlsl");
	InitPS("ShadowCaster", "ShadowCasterPS.hlsl");

	PipelineDesc scDesc;
	scDesc.Render.InputLayout.pInputElementDescs = ShadowCasterCommon::inputLayout;
	scDesc.Render.InputLayout.NumElements = _countof(ShadowCasterCommon::inputLayout);

	scDesc.RootSignature.ptr = SpRootSignature::Get("ShadowCaster")->rootsignature.Get();

	scDesc.Shader.pShader = GetShader("ShadowCaster");
	scDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

	GPipeline::Create(scDesc, "ShadowCaster");

#pragma endregion

#pragma region NormalCaster用
	RegisterShader("NormalCaster");
	InitVS("NormalCaster", "NormalCasterVS.hlsl");
	InitPS("NormalCaster", "NormalCasterPS.hlsl");

	PipelineDesc ncDesc;
	ncDesc.Render.InputLayout.pInputElementDescs = NormalCasterCommon::inputLayout;
	ncDesc.Render.InputLayout.NumElements = _countof(NormalCasterCommon::inputLayout);

	ncDesc.RootSignature.ptr = SpRootSignature::Get("NormalCaster")->rootsignature.Get();

	ncDesc.Shader.pShader = GetShader("NormalCaster");
	ncDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

	GPipeline::Create(ncDesc, "NormalCaster");

#pragma endregion


#pragma region シルエット用
	RegisterShader("Silhouette");
	InitVS("Silhouette", "SilhouetteVS.hlsl");
	InitPS("Silhouette", "SilhouettePS.hlsl");

	PipelineDesc silhouetteDesc;
	silhouetteDesc.Render.InputLayout.pInputElementDescs = ModelCommon::inputLayout;
	silhouetteDesc.Render.InputLayout.NumElements = _countof(ModelCommon::inputLayout);

	silhouetteDesc.RootSignature.ptr = SpRootSignature::Get("Silhouette")->rootsignature.Get();

	silhouetteDesc.Shader.pShader = GetShader("Silhouette");
	silhouetteDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

	silhouetteDesc.Depth.DepthStencilState.DepthEnable = true;
	silhouetteDesc.Depth.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	silhouetteDesc.Depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	GPipeline::Create(silhouetteDesc, "Silhouette");
#pragma endregion

#pragma region デフォルト3D
	RegisterShader("def");
	InitVS("def", "BasicVS.hlsl");
	InitGS("def", "BasicGS.hlsl");
	InitPS("def", "BasicPS.hlsl");

	PipelineDesc defDesc;
	defDesc.Render.InputLayout.pInputElementDescs = ModelCommon::inputLayout;
	defDesc.Render.InputLayout.NumElements = _countof(ModelCommon::inputLayout);

	defDesc.RootSignature.ptr = SpRootSignature::Get("3D")->rootsignature.Get();

	defDesc.Shader.pShader = GetShader("def");
	defDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	GPipeline::Create(defDesc, "def");

#pragma endregion

#pragma region ライトなし描画用シェーダーのコンパイル
	RegisterShader("lightless");
	InitVS("lightless", "NoLightVS.hlsl");
	InitPS("lightless", "NoLightPS.hlsl");
#pragma endregion

#pragma region パーティクル3D
	RegisterShader("particle");
	InitVS("particle", "ParticleVS.hlsl");
	InitGS("particle", "ParticleGS.hlsl");
	InitPS("particle", "ParticlePS.hlsl");

	PipelineDesc particleDesc;
	particleDesc.Render.InputLayout.pInputElementDescs = ParticleCommon::inputLayout;
	particleDesc.Render.InputLayout.NumElements = _countof(ParticleCommon::inputLayout);

	particleDesc.Render.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	particleDesc.Blend[0].Desc = PipelineUtil::Blend::GetBlendMode(PipelineUtil::BlendMode::Add);

	particleDesc.RootSignature.ptr = SpRootSignature::Get("Particle")->rootsignature.Get();

	particleDesc.Shader.pShader = GetShader("particle");

	particleDesc.Depth.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	particleDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	GPipeline::Create(particleDesc, "particle");
#pragma endregion

#pragma region ライン表示
	RegisterShader("line");
	InitVS("line", "LineVS.hlsl");
	InitGS("line", "LineGS.hlsl");
	InitPS("line", "LinePS.hlsl");

	PipelineDesc lineDesc;
	lineDesc.Render.InputLayout.pInputElementDescs = LineDrawerCommon::inputLayout;
	lineDesc.Render.InputLayout.NumElements = _countof(LineDrawerCommon::inputLayout);

	lineDesc.Render.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	lineDesc.RootSignature.ptr = SpRootSignature::Get("Particle")->rootsignature.Get();

	lineDesc.Shader.pShader = GetShader("line");

	lineDesc.Depth.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	lineDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	GPipeline::Create(lineDesc, "line");
#pragma endregion

#pragma region デフォルト2D
	RegisterShader("2d");
	InitVS("2d", "SpriteVS.hlsl");
	InitPS("2d", "SpritePS.hlsl");

	PipelineDesc pl2dDesc;
	pl2dDesc.Render.InputLayout.pInputElementDescs = SpriteCommon::inputLayout2D;
	pl2dDesc.Render.InputLayout.NumElements = _countof(SpriteCommon::inputLayout2D);

	pl2dDesc.RootSignature.ptr = SpRootSignature::Get("2D")->rootsignature.Get();

	pl2dDesc.Shader.pShader = GetShader("2d");

	pl2dDesc.Depth.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pl2dDesc.Depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	pl2dDesc.Depth.DepthStencilState.DepthEnable = false;
	pl2dDesc.Render.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pl2dDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	GPipeline::Create(pl2dDesc, "2d");
#pragma endregion

#pragma region Depth2D
	RegisterShader("2dDepthCheck");
	InitVS("2dDepthCheck", "DepthVS.hlsl");
	InitPS("2dDepthCheck", "DepthPS.hlsl");

	PipelineDesc pl2dDepthDesc;
	pl2dDepthDesc.Render.InputLayout.pInputElementDescs = SpriteCommon::inputLayout2D;
	pl2dDepthDesc.Render.InputLayout.NumElements = _countof(SpriteCommon::inputLayout2D);

	pl2dDepthDesc.RootSignature.ptr = SpRootSignature::Get("2D")->rootsignature.Get();

	pl2dDepthDesc.Shader.pShader = GetShader("2dDepthCheck");

	pl2dDepthDesc.Depth.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pl2dDepthDesc.Depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	pl2dDepthDesc.Depth.DepthStencilState.DepthEnable = false;
	pl2dDepthDesc.Render.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pl2dDepthDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	GPipeline::Create(pl2dDepthDesc, "2dDepthCheck");
#pragma endregion

#pragma region レンダーターゲット2つ
	RegisterShader("defd");
	InitVS("defd", "BasicInvVS.hlsl");
	InitPS("defd", "BasicInvPS.hlsl");

	PipelineDesc doubleDesc;
	doubleDesc.Render.InputLayout.pInputElementDescs = ModelCommon::inputLayout;
	doubleDesc.Render.InputLayout.NumElements = _countof(ModelCommon::inputLayout);

	doubleDesc.RootSignature.ptr = SpRootSignature::Get("3D")->rootsignature.Get();

	doubleDesc.Shader.pShader = GetShader("defd");
	doubleDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	doubleDesc.Render.NumRenderTargets = 2;
	doubleDesc.Render.RTVFormat[1] = DXGI_FORMAT_R8G8B8A8_UNORM;

	doubleDesc.Blend[0].Desc = PipelineUtil::Blend::GetBlendMode(PipelineUtil::BlendMode::Alpha);
	doubleDesc.Blend[1].Desc = PipelineUtil::Blend::GetBlendMode(PipelineUtil::BlendMode::Alpha);

	GPipeline::Create(doubleDesc, "double");
#pragma endregion

#pragma region 追加用（追加するときに下にコピペ）

#pragma endregion

#pragma region トゥーンモデル
	RegisterShader("ToonModel");
	InitVS("ToonModel", "ToonModelVS.hlsl");
	InitPS("ToonModel", "ToonModelPS.hlsl");

	PipelineDesc toonModelDesc;
	toonModelDesc.Render.InputLayout.pInputElementDescs = ModelCommon::inputLayout;
	toonModelDesc.Render.InputLayout.NumElements = _countof(ModelCommon::inputLayout);

	toonModelDesc.RootSignature.ptr = SpRootSignature::Get("3D")->rootsignature.Get();

	toonModelDesc.Shader.pShader = GetShader("ToonModel");
	toonModelDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

	GPipeline::Create(toonModelDesc, "ToonModel");
#pragma endregion

#pragma region UI板モデル
	RegisterShader("UIPlaneModel");
	InitVS("UIPlaneModel", "NoLightVS.hlsl");
	InitPS("UIPlaneModel", "NoLightPS.hlsl");

	PipelineDesc UIPlaneModelDesc;
	UIPlaneModelDesc.Render.InputLayout.pInputElementDescs = ModelCommon::inputLayout;
	UIPlaneModelDesc.Render.InputLayout.NumElements = _countof(ModelCommon::inputLayout);

	UIPlaneModelDesc.RootSignature.ptr = SpRootSignature::Get("NoLight3D")->rootsignature.Get();

	UIPlaneModelDesc.Blend->Desc.BlendOp = D3D12_BLEND_OP_ADD;
	UIPlaneModelDesc.Blend->Desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	UIPlaneModelDesc.Blend->Desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	UIPlaneModelDesc.Shader.pShader = GetShader("UIPlaneModel");
	UIPlaneModelDesc.Depth.DepthStencilState.DepthEnable = true;
	UIPlaneModelDesc.Depth.DepthStencilState.StencilEnable = false;
	//UIPlaneModelDesc.Depth.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	UIPlaneModelDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

	GPipeline::Create(UIPlaneModelDesc, "UIPlaneModel");
#pragma endregion


}
