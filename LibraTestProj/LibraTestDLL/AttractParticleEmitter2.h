#pragma once
#include<Emitter.h>
#include<AttractParticleTmp2.h>
#include<Easing.h>

//引き寄せるときのパーティクルのエミッター
class AttractParticleEmitter2 : public Emitter<AttractParticleTmp2>
{
private:
	const float kParticleLength = 2.0f;


public:
	AttractParticleEmitter2() { ; }

	void Initialize(float scale)
	{
		shape = Shape::Cube;
		quantity = 6;
		timeBetweenEmit = 1;
		radius.x = radius.y = radius.z = scale * kParticleLength;
	}

public:
	void Update(const Vec3& pos);

	void ParticlesUpdate();
};