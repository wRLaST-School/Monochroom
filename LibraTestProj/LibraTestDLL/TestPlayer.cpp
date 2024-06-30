#include "TestPlayer.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <format>
#include <functional>
#include <SpDS.h>
#include <Object3D.h>
#include <Input.h>
#include <ScriptComponent.h>
#include <format>
#include <ConsoleWindow.h>

void TestPlayer::Awake()
{
	OutputDebugStringA("TestPlayer Awake");
}

void TestPlayer::Init()
{
	OutputDebugStringA(std::format("TestPlayer Initialize, size: {}\n", sizeof(TestPlayer)).c_str());
	name_ = "TestPlayer";
}

void TestPlayer::Update()
{
	auto obj = This()->Parent()->CastTo<Object3D>();

	if (obj)
	{
		float move = (float)Input::Key::Down(DIK_UP) - (float)Input::Key::Down(DIK_DOWN);
		move *= 0.4f;

		obj->position.z += move;

		move = (float)(Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT));

		move *= 0.8f;
		
		obj->position.y += move;
	}

	auto& obj2 = This()->Parent()->GetAllComponents();
}

void TestPlayer::Draw()
{
}

void TestPlayer::CopyComponent(IComponent* src)
{
}


RegisterScriptBody(TestPlayer);