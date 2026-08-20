#pragma once

#include "geometry/Hittable.h"

class ProceduralSphere : public Hittable
{
public:
    ProceduralSphere(const Point3& center, const float radius) : center(center), radius(radius) {}
    virtual bool hit(const Ray& ray, const Range& rayRange, MeshHit& hitData) const override;

private:
    Point3 center;
    float radius;
};
