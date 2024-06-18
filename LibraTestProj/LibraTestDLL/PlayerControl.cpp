#include "PlayerControl.h"
#include <stdafx.h>
#include <ScriptComponent.h>
#include <Input.h>

void PlayerControl::Init()
{
	obj = This()->Parent()->CastTo<Object3D>();
	moveSpeed = 0.5f;
}

void PlayerControl::Update()
{
	frontVec.x = Input::Key::Down(DIK_D) - Input::Key::Down(DIK_A);
	frontVec.z = Input::Key::Down(DIK_W) - Input::Key::Down(DIK_S);

	obj->position += frontVec.GetNorm() * moveSpeed;
}

void PlayerControl::Draw()
{

}

void PlayerControl::CopyComponent(IComponent* src)
{

}
