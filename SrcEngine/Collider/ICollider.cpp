#include "ICollider.h"
#include "Collision.h"

bool ICollider::IsTrigger(ICollider* other, Vec3* pushOut)
{
	color = Color::White;
	other->color = Color::White;
	if (!isActive || !other->isActive)
	{
		return false;
	}

#pragma region 自分自身がRayの時
	if (type == ColliderType::Ray)
	{
		if (other->type == ColliderType::Sphere)
		{
			return Collision::RayTriggerSphere(
				dynamic_cast<RayCollider*>(this),
				dynamic_cast<SphereCollider*>(other));
		}
		else if (other->type == ColliderType::OBB)
		{
			return Collision::OBBTriggerRay(
				dynamic_cast<OBBCollider*>(other),
				dynamic_cast<RayCollider*>(this));
		}
	}
#pragma endregion

#pragma region 自分自身がOBBの時
	if (type == ColliderType::OBB)
	{
		if (other->type == ColliderType::OBB)
		{
			return Collision::OBBTriggerOBB(
				dynamic_cast<OBBCollider*>(this),
				dynamic_cast<OBBCollider*>(other));
		}
		else if (other->type == ColliderType::Sphere)
		{
			return Collision::OBBTriggerSphere(
				dynamic_cast<OBBCollider*>(this),
				dynamic_cast<SphereCollider*>(other), pushOut);
		}
	}
#pragma endregion

#pragma region 自分自身がSphereの時
	if (type == ColliderType::Sphere)
	{
		if (other->type == ColliderType::OBB)
		{
			return Collision::OBBTriggerSphere(
				dynamic_cast<OBBCollider*>(other),
				dynamic_cast<SphereCollider*>(this), pushOut);
		}
		else if (other->type == ColliderType::Ray)
		{
			return Collision::RayTriggerSphere(
				dynamic_cast<RayCollider*>(other),
				dynamic_cast<SphereCollider*>(this));
		}
	}
#pragma endregion

	return false;
}
