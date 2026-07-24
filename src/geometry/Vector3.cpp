#include "geometry\Vector3.h"

Vector3& Vector3::operator+= (const Vector3& vec)
{
    this->_x += vec._x;
    this->_y += vec._y;
    this->_z += vec._z;

    return *this;
}

Vector3& Vector3::operator-= (const Vector3& vec)
{
    this->_x -= vec._x;
    this->_y -= vec._y;
    this->_z -= vec._z;

    return *this;
}

Vector3& Vector3::operator*= (const double scalar)
{
    this->_x *= scalar;
    this->_y *= scalar;
    this->_z *= scalar;

    return *this;
}

Vector3& Vector3::operator/= (const double scalar)
{
    this->_x /= scalar;
    this->_y /= scalar;
    this->_z /= scalar;

    return *this;
}

Vector3 operator+ (const Vector3& lhs, const Vector3& rhs)
{
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y(),
        lhs.z() + rhs.z()
    };
}

Vector3 operator- (const Vector3& lhs, const Vector3& rhs)
{
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y(),
        lhs.z() - rhs.z()
    };
}

Vector3 operator* (const Vector3& vec, const double scalar)
{
    return {
        vec.x() * scalar,
        vec.y() * scalar,
        vec.z() * scalar
    };
}

Vector3 operator/ (const Vector3& vec, const double scalar)
{
    return {
        vec.x() / scalar,
        vec.y() / scalar,
        vec.z() / scalar
    };
}

bool operator== (const Vector3& lhs, const Vector3& rhs)
{
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z();
}

bool parallel(const Vector3& lhs, const Vector3& rhs)
{
    return cross(lhs, rhs) == Vector3{0, 0, 0};
}

bool perpendicular(const Vector3& lhs, const Vector3& rhs)
{
    return !dot(lhs, rhs);
}

double dot(const Vector3& lhs, const Vector3& rhs)
{
    return
        lhs.x() * rhs.x() +
        lhs.y() * rhs.y() +
        lhs.z() * rhs.z();
}

Vector3 cross(const Vector3& lhs, const Vector3& rhs)
{
    return {
        lhs.y()*rhs.z() - lhs.z()*rhs.y(),
        lhs.z()*rhs.x() - lhs.x()*rhs.z(),
        lhs.x()*rhs.y() - lhs.y()*rhs.x()
    };
}

Vector3 normalized(const Vector3& vec)
{
    return vec / vec.length();
}
