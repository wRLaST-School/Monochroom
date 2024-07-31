#pragma once
#include<Emitter.h>
#include<AttractParticleTmp.h>

//引き寄せるときのパーティクルのエミッター
class AttractParticleEmitter : public Emitter<AttractParticleTmp>
{
public:
	AttractParticleEmitter() {
		shape = Shape::Sphere;
		quantity = 2;
		timeBetweenEmit = 1;
		radius.x = 0.07f;
	}

private:
	Vec3 mStartPos = { 0,0,0 };
	Vec3 mEndPos = { 0,0,0 };

	float mTime = 0;
	const float kTimeMax = 70.0f;

	bool mIsEffect = false;


private:


public:
	void BeginAttractEffect(const Vec3& sPos, const Vec3& ePos)
	{
		mStartPos = sPos;
		mEndPos = ePos;

		mIsEffect = true;
	}

};