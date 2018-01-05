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

void Physics::accumulateAccel(){
    // no force yet
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
            pos_[i].x_ += 2 * (particles_[i]->radius_ - pos_[i].x_);
        } else if(pos_[i].x_ + particles_[i]->radius_ > bounds_.x_){
            pos_[i].x_ -= 2 * (pos_[i].x_ + particles_[i]->radius_ - bounds_.x_);
        }

        if(pos_[i].y_ - particles_[i]->radius_ < 0){
            pos_[i].y_ += 2 * (particles_[i]->radius_ - pos_[i].y_);
        } else if(pos_[i].y_ + particles_[i]->radius_ > bounds_.y_){
            pos_[i].y_ -= 2 * (pos_[i].y_ + particles_[i]->radius_ - bounds_.y_);
        }

        if(pos_[i].z_ - particles_[i]->radius_ < 0){
            pos_[i].z_ += 2 * (particles_[i]->radius_ - pos_[i].z_);
        } else if(pos_[i].z_ + particles_[i]->radius_ > bounds_.z_){
            pos_[i].z_ -= 2 * (pos_[i].z_ + particles_[i]->radius_ - bounds_.z_);
        }
    }

    for(size_t i = 0; i < numObjects_; ++ i){
        for(size_t j = 0; j < i; ++ j){
            Vector3 delta = pos_[i] - pos_[j];
            double deltaMag = delta.mag();
            double minLength = particles_[i]->radius_ + particles_[j]->radius_;

            if(delta.mag() < minLength){
                delta = (minLength / delta.mag()) * delta;
                //double totalMass = particles_[i]->mass_ + particles_[j]->mass_;
                //pos_[i] = pos_[i] + (particles_[i]->mass_ / totalMass) * delta;
                //pos_[j] = pos_[j] - (particles_[j]->mass_ / totalMass) * delta;
            }
        }
    }
}

void Physics::renderInto(Renderer& renderer) const{
    // 2-D system for now

    const double scaleX = (double)(renderer.getWidth() - 1) / bounds_.x_;
    const double scaleY = (double)(renderer.getHeight() - 1) / bounds_.y_;

    for(size_t i = 0; i < renderer.getWidth() * renderer.getHeight(); ++ i){
        renderer.getBuffer()[i] = black;
    }

    for(size_t n = 0; n < numObjects_; ++ n){
        const double yMin = pos_[n].y_ - particles_[n]->radius_;
        const double yMax = pos_[n].y_ + particles_[n]->radius_;

        for(size_t i = (size_t)(yMin * scaleY); i <= (size_t)(yMax * scaleY); ++ i){
            const double xMin = pos_[n].x_ - particles_[n]->radius_;
            const double xMax = pos_[n].x_ + particles_[n]->radius_;

            for(size_t j = (size_t)(xMin * scaleX); j <= (size_t)(xMax * scaleX); ++ j){
                const double jCirc = j / scaleX - pos_[n].x_;
                const double iCirc = i / scaleY - pos_[n].y_;
                if(jCirc * jCirc + iCirc * iCirc <= particles_[n]->radius_ * particles_[n]->radius_){
                    renderer.getBuffer()[i * renderer.getWidth() + j] = particles_[n]->colour_;
                }
            }
        }
    }
}