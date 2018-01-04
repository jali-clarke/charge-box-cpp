#include "Physics.h"

Physics::Physics(const Vector3* pos, const Vector3* vel, const Vector3& bounds, const size_t numObjects, const double timeStep){
    numObjects_ = numObjects;
    timeStep_ = timeStep;

    bounds_ = bounds;

    pos_ = new Vector3[numObjects_];
    oldPos_ = new Vector3[numObjects_];
    accel_ = new Vector3[numObjects_];

    for(size_t i = 0; i < numObjects_; ++ i){
        pos_[i] = pos[i];
        oldPos_[i] = pos_[i] - timeStep_ * vel[i];
        accel_[i] = 0;
    }
}

Physics::~Physics(){
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
        if(pos_[i].x_ < 0){
            pos_[i].x_ = -pos_[i].x_;
        } else if(pos_[i].x_ > bounds_.x_){
            pos_[i].x_ = 
        }
    }
}