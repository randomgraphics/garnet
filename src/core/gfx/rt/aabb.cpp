#include "pch.h"
#include <garnet/GNrt.h>
#include <stack>
#include <set>
#include <map>
#include <list>
#include <queue>
#include <fstream>
#include <unordered_map>
#include <optional>
#include <algorithm>

// set to 1 to do a more through but much slower tree verification.
#define VERIFY_AABB_TREE 0
#if VERIFY_AABB_TREE
// Use TREE_GN_VERIFY() on verification that could be slow.
// For fast and simple verification, just use GN_ASSERT()
#define TREE_GN_VERIFY GN_VERIFY
#else
#define TREE_GN_VERIFY(...) (void(0))
#endif

//static auto sLogger = GN::getLogger("GN.rt");

using namespace GN::rt;

// -----------------------------------------------------------------------------
//
struct Distance
{
    AABBTree::NodePtr n1, n2;
    float d;

    static Distance Make(AABBTree::NodePtr a, AABBTree::NodePtr b)
    {
        GN_ASSERT(a != b);
        AABB box(a->box, b->box);
        auto size = box.max - box.min;
        return {a, b, size.dot(size)};
    }
};
struct DistanceMap
{
    std::list<AABBTree::NodePtr> nodes;
    std::multimap<float, Distance> distances;

    void Clear()
    {
        nodes.clear();
        distances.clear();
    }

    void AddNode(AABBTree::NodePtr newNode) {
        TREE_GN_VERIFY(std::find(nodes.begin(), nodes.end(), newNode) == nodes.end());
        for (auto n : nodes) {
            AABB box(n->box, newNode->box);
            auto size = box.max - box.min;
            auto d = size.dot(size);
            distances.insert({d, {n, newNode, d}});
        }
        nodes.insert(nodes.end(), newNode);
        GN_ASSERT(nodes.size() * (nodes.size() - 1) == distances.size() * 2);
    }

    // void RebuildDistanceMap()
    // {
    //     distances.clear();
    //     for (auto a = nodes.begin(); a != nodes.end(); ++a) {
    //         for (auto b = a; b != nodes.end(); ++b) {
    //             if (a == b) continue;
    //             AABB box((*a)->box, (*b)->box);
    //             auto size = box.max - box.min;
    //             auto d = glm::dot(size, size);
    //             distances.insert({ d, {*a, *b, d} });
    //         }
    //     }
    //     GN_ASSERT(nodes.size() * (nodes.size() - 1) == distances.size() * 2);
    // }

    void RemoveNodePair(AABBTree::NodePtr n1, AABBTree::NodePtr n2)
    {
        // remove node from node list
        TREE_GN_VERIFY(std::find(nodes.begin(), nodes.end(), n1) != nodes.end());
        TREE_GN_VERIFY(std::find(nodes.begin(), nodes.end(), n2) != nodes.end());
        nodes.remove(n1);
        nodes.remove(n2);

        // remove distance(s) from distance map that contains either n1 or n2
        for (auto iter = distances.begin(); iter != distances.end();) {
            if (iter->second.n1 == n1 || iter->second.n2 == n1 ||
                iter->second.n1 == n2 || iter->second.n2 == n2) {
                iter = distances.erase(iter);
            }
            else {
                ++iter;
            }
        }
    }

    bool ContainsNode(const AABBTree::Node * n) const
    {
        return std::find(nodes.begin(), nodes.end(), n) != nodes.end();
    }

    void VerifySubTree(const AABBTree::Node * root) const
    {
        std::queue<const AABBTree::Node*> q;
        q.push(root);
        while (!q.empty()) {
            auto n = q.back(); q.pop();

            // the sub tree should not be part of the node list (except the root node)
            GN_ASSERT(n == root || !ContainsNode(n));

            if (!n->IsLeaf()) {
                GN_ASSERT(n->left && n->right);
                q.push(n->left);
                q.push(n->right);
                n->box.Enclose(n->left->box);
                n->box.Enclose(n->right->box);
            }
            else {
                GN_ASSERT(!n->left && !n->right);
            }
        }
    }

    // verify integrety of the distance map
    bool Verify() const
    {
        GN_VERIFY(nodes.size() * (nodes.size() - 1) == distances.size() * 2);

        for (auto a = nodes.begin(); a != nodes.end(); ++a) {

            // nodes in distance map should have no parents.
            GN_VERIFY(nullptr == (*a)->parent);

            VerifySubTree(*a);

            // each node pair should have 1 and only 1 instance in the distance map
            for (auto b = a; b != nodes.end(); ++b) {
                if (a == b) continue;
                auto d1 = Distance::Make(*a, *b);
                int count = 0;
                for (auto iter = distances.begin(); iter != distances.end(); ++iter) {
                    auto & d2 = iter->second;
                    GN_VERIFY(d2.d == iter->first);
                    if (d2.d != d1.d) continue;
                    if ((d2.n1 == *a && d2.n2 == *b) || (d2.n1 == *b && d2.n2 == *a)) {
                        ++count;
                    }
                }
                GN_VERIFY(1 == count);
            }
        }

        // each node pair should should show up exactly (nodes.size()-1) times in the map
        if (nodes.size() != 1) {
            std::map<AABBTree::NodePtr, size_t> times;
            for (auto iter = distances.begin(); iter != distances.end(); ++iter) {
                times[iter->second.n1]++;
                times[iter->second.n2]++;
            }
            GN_VERIFY(times.size() == nodes.size());
            for (auto t : times)
                GN_VERIFY(t.second == nodes.size() - 1);
        }

        return true;
    }
};

class NodeGrid
{
    std::vector<std::list<AABBTree::Node*>> _cells;
    Eigen::Vector3f _worldMin = {}, _worldMax = {};
    Eigen::Vector3f _cellSize = {};
    size_t _gridSize = 0, _gridSizeSquare = 0;

    static size_t GetCellCoord(float x, float min, float size)
    {
        GN_ASSERT(size >= 0.);
        if (0 == size) {
            GN_ASSERT(x == min);
            return 0;
        }
        float c = (x - min) / size;
        if ((c >= 1.) && ((c - (int)c) < 0.001f))
            c -= 1.;
        return (size_t)c;
    }

public:

    typedef std::list<AABBTree::Node*> Cell;

    // no copy
    NodeGrid(const NodeGrid&) = delete;
    NodeGrid & operator = (const NodeGrid&) = delete;

    // can move
    NodeGrid(NodeGrid&&) = default;
    NodeGrid& operator = (NodeGrid&&) = default;

    NodeGrid() {}

    void Allocate(Eigen::Vector3f worldMin, Eigen::Vector3f worldMax, size_t gridSize)
    {
        _cells.clear();
        _cells.resize(gridSize * gridSize * gridSize);
        _worldMin = worldMin;
        _worldMax = worldMax;
        _cellSize = (worldMax - worldMin) / (float)gridSize;
        _gridSize = gridSize;
        _gridSizeSquare = gridSize * gridSize;
    }

    std::vector<Cell>::iterator begin() { return _cells.begin(); }
    
    std::vector<Cell>::iterator end() { return _cells.end(); }

    size_t CountNodes() const
    {
        size_t n = 0;
        for (auto& c : _cells) {
            n += c.size();
        }
        return n;
    }

    const Eigen::Vector3f& CellSize() const { return _cellSize; }

    Cell * GetCell(size_t x, size_t y, size_t z)
    {
        GN_ASSERT(x < _gridSize && y < _gridSize && z < _gridSize);
        return &_cells[z * _gridSizeSquare + y * _gridSize + x];
    };

    size_t GridX(float x) { GN_ASSERT(_worldMin.x() <= x && x <= _worldMax.x()); return GetCellCoord(x, _worldMin.x(), _cellSize.x()); };
    
    size_t GridY(float y) { GN_ASSERT(_worldMin.y() <= y && y <= _worldMax.y()); return GetCellCoord(y, _worldMin.y(), _cellSize.y()); };
    
    size_t GridZ(float z) { GN_ASSERT(_worldMin.z() <= z && z <= _worldMax.z()); return GetCellCoord(z, _worldMin.z(), _cellSize.z()); };
};


// -----------------------------------------------------------------------------
//
static void BuildBranches(std::vector<AABBTree::Node*> & nodes)
{
    typedef AABBTree::Node Node;
    if (nodes.empty()) return;

    if (nodes.size() < 500) {
        // O(n^2) complexity. Very slow for big models.
        DistanceMap dm;
        for (auto i = nodes.begin(); i != nodes.end(); ++i) {
            dm.AddNode(*i);
        }
        while (dm.nodes.size() > 1) {
            const auto& d = dm.distances.begin()->second;
            auto n1 = d.n1;
            auto n2 = d.n2;

            // add new node to node array
            auto nn = new Node{ { d.n1->box, d.n2->box }, nullptr, n1, n2 };
            nodes.push_back(nn);
            //GN_ASSERT(0 == nn->box.model);
            GN_ASSERT(!nn->parent);
            GN_ASSERT(nn->left == n1);
            GN_ASSERT(nn->right == n2);
            GN_ASSERT(nn->primitive < 0);
            d.n1->parent = nn;
            d.n2->parent = nn;

            // remove distance(s) from distance map that contains either n1 or n2
            dm.RemoveNodePair(n1, n2);
            dm.AddNode(nn);
        };
    }
    else {
        // Much faster on big models. But slower on <500 items.
        auto world = nodes.front()->box;
        float scale = FLT_MAX;
        for (const auto& n : nodes) {
            world.MergeWith(n->box);
            scale = std::min(scale, n->box.GetDiagonalDistance());
        }
        const size_t MAX_GRID_SIZE = 256;
        size_t gridSize = 1;
        auto worldSize = world.GetDiagonalDistance();
        while (gridSize < MAX_GRID_SIZE && scale < worldSize) {
            scale *= 2; gridSize *= 2;
        }
        if (gridSize > 1) gridSize /= 2;
        GN_ASSERT(gridSize > 0);
        NodeGrid grid;
        grid.Allocate(world.min, world.max, gridSize);

        auto MergeNodes = [&](const std::vector<Node*> & leaves) {
            GN_ASSERT(!leaves.empty());
            DistanceMap dm;
            for (auto i = leaves.begin(); i != leaves.end(); ++i) {
                dm.AddNode(*i);
            }
            while (dm.nodes.size() > 1) {
                const auto& d = dm.distances.begin()->second;
                auto n1 = d.n1;
                auto n2 = d.n2;
                auto nn = new Node{ { d.n1->box, d.n2->box }, nullptr, n1, n2 };
                nodes.push_back(nn);
                GN_ASSERT(!nn->parent);
                GN_ASSERT(nn->left == n1);
                GN_ASSERT(nn->right == n2);
                GN_ASSERT(nn->primitive < 0);
                d.n1->parent = nn;
                d.n2->parent = nn;
                dm.RemoveNodePair(n1, n2);
                dm.AddNode(nn);
            }
            GN_ASSERT(1 == dm.nodes.size());
            if (leaves.size() > 1) {
                GN_ASSERT(*dm.nodes.begin() == nodes.back());
            }
            else {
                GN_ASSERT(*dm.nodes.begin() == leaves[0]);
            }
            return *dm.nodes.begin();
        };

        auto MergePrimitivesInCell = [&](NodeGrid::Cell & cell) {
            if (cell.size() <= 1) return;
            auto cellDiag = grid.CellSize().norm();
            std::vector<Node*> candidates;
            for (auto iter = cell.begin(); iter != cell.end();) {
                auto p = *iter;
                if (p->box.GetDiagonalDistance() <= cellDiag * 2.0f) {
                    candidates.push_back(p);
                    iter = cell.erase(iter);
                }
                else {
                    ++iter;
                }
            }
            if (candidates.size() > 0) {
                auto top = MergeNodes(candidates);
                cell.insert(cell.end(), top);
            }
        };

        // build the intial grids.
        for (const auto& n : nodes) {
            const auto& b = n->box;
            size_t x1 = grid.GridX(b.min.x());
            size_t y1 = grid.GridY(b.min.y());
            size_t z1 = grid.GridZ(b.min.z());
            grid.GetCell(x1, y1, z1)->push_back(n);
        }

        do {
            // merge small primitives in the same cell
            for (auto& cell : grid) {
                MergePrimitivesInCell(cell);
            }

            // Increase cell size by 2. Merge adjacent cells.
            if (gridSize > 1) {
                NodeGrid merged;
                merged.Allocate(world.min, world.max, gridSize / 2);
                for (size_t x = 0; x < gridSize; x += 2) {
                    for (size_t y = 0; y < gridSize; y += 2) {
                        for (size_t z = 0; z < gridSize; z += 2) {
                            auto dest = merged.GetCell(x / 2, y / 2, z / 2);
                            for (size_t i = 0; i < 8; ++i) {
                                auto l = x + (i % 2);
                                auto m = y + ((i % 4) / 2);
                                auto n = z + (i / 4);
                                auto source = grid.GetCell(l, m, n);
                                dest->insert(dest->end(), source->begin(), source->end());
                            }
                        }
                    }
                }
                TREE_GN_VERIFY(merged.CountNodes() == grid.CountNodes());
                grid = std::move(merged);
            }
            gridSize >>= 1;
        } while (gridSize > 0);
    }
}

template<typename T> struct PrimitiveTraits {};
template<> struct PrimitiveTraits<Eigen::Vector3f>
{
    typedef Eigen::Vector3f InputType;
    static const size_t ELEMENT_COUNT_PER_PRIMITIVE = 3;
    static AABB Construct(const Eigen::Vector3f * v)
    {
        GN_ASSERT(
               isfinite(v[0].x())
            && isfinite(v[1].y())
            && isfinite(v[2].z())
            && isfinite(v[0].x())
            && isfinite(v[1].y())
            && isfinite(v[2].z())
            && isfinite(v[0].x())
            && isfinite(v[1].y())
            && isfinite(v[2].z()));
        return { v[0], v[1], v[2] };
    }
};
template<> struct PrimitiveTraits<AABB>
{
    typedef AABB InputType;
    static const size_t ELEMENT_COUNT_PER_PRIMITIVE = 1;
    static AABB Construct(const AABB * b)
    {
        GN_ASSERT(b->IsFinite());
        return { *b, *b };
    }
};

// -----------------------------------------------------------------------------
// Utility function to verify integraty of the tree
template<typename T, typename TRAITS = PrimitiveTraits<T>>
static bool VerifyAABBTree(
    const std::vector<AABBTree::Node*> & nodes,
    const AABBTree::Node * root,
    const T * elements,
    size_t primitiveCount,
    size_t firstPrimitiveId)
{
    // all node pointers must be unqiue.
    auto clone = nodes;
    clone.erase(std::unique(clone.begin(), clone.end()), clone.end());
    GN_VERIFY(clone.size() == nodes.size());

    std::set<size_t> primitives;
    if (primitiveCount > 0) {
        for (size_t i = 0; i < primitiveCount; ++i) {
            primitives.insert(i + firstPrimitiveId);
        }
    }

    // With the current way we build the BVH, the first primitiveCount node are leaf nodes.
    // The last node is root node.
    GN_VERIFY(root == nodes.back());
    size_t leafcount1 = 0;
    for (const auto & n : nodes) {

        // check root node
        if (n->parent) {
            GN_VERIFY(n != root);
        }
        else
        {
            GN_VERIFY(n == root);
        }

        // All leaf nodes are at the beginning the node list.
        if (n->IsLeaf()) {
            // Once isLeafNode is set to false, we should see no more leaf nodes.
            if (primitiveCount > 0) {
                GN_VERIFY(firstPrimitiveId <= n->primitive && n->primitive <= (firstPrimitiveId + primitiveCount));
                GN_VERIFY(primitives.find(n->primitive) != primitives.end());
                primitives.erase(n->primitive);
            }
            leafcount1++;
        }
        else {
            GN_VERIFY(n->primitive < 0);
        }

        GN_VERIFY(!n->parent || std::find(nodes.begin(), nodes.end(), n->parent) != nodes.end());
        GN_VERIFY(!n->left   || std::find(nodes.begin(), nodes.end(), n->left  ) != nodes.end());
        GN_VERIFY(!n->right  || std::find(nodes.begin(), nodes.end(), n->right ) != nodes.end());
        if (n == root) {
            GN_VERIFY(!n->parent);
        }
    }
    GN_VERIFY(primitives.empty());
    if (primitiveCount > 0) {
        GN_VERIFY(leafcount1 == primitiveCount);
    }

    // DFS traverse the tree
    size_t leafcount2 = 0;
    size_t total = 0;
    std::stack<const AABBTree::Node*> s;
    if (root) s.push(root);
    while (!s.empty()) {
        auto p = s.top(); s.pop();
        GN_VERIFY(p);
        if (p->IsLeaf()) {
            ++leafcount2;
            GN_VERIFY(!p->left);
            GN_VERIFY(!p->right);
            GN_VERIFY((int)firstPrimitiveId <= p->primitive);
            if (primitiveCount > 0) { GN_VERIFY(p->primitive < (int)(firstPrimitiveId + primitiveCount)); }
            if (elements) {
                auto e = elements + (p->primitive - (int)firstPrimitiveId) * TRAITS::ELEMENT_COUNT_PER_PRIMITIVE;
                auto b = TRAITS::Construct(e);
                GN_VERIFY(p->box.Enclose(b));
            }
        }
        else {
            s.push(p->right);
            s.push(p->left);
            GN_VERIFY(p->left);
            GN_VERIFY(p->right);
            GN_VERIFY(p->left->parent == p);
            GN_VERIFY(p->right->parent == p);
            GN_VERIFY(p->box.Enclose(p->left->box));
            GN_VERIFY(p->box.Enclose(p->right->box));
        }
        ++total;
    }
    GN_VERIFY(leafcount2 == leafcount1);
    GN_VERIFY(total == nodes.size());

    // done
    return true;
}

//// -----------------------------------------------------------------------------
////
//void AABB::ExportToDGML(const char * filename, const AABB * tree, size_t count)
//{
//    if (!tree || !count) return;
//
//    // open file for writing
//    std::ofstream file(filename);
//    if (!file.good()) {
//        GN_ERROR(sLogger)("Failed to open file %s for writing.", filename);
//        return;
//    }
//
//    //header
//    file << R"(<?xml version="1.0" encoding="utf-8"?>
//<DirectedGraph Title="DrivingTest" Background="Blue" xmlns="http://schemas.microsoft.com/vs/2009/dgml">
//  <Categories>
//    <Category Id="Branch" Background="Brown"/>
//    <Category Id="Leaf" Background="Green"/>
//    <Category Id="Triangle" Background="Yellow"/>
//  </Categories>
//)";
//    // create node list
//    file << "  <Nodes>\n";
//    for (size_t i = 0; i < count; ++i) {
//        const auto & n = tree[i];
//        std::string label = n.rope >= 0 ? FormatString("%d->%d", i, n.rope) : FormatString("%d", i);
//        file << "   <Node Id=\"" << i << "\" Label=\"" << label << "\" Category=\"Branch\"/>\n";
//        auto children = &n.left;
//        for (int c = 0; c < 2; ++c) {
//            int child = children[c];
//            if (child <= 0)
//                file << "   <Node Id=\"Tri #" << -child << "\" Category=\"Triangle\"/>\n";
//        }
//    }
//    file << "  </Nodes>\n";
//
//    // create links
//    file << "  <Links>\n";
//    for (size_t i = 0; i < count; ++i) {
//        const auto & n = tree[i];
//        auto children = &n.left;
//        for (int c = 0; c < 2; ++c) {
//            int child = children[c];
//            if (child <= 0)
//                file << "   <Link Source=\"" << i << "\" Target=\"Tri #" << -child << "\"/>\n";
//            else
//                file << "   <Link Source=\"" << i << "\" Target=\"" << child << "\"/>\n";
//        }
//    }
//    file << "  </Links>\n";
//
//    // footer
//    file << "</DirectedGraph>";
//}

// -----------------------------------------------------------------------------
//
AABBTree::AABBTree(std::vector<AABBTree> && subtrees)
{
    GN_FUNCTION_PROFILER();

    // Build TLAS
    std::vector<Node*> tlas;
    size_t subtreeNodeCount = 0;
    for (size_t i = 0; i < subtrees.size(); ++i) {
        auto& t = subtrees[i];
        TREE_GN_VERIFY(VerifyAABBTree<Eigen::Vector3f>(t._nodes, t._nodes.back(), nullptr, 0, 0));
        if (!t.Empty()) {
            tlas.push_back(t._nodes.back());
            //tlas.back()->box.model = (int)i + 1;
            subtreeNodeCount += t._nodes.size() - 1;
        }
    }
    BuildBranches(tlas);

    // Move all nodes from subtrees to this tree.
    _nodes.reserve(subtreeNodeCount + tlas.size());
    for(size_t i = 0; i < subtrees.size(); ++i) {
        auto& t = subtrees[i];
        if (!t.Empty()) {
            t._nodes.pop_back(); // pop out of the root node, it is already in TLAS.
            //for (auto p : t._nodes) p->box.model = (int)i + 1;
            _nodes.insert(_nodes.begin(), t._nodes.begin(), t._nodes.end());
            t._nodes.clear();
        }
    }

    // append TLAS at the end. This ensures that root node is the last node
    _nodes.insert(_nodes.end(), tlas.begin(), tlas.end());
    GN_ASSERT(_nodes.size() == subtreeNodeCount + tlas.size());
    TREE_GN_VERIFY(VerifyAABBTree<Eigen::Vector3f>(_nodes, _nodes.back(), nullptr, 0, 0));
    subtrees.clear();
}

// -----------------------------------------------------------------------------
//
void AABBTree::Clear()
{
    for (auto p : _nodes) delete p;
    _nodes.clear();
}

// -----------------------------------------------------------------------------
//
void AABBTree::Rebuild(const Eigen::Vector3f * vertices, size_t triangleCount, size_t startTriangleIndex)
{
    Clear();

    // added leaf nodes
    for (size_t i = 0; i < triangleCount; ++i) {
        int primitive = (int)(startTriangleIndex + i);
        auto v = vertices + i * 3;
        _nodes.push_back(new Node{ {v[0], v[1], v[2]}, nullptr, nullptr, nullptr, primitive });
        GN_ASSERT(_nodes.back()->primitive == primitive);
    }

    BuildBranches(_nodes);

    // verify the tree
    TREE_GN_VERIFY(VerifyAABBTree(_nodes, _nodes.back(), vertices, triangleCount, startTriangleIndex));
}

// -----------------------------------------------------------------------------
//
void AABBTree::Rebuild(const AABB * boxes, size_t count)
{
    Clear();

    // added leaf nodes
    for (size_t i = 0; i < count; ++i) {
        int primitive = (int)(i);
        _nodes.push_back(new Node{ boxes[i], nullptr, nullptr, nullptr, primitive });
        GN_ASSERT(_nodes.back()->primitive == primitive);
    }

    BuildBranches(_nodes);

    // verify the tree
    TREE_GN_VERIFY(VerifyAABBTree(_nodes, _nodes.back(), boxes, count, 0));
}

// -----------------------------------------------------------------------------
//
AABBTree AABBTree::Clone() const
{
    AABBTree c;

    // clone node array
    c._nodes.resize(_nodes.size());
    std::unordered_map<const Node*, Node*> m;
    for (size_t i = 0; i < _nodes.size(); ++i) {
        auto n = _nodes[i];
        auto p = new Node{};
        *p = *n;
        c._nodes[i] = p;
        TREE_GN_VERIFY(m.find(n) == m.end());
        m[n] = p;
    }

    // update node pointers
    GN_ASSERT(m.size() == _nodes.size());
    m[nullptr] = nullptr;
    for (auto p : c._nodes) {
        p->left = m[p->left];
        p->right = m[p->right];
        p->parent = m[p->parent];
    }

    // done
    //TREE_GN_VERIFY(VerifyAABBTree(c._nodes, c._root, nullptr, 0, 0));
    return c;
}

// -----------------------------------------------------------------------------
// Serialize the tree into DFS array
std::vector<AABB> AABBTree::Serialize() const
{
    GN_FUNCTION_PROFILER();

    struct IndexData
    {
        Node *rope = nullptr;
        int dfs = -1; // dfs order
    };
    std::vector<IndexData> indices(_nodes.size());
    std::unordered_map<Node*, size_t> ptr2idx; // node ptr to node index into _nodes array

    // Update rope pointer.
    for (size_t i = 0; i < _nodes.size(); ++i) {
        auto node = _nodes[i];
        ptr2idx[node] = i;
    }

    // update dfs index and rope pointer
    {
        int index = 0;
        std::stack<NodePtr> s;
        s.push(_nodes.back());
        while (!s.empty()) {
            auto p = s.top(); s.pop();
            GN_ASSERT(p);
            if (!p->IsLeaf()) {
                auto& id = indices[ptr2idx[p]];
                id.dfs = index++;
                if (!s.empty()) {
                    id.rope = s.top();
                }
                s.push(p->right);
                s.push(p->left);
            }
        }
    }

    std::vector<AABB> v;
    v.reserve(_nodes.size());
    DfsTraverse([&](NodePtr p) {
        if (!p->IsLeaf()) {
            AABB box = p->box;
            box.left = p->left->IsLeaf() ? -p->left->primitive : indices[ptr2idx[p->left]].dfs;
            box.right = p->right->IsLeaf() ? -p->right->primitive : indices[ptr2idx[p->right]].dfs;
            auto rope = indices[ptr2idx[p]].rope;
            box.rope = rope ? indices[ptr2idx[rope]].dfs : -1;
            v.push_back(box);
        }
    });

#if VERIFY_AABB_TREE
    // the first element in the output vector is the root node
    {
        std::vector<bool> pointers;
        std::set<int> primitives;
        int primMin = INT_MAX;
        int primMax = -1;
        int leafCount = 0;
        pointers.resize(v.size());
        //for (const auto & n : v) {
        for(size_t i = 0; i < v.size(); ++i) {
            const auto& n = v[i];
            for (auto c : n.children) {
                if (c > 0) {
                    GN_VERIFY(!pointers[c]);
                    pointers[c] = true;

                    // the vector is sorted in DFS order. So child index
                    // must be bigger than parent index.
                    GN_VERIFY(i < c && c < v.size());

                    GN_VERIFY(n.Enclose(v[c]));
                }
                else {
                    primitives.insert(primitives.end(), -c);
                    primMin = std::min(primMin, -c);
                    primMax = std::max(primMax, -c);
                    ++leafCount;
                }
            }
        }
        // all nodes, except root, should have one and only one node pointing to it.
        GN_VERIFY(!pointers[0]);
        for(size_t i = 1; i < pointers.size(); ++i) {
            GN_VERIFY(pointers[i]);
        }

        // all primitives have been included in the 
        GN_VERIFY(leafCount == primitives.size());

        // traverse the tree
        int leafCount2 = 0;
        int totalCount = 0;
        int stack[256];
        int top = 1;
        stack[0] = 0;
        std::vector<int> traverseHistory;
        while (top > 0) {
            ++totalCount;
            int i = stack[--top];
            traverseHistory.push_back(i);
            const auto& n = v[i];
            for (int c = 1; c >= 0; --c) {
                if (n.children[c] > 0) {
                    stack[top++] = n.children[c];
                }
                else {
                    leafCount2++;
                }
            }
        }
        GN_VERIFY(totalCount == v.size());
        GN_VERIFY(leafCount2 == leafCount);
    }
#endif

    // done
    return v;
}

// -----------------------------------------------------------------------------
//
size_t AABBTree::CountLeafNodes() const
{
    size_t c = 0;
    DfsTraverse([&](auto * p) {
        if (p->IsLeaf()) ++c;
    });
    return c;
}
