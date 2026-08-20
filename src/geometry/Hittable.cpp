#include "geometry/Hittable.h"

bool Hittable::hit(const Ray& ray, const Range& rayRange, MeshHit& hitData) const
{
    return false;
}

void Hittable::set_material(const int idx)
{
    materialIdx = idx;
}

int Hittable::get_material_idx() const
{
    return materialIdx;
}
