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

void BoundingBox::expand(const Point3 vert)
{
    minBound.x() = std::min(minBound.x(), vert.x());
    minBound.y() = std::min(minBound.y(), vert.y());
    minBound.z() = std::min(minBound.z(), vert.z());

    maxBound.x() = std::max(maxBound.x(), vert.x());
    maxBound.y() = std::max(maxBound.y(), vert.y());
    maxBound.z() = std::max(maxBound.z(), vert.z());
}

void BoundingBox::expand_by_triangle(const std::vector<Point3> verts, const MeshTriangle tri)
{
    expand(verts[tri.v[0]]);
    expand(verts[tri.v[1]]);
    expand(verts[tri.v[2]]);
}

void BoundingBox::split_half(BoundingBox& left, BoundingBox& right) const
{
    double xLength = maxBound.x() - minBound.x();
    double yLength = maxBound.y() - minBound.y();
    double zLength = maxBound.z() - minBound.z();

    left = *this;
    right = *this;

    if (xLength >= yLength && xLength >= zLength) {
        left.maxBound.x() = minBound.x() + xLength / 2;
        right.minBound.x() = minBound.x() + xLength / 2;
    }
    else if (yLength >= xLength && yLength >= zLength) {
        left.maxBound.y() = minBound.y() + yLength / 2;
        right.minBound.y() = minBound.y() + yLength / 2;
    }
    else {
        left.maxBound.z() = minBound.z() + zLength / 2;
        right.minBound.z() = minBound.z() + zLength / 2;
    }
}

int BoundingBox::longest_axis() const
{
    double xLen = maxBound.x() - minBound.x();
    double yLen = maxBound.y() - minBound.y();
    double zLen = maxBound.z() - minBound.z();

    if (xLen >= yLen && xLen >= zLen) return 0;
    
    if (yLen >= xLen && yLen >= zLen) return 1;
    
    if (zLen >= xLen && zLen >= yLen) return 2;

    return -1;
}

Point3 BoundingBox::centroid() const
{
    return Point3(
        (maxBound.x() + minBound.x()) / 2,
        (maxBound.y() + minBound.y()) / 2,
        (maxBound.z() + minBound.z()) / 2
    );
}

//
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

    //recursively init children - split curr aabb and triangle list
    // ^ top-down
    std::vector<int> indices(tris.size());
    std::iota(indices.begin(), indices.end(), 0);

    build(nodes, 0, verts, tris, indices);
}

void AccTree::build(
    std::vector<Node>& nodes, int currNode,
    const std::vector<Point3> verts, const std::vector<MeshTriangle> tris,
    std::vector<int> indices)
{
    //base case
    if (indices.size() <= MAX_LEAF_TRI_COUNT) {
        nodes[currNode].triangles = indices;
        return;
    }

    //split AABB
    //include and expand

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

    /*
    for (const int& i : indices) {
        Point3 triCentroid = centroid(
            verts[tris[i].v[0]],
            verts[tris[i].v[1]],
            verts[tris[i].v[2]]
        );
        
        //TODO - ugly code, clean it
        switch (splitAxis)
        {
        case 0:
            if (triCentroid.x() < aabbCentroid.x()) {
                leftIndices.push_back(i);
                leftChildAABB.expand_by_triangle(verts, tris[i]);
            } else {
                rightIndices.push_back(i);
                rightChildAABB.expand_by_triangle(verts, tris[i]);
            }
            break;
        
        case 1:
            if (triCentroid.y() < aabbCentroid.y()) {
                leftIndices.push_back(i);
                leftChildAABB.expand_by_triangle(verts, tris[i]);
            } else {
                rightIndices.push_back(i);
                rightChildAABB.expand_by_triangle(verts, tris[i]);
            }
            break;

        case 2:
            if (triCentroid.z() < aabbCentroid.z()) {
                leftIndices.push_back(i);
                leftChildAABB.expand_by_triangle(verts, tris[i]);
            } else {
                rightIndices.push_back(i);
                rightChildAABB.expand_by_triangle(verts, tris[i]);
            }
            break;
        }

    }
    */

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

    /*
    int index = 0;
    while (nodes[index].aabb.intersect(ray)) {
        if (nodes[index].is_leaf()) {
            return nodes[index].triangles;
        }
        
        int leftChild = nodes[index].children[0];
        int rightChild = nodes[index].children[1];
        
        if (nodes[leftChild].aabb.intersect(ray)) {
            index = leftChild;
        }
        else {
            index = rightChild;
        }
    }
    
    return std::vector<int>();
    */
}

/*
void AccTree::build(int parent, int depth, const std::vector<int> triangles)
{
    if (triangles.size() <= MAX_TRI_COUNT) {
        nodes[parent].triangles = triangles;
        //aabb
    }
}

AccTree::AccTree(const std::vector<int> triangles)
: nodes()
{
    //sort mesh triangles
    std::vector<Point3> verts; // from triangles
    
    nodes.push_back(Node(BoundingBox(verts), 0, -1, -1, {}));
    build(0, 0, triangles);
    //...
}

bool AccTree::intersect(const Ray& ray) const
{
    int cnt = 0;
    while(true) {
        if (!nodes[cnt].aabb.intersect(ray)) {
            return false;
        }
        
        int leftChildIdx = nodes[cnt].left;
        int rightChildIdx = nodes[cnt].right;
        
        if(leftChildIdx > 0 && nodes[leftChildIdx].aabb.intersect(ray)) {
            //...
        }
        else if (true) {
            //...
        }
        else {
            //...
        }
    }
}
*/
