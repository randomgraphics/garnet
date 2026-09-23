#pragma once
#if !defined(__GN_INSIDE_FIZ_H__)
    #error "Do not include <garnet/fiz/hull.h> directly. Include <garnet/GNfiz.h> instead."
#endif

#include <vector>

namespace GN::fiz {

/// Shape classification enum for collision hulls.
enum class HullType : uint8_t { BOX, SPHERE, CAPSULE, CYLINDER, CONVEX, MESH };

/// Abstract base class for collision and simulation geometries.
struct Hull : public RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

protected:
    using RCRT64::RCRT64;

public:
    virtual HullType type() const            = 0;
    virtual Box      localAabb() const       = 0;
    virtual Scalar   calculateVolume() const = 0;

    /// Factory methods creating individual concrete hull types.
    static GN_API AutoRef<Hull> createBox(const Vector3 & halfExtents);
    static GN_API AutoRef<Hull> createSphere(Scalar radius);
    static GN_API AutoRef<Hull> createCapsule(Scalar radius, Scalar halfHeight);
    static GN_API AutoRef<Hull> createCylinder(Scalar radius, Scalar halfHeight);
    static GN_API AutoRef<Hull> createConvex(const Vector3 * points, size_t count);
    static GN_API AutoRef<Hull> createMesh(const Vector3 * vertices, size_t vertexCount, const uint32_t * indices, size_t indexCount);
};

/// Axis-aligned box collision geometry.
struct BoxHull : public Hull {
    GN_API GN_REGISTER_RUNTIME_TYPE(Hull);

protected:
    using Hull::Hull;

public:
    virtual const Vector3 & halfExtents() const = 0;
};

/// Spherical collision geometry.
struct SphereHull : public Hull {
    GN_API GN_REGISTER_RUNTIME_TYPE(Hull);

protected:
    using Hull::Hull;

public:
    virtual Scalar radius() const = 0;
};

/// Capsule collision geometry (oriented along Y axis).
struct CapsuleHull : public Hull {
    GN_API GN_REGISTER_RUNTIME_TYPE(Hull);

protected:
    using Hull::Hull;

public:
    virtual Scalar radius() const     = 0;
    virtual Scalar halfHeight() const = 0;
};

/// Cylinder collision geometry (oriented along Y axis).
struct CylinderHull : public Hull {
    GN_API GN_REGISTER_RUNTIME_TYPE(Hull);

protected:
    using Hull::Hull;

public:
    virtual Scalar radius() const     = 0;
    virtual Scalar halfHeight() const = 0;
};

/// Convex polyhedron collision geometry.
struct ConvexHull : public Hull {
    GN_API GN_REGISTER_RUNTIME_TYPE(Hull);

protected:
    using Hull::Hull;

public:
    virtual const std::vector<Vector3> & vertices() const = 0;
};

/// Arbitrary indexed triangle mesh collision geometry (primarily for static scenery).
struct MeshHull : public Hull {
    GN_API GN_REGISTER_RUNTIME_TYPE(Hull);

protected:
    using Hull::Hull;

public:
    virtual const std::vector<Vector3> &  vertices() const = 0;
    virtual const std::vector<uint32_t> & indices() const  = 0;
};

} // namespace GN::fiz
