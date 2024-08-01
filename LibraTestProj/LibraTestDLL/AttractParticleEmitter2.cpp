#include "AttractParticleEmitter2.h"
#include <ConsoleWindow.h>

void AttractParticleEmitter2::Update(const Vec3& pos)
{
	position = pos + Vec3(
		(float)Util::RNG(-4, 4) * 0.1f,
		(float)Util::RNG(-4, 4) * 0.1f,
		(float)Util::RNG(-4, 4) * 0.1f);
}

void AttractParticleEmitter2::ParticlesUpdate()
{
	particleSPos = position;

	Emitter::Update();
}
