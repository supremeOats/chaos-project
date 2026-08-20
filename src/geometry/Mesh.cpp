#include "geometry/Mesh.h"

// bool Mesh::hit(const Ray& ray, const Range& rayRange, MeshHit& hitData) const
// {
//     return bvh.hit(*this, ray, rayRange, hitData);
// }

bool Mesh::hit(const Ray& ray, const Range& rayRange, MeshHit& hitData) const
{
    if (!aabb.intersect(ray)) {
        return false;
    }

    double closest = rayRange.maxVal;
    MeshHit currHit = {hitData.t, hitData.normal, hitData.point};
    
    bool successfulHit = false;
    
    for (const auto& tri : triangles) {        
        if (hit_triangle(ray, tri, rayRange, currHit)) {
            if (currHit.t > rayRange.minVal && currHit.t < closest) {                
                closest = currHit.t;
                
                hitData = currHit;
                hitData.materialIdx = materialIdx;
                
                successfulHit = true;
            }
        }
    }
    
    return successfulHit;
}

void Mesh::add_vert(const Point3& v)
{
    vertices.push_back(v);
    vertNorms.push_back(Vector3(0.0));
}

void Mesh::add_tri(const MeshTriangle& tri)
{
    triangles.push_back(tri);
    update_triangle_normal(triangles[triangles.size()-1]);
}

void Mesh::update_normals()
{
    for (MeshTriangle& tri : triangles) {
        update_triangle_normal(tri);
    }
}

void Mesh::update_vert_normals()
{
    for (const MeshTriangle& tri : triangles) {
        vertNorms[tri.v[0]] += tri.normal;
        vertNorms[tri.v[1]] += tri.normal;
        vertNorms[tri.v[2]] += tri.normal;
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        vertNorms[i] = normalized(vertNorms[i]);
    }
}

void Mesh::update_aabb()
{
    aabb.update(vertices);
}

bool MeshList::hit(const Ray& ray, const Range& rayRange, MeshHit& hitData) const
{   
    double closest = rayRange.maxVal;
    MeshHit currHit = hitData;

    bool successfulHit = false;

    for (size_t i = 0; i < objects.size(); ++i) {
        
        if (objects[i]->hit(ray, rayRange, currHit)) {
            if (currHit.t > rayRange.minVal && currHit.t < closest) {                
                closest = currHit.t;
                
                hitData = currHit;
                
                successfulHit = true;
            }
        }
    }

    return successfulHit;
}

void MeshList::add(const std::shared_ptr<Hittable> obj)
{
    objects.push_back(obj);
}

const Point3 Mesh::get_vert(const int idx) const
{
    return vertices[idx];
}

const MeshTriangle& Mesh::get_tri(const int idx) const
{
    return triangles[idx];
}

const Vector3& Mesh::get_vert_norm(const int idx) const
{
    return vertNorms[idx];
}

//MeshTriangle
MeshTriangle::MeshTriangle(const int v0, const int v1, const int v2)
    : v {v0, v1, v2}, normal()
{}

void Mesh::update_triangle_normal(MeshTriangle& tri)
{
    Point3 v0 = vertices[tri.v[0]];
    Point3 v1 = vertices[tri.v[1]];
    Point3 v2 = vertices[tri.v[2]];

    Vector3 e0 = v1 - v0;
    Vector3 e1 = v2 - v0;
    Vector3 n = cross(e0, e1);

    tri.normal = normalized(n);
}

bool Mesh::hit_triangle(const Ray& ray, const MeshTriangle& tri, const Range& rayRange, MeshHit& hitData) const
{
    double rayNormDot = dot(tri.normal, ray.direction());

    // if (0 <= rayNormDot)
    //     return false;

    double dist = dot(tri.normal, vertices[tri.v[0]] - ray.origin()) / rayNormDot;
    
    if (dist >= rayRange.minVal &&
        dist <= rayRange.maxVal &&
        !perpendicular(ray.direction(), tri.normal))
    {
        Point3 intersectionPoint = ray.at(dist);
    
        if (in_triangle(intersectionPoint, tri)) {
            hitData.t = dist;
            hitData.point = intersectionPoint;
            hitData.normal = tri.normal;
            hitData.pointNormal = interpolated_normal(intersectionPoint, tri);
            
            return true;
        }
    }
    
    return false;
}

bool Mesh::in_triangle(const Point3& p, const MeshTriangle& tri) const
{
    Point3 v0 = vertices[tri.v[0]];
    Point3 v1 = vertices[tri.v[1]];
    Point3 v2 = vertices[tri.v[2]];
    
    Vector3 edge0 = v1 - v0;
    Vector3 edge1 = v2 - v1;
    Vector3 edge2 = v0 - v2;
    
    Vector3 c0 = p - v0;
    Vector3 c1 = p - v1;
    Vector3 c2 = p - v2;

    return dot(tri.normal, cross(edge0, c0)) >= 0 &&
           dot(tri.normal, cross(edge1, c1)) >= 0 &&
           dot(tri.normal, cross(edge2, c2)) >= 0;
}

bool MeshTriangle::is_vertex(const int idx) const
{
    return idx == v[0] || idx == v[1] || idx == v[2];
}

Vector3 Mesh::interpolated_normal(const Point3& p, const MeshTriangle& tri) const
{   
    Point3 v0 = vertices[tri.v[0]];
    Point3 v1 = vertices[tri.v[1]];
    Point3 v2 = vertices[tri.v[2]];

    double totalArea = triangle_area(v0, v1, v2);
    double areaA = triangle_area(v0, v2, p);
    double areaB = triangle_area(v0, v1, p);
    
    double u = areaA / totalArea;
    double v = areaB / totalArea;
 
    Vector3 interpolatedVec =
            vertNorms[tri.v[1]] * u +
            vertNorms[tri.v[2]] * v +
            vertNorms[tri.v[0]] * (1 - u - v);

    return normalized(interpolatedVec);
}
