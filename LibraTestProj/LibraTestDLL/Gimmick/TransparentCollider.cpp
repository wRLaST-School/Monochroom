#include "TransparentCollider.h"
#include <ScriptComponent.h>

void TransparentCollider::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
}

void TransparentCollider::Update()
{
	Vec3 pos = mObj->position;
	Quaternion rot = Quaternion::EulerToQuaternion(mObj->rotationE);
	Vec3 scale = mObj->scale;
	scale.z *= 2;
	mBodyCollider.Setting(pos, rot, scale);
}

void TransparentCollider::Draw()
{
	mBodyCollider.DrawCollider();
}

OBBCollider TransparentCollider::GetBodyCollider()
{
	return mBodyCollider;
}

RegisterScriptBody(TransparentCollider);
