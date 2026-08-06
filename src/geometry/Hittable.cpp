#include "geometry/Hittable.h"
#include <iostream>

bool HittableList::hit(const Ray& ray, const Range& rayRange, Hit& hitData) const
{
    double closest = rayRange.maxDist;
    Hit currHit = hitData;

    bool successfulHit = false;

    for (const auto& obj : objects) {
        if (obj->hit(ray, rayRange, currHit)) {
            if (currHit.t > rayRange.minDist && currHit.t < closest) {
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
