#pragma once
#include "SpConstBuffer.h"
#include "Model.h"
#include "SpTextureManager.h"
#include <Quaternion.h>
#include <ComponentFactory.h>
#include <DLLDef.h>
#include <ShadowCaster.h>
#include <NormalCaster.h>
#include <Silhouette.h>
#include <HolographicCaster.h>

struct ConstBufferDataTransform {
	Matrix mat;//3D変換行列
};

struct ConstBufferDataMisc {
	Float4 rimColor;
	float rimStrength;
	Float3 padding;
	float dissolveStrength;
	Float3 padding2;
	Vec2 tiling;
	Vec2 offset;
};

class Object3D : public IComponent
{
private:
	int normalType = 0;

public:
	ComponentFactoryRegister(Object3D)

		DLLExport Object3D();
	void DLLExport UpdateMatrix();
	void DLLExport DecomposeMatrix();

	void DLLExport Update()override;

	void DLLExport Draw();
	//別途読み込んだテクスチャを使う場合
	void DLLExport Draw(const TextureKey& key);

	void DLLExport DrawCommands(const TextureKey& key);

	//加算合成
	void DLLExport DrawAdd();
	void DLLExport DrawAdd(const TextureKey& key);

	//透過表示用
	void DLLExport DrawAlpha();
	void DLLExport DrawAlpha(const TextureKey& key);

	//Toon用
	void DLLExport DrawToon();
	void DLLExport DrawToon(const TextureKey& key);

	// UIPlane用
	void DLLExport DrawUIPlane(const TextureKey& key);

	//PostRender用
	void DLLExport DrawPostRender();
	void DLLExport DrawPostRender(const TextureKey& key);

	//TransparentWall用
	void DLLExport DrawTransparentWall();
	void DLLExport DrawTransparentWall(const TextureKey& key);

	//Inspector Window用
	void DLLExport OnInspectorWindowDraw();
	void DLLExport DrawGizmo();

	void DLLExport ReadParamJson([[maybe_unused]] const nlohmann::json& jsonObject) override;
	void DLLExport WriteParamJson([[maybe_unused]] nlohmann::json& jsonObject) override;

	void DLLExport CopyComponent(IComponent* src) override;

	TextureKey GetRT();

	SpConstBuffer<ConstBufferDataTransform> transformCB;
	SpConstBuffer<Float4> brightnessCB;
	SpConstBuffer<ConstBufferDataMisc> miscCB;

	Float3 scale = { 1, 1, 1 };
	Quaternion rotation = Quaternion::Identity();
	Float3 rotationE = {};
	enum class RotMode {
		Euler,
		Quaternion
	} rotMode = RotMode::Euler;

	enum class BlendMode {
		Opaque,
		Add,
		Alpha,
		Toon,
		UIPlane,
		PostRender,
		TransparentWall,
	} blendMode = BlendMode::Opaque;

	Float3 position = { 0, 0, 0 };

	Matrix matWorld = Matrix::Identity();
	Matrix matLocal = Matrix::Identity();

	Object3D* parent = nullptr;

	TextureKey texture = "";

	TextureKey renderTarget = "";

	TextureKey dissolveTex = "";

	Model* model = nullptr;

	float distanceToCam = 0;
	TextureKey alphaTexKey;

	bool useSilhouette = false;

	bool disableDraw = false;

	std::unique_ptr<ShadowCaster> shadowCaster;
	std::unique_ptr<NormalCaster> normalCaster;
	std::unique_ptr<Silhouette> silhouette;
	std::unique_ptr<HolographicCaster> holographicCaster;

	DLLExport static float dissolveStrength;
	bool isUseMyselfDissolveStrength;

	Vec2 tiling;
	Vec2 offset;

	int texType = 0;

};

