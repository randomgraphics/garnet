#ifndef __GN_RT_H
#define __GN_RT_H
// *****************************************************************************
/// \file
/// \brief   Ray tracing system
/// \author  chenlee(2019.05.17)
// *****************************************************************************

#include "GNgfx2.h"
#include <Eigen/Eigen>

namespace GN { namespace rt
{
    using namespace gfx;
    using Eigen::Vector2f;
    using Eigen::Vector3f;
    using Eigen::Vector4f;

    union Vec4
    {
        struct
        {
            Vector3f v3;
            union
            {
                float    f;
                int32_t  i;
                uint32_t u;
            } w;
        };
        Vector4f v4;
    };
    static_assert(sizeof(Vec4) == 16);

    struct AABB
    {
        Vector3f min;
        int             rope = 0;
        Vector3f max;
        int : 32;
        int             left = 0, right = 0;
        int : 32;
        int : 32;

        AABB() = default;

        // construct from triangle vertices.
        AABB(const Vector3f & v0, const Vector3f & v1, const Vector3f & v2)
        {
            min = v0.cwiseMin(v1).cwiseMin(v2);
            max = v0.cwiseMax(v1).cwiseMax(v2);
        }

        AABB(const AABB & a, const AABB & b)
        {
            Merge(a, b);
            rope = -1;
        }

        bool IsFinite() const
        {
            return isfinite(min.x())
                && isfinite(min.y())
                && isfinite(min.z())
                && isfinite(max.x())
                && isfinite(max.y())
                && isfinite(max.z());
        }

        bool Enclose(const AABB & a) const
        {
            GN_ASSERT(a.min.x() <= a.max.x());
            GN_ASSERT(a.min.y() <= a.max.y());
            GN_ASSERT(a.min.z() <= a.max.z());
            return min.x() <= a.min.x() && a.max.x() <= max.x()
                && min.y() <= a.min.y() && a.max.y() <= max.y()
                && min.z() <= a.min.z() && a.max.z() <= max.z();
        }

        bool Enclose(const Vector3f & v) const
        {
            return min.x() <= v.x() && v.x() <= max.x()
                && min.y() <= v.y() && v.y() <= max.y()
                && min.z() <= v.z() && v.z() <= max.z();
        }

        Vector3f GetCenter() const { return (min + max) / 2.0f; }

        Vector3f GetExtent() const { return max - min; }

        float GetDiagonalDistance() const
        {
            return (max - min).norm();
        }

        void Merge(const AABB & a, const AABB & b)
        {
            min = min.cwiseMin(a.min).cwiseMin(b.min);
            max = min.cwiseMax(a.max).cwiseMax(b.max);
            GN_ASSERT(Enclose(a) && Enclose(b));
        }

        void MergeWith(const AABB& a)
        {
            min = min.cwiseMin(a.min);
            max = max.cwiseMax(a.max);
            GN_ASSERT(Enclose(a));
        }

        /* Create an AABB in space defined by t that encloses b.
        static AABB Transform(const glm::mat4 & t, const AABB & b)
        {
            glm::vec4 corners[] = {
                glm::vec4(b.min.x(), b.min.y(), b.min.z(), 1),
                glm::vec4(b.min.x(), b.min.y(), b.max.z(), 1),
                glm::vec4(b.min.x(), b.max.y(), b.min.z(), 1),
                glm::vec4(b.min.x(), b.max.y(), b.max.z(), 1),
                glm::vec4(b.max.x(), b.min.y(), b.min.z(), 1),
                glm::vec4(b.max.x(), b.min.y(), b.max.z(), 1),
                glm::vec4(b.max.x(), b.max.y(), b.min.z(), 1),
                glm::vec4(b.max.x(), b.max.y(), b.max.z(), 1),
            };
            static_GN_ASSERT(8 == COUNT_OF(corners));
            AABB r = b;
            r.min = Vector3f(t * corners[0]);
            r.max = Vector3f(t * corners[0]);
            for (size_t i = 1; i < COUNT_OF(corners); ++i)
            {
                auto c = Vector3f(t * corners[i]);
                r.min = glm::min(r.min, c);
                r.max = glm::max(r.max, c);
            }
            return r;
        }*/
    };

    class AABBTree
    {
    public:
        struct Node
        {
            AABB box;
            Node *parent = nullptr, *left = nullptr, *right = nullptr;
            int primitive = -1;
            bool IsLeaf() const { GN_ASSERT(left && right || !left && !right); return nullptr == left; }
        };
        typedef Node * NodePtr;

        // no copy
        AABBTree(const AABBTree &) = delete;
        AABBTree & operator=(const AABBTree &) = delete;

        // can move
        AABBTree(AABBTree &&) = default;
        AABBTree & operator=(AABBTree &&) = default;

        AABBTree() = default;
        AABBTree(const Vector3f * vertices, size_t triangleCount, size_t startTriangleIndex = 0) { Rebuild(vertices, triangleCount, startTriangleIndex); }
        AABBTree(const AABB * boxes, size_t count) { Rebuild(boxes, count); }

        // Merging a collection of subtrees to build a new tree.
        // Once the new tree is built, all nodes in the subtrees will be cleared out.
        AABBTree(std::vector<AABBTree> &&);

        ~AABBTree() { Clear(); }

        bool Empty() const { return _nodes.empty(); }

        void Clear();

        size_t Size() const { return _nodes.size(); }

        const AABB & Root() const { GN_ASSERT(!_nodes.empty()); return _nodes.back()->box; }

        template<typename F>
        void DfsTraverse(F f) const
        {
            if (_nodes.empty()) return;
            std::stack<NodePtr> s;
            s.push(_nodes.back());
            while (!s.empty()) {
                auto p = s.top(); s.pop();
                GN_ASSERT(p);
                if (!p->IsLeaf()) {
                    s.push(p->right);
                    s.push(p->left);
                }
                f(p);
            }
        }

        template<typename F>
        void ForEach(F f) const
        {
            for (auto p : _nodes) f(p);
        }

        // rebuild tree out of a collection of triangle vertices
        // startTriangleIndex specifies primitive ID of vertices[0]
        void Rebuild(const Vector3f * vertices, size_t triangleCount, size_t startTriangleIndex);

        // rebuild the tree out of a collection of bounding boxes
        void Rebuild(const AABB * boxes, size_t count);

        void ExportBVH(const char* assetName);

        static AABBTree ImportBVH(const char * assetName);

        AABBTree Clone() const;

        // Serialize the tree into an array in DFS order.
        // the array is ready to be uploaded to GPU for rendering.
        std::vector<AABB> Serialize() const;

    private:
        std::vector<Node*> _nodes;
        size_t CountLeafNodes() const;
    };
}}

// *****************************************************************************
//                         END of GNrt.h
// *****************************************************************************
#endif
