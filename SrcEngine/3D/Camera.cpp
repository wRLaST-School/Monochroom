#include "Camera.h"

#include <SpDirectX.h>
#include <SpEffekseer.h>
#include <SpImGui.h>
#include <DirectionalLight.h>

Camera::Camera()
{
	renderWidth = 0;
	renderHeight = 0;

	useWindowSize = true;
	nearZ = .1f;
	farZ = 1000.f;

	position = { 0.f, 0.f, -10.f };
	rotation = Quaternion::Identity();

	fov = PIf / 2;
	nearZ = 0.1f;
	farZ = 1000.0f;

	frustum = Frustum(this);
}

void Camera::SetRenderSize(float w, float h)
{
	renderWidth = w;
	renderHeight = h;
}

void Camera::UseDefaultParams()
{
	useWindowSize = true;
	nearZ = .1f;
	farZ = 1000.f;

	position = { 0.f, 0.f, -10.f };
	rotation = Quaternion::Identity();

	fov = PIf / 2;
	nearZ = 0.1f;
	farZ = 1000.0f;
}

Matrix Camera::GetBillboardMat()
{
	Matrix camRot = targetMode == CameraTargetMode::LookAt ?
		Matrix::ViewLookAt(position, target, matWorld.ExtractAxisY()) :
		Matrix::ViewLookTo(position, matWorld.ExtractAxisZ(), matWorld.ExtractAxisY());
	camRot[3] = { 0,0,0,1 };

	return -camRot;
}

bool Camera::CheckisInCameraInside(Vec3 pos, float r)
{
	Vec3 toPlaneNorm = (pos - position)/*.GetNorm()*/;
	bool isInside = true;
	for (int i = 0; i < 4; i++)
	{
		float dot = Vec3::Dot(frustum.normal[i], toPlaneNorm);
		if (dot > r * 2)
		{
			isInside = false;
			break;
		}
	}

	return isInside;
}

void Camera::OnInspectorWindowDraw()
{
	Object3D::OnInspectorWindowDraw();

	if (ImGui::Button("Activate Camera"))
	{
		Camera::Set(*this);
	}
}

void Camera::FrustumCulling()
{
	frustum.CalcFrustum(this);
}

void Camera::UpdateViewProjMatrix()
{
	view = targetMode == CameraTargetMode::LookAt ?
		Matrix::ViewLookAt(position, target, matWorld.ExtractAxisY()) :
		Matrix::ViewLookTo(position, matWorld.ExtractAxisZ(), matWorld.ExtractAxisY());

	proj = projectionMode == ProjectionMode::Perspective ?
		Matrix::Projection(fov, (float)renderWidth / (float)renderHeight, nearZ, farZ) :
		Matrix::ProjectionOrtho((int32_t)renderWidth, (int32_t)renderHeight, nearZ, farZ, 20);
}

void Camera::Set(Camera& camera)
{
	sCurrent = &camera;
}

void Camera::UseCurrent()
{
	if (sCurrent->useWindowSize)
	{
		SpWindow* wnd = GetSpWindow();
		sCurrent->renderWidth = (float)wnd->width;
		sCurrent->renderHeight = (float)wnd->height;
	}

	D3D12_VIEWPORT viewport{};

	viewport.Width = sCurrent->renderWidth;
	viewport.Height = sCurrent->renderHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	GetSpDX()->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // 切り抜き座標左
	scissorrect.right = scissorrect.left + (LONG)sCurrent->renderWidth;        // 切り抜き座標右
	scissorrect.top = 0;                                        // 切り抜き座標上
	scissorrect.bottom = scissorrect.top + (LONG)sCurrent->renderHeight;       // 切り抜き座標下

	GetSpDX()->cmdList->RSSetScissorRects(1, &scissorrect);

	sCurrent->UpdateViewProjMatrix();

	//sCurrent->view = sCurrent->targetMode == CameraTargetMode::LookAt ?
	//	Matrix::ViewLookAt(sCurrent->position, sCurrent->target, sCurrent->matWorld.ExtractAxisY()) :
	//	Matrix::ViewLookTo(sCurrent->position, sCurrent->matWorld.ExtractAxisZ(), sCurrent->matWorld.ExtractAxisY());
	//
	//sCurrent->proj = sCurrent->projectionMode == ProjectionMode::Perspective ?
	//	Matrix::Projection(sCurrent->fov, (float)sCurrent->renderWidth / (float)sCurrent->renderHeight, sCurrent->nearZ, sCurrent->farZ) :
	//	Matrix::ProjectionOrtho((int32_t)sCurrent->renderWidth, (int32_t)sCurrent->renderHeight, sCurrent->nearZ, sCurrent->farZ, 20);

	sCurrent->cameraViewProjMatrixCB.contents->vproj = sCurrent->view * sCurrent->proj;
	sCurrent->cameraViewProjMatrixCB.contents->cameraPos = sCurrent->position;

	sCurrent->cameraViewProjMatrixCB.contents->billboardMat = GetCurrentCameraBillboardMat();

	GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(3, sCurrent->cameraViewProjMatrixCB.buffer->GetGPUVirtualAddress());

	//Effekseerの情報を更新
	std::function<Effekseer::Matrix44(Matrix)> SpMatToEfkMat = [](Matrix in) {
		Effekseer::Matrix44 out;

		for (int32_t i = 0; i < 4; i++)
		{
			for (int32_t j = 0; j < 4; j++)
			{
				out.Values[i][j] = in[i][j];
			}
		}

		return out;
		};

	Effekseer::Matrix44 efkViewMat = SpMatToEfkMat(sCurrent->view);
	Effekseer::Matrix44 efkProjMat = SpMatToEfkMat(sCurrent->proj);

	SpEffekseer::SetMatrices(efkViewMat, efkProjMat);

	sCurrent->FrustumCulling();
}

Matrix Camera::GetCurrentCameraBillboardMat()
{
	Matrix camRot = sCurrent->targetMode == CameraTargetMode::LookAt ?
		Matrix::ViewLookAt(sCurrent->position, sCurrent->target, sCurrent->matWorld.ExtractAxisY()) :
		Matrix::ViewLookTo(sCurrent->position, sCurrent->matWorld.ExtractAxisZ(), sCurrent->matWorld.ExtractAxisY());
	camRot[3] = { 0,0,0,1 };

	return -camRot;
}

void Camera::ReadParamJson(const nlohmann::json& jsonObject)
{
	if (jsonObject.contains("Name"))
	{
		name_ = jsonObject["Name"];
	}

	position.x = jsonObject["Position"]["X"];
	position.y = jsonObject["Position"]["Y"];
	position.z = jsonObject["Position"]["Z"];

	scale.x = jsonObject["Scale"]["X"];
	scale.y = jsonObject["Scale"]["Y"];
	scale.z = jsonObject["Scale"]["Z"];

	rotation.v.x = jsonObject["Rotation"]["X"];
	rotation.v.y = jsonObject["Rotation"]["Y"];
	rotation.v.z = jsonObject["Rotation"]["Z"];
	rotation.w = jsonObject["Rotation"]["W"];

	rotationE.x = jsonObject["RotationEuler"]["X"];
	rotationE.y = jsonObject["RotationEuler"]["Y"];
	rotationE.z = jsonObject["RotationEuler"]["Z"];

	rotMode = jsonObject["UseQuaternionRot"];
}

void Camera::WriteParamJson(nlohmann::json& jsonObject)
{
	jsonObject["Name"] = name_;

	jsonObject["Position"]["X"] = position.x;
	jsonObject["Position"]["Y"] = position.y;
	jsonObject["Position"]["Z"] = position.z;

	jsonObject["Scale"]["X"] = scale.x;
	jsonObject["Scale"]["Y"] = scale.y;
	jsonObject["Scale"]["Z"] = scale.z;

	jsonObject["Rotation"]["X"] = rotation.v.x;
	jsonObject["Rotation"]["Y"] = rotation.v.y;
	jsonObject["Rotation"]["Z"] = rotation.v.z;
	jsonObject["Rotation"]["W"] = rotation.w;

	jsonObject["RotationEuler"]["X"] = rotationE.x;
	jsonObject["RotationEuler"]["Y"] = rotationE.y;
	jsonObject["RotationEuler"]["Z"] = rotationE.z;

	jsonObject["UseQuaternionRot"] = rotMode;
}

void Camera::CopyComponent(IComponent* src)
{
	Camera* cast = dynamic_cast<Camera*>(src);
	name_ = cast->name_;
	target = cast->target;
	renderWidth = cast->renderWidth;
	renderHeight = cast->renderHeight;
	useWindowSize = cast->useWindowSize;
	nearZ = cast->nearZ;
	farZ = cast->farZ;
	fov = cast->fov;
	view = cast->view;
	proj = cast->proj;
	farZ = cast->farZ;
	frustum = cast->frustum;
	Object3D::CopyComponent(src);
}

Float3 Camera::GetWorldPosFromScreen(const Float2& screen, float depth)
{
	if (useWindowSize)
	{
		SpWindow* wnd = GetSpWindow();
		renderWidth = (float)wnd->width;
		renderHeight = (float)wnd->height;
	}

	Matrix vMat = targetMode == CameraTargetMode::LookAt ?
		Matrix::ViewLookAt(position, target, matWorld.ExtractAxisY()) :
		Matrix::ViewLookTo(position, matWorld.ExtractAxisZ(), matWorld.ExtractAxisY());

	Matrix pMat = projectionMode == ProjectionMode::Perspective ?
		Matrix::Projection(fov, (float)renderWidth / (float)renderHeight, nearZ, farZ) :
		Matrix::ProjectionOrtho((int32_t)renderWidth, (int32_t)renderHeight, nearZ, farZ, 20);

	Matrix vport(
		(float)GetSpWindow()->width / 2, 0.f, 0.f, 0.f,
		0.f, (float)-GetSpWindow()->height / 2, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		(float)GetSpWindow()->width / 2, (float)GetSpWindow()->height / 2, 0, 1
	);

	Matrix wMat = Matrix::Identity();
	wMat[3][0] = screen.x;
	wMat[3][1] = screen.y;
	wMat[3][2] = 0.f;

	Matrix vpvpMat = vMat * pMat * vport;
	Matrix invMat = -vpvpMat;

	Matrix result = wMat * (-vpvpMat);

	result[3][0] /= result[3][3];
	result[3][1] /= result[3][3];
	result[3][2] /= result[3][3];

	Vec3 nearPos(result[3][0], result[3][1], result[3][2]);

	wMat = Matrix::Identity();
	wMat[3][0] = screen.x;
	wMat[3][1] = screen.y;
	wMat[3][2] = 1.f;

	result = wMat * (-vpvpMat);

	result[3][0] /= result[3][3];
	result[3][1] /= result[3][3];
	result[3][2] /= result[3][3];

	Vec3 farPos(result[3][0], result[3][1], result[3][2]);

	Vec3 ray = farPos - nearPos;

	return nearPos + ray.SetLength(depth);
}

Ray Camera::GetScreenPosRay(const Float2& screen)
{
	if (useWindowSize)
	{
		SpWindow* wnd = GetSpWindow();
		renderWidth = (float)wnd->width;
		renderHeight = (float)wnd->height;
	}

	Matrix vMat = targetMode == CameraTargetMode::LookAt ?
		Matrix::ViewLookAt(position, target, matWorld.ExtractAxisY()) :
		Matrix::ViewLookTo(position, matWorld.ExtractAxisZ(), matWorld.ExtractAxisY());

	Matrix pMat = projectionMode == ProjectionMode::Perspective ?
		Matrix::Projection(fov, (float)renderWidth / (float)renderHeight, nearZ, farZ) :
		Matrix::ProjectionOrtho((int32_t)renderWidth, (int32_t)renderHeight, nearZ, farZ, 20);

	Matrix vport(
		(float)GetSpWindow()->width / 2, 0.f, 0.f, 0.f,
		0.f, -(float)GetSpWindow()->height / 2, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		(float)GetSpWindow()->width / 2, (float)GetSpWindow()->height / 2, 0, 1
	);

	Matrix wMat = Matrix::Identity();
	wMat[3][0] = screen.x;
	wMat[3][1] = screen.y;
	wMat[3][2] = 0.f;

	Matrix vpvpMat = vMat * pMat * vport;
	Matrix invMat = -vpvpMat;

	Matrix result = wMat * (-vpvpMat);

	result[3][0] /= result[3][3];
	result[3][1] /= result[3][3];
	result[3][2] /= result[3][3];

	Vec3 nearPos(result[3][0], result[3][1], result[3][2]);

	wMat = Matrix::Identity();
	wMat[3][0] = screen.x;
	wMat[3][1] = screen.y;
	wMat[3][2] = 1.f;

	result = wMat * (-vpvpMat);

	result[3][0] /= result[3][3];
	result[3][1] /= result[3][3];
	result[3][2] /= result[3][3];

	Vec3 farPos(result[3][0], result[3][1], result[3][2]);

	Vec3 ray = farPos - nearPos;

	Ray r;
	r.ray = ray;
	r.origin = nearPos;
	return r;
}

Matrix Camera::GetViewMat()
{
	return view;
}

Matrix Camera::GetProjMat()
{
	return proj;
}

Frustum::Frustum(Camera* cam)
{
	// カメラの情報
	//float aspectRatio = (float)cam->renderWidth / (float)cam->renderHeight;
	float aspectRatio = 1920.f / 1080.f;
	float tanHalfFov = tanf(cam->fov / 2);

	// 水平Fovと垂直Fovを計算
	horizontalFov = atanf(tanHalfFov * aspectRatio);
	verticalFov = cam->fov / 2;

	// 向いてる方向
	vec[(int)FrustumPlane::Left] = Vec3(-sinf(horizontalFov), 0, cosf(horizontalFov));
	vec[(int)FrustumPlane::Right] = Vec3(sinf(horizontalFov), 0, cosf(horizontalFov));
	vec[(int)FrustumPlane::Top] = Vec3(0, sinf(verticalFov), cosf(verticalFov));
	vec[(int)FrustumPlane::Bottom] = Vec3(0, -sinf(verticalFov), cosf(verticalFov));

	// 左の面の法線ベクトルの計算
	frontVec[(int)FrustumPlane::Left] = vec[(int)FrustumPlane::Left];
	rightVec[(int)FrustumPlane::Left] = Vec3::Cross(frontVec[(int)FrustumPlane::Left], Vec3::up);
	upVec[(int)FrustumPlane::Left] = Vec3::Cross(rightVec[(int)FrustumPlane::Left], frontVec[(int)FrustumPlane::Left]);
	rightVec[(int)FrustumPlane::Left] = Vec3::Cross(frontVec[(int)FrustumPlane::Left], upVec[(int)FrustumPlane::Left]);
	initalNormal[(int)FrustumPlane::Left] = rightVec[(int)FrustumPlane::Left];

	// 右の面の法線ベクトルの計算
	frontVec[(int)FrustumPlane::Right] = vec[(int)FrustumPlane::Right];
	rightVec[(int)FrustumPlane::Right] = Vec3::Cross(frontVec[(int)FrustumPlane::Right], Vec3::down);
	upVec[(int)FrustumPlane::Right] = Vec3::Cross(rightVec[(int)FrustumPlane::Right], frontVec[(int)FrustumPlane::Right]);
	rightVec[(int)FrustumPlane::Right] = Vec3::Cross(frontVec[(int)FrustumPlane::Right], upVec[(int)FrustumPlane::Right]);
	initalNormal[(int)FrustumPlane::Right] = rightVec[(int)FrustumPlane::Right];

	// 上の面の法線ベクトルの計算
	frontVec[(int)FrustumPlane::Top] = vec[(int)FrustumPlane::Top];
	upVec[(int)FrustumPlane::Top] = Vec3::Cross(frontVec[(int)FrustumPlane::Top], Vec3::left);
	rightVec[(int)FrustumPlane::Top] = Vec3::Cross(upVec[(int)FrustumPlane::Top], frontVec[(int)FrustumPlane::Top]);
	upVec[(int)FrustumPlane::Top] = Vec3::Cross(rightVec[(int)FrustumPlane::Top], frontVec[(int)FrustumPlane::Top]);
	initalNormal[(int)FrustumPlane::Top] = upVec[(int)FrustumPlane::Top];

	// 下の面の法線ベクトルの計算
	frontVec[(int)FrustumPlane::Bottom] = vec[(int)FrustumPlane::Bottom];
	upVec[(int)FrustumPlane::Bottom] = Vec3::Cross(frontVec[(int)FrustumPlane::Bottom], Vec3::right);
	rightVec[(int)FrustumPlane::Bottom] = Vec3::Cross(upVec[(int)FrustumPlane::Bottom], frontVec[(int)FrustumPlane::Bottom]);
	upVec[(int)FrustumPlane::Bottom] = Vec3::Cross(rightVec[(int)FrustumPlane::Bottom], frontVec[(int)FrustumPlane::Bottom]);
	initalNormal[(int)FrustumPlane::Bottom] = upVec[(int)FrustumPlane::Bottom];
}

void Frustum::CalcFrustum(Camera* cam)
{
	// カメラの情報
	float aspectRatio = (float)cam->renderWidth / (float)cam->renderHeight;
	float tanHalfFov = tanf(cam->fov / 2);

	// 水平Fovと垂直Fovを計算
	horizontalFov = atanf(tanHalfFov * aspectRatio);
	verticalFov = cam->fov / 2;

	// 向いてる方向
	vec[(int)FrustumPlane::Left] = Vec3(-sinf(horizontalFov), 0, cosf(horizontalFov));
	vec[(int)FrustumPlane::Right] = Vec3(sinf(horizontalFov), 0, cosf(horizontalFov));
	vec[(int)FrustumPlane::Top] = Vec3(0, sinf(verticalFov), cosf(verticalFov));
	vec[(int)FrustumPlane::Bottom] = Vec3(0, -sinf(verticalFov), cosf(verticalFov));

	// カメラのy軸回転を考慮し回転後のベクトルを作成
	float camAngleX = cam->rotationE.x;
	float camAngleY = cam->rotationE.y;

	// y軸周りの回転行列を作成
	Quaternion xRotation = Quaternion::AngleAxis(Vec3::right, ConvertRadianToAngle(camAngleX));
	Quaternion yRotation = Quaternion::AngleAxis(Vec3::up, ConvertRadianToAngle(camAngleY));

	for (int i = 0; i < 4; i++)
	{
		normal[i] = Quaternion::AnyAxisRotation(initalNormal[i], Vec3::right, camAngleX);	// X軸回転
		normal[i] = Quaternion::AnyAxisRotation(normal[i], Vec3::up, camAngleY);		// Y軸回転
	}
}
