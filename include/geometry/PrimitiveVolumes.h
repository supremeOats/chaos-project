#pragma once

#include "Vector3.h"
#include "Hittable.h"
#include "Triangle.h"

class ProceduralSphere : public Hittable
{
public:
    ProceduralSphere(const Point3& center, const float radius) : center(center), radius(radius) {}
    bool hit(const Ray& ray, Hit& hitData) const override;

private:
    Point3 center;
    float radius;
};

bool ProceduralSphere::hit(const Ray& ray, Hit& hitData) const
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
    
    //
    hitData.t = root;
    hitData.point = ray.origin() + ray.direction() * hitData.t;
    hitData.normal = (hitData.point - center) / radius;

    return  true;
}

class Plane : public Hittable
{
public:
    Plane(const Vector3& normal, const Point3 point) : n(normal), p(point) {}

    bool hit(const Ray& ray, Hit& hitData) const override
    {
        hitData = {0, {0, 0, 1}, {0, 0, 0}};
        return dot(n, ray.direction()) >= 0;
    }

private:
    Vector3 n;
    Point3 p;
};

// class SinglePolygon : public Hittable, private Triangle
// {
// public:

//     bool hit(const Ray& ray) const override;

// };
