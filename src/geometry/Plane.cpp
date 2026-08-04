#include "geometry/Plane.h"

Plane::Plane(const Vector3& normal, const Point3 point)
    : n(normal), p(point)
{}

bool Plane::hit(const Ray& ray, const double rayMaxDist, Hit& hitData) const
{
    if (0 <= dot(n, ray.direction()))
        return false;

    hitData.t = dot(n, p - ray.origin()) / dot(n, ray.direction());
    hitData.point = ray.at(hitData.t);
    hitData.normal = n;

    return true;
}
