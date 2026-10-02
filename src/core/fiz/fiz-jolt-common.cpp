#include "fiz-jolt-common.h"

#include <cstdarg>
#include <cstdio>

namespace GN::fiz {

static void JoltTrace(const char * inFMT, ...) {
    va_list list;
    va_start(list, inFMT);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), inFMT, list);
    va_end(list);
    fprintf(stderr, "[JoltTrace] %s\n", buffer);
    fflush(stderr);
}

#ifdef JPH_ENABLE_ASSERTS
static bool JoltAssertFailed(const char * inExpression, const char * inMessage, const char * inFile, JPH::uint inLine) {
    fprintf(stderr, "\n[JoltAssert] %s (%s) at %s:%u\n", inExpression, inMessage ? inMessage : "", inFile, inLine);
    fflush(stderr);
    return false; // return false to avoid __debugbreak()
}
#endif

static std::once_flag sJoltInitOnce;
void                  ensureJoltInitialized() {
    std::call_once(sJoltInitOnce, []() {
        JPH::RegisterDefaultAllocator();
        JPH::Trace = JoltTrace;
        JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = JoltAssertFailed;)
        JPH::Factory::sInstance = new JPH::Factory();
        JPH::RegisterTypes();
    });
}

JPH::ShapeRefC createJoltShape(const Hull * hull) {
    if (!hull) return nullptr;
    switch (hull->type()) {
    case HullType::BOX: {
        const auto *          b   = static_cast<const BoxHull *>(hull);
        const auto &          ext = b->halfExtents();
        JPH::BoxShapeSettings settings(JPH::Vec3(ext.x, ext.y, ext.z));
        auto                  res = settings.Create();
        return res.IsValid() ? res.Get() : nullptr;
    }
    case HullType::SPHERE: {
        const auto *             s = static_cast<const SphereHull *>(hull);
        JPH::SphereShapeSettings settings(s->radius());
        auto                     res = settings.Create();
        return res.IsValid() ? res.Get() : nullptr;
    }
    case HullType::CAPSULE: {
        const auto *              c = static_cast<const CapsuleHull *>(hull);
        JPH::CapsuleShapeSettings settings(c->halfHeight(), c->radius());
        auto                      res = settings.Create();
        return res.IsValid() ? res.Get() : nullptr;
    }
    case HullType::CYLINDER: {
        const auto *               c = static_cast<const CylinderHull *>(hull);
        JPH::CylinderShapeSettings settings(c->halfHeight(), c->radius());
        auto                       res = settings.Create();
        return res.IsValid() ? res.Get() : nullptr;
    }
    case HullType::CONVEX: {
        const auto * cv  = static_cast<const ConvexHull *>(hull);
        const auto & pts = cv->vertices();
        if (pts.empty()) return nullptr;
        std::vector<JPH::Vec3> jpts;
        jpts.reserve(pts.size());
        for (const auto & p : pts) { jpts.emplace_back(p.x, p.y, p.z); }
        JPH::ConvexHullShapeSettings settings(jpts.data(), static_cast<int>(jpts.size()));
        auto                         res = settings.Create();
        return res.IsValid() ? res.Get() : nullptr;
    }
    case HullType::MESH: {
        const auto * m   = static_cast<const MeshHull *>(hull);
        const auto & v   = m->vertices();
        const auto & idx = m->indices();
        if (v.empty() || idx.empty()) return nullptr;
        JPH::VertexList jv;
        jv.reserve(v.size());
        for (const auto & pt : v) { jv.emplace_back(pt.x, pt.y, pt.z); }
        JPH::IndexedTriangleList jt;
        jt.reserve(idx.size() / 3);
        for (size_t i = 0; i + 2 < idx.size(); i += 3) { jt.emplace_back(idx[i], idx[i + 1], idx[i + 2], 0); }
        JPH::MeshShapeSettings settings(std::move(jv), std::move(jt));
        auto                   res = settings.Create();
        return res.IsValid() ? res.Get() : nullptr;
    }
    }
    return nullptr;
}

} // namespace GN::fiz
