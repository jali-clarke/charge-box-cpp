#include "Physics.h"
#include "Renderer.h"

int main(int argc, char** argv){
    srand(time(NULL));

    Particle electron(1, -1, 2, Pixel(0, 0, 255, 0));
    Particle redElectron(1, -1, 2, Pixel(255, 0, 0, 0));
    Particle positron(1, 1, 2, Pixel(255, 255, 0, 0));

    const size_t numObjects = std::atoi(argv[1]);
    const double timeStep = std::atof(argv[2]);
    const Vector3 bounds(100, 100, 100);

    Particle* particles[numObjects];

    particles[0] = &redElectron;

    for(size_t i = 1; i < numObjects / 2; ++ i){
        particles[i] = &electron;
    }

    for(size_t i = numObjects / 2; i < numObjects; ++ i){
        particles[i] = &positron;
    }

    Vector3 pos[numObjects];
    Vector3 vel[numObjects];

    for(size_t i = 0; i < numObjects; ++ i){
        pos[i] = Vector3(
            (double)rand() / RAND_MAX * (bounds.x_ - 2 * particles[i]->radius_) + particles[i]->radius_,
            (double)rand() / RAND_MAX * (bounds.y_ - 2 * particles[i]->radius_) + particles[i]->radius_,
            0
        );

        vel[i] = Vector3((double)rand() / RAND_MAX * 6 - 3, (double)rand() / RAND_MAX * 6 - 3, 0);
    }

    Physics physics(particles, pos, vel, bounds, numObjects, timeStep);

    Renderer renderer("Charge Box", 600, 600);

    while(renderer.isActive()){
        physics.renderInto(renderer);
        renderer.update();
        physics.accumulateAccel();
        physics.advanceTimeStep();
        physics.satisfyConstraints();
    }

    return 0;
}