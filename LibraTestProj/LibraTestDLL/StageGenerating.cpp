#include "StageGenerating.h"
#include <SceneManager.h>
#include <ScriptComponent.h>
#include <Input.h>

void StageGenerating::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
	mPlaneAlpha = SceneManager::FindObject<Object3D>("PlaneAlpha");
	mPlane = SceneManager::FindObject<Object3D>("Plane");

	mObj->position = Vec3(0, 0, 330);

	Vec3 scale = Vec3(100, 100, 1);
	mPlaneAlpha->scale = scale;
	mPlane->scale = scale;

	isStart = false;
}

void StageGenerating::Update()
{
	if (Input::Key::Triggered(DIK_G))
	{
		isStart = true;
		mObj->position = Vec3(0, 0, -50);
	}

	if (isStart)
	{
		Vec3 vec = Vec3::front;
		const float moveSpeed = 1.5f;
		mObj->position += vec.Norm() * moveSpeed;

		if (mObj->position.z >= 330)
		{
			mObj->position.z = 330;
			isStart = false;
		}
	}
}

void StageGenerating::Draw()
{

}

RegisterScriptBody(StageGenerating);
