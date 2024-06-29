#include "ButtonCollider.h"
#include <ScriptComponent.h>
#include <SpImGui.h>

void ButtonCollider::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
	bodyColliderPos = Vec3(0.f,0.125f,0.f);
	FlameColliderPos = Vec3(0.f, 0., 0.f);

	bodyColliderScale = Vec3(1.5f, 0.75f, 1.5f);
	FlameColliderScale = Vec3(2.f, 0.375f, 2.f);
}

void ButtonCollider::Update()
{
	Vec3 pos = Vec3(mObj->position);
	Quaternion rot = Quaternion::EulerToQuaternion(mObj->rotationE);

	mBodyCollider.Setting(bodyColliderPos + pos, rot, Vec3::Mul(bodyColliderScale, Vec3(mObj->scale)));
	mFlameCollider.Setting(FlameColliderPos + pos, rot, Vec3::Mul(FlameColliderScale, Vec3(mObj->scale)));
}

void ButtonCollider::Draw()
{
	mBodyCollider.DrawCollider();
	mFlameCollider.DrawCollider();
}

void ButtonCollider::OnInspectorWindowDraw()
{
	if (ImGui::CollapsingHeader("Collider"))
	{
		ImGui::DragFloat3("Body Pos", &bodyColliderPos.x);
		ImGui::DragFloat3("Body Scale", &bodyColliderScale.x);
		ImGui::DragFloat3("Flame Pos", &FlameColliderPos.x);
		ImGui::DragFloat3("Flame Scale", &FlameColliderScale.x);
	}
}

OBBCollider ButtonCollider::GetBodyCollider()
{
	return mBodyCollider;
}

OBBCollider ButtonCollider::GetFlameCollider()
{
	return mFlameCollider;
}

RegisterScriptBody(ButtonCollider);
