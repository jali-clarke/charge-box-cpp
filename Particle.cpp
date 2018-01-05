#include "Particle.h"

Particle::Particle(const double mass, const double charge, const double radius, const Pixel& colour):
    mass_(mass), charge_(charge), radius_(radius), colour_(toPixelRGBA(colour)){}