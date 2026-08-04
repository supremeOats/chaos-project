#include "geometry/Hittable.h"
#include <iostream>

bool HittableList::hit(const Ray& ray, const double rayMaxDist, Hit& hitData) const
{
    double closest = rayMaxDist;
    Hit currHit = hitData;

    bool successfulHit = false;

    for (const auto& obj : objects) {
        if (obj->hit(ray, rayMaxDist, currHit)) {
            if (currHit.t < closest) {
                closest = currHit.t;
                hitData = currHit;

                successfulHit = true;
            }
        }
    }

    return successfulHit;
}

void HittableList::add(std::shared_ptr<Hittable> obj)
{
    objects.push_back(obj);
}
