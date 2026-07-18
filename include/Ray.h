#pragma once

#include "Vector3.h"

class Ray
{
public:
    Ray(const Point3& origin, const Vector3& direction) : orig(origin), dir(direction) {}

    const Point3& origin() const { return orig; }
    const Vector3& direction() const { return dir; }

    Point3 at(const double t) const { return orig + dir*t; }

private:
    Vector3 dir;
    Point3 orig;
};
