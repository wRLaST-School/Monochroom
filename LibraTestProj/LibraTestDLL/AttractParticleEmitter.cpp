#include "AttractParticleEmitter.h"
#include <Util.h>

void AttractParticleEmitter::Update()
{
	mEasing->Update();

	position = mEasing->In(mStartPos, mEndPos);

	//最初の位置からもう一度移動開始
	if (mEasing->GetTimeRate() >= 1.0f)
	{
		mEasing->Reset();

		mEasing->SetEaseTimer(Util::RNG(0, (int32_t)kTimeMax, true));
	}
}

void AttractParticleEmitter::ParticlesUpdate()
{
	Vec3 dir = mEndPos - position;
	float length = dir.GetLength();
	Vec3 moveVec = dir.GetNorm() * length * kParticleMoveLength * (float)Util::RNG(1, 10) * 0.1f;

	//パーティクルの移動用に記録しておく
	Emitter::particleSPos = position;
	Emitter::particleEPos = Float3(position.x + moveVec.x, position.y + moveVec.y, position.z + moveVec.z);

	Emitter::Update();
}
