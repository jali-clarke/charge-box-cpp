#include "Physics.h"

Physics::Physics(Particle** particles, const Vector3* pos, const Vector3* vel, const Vector3& bounds, const size_t numObjects, const double timeStep){
    numObjects_ = numObjects;
    timeStep_ = timeStep;

    bounds_ = bounds;

    particles_ = new Particle*[numObjects_];
    pos_ = new Vector3[numObjects_];
    oldPos_ = new Vector3[numObjects_];
    accel_ = new Vector3[numObjects_];

    for(size_t i = 0; i < numObjects_; ++ i){
        particles_[i] = particles[i];
        pos_[i] = pos[i];
        oldPos_[i] = pos_[i] - timeStep_ * vel[i];
        accel_[i] = Vector3(0, 0, 0);
    }
}

Physics::~Physics(){
    delete particles_;
    delete pos_;
    delete oldPos_;
    delete accel_;
}

#define G 10.0
#define K 2000.0

void Physics::accumulateAccel(){
    for(size_t i = 0; i < numObjects_; ++ i){
        accel_[i] = Vector3(0, G, 0);
    }

    for(size_t i = 0; i < numObjects_; ++ i){
        for(size_t j = 0; j < i; ++ j){
            Vector3 force = pos_[i] - pos_[j];
            double forceMag = force.mag();

            force = (K * particles_[i]->charge_ * particles_[j]->charge_ / (forceMag * forceMag * forceMag)) * force;
            accel_[i] = accel_[i] + (1.0 / particles_[i]->mass_) * force;
            accel_[j] = accel_[j] - (1.0 / particles_[j]->mass_) * force;
        }
    }
}

void Physics::advanceTimeStep(){
    // verlet integration
    for(size_t i = 0; i < numObjects_; ++ i){
        oldPos_[i] = 2 * pos_[i] - oldPos_[i] + (timeStep_ * timeStep_) * accel_[i];
    }

    Vector3* temp = pos_;
    pos_ = oldPos_;
    oldPos_ = temp;
}

void Physics::satisfyConstraints(){
    // elements should be in the box, bouncing off the wall
    for(size_t i = 0; i < numObjects_; ++ i){
        if(pos_[i].x_ - particles_[i]->radius_ < 0){
            oldPos_[i].x_ = pos_[i].x_ - oldPos_[i].x_ + particles_[i]->radius_;
            pos_[i].x_ = particles_[i]->radius_;
        } else if(pos_[i].x_ + particles_[i]->radius_ > bounds_.x_){
            oldPos_[i].x_ = pos_[i].x_ - oldPos_[i].x_ + bounds_.x_ - particles_[i]->radius_;
            pos_[i].x_ = bounds_.x_ - particles_[i]->radius_;
        }

        if(pos_[i].y_ - particles_[i]->radius_ < 0){
            oldPos_[i].y_ = pos_[i].y_ - oldPos_[i].y_ + particles_[i]->radius_;
            pos_[i].y_ = particles_[i]->radius_;
        } else if(pos_[i].y_ + particles_[i]->radius_ > bounds_.y_){
            oldPos_[i].y_ = pos_[i].y_ - oldPos_[i].y_ + bounds_.y_ - particles_[i]->radius_;
            pos_[i].y_ = bounds_.y_ - particles_[i]->radius_;
        }

        if(pos_[i].z_ - particles_[i]->radius_ < 0){
            oldPos_[i].z_ = pos_[i].z_ - oldPos_[i].z_ + particles_[i]->radius_;
            pos_[i].z_ = particles_[i]->radius_;
        } else if(pos_[i].z_ + particles_[i]->radius_ > bounds_.z_){
            oldPos_[i].z_ = pos_[i].z_ - oldPos_[i].z_ + bounds_.z_ - particles_[i]->radius_;
            pos_[i].z_ = bounds_.z_ - particles_[i]->radius_;
        }
    }

    for(size_t i = 0; i < numObjects_; ++ i){
        for(size_t j = 0; j < i; ++ j){
            Vector3 delta = pos_[i] - pos_[j];
            double deltaMag = delta.mag();
            double minLength = particles_[i]->radius_ + particles_[j]->radius_ - deltaMag;

            if(minLength > 0){
                double totalMass = particles_[i]->mass_ + particles_[j]->mass_;

                delta = (minLength / deltaMag) * delta;

                Vector3 vel_i = pos_[i] - oldPos_[i];
                Vector3 vel_j = pos_[j] - oldPos_[j];

                pos_[i] = (particles_[j]->mass_ / totalMass) * delta + pos_[i];
                pos_[j] = (-particles_[i]->mass_ / totalMass) * delta + pos_[j];

                Vector3 adjustment = (2 * ((vel_j - vel_i) * delta) / (totalMass * minLength * minLength)) * delta;

                oldPos_[i] = pos_[i] - particles_[j]->mass_ * adjustment - vel_i;
                oldPos_[j] = pos_[j] + particles_[i]->mass_ * adjustment - vel_j;
            }
        }
    }
}

Uint8 diffuse(const double maxVal, const double maxDistSq, const double distSq){
    const double scale = distSq / maxDistSq - 1.0;
    return (Uint8)(maxVal * scale * scale);
}

void Physics::renderInto(Renderer& renderer) const{
    // 2-D system for now

    const double scaleX = (double)(renderer.getWidth() - 1) / bounds_.x_;
    const double scaleY = (double)(renderer.getHeight() - 1) / bounds_.y_;

    for(size_t i = 0; i < renderer.getWidth() * renderer.getHeight(); ++ i){
        renderer.getBuffer()[i] = black;
    }

    for(size_t n = 0; n < numObjects_; ++ n){
        const double maxRad = particles_[n]->radius_ * (1.0 - 0.1 * (double)rand() / RAND_MAX);
        const double maxRadSq = maxRad * maxRad;

        const double yMin = (pos_[n].y_ < maxRad) ? 0 : pos_[n].y_ - maxRad;
        const double yMax = (pos_[n].y_ + maxRad > bounds_.y_) ? bounds_.y_ : pos_[n].y_ + maxRad;
        const double xMin = (pos_[n].x_ < maxRad) ? 0 : pos_[n].x_ - maxRad;
        const double xMax = (pos_[n].x_ + maxRad > bounds_.x_) ? bounds_.x_ : pos_[n].x_ + maxRad;

        for(size_t i = (size_t)(yMin * scaleY); i <= (size_t)(yMax * scaleY); ++ i){
            for(size_t j = (size_t)(xMin * scaleX); j <= (size_t)(xMax * scaleX); ++ j){
                const double jCirc = j / scaleX - pos_[n].x_;
                const double iCirc = i / scaleY - pos_[n].y_;
                const double radSq = jCirc * jCirc + iCirc * iCirc;

                if(radSq < maxRadSq){
                    Uint32 toRender = particles_[n]->colour_;
                    Uint8* hack = (Uint8*)&toRender;

                    for(size_t i = 0; i < 4; ++ i){
                        const double scale = radSq / maxRadSq - 1.0;
                        hack[i] = (Uint8)((double)hack[i] * scale * scale);
                    }

                    renderer.getBuffer()[i * renderer.getWidth() + j] = toRender;
                }
            }
        }
    }
}