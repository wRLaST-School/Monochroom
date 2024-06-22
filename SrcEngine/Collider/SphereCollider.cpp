#include "SphereCollider.h"
#include <LineDrawer.h>

SphereCollider::SphereCollider()
{
    isActive = true;
    color = Color::White;
    type = ColliderType::Sphere;
}

//bool SphereCollider::Collide(PlaneCollider other)
//{
//    float oDist = ((Vec3)this->pos).Dot(other.norm);
//    float aDist = oDist - other.distance;
//    if (aDist < 0) aDist = -aDist;
//
//    return aDist <= this->r;
//}

void SphereCollider::DrawCollider()
{
    LineDrawer::DrawSphere(pos, r, color);
}
