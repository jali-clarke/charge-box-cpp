#include "Physics.h"
#include "Renderer.h"

int main(int argc, char** argv){
    Particle electron(1, -1, 1, Pixel(0, 0, 255, 0));
    Particle positron(1, -1, 1, Pixel(255, 255, 0, 0));

    const size_t numObjects = 10;
    const Vector3 bounds(100, 100, 100);

    Particle* particles[numObjects] = {&electron, &electron, &electron, &electron, &electron, &positron, &positron, &positron, &positron, &positron};
    Vector3 pos[numObjects] = {
        Vector3(10, 25, 0),
        Vector3(30, 25, 0),
        Vector3(50, 25, 0),
        Vector3(70, 25, 0),
        Vector3(90, 25, 0),
        Vector3(15, 75, 0),
        Vector3(35, 75, 0),
        Vector3(55, 75, 0),
        Vector3(75, 75, 0),
        Vector3(95, 75, 0)
    };

    Vector3 vel[numObjects]{
        Vector3(1, 0, 0),
        Vector3(0, 1, 0),
        Vector3(-1, 0, 0),
        Vector3(0, -1, 0),
        Vector3(1, 1, 0),
        Vector3(1, -1, 0),
        Vector3(-1, 1, 0),
        Vector3(-1, -1, 0),
        Vector3(1, 2, 0),
        Vector3(2, 1, 0)
    };

    Physics physics(particles, pos, vel, bounds, numObjects, 0.1);

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