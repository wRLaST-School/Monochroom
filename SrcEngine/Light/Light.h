#pragma once
#include "Essentials.h"
#include "SpMath.h"
#include "SpConstBuffer.h"
#include "DirectionalLight.h"
#include "PointLight.h"
class Light
{
public:
	struct LightCBData
	{
		Vec3 dLightVec;//光の方向の逆ベクトル
		float padding1 = 0;
		Float3 dLightColor;
		float padding2 = 0;

		PointLight::CBData pointLights[PointLight::MAX_LIGHTS];
	};

	static void Init();

	static void Use();

	static void UpdateLightData();

	static Light* GetInstance();


	static PointLightKey CreatePointLight(const Float3& position, const Float3& color, const Float3& attenuation, const PointLightKey& key);

	static void RegisterPointLight(PointLight* ptr);

	static void ClearPointLight(PointLight* ptr);

	static void ClearAllPointLights();

	static DirectionalLight sDirectional;

private:
	SpConstBuffer<LightCBData> lightCB_;

	std::list<PointLight*> pointLights_;

	Light(){ };
	~Light(){ 
	};
};

