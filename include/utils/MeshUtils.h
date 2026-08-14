#pragma once

#include "utils/Material.h"
#include "geometry/Ray.h"

struct MeshHit
{
    double t;
    Vector3 normal;
    Vector3 pointNormal;
    Point3 point;
    int materialIdx;
};

struct MeshTriangle
{
    MeshTriangle(const int v0, const int v1, const int v2);

    int v[3];
    Vector3 normal;

    bool is_vertex(const int idx) const;
};

float triangle_area(const Point3 p0, const Point3 p1, const Point3 p2);
