#pragma once
#include <Essentials.h>
#include <functional>
#include <GPipelineManager.h>
#include <RootSignatureManager.h>
#include <IRenderStage.h>
#include <SrShadowCasterStage.h>
#include <SrNormalCasterStage.h>
#include <SrSilhouetteStage.h>
#include <SrOpaqueStage.h>
#include <SrAddStage.h>
#include <SrAlphaStage.h>
#include <SrSpriteStage.h>
#include <SrPostEffectStage.h>
#include <SrImGuiStage.h>
#include <SrParticleStage.h>
#include <SrToonStage.h>
#include <SrUIPlaneModel.h>
#include <SrPostRenderStage.h>
#include <SrHolographicCasterStage.h>
#include <SrTransparentWallStage.h>
class SpRenderer
{
public:
	static void Init();
	static void Render();

	static SpRenderer* GetInstance(); 
	
	enum class Stage {
		ShadowCaster,
		NormalCaster,
		HolographicCaster,
		Opaque,
		Add,
		Toon,
		Silhouette,
		Alpha,
		TransparentWall,
		Particle,
		PostEffect,
		PostRender,
		UIPlane,
		Sprite,
		ImGui,
		StageNumMax
	};

	static void DrawCommand(std::function<void()> cmd, const Stage& stg, const TextureKey& target = RTVManager::defaultRT);
	static void RegisterAlphaObj(Object3D* obj);
private:
	std::unique_ptr<IRenderStage> stages_[(uint32_t)Stage::StageNumMax] = {
		std::make_unique<SrShadowCasterStage>(),
		std::make_unique<SrNormalCasterStage>(),
		std::make_unique<SrHolographicCasterStage>(),
		std::make_unique<SrOpaqueStage>(),
		std::make_unique<SrAddStage>(),
		std::make_unique<SrToonStage>(),
		std::make_unique<SrSilhouetteStage>(),
		std::make_unique<SrAlphaStage>(),
		std::make_unique<SrTransparentWallStage>(),
		std::make_unique<SrParticleStage>(),
		std::make_unique<SrPostEffectStage>(),
		std::make_unique<SrPostRenderStage>(),
		std::make_unique<SrUIPlaneModel>(),
		std::make_unique<SrSpriteStage>(),
		std::make_unique<SrImGuiStage>(),
	};
};

