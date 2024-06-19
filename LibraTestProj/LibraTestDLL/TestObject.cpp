#include "TestObject.h"
#include <ScriptComponent.h>
#include <Input.h>
#include <Object3D.h>
#include <SceneManager.h>
#include <SpImGui.h>

void TestObject::Init()
{
	name_ = "TestObject";
	auto obj = SceneManager::FindObject<Object3D>("Object3D");
}

void TestObject::Update()
{
	auto obj = This()->Parent()->CastTo<Object3D>();

	if (obj)
	{
		float move = (float)Input::Key::Down(DIK_UP) - (float)Input::Key::Down(DIK_DOWN);
		move *= 0.1f;

		obj->position.y += move;

		move = (float)(Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT));

		move *= 0.1f;

		obj->position.z += move;
	}
}

void TestObject::Draw()
{
}

void TestObject::OnInspectorWindowDraw()
{
	ImGui::Text("taumatawhakatangihangakoauauotamateaturipukakapikimaungahoronukupokaiwhenuakitanatahu");
}

void TestObject::CopyComponent(IComponent* src)
{
}
