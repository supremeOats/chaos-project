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
