#pragma once

#include "Vector3.h"
#include "Hittable.h"
#include "Triangle.h"

class Triangle : public Hittable
{
public:
    Triangle() {}
    Triangle(const Point3& p1, const Point3& p2, const Point3& p3);
    
    const Vector3& normal() const { return norm; }
    float area() const;

    bool hit(const Ray& ray, const double rayMaxDist, Hit& hitData) const override;

    const Point3& Triangle::operator[] (const int i) const;

private:
    void update_norm();

    bool in_triangle(const Point3& p) const;
    bool in_plane(const Point3& p) const;

private:
    Point3 p[3];
    Vector3 norm;
};
