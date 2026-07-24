#pragma once

#include "geometry\Vector3.h"
#include "utils\Matrix.h"

#define RTMatrix Matrix<double, 3, 3>

class Moveable
{
public:
    Moveable(const Point3& pos, const Vector3& dir) : _pos(pos), _dir(dir) {}

    const Point3& pos() const { return _pos; }
    const Vector3& dir() const { return _dir; }

    //Translations
    void dolly(const float dist);
    void truck(const float dist);
    void pedestal(const float dist);

    //Rotations
    void tilt(const float rad);
    void pan(const float rad);
    void roll(const float rad);

    const RTMatrix& rotation_matrix() const { return localCoord; }

private:
    RTMatrix localCoord = IDENTITY_MATRIX;
    Point3 _pos;
    Vector3 _dir;

    static double IDENTITY_VALS[9];

    static RTMatrix IDENTITY_MATRIX;
};

Vector3 operator*(const RTMatrix& mat, const Vector3& vec);
