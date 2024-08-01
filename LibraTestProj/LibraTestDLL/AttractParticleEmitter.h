#pragma once
#include<Emitter.h>
#include<AttractParticleTmp.h>
#include<Easing.h>

//引き寄せるときのパーティクルのエミッター
class AttractParticleEmitter : public Emitter<AttractParticleTmp>
{
private:
	Vec3 mStartPos = { 0,0,0 };
	Vec3 mEndPos = { 0,0,0 };

	const float kTimeMax = 12.3f;
	const float kParticleMoveLength = 1.0f;
	std::unique_ptr<Easing> mEasing = nullptr;


public:
	AttractParticleEmitter() { ; }

	void Initialize(const Vec3& sPos, const Vec3& ePos)
	{
		shape = Shape::Sphere;
		quantity = 40;
		timeBetweenEmit = 1;
		radius.x = 0.05f * (float)Util::RNG(1, 10) * 0.1f;

		mStartPos = sPos;
		mEndPos = ePos;

		mEasing = std::make_unique<Easing>();
		mEasing->SetEaseTimer((int32_t)kTimeMax);
		mEasing->SetPowNum(2.0f);
	}

public:
	void Update();

	void ParticlesUpdate();
};