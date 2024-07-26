#include "DoorCollider.h"
#include <ScriptComponent.h>
#include <SceneManager.h>
#include <SpImGui.h>
#include <ConsoleWindow.h>

void DoorCollider::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
	mLeft = SceneManager::FindChildObject<Object3D>("Left", mObj);
	mRight = SceneManager::FindChildObject<Object3D>("Right", mObj);

	colliderScale = Vec3(1.f, 2.f, 0.5f);
}

void DoorCollider::Update()
{
	Vec3 leftWorldPos = mLeft->matWorld.ExtractTranslation();
	Vec3 rightWorldPos = mRight->matWorld.ExtractTranslation();

	ConsoleWindow::LogVec3("leftWorldPos", leftWorldPos);
	ConsoleWindow::LogVec3("rightWorldPos", rightWorldPos);

	mLeftCollider.Setting(leftColliderPos + leftWorldPos, Quaternion(), Vec3::Mul(mObj->scale, colliderScale));
	mRightCollider.Setting(rightColliderPos + rightWorldPos, Quaternion(), Vec3::Mul(mObj->scale, colliderScale));
}

void DoorCollider::Draw()
{
	mLeftCollider.DrawCollider();
	mRightCollider.DrawCollider();
}

void DoorCollider::OnInspectorWindowDraw()
{
	if (ImGui::CollapsingHeader("Collider"))
	{
		ImGui::DragFloat3("Left", &leftColliderPos.x);
		ImGui::DragFloat3("Right", &rightColliderPos.x);
		ImGui::DragFloat3("Scale", &colliderScale.x);
	}
}

OBBCollider DoorCollider::GetLeftCollider()
{
	return mLeftCollider;
}

OBBCollider DoorCollider::GetRightCollider()
{
	return mRightCollider;
}

RegisterScriptBody(DoorCollider);
