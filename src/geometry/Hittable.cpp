#include "geometry\Hittable.h"
#include <iostream>

bool HittableList::hit(const Ray& ray, Hit& hitData) const
{
    for (const auto& obj : objects) {
        if (obj->hit(ray, hitData)) {
            return true;
        }
    }

    return false;
}

void HittableList::add(std::shared_ptr<Hittable> obj)
{
    objects.push_back(obj);
}
