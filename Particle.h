#ifndef PARTICLE_HEADER
#define PARTICLE_HEADER

#include "Renderer.h"

struct Particle{
    const double mass_;
    const double charge_;
    const double radius_;
    const Uint32 colour_;

    Particle(const double mass, const double charge, const double radius, const Pixel& colour);
};


#endif