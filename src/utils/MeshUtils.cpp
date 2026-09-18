#include "utils/MeshUtils.h"

double triangle_area(const Point3 p0, const Point3 p1, const Point3 p2)
{
    Vector3 v1 = p1 - p0;
    Vector3 v2 = p2 - p0;
    Vector3 v3 = cross(v1, v2);

    return v3.length() / 2;
}

Point3 centroid(const Point3 p0, const Point3 p1, const Point3 p2)
{
    return Point3(
        (p0.x() + p1.x() + p2.x()) / 3,
        (p0.y() + p1.y() + p2.y()) / 3,
        (p0.z() + p1.z() + p2.z()) / 3
    );
}
