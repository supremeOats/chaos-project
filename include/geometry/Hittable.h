#pragma once

#include "utils/MeshUtils.h"

class Hittable
{
public:
    virtual ~Hittable() = default;
    
    virtual bool hit(const Ray& ray, const Range& rayRange, MeshHit& hitData) const = 0;
};
