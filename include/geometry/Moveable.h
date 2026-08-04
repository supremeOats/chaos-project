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

    //Forward & Backward | Move along Z axis
    void dolly(const float dist);

    //Left & Right | Move along X axis
    void truck(const float dist);

    //Up & Down | Move along Y axis
    void pedestal(const float dist);

    //Up & Down | Rotate along X axis
    void tilt(const float rad);
    
    //Left & Right | Rotate along Y axis
    void pan(const float rad);
    
    //Around | Rotate along Y axis
    void roll(const float rad);

    //Other
    void turn_table(const Point3 center, const float rad);
    void move_to(const Point3 newPos) { _pos = newPos; }
    void set_rotation(const RTMatrix& matrix) { localCoord = matrix; }

    const RTMatrix& rotation_matrix() const { return localCoord; }

private:
    RTMatrix localCoord = IDENTITY_MATRIX;
    Point3 _pos;
    Vector3 _dir;

    static double IDENTITY_VALS[9];

    static RTMatrix IDENTITY_MATRIX;
};

Vector3 operator*(const RTMatrix& mat, const Vector3& vec);
