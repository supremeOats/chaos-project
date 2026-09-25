#pragma once

#include "BoundingBox.h"

struct Node {
    BoundingBox aabb;

    int parentIdx;
    int children[2] = {-1, -1};

    std::vector<int> triangles;

    bool is_leaf() const;
};

class AccTree
{
public:
    AccTree();
    AccTree(const std::vector<Point3> verts, const std::vector<MeshTriangle> tris);
    std::vector<int> intersect(const Ray& ray) const; //returns vector of indices of the trianlges in the intersected leaf

private:
    static void build(
        std::vector<Node>& nodes, int currNode,
        const std::vector<Point3> verts, const std::vector<MeshTriangle> tris,
        const std::vector<int> indices
    );

    std::vector<int> triangleIndices;
    std::vector<Node> nodes;

    static const int MAX_LEAF_TRI_COUNT = 4;
};
