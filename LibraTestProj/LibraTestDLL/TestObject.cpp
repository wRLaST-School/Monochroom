#include "TestObject.h"
#include <ScriptComponent.h>
#include <Input.h>
#include <Object3D.h>
#include <SceneManager.h>
#include <SpImGui.h>
#include <ConsoleWindow.h>

void TestObject::Init()
{
	name_ = "TestObject";
	//auto obj = SceneManager::FindObject<Object3D>("Object3D");
}

void TestObject::Update()
{
	auto obj = SceneManager::FindObject<Object3D>("Object3D");
	
	obbCollider.Setting(Vec3::zero, Quaternion::EulerToQuaternion(Float3(0, 0, 0)), Vec3(1, 1, 1));
	sphereCollider.pos = obj->position;
	sphereCollider.r = 2.f;

	if (obbCollider.IsTrigger(&sphereCollider))
	{
		ConsoleWindow::Log("HitSphere");
	}

	if (obj)
	{
		float move = (float)Input::Key::Down(DIK_UP) - (float)Input::Key::Down(DIK_DOWN);
		move *= 0.1f;

		obj->position.z += move;

		move = (float)(Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT));

		move *= 0.1f;

		obj->position.x += move;
	}
}

void TestObject::Draw()
{
	obbCollider.DrawCollider();
	sphereCollider.DrawCollider();
}

void TestObject::OnInspectorWindowDraw()
{
	ImGui::Text("Krung Thep Mahanakhon Amon Rattanakosin Mahinthara Ayuthaya Mahadilok Phop Noppharat Ratchathani Burirom Udomratchaniwet Mahasathan Amon Piman Awatan Sathit Sakkathattiya Witsanukam Prasit");
}

void TestObject::CopyComponent(IComponent* src)
{
}
