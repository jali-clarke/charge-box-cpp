#ifndef VECTOR3_HEADER
#define VECTOR3_HEADER

struct Vector3{
    double x_;
    double y_;
    double z_;

    Vector3(const double x, const double y, const double z);
};

Vector3 operator+(const Vector3& vec0, const vector3& vec1);
Vector3 operator-(const Vector3& vec0, const vector3& vec1);
Vector3 operator*(const double scale, const vector3& vec0);

#endif