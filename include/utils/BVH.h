#pragma once

#include "geometry/Vector3.h"
#include "utils/MeshUtils.h"
#include <vector>
#include <numeric>

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const std::vector<Point3> verts);

    void update(const std::vector<Point3> verts);
    void expand(const Point3 vert);
    

    bool intersect(const Ray& ray) const;
    void split_half(BoundingBox& left, BoundingBox& right) const;

    int longest_axis() const;
    Point3 centroid() const;

private:
    Point3 minBound, maxBound;
};

struct Node {
    BoundingBox aabb;

    int parentIdx;
    int children[2];

    std::vector<int> triangles;

    bool is_leaf() const;

    // int triStartIdx = -1;
    // unsigned triCount = 0;
};

class AccTree
{
public:
    AccTree(const std::vector<Point3> verts, const std::vector<MeshTriangle> tris);
    std::vector<int> intersect(const Ray& ray); //returns index of the intersected trianlge, if there is no intersection returns -1

private:
    // static void build(std::vector<Node>& nodes, const std::vector<Point3> verts, const std::vector<MeshTriangle> tris);

    static void build(
        std::vector<Node>& nodes, int currNode,
        const std::vector<Point3> verts, const std::vector<MeshTriangle> tris,
        const std::vector<int> indices
        /*int triStart, int triEnd*/
    );

    std::vector<int> triangleIndices;
    std::vector<Node> nodes;

    static const int MAX_LEAF_TRI_COUNT = 2;
};

/*
class AccTree
{
public:
    AccTree(const std::vector<int> triangles);
    bool intersect(const Ray& ray) const;

private:
    void build(int parent, int depth, const std::vector<int> trianlges);

    struct Node
    {
        Node(const BoundingBox aabb, int parent, int left, int right, const std::vector<int> tris);
        void split(Node* left, Node* right);

        bool is_leaf() const;

        BoundingBox aabb;
        
        int left = -1, right = -1;
        int parent;

        std::vector<int> triangles;
    };

    std::vector<Node> nodes;

    const int MAX_TRI_COUNT = 2;
    //Mesh
};
*/
