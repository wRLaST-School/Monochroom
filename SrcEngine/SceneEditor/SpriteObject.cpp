#include "stdafx.h"
#include "SpriteObject.h"
#include <Sprite.h>
#include <SpImGui.h>
#include <Input.h>
#include <SpDS.h>

void SpriteObject::Update()
{
}

void SpriteObject::Draw()
{
	SpDS::DrawRotaGraph(position.x, position.y,
		scale.x, scale.y,
		0.f,
		tex,
		Anchor::Center,
		color
	);
}

void SpriteObject::OnInspectorWindowDraw()
{
	ImGui::DragFloat2("Pos", &position.x);
	ImGui::DragFloat2("Scale", &scale.x);

	SpImGui::InputText("Texture", &tex, NULL);
	ImGui::ColorEdit4("Brightness", &color.f4.x);

	//gizmo
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
		ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	ImGuizmo::AllowAxisFlip(true);

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

	ImGuizmo::Enable(true);

	if (Input::Key::Down(DIK_T))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (Input::Key::Down(DIK_S))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	Matrix view;
	Matrix proj = Sprite::sProj;

	Matrix matWorld = Matrix(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, 1, 0,
		position.x, position.y, 0, 1
	);

	ImGuizmo::Manipulate(reinterpret_cast<float*>(&view),
		reinterpret_cast<float*>(&proj), mCurrentGizmoOperation, ImGuizmo::WORLD, &matWorld[0][0], NULL, NULL);

	if (ImGuizmo::IsUsing())
	{
		scale.x = matWorld[0][0];
		scale.y = matWorld[1][1];

		position.x = matWorld[3][0];
		position.y = matWorld[3][1];
	}
}

void SpriteObject::WriteParamJson(nlohmann::json& jsonObj)
{
	jsonObj["Name"] = name_;
	jsonObj["Texture"] = tex;
	jsonObj["posX"] = position.x;
	jsonObj["posY"] = position.y;
	jsonObj["scaleX"] = scale.x;
	jsonObj["scaleY"] = scale.y;
	jsonObj["Brightness"]["X"] = color.f4.x;
	jsonObj["Brightness"]["Y"] = color.f4.y;
	jsonObj["Brightness"]["Z"] = color.f4.z;
	jsonObj["Brightness"]["W"] = color.f4.w;
}

void SpriteObject::ReadParamJson(const nlohmann::json& jsonObj)
{
	name_ = jsonObj["Name"];
	tex = jsonObj["Texture"];
	position.x = jsonObj["posX"];
	position.y = jsonObj["posY"];
	scale.x = jsonObj["scaleX"];
	scale.y = jsonObj["scaleY"];

	if (jsonObj.contains("Brightness"))
	{
		color.f4.x = jsonObj["Brightness"]["X"];
		color.f4.y = jsonObj["Brightness"]["Y"];
		color.f4.z = jsonObj["Brightness"]["Z"];
		color.f4.w = jsonObj["Brightness"]["W"];
	}
}

void SpriteObject::CopyComponent(IComponent* src)
{
	SpriteObject* cast = dynamic_cast<SpriteObject*>(src);
	tex = cast->tex;
	position = cast->position;
	scale = cast->scale;
}
