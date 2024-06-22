#include "stdafx.h"
#include "DebugCamera.h"
#include <Input.h>

using namespace Input;

DebugCamera::DebugCamera() :
	debugCamera(std::make_unique<Camera>())
{
	debugCamera->rotMode = Object3D::RotMode::Euler;
	yawAngle = 0;
	pitchAngle = 0;
}

void DebugCamera::Update()
{
	static Vec3 target = Vec3::zero;

	Vec2 mouseMove =
	{
		Mouse::GetVel().x,
		Mouse::GetVel().y
	};

	// 回転
	if (Mouse::Down(Click::Middle) && !Key::Down(DIK_LSHIFT))
	{
		const float moveSpeed = 0.1f;

		if (mouseMove.x != 0 || mouseMove.y != 0)
		{
			yawAngle += mouseMove.x * moveSpeed;
			pitchAngle += mouseMove.y * moveSpeed;
		}

		static Vec3 vec;

		float yawRadian = ConvertAngleToRadian(yawAngle);
		float pitchRadian = ConvertAngleToRadian(pitchAngle);

		// 方向ベクトル
		vec =
		{
			-sinf(yawRadian) * cosf(pitchRadian),
			+sinf(pitchRadian),
			-cosf(yawRadian) * cosf(pitchRadian),
		};

		// 球面座標
		float length = Vec3::Distance(target, debugCamera->position);
		debugCamera->position = target + vec.Norm() * length;

		// 回転
		debugCamera->rotationE.x = pitchRadian;
		debugCamera->rotationE.y = yawRadian;
	}

	// 平行移動
	if (Mouse::Down(Click::Middle) && Key::Down(DIK_LSHIFT))
	{
		const float moveSpeed = 0.1f;
		Vec3 frontVec = (target - debugCamera->position).Norm();
		Vec3 rightVec = frontVec.Cross(Vec3::up);
		Vec3 upVec = rightVec.Cross(frontVec);
		rightVec = frontVec.Cross(upVec);
		if (mouseMove.x != 0 || mouseMove.y != 0)
		{
			const Vec3 xVel = rightVec * mouseMove.x * moveSpeed;
			const Vec3 yVel = upVec * mouseMove.y * moveSpeed;

			debugCamera->position += xVel + yVel;
			target += xVel + yVel;
		}
	}

	// スクロール
	if (Mouse::GetWheelVel() != 0 && !Mouse::Down(Click::Middle))
	{
		float dis = (target - debugCamera->position).GetLength();
		float speedRate = dis * 0.01f;
		if (speedRate < 0.01f)
		{
			speedRate = 0.01f;

			if (Mouse::GetWheelVel() > 0)
			{
				speedRate = 0;
			}
		}
		else if (speedRate >= 5)
		{
			speedRate = 5;
		}


		const float moveSpeed = 0.05f * speedRate;
		Matrix viewMat = debugCamera->GetViewMat();
		Vec3 vec = Vec3(-viewMat.ExtractAxisZ().x, -viewMat.ExtractAxisZ().y, viewMat.ExtractAxisZ().z);
		debugCamera->position += vec * Mouse::GetWheelVel() * moveSpeed;
	}

	if (Key::Down(DIK_LSHIFT) && Key::Down(DIK_C))
	{
		target = Vec3::zero;
		debugCamera->position = Vec3(0, 0, -10);
		debugCamera->rotationE = Vec3(0, 0, 0);
		yawAngle = 0;
		pitchAngle = 0;
	}
	if (Key::Down(DIK_LSHIFT) && Key::Down(DIK_T))
	{
		target = Vec3::zero;
		debugCamera->position = Vec3(0, 0, -10);
		debugCamera->rotationE = Vec3(ConvertAngleToRadian(45), 0, 0);
		yawAngle = 0;
		pitchAngle = 0;
	}

	debugCamera->UpdateMatrix();
	Camera::Set(*debugCamera.get());
}
