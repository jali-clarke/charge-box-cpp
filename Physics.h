#ifndef PHYSICS_HEADER
#define PHYSICS_HEADER

#include "Vector3.h"

class Physics{
private:
    Vector3* pos_;
    Vector3* oldPos_;
    Vector3* accel_;

    Vector3 bounds_;

    size_t numObjects_;
    double timeStep_;

public:
    Physics(const Vector3* pos, const Vector3* vel, const Vector3& bounds, const size_t numObjects, const double timeStep);
    ~Physics();

    Physics() = delete;
    Physics(const Physics& other) = delete;
    Physics& operator=(const Physics& other) = delete;

    void accumulateAccel();
    void advanceTimeStep();
    void satisfyConstraints();
};

#endif