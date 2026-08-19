#pragma once

#include "geometry/Vector3.h"
#include "geometry/Ray.h"
#include <vector>

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const std::vector<Point3> verts);

    void update(const std::vector<Point3> verts);

    bool intersect(const Ray& ray) const;
    // void split();

private:
    Point3 minBound, maxBound;
};

/*
struct BVHNode
{
    bool is_leaf() { return left < 0 && right < 0;}
    
    void build(const std::vector<Point3>& verts, const std::vector<MeshTriangle>& tris);
    
    BoundingBox box;
    int left = -1, right = -1;
    int triStart = -1, triCount = 0;
};

class BVH
{
    public:
    void build(const std::vector<Point3>& verts, const std::vector<MeshTriangle>& tris);
    
    bool hit(const Mesh& mesh, const Ray& ray, const Range& rayRange, MeshHit& hitData) const;
    
    private:
    const std::vector<BVHNode> nodes;
    const std::vector<unsigned> triIndices;
};
*/
