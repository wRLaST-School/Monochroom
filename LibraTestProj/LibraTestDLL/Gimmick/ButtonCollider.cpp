#include "ButtonCollider.h"
#include <ScriptComponent.h>
#include <SpImGui.h>

void ButtonCollider::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
	bodyColliderPos = Vec3(0.f,0.125f,0.f);
	FrameColliderPos = Vec3(0.f, 0., 0.f);

	bodyColliderScale = Vec3(1.5f, 0.75f, 1.5f);
	FrameColliderScale = Vec3(2.f, 0.375f, 2.f);
}

void ButtonCollider::Update()
{
	Vec3 pos = Vec3(mObj->position);
	Quaternion rot = Quaternion::EulerToQuaternion(mObj->rotationE);

	mBodyCollider.Setting(bodyColliderPos + pos, rot, Vec3::Mul(bodyColliderScale, Vec3(mObj->scale)));
	mFrameCollider.Setting(FrameColliderPos + pos, rot, Vec3::Mul(FrameColliderScale, Vec3(mObj->scale)));
}

void ButtonCollider::Draw()
{
	mBodyCollider.DrawCollider();
	mFrameCollider.DrawCollider();
}

void ButtonCollider::OnInspectorWindowDraw()
{
	if (ImGui::CollapsingHeader("Collider"))
	{
		ImGui::DragFloat3("Body Pos", &bodyColliderPos.x);
		ImGui::DragFloat3("Body Scale", &bodyColliderScale.x);
		ImGui::DragFloat3("Frame Pos", &FrameColliderPos.x);
		ImGui::DragFloat3("Frame Scale", &FrameColliderScale.x);
	}
}

OBBCollider ButtonCollider::GetBodyCollider()
{
	return mBodyCollider;
}

OBBCollider ButtonCollider::GetFrameCollider()
{
	return mFrameCollider;
}

RegisterScriptBody(ButtonCollider);
