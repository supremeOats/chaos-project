#pragma once

#include "Vector3.h"
#include "Hittable.h"

class Plane : public Hittable
{
public:
    Plane(const Vector3& normal, const Point3 point);

    bool hit(const Ray& ray, const Range& rayRange, Hit& hitData) const override;

private:
    Vector3 n;
    Point3 p;
};
