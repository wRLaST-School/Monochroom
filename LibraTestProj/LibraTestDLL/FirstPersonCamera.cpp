#include "FirstPersonCamera.h"
#include <stdafx.h>
#include <ScriptComponent.h>
#include <Input.h>

void FirstPersonCamera::Init()
{
	player = This()->Parent()->Parent()->CastTo<Object3D>();
	obj = This()->Parent()->CastTo<Object3D>();
	offset = Vec3(0, 1, -1);
}

void FirstPersonCamera::Update()
{
	obj->position = Vec3(player->position) + offset;
}

void FirstPersonCamera::Draw()
{

}

void FirstPersonCamera::CopyComponent(IComponent* src)
{

}
