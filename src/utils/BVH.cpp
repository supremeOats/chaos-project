#include "utils/BVH.h"

BoundingBox::BoundingBox() : minBound(Vector3(0.0)), maxBound(Vector3(0.0)) {}

BoundingBox::BoundingBox(const std::vector<Point3> verts)
{
    update(verts);
}

bool BoundingBox::intersect(const Ray& ray) const
{
    if (ray.origin().x() >= minBound.x() && ray.origin().x() <= maxBound.x() &&
        ray.origin().y() >= minBound.y() && ray.origin().y() <= maxBound.y() &&
        ray.origin().z() >= minBound.z() && ray.origin().z() <= maxBound.z())
    {
        return true;
    }

    double tMinX = (minBound.x() - ray.origin().x()) / ray.direction().x();
    double tMaxX = (maxBound.x() - ray.origin().x()) / ray.direction().x();

    if (tMinX > tMaxX) std::swap(tMinX, tMaxX);

    double tMinY = (minBound.y() - ray.origin().y()) / ray.direction().y();
    double tMaxY = (maxBound.y() - ray.origin().y()) / ray.direction().y();
    
    if (tMinY > tMaxY) std::swap(tMinY, tMaxY);

    if (tMinX > tMaxY || tMinY > tMaxX)
        return false;

    double tMinZ = (minBound.z() - ray.origin().z()) / ray.direction().z();
    double tMaxZ = (maxBound.z() - ray.origin().z()) / ray.direction().z();

    if (tMinZ > tMaxZ) std::swap(tMinZ, tMaxZ);

    if (tMinX > tMaxZ || tMinZ > tMaxX)
        return false;

    if (tMinY > tMaxZ || tMinZ > tMaxY)
        return false;

    return true;
}

void BoundingBox::update(const std::vector<Point3> verts)
{
    minBound = maxBound = verts[0];

    for (const Point3& v : verts) {
        minBound.x() = (minBound.x() > v.x()) ? v.x() : minBound.x();
        minBound.y() = (minBound.y() > v.y()) ? v.y() : minBound.y();
        minBound.z() = (minBound.z() > v.z()) ? v.z() : minBound.z();

        maxBound.x() = (maxBound.x() < v.x()) ? v.x() : maxBound.x();
        maxBound.y() = (maxBound.y() < v.y()) ? v.y() : maxBound.y();
        maxBound.z() = (maxBound.z() < v.z()) ? v.z() : maxBound.z();
    }
}

/*
void BVH::build(const std::vector<Point3>& verts, const std::vector<MeshTriangle>& tris)
{
    // nodes[0] = BVHNode{BoundingBox(verts)};
    
    
}

bool BVH::hit(const Mesh& mesh, const Ray& ray, const Range& rayRange, MeshHit& hitData) const
{
    int stack[64];
    int sp = 0;
    stack[sp++] = 0; // root
    bool hitAnything = false;
    Range range = rayRange;
    
    while (sp > 0)
    {
        const BVHNode& node = nodes[stack[--sp]];
        if (!node.box.intersect(ray)) continue;
        
        if (node.triCount > 0)
        {
            for (int i = 0; i < node.triCount; ++i)
            {
                unsigned triIdx = triIndices[node.triStart + i];
                if (mesh.hit_triangle(ray, mesh.get_tri(triIdx), range, hitData))
                {
                    hitAnything = true;
                    range.maxVal = hitData.t;
                }
            }
        }
        else
        {
            stack[sp++] = node.left;
            stack[sp++] = node.right;
        }
    }
    
    return hitAnything;
}
*/

