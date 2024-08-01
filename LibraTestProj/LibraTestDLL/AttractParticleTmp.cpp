#include "AttractParticleTmp.h"
#include "stdafx.h"
#include <ConsoleWindow.h>

TextureKey AttractParticleTmp::texture = "particle1";


void AttractParticleTmp::Update()
{
	float t = (float)time / maxTime;

	color = { 0, 0.5f - t / 2.0f, t * 2.0f, 1.0f - t };

	mEasing->Update();
	position = mEasing->Out(mSPos, mEPos);
	position += Vec3{ (float)Util::RNG(-10, 10), (float)Util::RNG(-10, 10), (float)Util::RNG(-10, 10) } *0.1f * scale;

	IParticle::Update();

	ConsoleWindow::Log("ParticleUPDATE!!!!!!!!!!!!!!!!!!");
}

