#pragma once
#include "SpMath.h"
#include "Essentials.h"
#include <ComponentFactory.h>

class PointLight : public IComponent
{
public:
	ComponentFactoryRegister(PointLight)

	DLLExport PointLight();
	DLLExport PointLight(const Float3& pos, const Float3& color, const Float3& att);
	DLLExport ~PointLight() override;
	Float3 pos;
	Float3 color;
	Float3 att;

	void DrawFrame();
	void OnInspectorWindowDraw();
	void DrawGizmo();
	void CopyComponent(IComponent* src) override;

	bool isActive = false;

public:
	struct DLLExport CBData {
		Float3 pos;
		float padding1;
		Float3 color;
		float padding;
		Float3 att;
		uint32_t isActive = false;
	};

	//change MAX_PLIGHTS in hlsli aswell
	static const uint32_t MAX_LIGHTS = 16;

	//点光源数が上限を超えた場合のエラー
	class DLLExport ReachedLightLimit {
	public:
		ReachedLightLimit(int32_t limit, int32_t actual):limit(limit),actual(actual) {};
		int32_t limit;
		int32_t actual;
	};

	std::string name = "unnamed";

private:
	bool showFrame_ = false;
};

typedef std::string PointLightKey;

