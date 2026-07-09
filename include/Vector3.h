#pragma once

#include <cmath>

class Vector3
{
public:
    Vector3() {}
    Vector3(const float x, const float y, const float z) : _x(x), _y(y), _z(z) {}
    
    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }

    float length_squared() const { return _x*_x + _y*_y + _z*_z; }
    
    float length() const { return std::sqrt(length_squared()); }

    Vector3& operator+= (const Vector3& vec);
    Vector3& operator-= (const Vector3& vec);
    Vector3& operator*= (const double scalar);
    Vector3& operator/= (const double scalar);

private:
    double _x, _y, _z;
};

//Utility
Vector3& operator+ (const Vector3& lhs, const Vector3& rhs);
Vector3& operator- (const Vector3& lhs, const Vector3& rhs);
Vector3& operator* (const Vector3& vec, const double scalar);
Vector3& operator/ (const Vector3& vec, const double scalar);

bool operator== (const Vector3& lhs, const Vector3& rhs);
bool parallel(const Vector3& lhs, const Vector3& rhs);
bool perpendicular(const Vector3& lhs, const Vector3& rhs);

double dot(const Vector3& lhs, const Vector3& rhs);
Vector3 cross(const Vector3& lhs, const Vector3& rhs);

Vector3 normalized(const Vector3& vec);

using Point3 = Vector3;