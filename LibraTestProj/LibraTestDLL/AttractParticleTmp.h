#pragma once
#include "IParticle.h"
#include <Util.h>


class AttractParticleTmp :
    public IParticle
{
public:
    AttractParticleTmp(Float3 pos) : IParticle(pos) { 
        maxTime=40;
    }

    void Update();

private:
    static TextureKey texture;
};

