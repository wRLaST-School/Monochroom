#include "AttractParticleTmp2.h"
#include "stdafx.h"
#include <ConsoleWindow.h>

TextureKey AttractParticleTmp2::texture = "particle1";


void AttractParticleTmp2::Update()
{
	float t = (float)time / maxTime;

	color = { 0, 0.5f - t / 2.0f, t * 2.0f, 1.0f - t / 2.0f };

	position = Float3{ mPosTmp.x + Util::RNG(-10, 10) * 0.01f
		,mPosTmp.y + Util::RNG(-10, 10) * 0.01f,
		mPosTmp.z + Util::RNG(-10, 10) * 0.01f };

	IParticle::Update();

	ConsoleWindow::LogVec3("ParticlePos::", position);
}

