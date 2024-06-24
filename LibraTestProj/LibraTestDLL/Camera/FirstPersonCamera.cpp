#include "FirstPersonCamera.h"
#include <stdafx.h>
#include <ScriptComponent.h>
#include <Input.h>
#include <SceneManager.h>

void FirstPersonCamera::Init()
{
	parentObj = This()->Parent()->CastTo<Object3D>();
}

void FirstPersonCamera::Update()
{
	player = SceneManager::FindObject<Object3D>("Player");
	if (!player)
	{
		OutputDebugStringA("Player NULL");
		return;
	}

	Vec3 frontOffset;
	frontOffset.x = -(OFFSET.x * cosf(parentObj->rotationE.y) - OFFSET.z * sinf(parentObj->rotationE.y));
	frontOffset.z = OFFSET.x * sinf(parentObj->rotationE.y) + OFFSET.z * cosf(parentObj->rotationE.y);

	parentObj->position = Vec3(player->position) + frontOffset;
	parentObj->rotationE = Vec3(player->rotationE);
}

void FirstPersonCamera::Draw()
{

}

void FirstPersonCamera::CopyComponent(IComponent* src)
{

}

RegisterScriptBody(FirstPersonCamera);