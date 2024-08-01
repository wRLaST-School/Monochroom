#include "Gravity.h"
float Gravity::sGravityAccel = 0.018f;
Vec3 Gravity::sLimitVelocityMin = Vec3(0.f, -0.04f, 0.f);
Vec3 Gravity::sLimitVelocityMax = Vec3(0.f, 0.12f, 0.f);

Gravity::Gravity()
{
	mGravityVec = Vec3::down;
	mVelocity = Vec3::zero;
	mUseGravity = true;
}

Vec3 Gravity::CalcGravity()
{
	if (!mUseGravity)
	{
		return Vec3::zero;
	}

	mVelocity += mGravityVec.GetNorm() * sGravityAccel;
	mVelocity.ClampMax(sLimitVelocityMax);
	mVelocity.ClampMin(sLimitVelocityMin);

	return mVelocity;
}

Vec3 Gravity::GetVelocity()
{
	return mVelocity;
}

void Gravity::ZeroVelocity()
{
	mVelocity = Vec3::zero;
}

void Gravity::AddForce(const Vec3 force)
{
	mVelocity += force;
}

void Gravity::SetUseGravity(const bool useGravity)
{
	mUseGravity = useGravity;
}