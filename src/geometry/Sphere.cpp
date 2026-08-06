#include "geometry/Sphere.h"

ProceduralSphere::ProceduralSphere(const Point3& center, const float radius)
    : center(center), radius(radius)
{}

bool ProceduralSphere::hit(const Ray& ray, const Range& rayRange, Hit& hitData) const
{
    Vector3 p = center - ray.origin();

    double a = ray.direction().length_squared();
    double b = -2 * dot(p, ray.direction());
    double c = p.length_squared() - radius * radius;

    double discriminant = (b*b - 4*a*c);

    if (discriminant < 0)
        return false;

    double discSqrt = std::sqrt(discriminant);
    double root = std::min(
        (-1*b - discSqrt) / (2 * a),
        (-1*b + discSqrt) / (2 * a)
    );
    
    if (root < rayRange.minDist || root > rayRange.maxDist)
        return false;

    //
    hitData.t = root;
    hitData.point = ray.origin() + ray.direction() * hitData.t;
    hitData.normal = (hitData.point - center) / radius;

    return  true;
}


