#include "utils/BVH.h"

BoundingBox::BoundingBox() : minBound(Vector3(0.0)), maxBound(Vector3(0.0)) {}

BoundingBox::BoundingBox(const std::vector<Point3> verts)
{
    update(verts);
}

bool BoundingBox::intersect(const Ray& ray) const
{
    if (ray.origin().x() >= minBound.x() && ray.origin().x() <= maxBound.x() &&
        ray.origin().y() >= minBound.y() && ray.origin().y() <= maxBound.y() &&
        ray.origin().z() >= minBound.z() && ray.origin().z() <= maxBound.z())
    {
        return true;
    }

    double tMinX = (minBound.x() - ray.origin().x()) / ray.direction().x();
    double tMaxX = (maxBound.x() - ray.origin().x()) / ray.direction().x();

    if (tMinX > tMaxX) std::swap(tMinX, tMaxX);

    double tMinY = (minBound.y() - ray.origin().y()) / ray.direction().y();
    double tMaxY = (maxBound.y() - ray.origin().y()) / ray.direction().y();
    
    if (tMinY > tMaxY) std::swap(tMinY, tMaxY);

    if (tMinX > tMaxY || tMinY > tMaxX)
        return false;

    double tMinZ = (minBound.z() - ray.origin().z()) / ray.direction().z();
    double tMaxZ = (maxBound.z() - ray.origin().z()) / ray.direction().z();

    if (tMinZ > tMaxZ) std::swap(tMinZ, tMaxZ);

    if (tMinX > tMaxZ || tMinZ > tMaxX)
        return false;

    if (tMinY > tMaxZ || tMinZ > tMaxY)
        return false;

    return true;
}

void BoundingBox::update(const std::vector<Point3> verts)
{
    minBound = maxBound = verts[0];

    for (const Point3& v : verts) {
        minBound.x() = (minBound.x() > v.x()) ? v.x() : minBound.x();
        minBound.y() = (minBound.y() > v.y()) ? v.y() : minBound.y();
        minBound.z() = (minBound.z() > v.z()) ? v.z() : minBound.z();

        maxBound.x() = (maxBound.x() < v.x()) ? v.x() : maxBound.x();
        maxBound.y() = (maxBound.y() < v.y()) ? v.y() : maxBound.y();
        maxBound.z() = (maxBound.z() < v.z()) ? v.z() : maxBound.z();
    }
}

void BoundingBox::split(BoundingBox& left, BoundingBox& right) const
{
    double xLength = maxBound.x() - minBound.x();
    double yLength = maxBound.y() - minBound.y();
    double zLength = maxBound.z() - minBound.z();

    left = *this;
    right = *this;

    if (xLength >= yLength && xLength >= zLength) {
        left.maxBound.x() = minBound.x() + xLength / 2;
        right.minBound.x() = minBound.x() + xLength / 2;
    }
    else if (yLength >= xLength && yLength >= zLength) {
        left.maxBound.y() = minBound.y() + yLength / 2;
        right.minBound.y() = minBound.y() + yLength / 2;
    }
    else {
        left.maxBound.z() = minBound.z() + zLength / 2;
        right.minBound.z() = minBound.z() + zLength / 2;
    }
}
