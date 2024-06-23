#include "SphereCollider.h"
#include <LineDrawer.h>

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
    LineDrawer::DrawSphere(pos, r, color);
}
