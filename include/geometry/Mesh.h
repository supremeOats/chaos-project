#pragma once

#include "geometry/Hittable.h"
#include "utils/BVH.h"
#include <memory>

class Mesh : public Hittable
{
public:
    virtual bool hit(const Ray& ray, const Range& rayRange, MeshHit& hitData) const override;

    const Point3 get_vert(const int idx) const;
    const MeshTriangle& get_tri(const int idx) const;
    const Vector3& get_vert_norm(const int idx) const;

    void add_vert(const Point3& v);
    void add_tri(const MeshTriangle& tri);

    void update_normals();
    void update_vert_normals();
    void update_acc_tree();
    // void update_aabb();

private:
    bool hit_triangle(const Ray& ray, const MeshTriangle& tri, const Range& rayRange, MeshHit& hitData) const;
    bool in_triangle(const Point3& p, const MeshTriangle& tri) const;
    
    Vector3 interpolated_normal(const Point3& p, const MeshTriangle& tri) const;
    void update_triangle_normal(MeshTriangle& tri);
    
private:
    std::vector<Point3> vertices;
    std::vector<MeshTriangle> triangles;
    std::vector<Vector3> vertNorms;

    // BoundingBox aabb;
    AccTree accTree;
};

class MeshList
{
public:
    bool hit(const Ray& ray, const Range& rayRange, MeshHit& hitData) const;

    void add(const std::shared_ptr<Hittable> obj);

private:
    std::vector<std::shared_ptr<Hittable>> objects;
};
