#pragma once
#include "IParticle.h"
#include <Util.h>
#include <Easing.h>


class AttractParticleTmp2 :
	public IParticle
{
public:
	AttractParticleTmp2(const Float3& sPos, const Float3& ePos) : IParticle(sPos, ePos) {
		maxTime = Util::RNG(30, 80);
		scale = 0.2f * (float)Util::RNG(1, 10) * 0.1f;

		mPosTmp = sPos;
		ePos;

		IParticle::Init();
	}

	void Update();

private:
	static TextureKey texture;

	Vec3 mPosTmp;
};

