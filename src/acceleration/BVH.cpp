#include "acceleration/BVH.h"

bool Node::is_leaf() const
{
    return !triangles.empty();
    // return children[0] == -1 && children[1] == -1;
}

//AccTree
AccTree::AccTree() {}

AccTree::AccTree(const std::vector<Point3> verts, const std::vector<MeshTriangle> tris)
    : nodes()
{
    //init root node
    BoundingBox meshAABB(verts);
    nodes.push_back(Node{meshAABB, -1, {-1, -1}, {}});

    //recursively init children - split curr aabb and triangle list, top-down
    std::vector<int> indices(tris.size());
    std::iota(indices.begin(), indices.end(), 0);

    build(nodes, 0, verts, tris, indices);
}

void AccTree::build(
    std::vector<Node>& nodes, int currNode,
    const std::vector<Point3> verts, const std::vector<MeshTriangle> tris,
    std::vector<int> indices)
{
    // Base case
    if (indices.size() <= MAX_LEAF_TRI_COUNT) {
        nodes[currNode].triangles = indices;
        return;
    }

    // Split AABB - include and expand
    int splitAxis = nodes[currNode].aabb.longest_axis();
    auto mid = indices.begin() + indices.size() / 2;

    std::nth_element(
        indices.begin(), mid, indices.end(),
        [&] (int idx1, int idx2) -> bool {
            Point3 centroid1 = centroid(idx1, verts, tris);
            Point3 centroid2 = centroid(idx2, verts, tris);
            return centroid1[splitAxis] < centroid2[splitAxis];
        }
    );

    std::vector<int> leftIndices(indices.begin(), mid);
    std::vector<int> rightIndices(mid, indices.end());

    BoundingBox leftChildAABB;
    for (int index : leftIndices) {
        leftChildAABB.expand_by_triangle(verts, tris[index]);
    }

    BoundingBox rightChildAABB;
    for (int index : rightIndices) {
        rightChildAABB.expand_by_triangle(verts, tris[index]);
    }

    // Build and add left subtree
    nodes.push_back(Node{
        leftChildAABB,
        currNode,
        {-1, -1},
        {}
    });
    nodes[currNode].children[0] = currNode+1;
    
    build(nodes, currNode+1, verts, tris, leftIndices);

    // Build and add right subtree
    int rightChildIdx = (int)nodes.size();

    nodes.push_back(Node{
        rightChildAABB,
        currNode,
        {-1, -1},
        {}
    });
    nodes[currNode].children[1] = rightChildIdx;
    
    build(nodes, rightChildIdx, verts, tris, rightIndices);
}

std::vector<int> AccTree::intersect(const Ray& ray) const
{
    std::vector<int> nodesToCheck = {0};
    std::vector<int> candidateTriangles{};

    while (!nodesToCheck.empty()) {
        const Node& currNode = nodes[nodesToCheck.back()];
        nodesToCheck.pop_back();

        if (!currNode.aabb.intersect(ray)) {
            continue;
        }
    
        if (currNode.is_leaf()) {
            candidateTriangles.insert(
                candidateTriangles.end(),
                currNode.triangles.begin(),
                currNode.triangles.end()
            );
        }
        else {
            if (currNode.children[0] != -1) {
                nodesToCheck.push_back(currNode.children[0]);
            }

            if (currNode.children[1] != -1) {
                nodesToCheck.push_back(currNode.children[1]);
            }
        }
    }

    return candidateTriangles;
}
