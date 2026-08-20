#include "utils/MeshUtils.h"

double triangle_area(const Point3 p0, const Point3 p1, const Point3 p2)
{
    Vector3 v1 = p1 - p0;
    Vector3 v2 = p2 - p0;
    Vector3 v3 = cross(v1, v2);

    return v3.length() / 2;
}
