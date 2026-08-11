#pragma once

#include <vector>
#include "geometry/Ray.h"
#include <iostream>

struct MeshHit
{
    double t;
    Vector3 normal;
    Vector3 pointNormal;
    Point3 point;
};

struct Range
{
    double minDist;
    double maxDist;
};

struct MeshTriangle
{
    MeshTriangle(const int v0, const int v1, const int v2);

    int v[3];
    Vector3 normal;

    bool is_vertex(const int idx) const;
};

float triangle_area(const Point3 p0, const Point3 p1, const Point3 p2);

class Mesh
{
public:
    bool hit(const Ray& ray, const Range& rayRange, MeshHit& hitData) const;

    const Point3 get_vert(const int idx) const;
    const MeshTriangle& get_tri(const int idx) const;
    const Vector3& get_vert_norm(const int idx) const;

    void add_vert(const Point3& v);
    void add_tri(const MeshTriangle& tri);

    void update_normals();
    void update_vert_normals();

    private:
    bool hit_triangle(const Ray& ray, const MeshTriangle& tri, const Range& rayRange, MeshHit& hitData) const;
    bool in_triangle(const Point3& p, const MeshTriangle& tri) const;
    
    Vector3 interpolated_normal(const Point3& p, const MeshTriangle& tri) const;
    void update_triangle_normal(MeshTriangle& tri);
    

private:
    std::vector<Point3> vertices;
    std::vector<MeshTriangle> triangles;
    std::vector<Vector3> vertNorms;

    //Material
};

class MeshList
{
public:
    bool hit(const Ray& ray, const Range& rayRange, MeshHit& hitData) const;

    void add(const Mesh& obj);

private:
    std::vector<Mesh> objects;
};
