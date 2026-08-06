#pragma once

#include "Vector3.h"
#include "Hittable.h"

class ProceduralSphere : public Hittable
{
public:
    ProceduralSphere(const Point3& center, const float radius);
    bool hit(const Ray& ray, const Range& rayRange, Hit& hitData) const override;

private:
    Point3 center;
    float radius;
};
