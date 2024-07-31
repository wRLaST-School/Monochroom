#pragma once
#include <Vec3.h>

class Gravity
{
public:
	static float sGravityAccel;
	static Vec3 sLimitVelocityMin;
	static Vec3 sLimitVelocityMax;

private:
	Vec3 mGravityVec;
	Vec3 mVelocity;
	bool mUseGravity;

public:
	Gravity();
	Vec3 CalcGravity();
	Vec3 GetVelocity();
	void ZeroVelocity();
	void AddForce(const Vec3 force);
	void SetUseGravity(const bool useGravity);

};

