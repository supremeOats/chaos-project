#pragma once

#include "Vector3.h"

class Triangle
{    
public:
    Triangle() {}
    Triangle(const Point3& p1, const Point3& p2, const Point3& p3);
    
    float area() const;
    Vector3 normal() const;
    
    bool in_triangle(const Point3& p) const;
    bool in_plane(const Point3& p) const;

    const Point3& operator[] (const int i) const;

private:
    Point3 p[3];
};
