#include "geometry/Plane.h"

Plane::Plane(const Vector3& normal, const Point3 point)
    : n(normal), p(point)
{}

bool Plane::hit(const Ray& ray, const Range& rayRange, Hit& hitData) const
{
    if (0 <= dot(n, ray.direction()))
        return false;

    double dist = dot(n, p - ray.origin()) / dot(n, ray.direction());
    if(dist < rayRange.minDist || dist > rayRange.maxDist)
        return false;

    hitData.t = dist;
    hitData.point = ray.at(hitData.t);
    hitData.normal = n;

    return true;
}
