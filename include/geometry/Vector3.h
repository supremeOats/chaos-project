#pragma once

#include <cmath>
#include <algorithm>

class Vector3
{
public:
    Vector3() {}
    Vector3(const double x) : Vector3(x, x, x) {}
    Vector3(const double x, const double y, const double z) : _x(x), _y(y), _z(z) {}
    
    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }

    double& x() { return _x; }
    double& y() { return _y; }
    double& z() { return _z; }

    double length_squared() const { return _x*_x + _y*_y + _z*_z; }
    
    double length() const { return std::sqrt(length_squared()); }

    Vector3& operator+= (const Vector3& vec);
    Vector3& operator-= (const Vector3& vec);
    Vector3& operator*= (const double scalar);
    Vector3& operator/= (const double scalar);

private:
    double _x, _y, _z;
};

//Utility
struct Range
{
    double minVal;
    double maxVal;
};

Vector3 operator+ (const Vector3& lhs, const Vector3& rhs);
Vector3 operator- (const Vector3& lhs, const Vector3& rhs);
Vector3 operator* (const Vector3& vec, const double scalar);
Vector3 operator* (const double scalar, const Vector3& vec);
Vector3 operator/ (const Vector3& vec, const double scalar);

bool operator== (const Vector3& lhs, const Vector3& rhs);
bool parallel(const Vector3& lhs, const Vector3& rhs);
bool perpendicular(const Vector3& lhs, const Vector3& rhs);

double dot(const Vector3& lhs, const Vector3& rhs);
Vector3 cross(const Vector3& lhs, const Vector3& rhs);
Vector3 component_wise(const Vector3& lhs, const Vector3& rhs);

Vector3 normalized(const Vector3& vec);

Vector3 clamp(const Vector3& vec, const Range& range);

using Point3 = Vector3;
