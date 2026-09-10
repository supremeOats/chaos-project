#pragma once

#include "geometry/Vector3.h"
#include "geometry/Ray.h"
#include <vector>

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const std::vector<Point3> verts);

    void update(const std::vector<Point3> verts);

    bool intersect(const Ray& ray) const;
    
    void split(BoundingBox& left, BoundingBox& right) const;

private:
    Point3 minBound, maxBound;
};
