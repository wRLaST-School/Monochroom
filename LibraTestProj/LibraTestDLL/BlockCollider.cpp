#include "BlockCollider.h"
#include <ScriptComponent.h>

void BlockCollider::Init()
{
	obj = This()->Parent()->CastTo<Object3D>();
}

void BlockCollider::Update()
{
	Vec3 pos = Vec3(obj->position);
	Quaternion rot = Quaternion::EulerToQuaternion(obj->rotationE);
	Vec3 scale = obj->scale;
	bodyCollider.Setting(pos, rot, scale);
}

void BlockCollider::Draw()
{
	bodyCollider.DrawCollider();
}

void BlockCollider::CopyComponent(IComponent* src)
{

}
