#include "AttractParticleTmp.h"
#include "stdafx.h"

TextureKey AttractParticleTmp::texture = "particle1";


void AttractParticleTmp::Update()
{
	float t = (float)time / maxTime;

	color = { 1.0f - t, 1.f - t, 0, 1.0f - t };
}
