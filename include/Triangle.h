#pragma once

#include "vector3.h"

class Triangle
{    
public:
    Triangle() {}
    Triangle(const Point3& p1, const Point3& p2, const Point3& p3);
    
    float area() const;
    Vector3 normal() const;
    
    // const Point3 get_vertex (const int i) const;

    bool in_triangle(const Point3& p) const;
    bool in_plane(const Point3& p) const;

private:
    Point3 p[3];

};
