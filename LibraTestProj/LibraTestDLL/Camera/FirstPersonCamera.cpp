#include "FirstPersonCamera.h"
#include <stdafx.h>
#include <ScriptComponent.h>
#include <Input.h>
#include <SceneManager.h>
#include <Camera.h>
#include <AppOperationCommand.h>


void FirstPersonCamera::Init()
{
	isSet = false;
}

void FirstPersonCamera::Update()
{
	parentObj = This()->Parent()->CastTo<Object3D>();

	if (!isSet)
	{
		isSet = true;
	}
	Camera::Set(*parentObj->CastTo<Camera>());

	player = SceneManager::FindObject<Object3D>("Player");
	if (!player || !parentObj)
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

	parentObj->position = Vec3(player->position) + Vec3(0, kMoveWiggleOffsetPos, 0);
	parentObj->rotationE = Vec3(player->rotationE) + Vec3(0, 0, kMoveWiggleOffsetRot);
}

void FirstPersonCamera::Draw()
{

}

void FirstPersonCamera::CopyComponent(IComponent* src)
{

}

RegisterScriptBody(FirstPersonCamera);