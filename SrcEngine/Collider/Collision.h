#pragma once
#include <RayCollider.h>
#include <OBBCollider.h>
#include <SphereCollider.h>

namespace Collision
{
	DLLExport bool RayTriggerSphere(RayCollider* ray, SphereCollider* sphere);
	DLLExport bool OBBTriggerOBB(OBBCollider* obb1, OBBCollider* obb2, Vec3* pushOut = nullptr);
	DLLExport bool OBBTriggerSphere(OBBCollider* obb, SphereCollider* sphere, Vec3* pushOut = nullptr);
	DLLExport bool OBBTriggerRay(OBBCollider* obb, RayCollider* ray);
};

