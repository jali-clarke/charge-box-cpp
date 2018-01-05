#include <cmath>
#include "Vector3.h"

Vector3::Vector3(): x_(0), y_(0), z_(0){};
Vector3::Vector3(const double x, const double y, const double z): x_(x), y_(y), z_(z){};

double Vector3::mag() const{
    return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

Vector3 operator+(const Vector3& vec0, const Vector3& vec1){
    return Vector3(vec0.x_ + vec1.x_, vec0.y_ + vec1.y_, vec0.z_ + vec1.z_);
}

Vector3 operator-(const Vector3& vec0, const Vector3& vec1){
    return Vector3(vec0.x_ - vec1.x_, vec0.y_ - vec1.y_, vec0.z_ - vec1.z_);
}

Vector3 operator*(const double scale, const Vector3& vec0){
    return Vector3(scale * vec0.x_, scale * vec0.y_, scale * vec0.z_);
}