#include "acceleration/BoundingBox.h"

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

void BoundingBox::expand(const Point3 vert)
{
    minBound.x() = std::min(minBound.x(), vert.x());
    minBound.y() = std::min(minBound.y(), vert.y());
    minBound.z() = std::min(minBound.z(), vert.z());

    maxBound.x() = std::max(maxBound.x(), vert.x());
    maxBound.y() = std::max(maxBound.y(), vert.y());
    maxBound.z() = std::max(maxBound.z(), vert.z());
}

void BoundingBox::expand_by_triangle(const std::vector<Point3> verts, const MeshTriangle tri)
{
    expand(verts[tri.v[0]]);
    expand(verts[tri.v[1]]);
    expand(verts[tri.v[2]]);
}

void BoundingBox::split_half(BoundingBox& left, BoundingBox& right) const
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

int BoundingBox::longest_axis() const
{
    double xLen = maxBound.x() - minBound.x();
    double yLen = maxBound.y() - minBound.y();
    double zLen = maxBound.z() - minBound.z();

    if (xLen >= yLen && xLen >= zLen) return 0;
    
    if (yLen >= xLen && yLen >= zLen) return 1;
    
    if (zLen >= xLen && zLen >= yLen) return 2;

    return -1;
}

Point3 BoundingBox::centroid() const
{
    return Point3(
        (maxBound.x() + minBound.x()) / 2,
        (maxBound.y() + minBound.y()) / 2,
        (maxBound.z() + minBound.z()) / 2
    );
}
