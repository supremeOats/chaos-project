#include "Triangle.h"

Triangle::Triangle(const Point3& p1, const Point3& p2, const Point3& p3)
{
    /*
    Vector3 v1 = p[1] - p[0];
    Vector3 v2 = p[2] - p[0];
    
    if (parallel(v1, v2))
        throw std::invalid_argument("Points of a triangle cannot be collinear");
    */

    p[0] = p1;
    p[1] = p2;
    p[2] = p3;
}

float Triangle::area() const
{
    Vector3 v1 = p[1] - p[0];
    Vector3 v2 = p[2] - p[0];
    Vector3 v3 = cross(v1, v2);

    return v3.length() / 2;
}

Vector3 Triangle::normal() const
{
    Vector3 v1 = p[1] - p[0];
    Vector3 v2 = p[2] - p[0];
    Vector3 v3 = cross(v1, v2);

    return normalized(v3);
}

bool Triangle::in_triangle(const Point3& p) const
{
    Vector3 norm = this->normal();
    
    Vector3 edge0 = this->p[1] - this->p[0];
    Vector3 edge1 = this->p[2] - this->p[1];
    Vector3 edge2 = this->p[0] - this->p[2];
    
    Vector3 c0 = p - this->p[0];
    Vector3 c1 = p - this->p[1];
    Vector3 c2 = p - this->p[2];

    return dot(norm, cross(edge0, c0)) >= 0 &&
           dot(norm, cross(edge1, c1)) >= 0 &&
           dot(norm, cross(edge2, c2)) >= 0;
}

bool Triangle::in_plane(const Point3& p) const
{
    Vector3 normVec = this->normal();

    return !(
        p.x() * normVec.x() +
        p.y() * normVec.y() +
        p.z() * normVec.z()
    );
}

const Point3& Triangle::operator[] (const int i) const
{
    return p[i];
}

