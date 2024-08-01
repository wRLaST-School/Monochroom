#pragma once
#include "IParticle.h"
#include <Util.h>
#include <Easing.h>


class AttractParticleTmp :
	public IParticle
{
public:
	AttractParticleTmp(const Float3& sPos, const Float3& ePos) : IParticle(sPos, ePos) {
		maxTime = Util::RNG(0, 50);
		scale = 0.067f * (float)Util::RNG(1, 10) * 0.1f;
		mEasing = std::make_unique<Easing>();
		mEasing->SetEaseTimer(maxTime);
		mEasing->SetPowNum(4.0f);

		mSPos = sPos;
		mEPos = ePos;

		IParticle::Init();
	}

	void Update();

private:
	static TextureKey texture;

	std::unique_ptr<Easing> mEasing = nullptr;

	Vec3 mSPos = { 0,0,0 };
	Vec3 mEPos = { 0,0,0 };
};

