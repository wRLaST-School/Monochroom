#pragma once
#include "Object3D.h"

struct ConstBufferDataVProj {
	Matrix vproj;
	Matrix billboardMat;
	Float3 cameraPos;
};

enum class CameraTargetMode {
	LookAt,
	LookTo
};

enum class ProjectionMode {
	Orthographic,
	Perspective
};

enum class FrustumPlane {
	Left, Right, Top, Bottom, Front, Back, Max
};

class Camera;

struct Frustum
{
	bool isInCamera;

	std::array<Vec3, (int)FrustumPlane::Max> vec;
	std::array<Vec3, (int)FrustumPlane::Max> normal;
	std::array<Vec3, (int)FrustumPlane::Max> initalNormal;
	float horizontalFov = 0;    //　水平Fov
	float verticalFov = 0;      //　垂直Fov

	std::array<Vec3, (int)FrustumPlane::Max> frontVec;
	std::array<Vec3, (int)FrustumPlane::Max> rightVec;
	std::array<Vec3, (int)FrustumPlane::Max> upVec;

	Frustum() {};
	Frustum(Camera* cam);
	void CalcFrustum(Camera* cam);
};

class Camera :
	public Object3D
{
public:
	ComponentFactoryRegister(Camera)

		DLLExport Camera();
	DLLExport void SetRenderSize(float w, float h);
	void UseDefaultParams();

	DLLExport Float3 GetWorldPosFromScreen(const Float2& screen, float depth);
	DLLExport Ray GetScreenPosRay(const Float2& screen);

	//UseCurrentより後に呼ばれる場合のみ動作保証
	Matrix GetViewMat();
	//UseCurrentより後に呼ばれる場合のみ動作保証
	Matrix GetProjMat();

	Matrix GetBillboardMat();

	DLLExport bool CheckisInCameraInside(Vec3 pos, float r = 0);

	void OnInspectorWindowDraw();

	void FrustumCulling();

	void UpdateViewProjMatrix();

	Float3 target = { 0.0f, 0.0f, 0.0f };

	//レンダーサイズ
	float renderWidth;
	float renderHeight;

	bool useWindowSize = false;

	float nearZ;
	float farZ;
	float fov;

	SpConstBuffer<ConstBufferDataVProj> cameraViewProjMatrixCB;

	CameraTargetMode targetMode = CameraTargetMode::LookTo;

	static DLLExport void Set(Camera& camera);
	inline static Camera* sCurrent = nullptr;
	static void UseCurrent();

	static DLLExport Matrix GetCurrentCameraBillboardMat();

	ProjectionMode projectionMode = ProjectionMode::Perspective;

	void ReadParamJson([[maybe_unused]] const nlohmann::json& jsonObject) override;
	void WriteParamJson([[maybe_unused]] nlohmann::json& jsonObject) override;

	void CopyComponent(IComponent* src) override;

private:
	Matrix view;
	Matrix proj;
	Frustum frustum;
};