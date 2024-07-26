#include "FirstPersonCamera.h"
#include <stdafx.h>
#include <ScriptComponent.h>
#include <Input.h>
#include <SceneManager.h>
#include <AppOperationCommand.h>
#include <GameManager.h>
#include <ConsoleWindow.h>

void FirstPersonCamera::Init()
{
	isSet = false;

	mParentObj = This()->Parent()->CastTo<Camera>();
	Camera::Set(*mParentObj);

	CameraUpdate();
}

void FirstPersonCamera::Update()
{
	if (!GameManager::GetInstance()->GetStageGenerater()) {
		ConsoleWindow::Log("StageGenerator is null");
		return;
	}
	//デバッグ画面なら
	if (GameManager::GetInstance()->GetisStop())
	{
		return;
	}
	else if(!GameManager::GetInstance()->GetStageGenerater()->GetisEnd())
	{
		ConsoleWindow::Log("PLAYER_CAMERA_SET");
		CameraPosRotUpdateOnly();
		Camera::Set(*mParentObj);
	}
	//ステージ演出中なら
	if (!GameManager::GetInstance()->GetStageGenerater()->GetisEnd())
	{
		return;
	}


	if (!isSet)
	{
		isSet = true;
	}

	CameraUpdate();
}

void FirstPersonCamera::CameraUpdate()
{
	//プレイヤーオブジェクト
	player = SceneManager::FindObject<Object3D>("Player");
	if (!player || !mParentObj)
	{
		OutputDebugStringA("Player NULL");
		return;
	}

	//移動時のカメラ揺れ
	Vec3 playerMoveVec = AppOperationCommand::GetInstance()->PlayerMoveCommand();
	float kPlayerMoveLength = (fabs(playerMoveVec.x) + fabs(playerMoveVec.z));
	if (kPlayerMoveLength)
	{
		mTimer += 1.0f;
	}

	const float kWigglePosSpeed = kMoveWigglePosLength * kPlayerMoveLength;
	float kMoveWiggleOffsetPos = sinf(mTimer * kMoveWigglePosSpeed)
		* kWigglePosSpeed;

	const float kWiggleRotSpeed = kMoveWiggleRotLength * kPlayerMoveLength;
	float kMoveWiggleOffsetRot = sinf(mTimer * kMoveWiggleRotSpeed)
		* kWiggleRotSpeed;

	mParentObj->position = Vec3(player->position) + Vec3(0, kMoveWiggleOffsetPos, 0);
	mParentObj->rotationE = Vec3(player->rotationE) + Vec3(0, 0, kMoveWiggleOffsetRot);
}

void FirstPersonCamera::CameraPosRotUpdateOnly()
{
	player = SceneManager::FindObject<Object3D>("Player");
	mParentObj->position = Vec3(player->position);
	mParentObj->rotationE = Vec3(player->rotationE);
}

void FirstPersonCamera::Draw()
{

}

void FirstPersonCamera::CopyComponent(IComponent* src)
{

}

RegisterScriptBody(FirstPersonCamera);