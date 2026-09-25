#pragma once

#include "utils/MeshUtils.h"
#include <numeric>

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const std::vector<Point3> verts);

    void update(const std::vector<Point3> verts);
    void expand(const Point3 vert);
    void expand_by_triangle(const std::vector<Point3> verts, const MeshTriangle tri);
    
    bool intersect(const Ray& ray) const;
    void split_half(BoundingBox& left, BoundingBox& right) const;

    int longest_axis() const;
    Point3 centroid() const;

private:
    Point3 minBound, maxBound;
};
