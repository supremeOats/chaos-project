#pragma once

#include "utils/MeshUtils.h"

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& ray, const Range& rayRange, MeshHit& hitData) const = 0;

    void set_material(const int idx);
    int get_material_idx() const;

protected:
    int materialIdx;
};
