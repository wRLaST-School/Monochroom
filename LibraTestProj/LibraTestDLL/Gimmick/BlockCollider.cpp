#include "BlockCollider.h"
#include <ScriptComponent.h>

void BlockCollider::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
}

void BlockCollider::Update()
{
	Vec3 pos = Vec3(mObj->matWorld.ExtractTranslation());
	Quaternion rot = Quaternion::EulerToQuaternion(mObj->rotationE);
	Vec3 scale = mObj->scale;
	mBodyCollider.Setting(pos, rot, scale);
}

void BlockCollider::Draw()
{
	mBodyCollider.DrawCollider();
}

void BlockCollider::CopyComponent(IComponent* src)
{

}

OBBCollider BlockCollider::GetBodyCollider()
{
	return mBodyCollider;
}

RegisterScriptBody(BlockCollider);