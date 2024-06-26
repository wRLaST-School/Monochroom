#include "SphereCollider.h"
#include <LineDrawer.h>
#include <GameManager.h>

SphereCollider::SphereCollider()
{
	isActive = true;
	color = Color::White;
	type = ColliderType::Sphere;
}

void SphereCollider::Setting(const Vec3 aPos, const float aR)
{
	pos = aPos;
	r = aR;
}

void SphereCollider::DrawCollider()
{
	if (GameManager::sShowHitBoxes)
	{
		LineDrawer::DrawSphere(pos, r, color);
	}
}
