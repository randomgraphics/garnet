#include <garnet/GNfiz.h>
#include <cmath>
#include <algorithm>

namespace GN::fiz {

static constexpr float PI = 3.14159265358979323846f;

class BoxHullImpl : public BoxHull {
    GN_FIZ_DEFINE_A_BEING(BoxHull);

public:
    BoxHullImpl(const Vector3 & halfExtents)
        : BoxHull(TYPE_INFO(), "BoxHull"), mHalfExtents(halfExtents) {}

    HullType type() const override { return HullType::BOX; }
    Box localAabb() const override {
        return Box(-mHalfExtents, mHalfExtents);
    }
    Scalar calculateVolume() const override {
        return 8.0f * mHalfExtents.x * mHalfExtents.y * mHalfExtents.z;
    }
    const Vector3 & halfExtents() const override { return mHalfExtents; }

private:
    Vector3 mHalfExtents;
};

class SphereHullImpl : public SphereHull {
    GN_FIZ_DEFINE_A_BEING(SphereHull);

public:
    SphereHullImpl(Scalar radius)
        : SphereHull(TYPE_INFO(), "SphereHull"), mRadius(radius) {}

    HullType type() const override { return HullType::SPHERE; }
    Box localAabb() const override {
        Vector3 ext(mRadius, mRadius, mRadius);
        return Box(-ext, ext);
    }
    Scalar calculateVolume() const override {
        return (4.0f / 3.0f) * PI * mRadius * mRadius * mRadius;
    }
    Scalar radius() const override { return mRadius; }

private:
    Scalar mRadius;
};

class CapsuleHullImpl : public CapsuleHull {
    GN_FIZ_DEFINE_A_BEING(CapsuleHull);

public:
    CapsuleHullImpl(Scalar radius, Scalar halfHeight)
        : CapsuleHull(TYPE_INFO(), "CapsuleHull"), mRadius(radius), mHalfHeight(halfHeight) {}

    HullType type() const override { return HullType::CAPSULE; }
    Box localAabb() const override {
        Vector3 ext(mRadius, mRadius + mHalfHeight, mRadius);
        return Box(-ext, ext);
    }
    Scalar calculateVolume() const override {
        return (4.0f / 3.0f) * PI * mRadius * mRadius * mRadius + PI * mRadius * mRadius * (2.0f * mHalfHeight);
    }
    Scalar radius() const override { return mRadius; }
    Scalar halfHeight() const override { return mHalfHeight; }

private:
    Scalar mRadius;
    Scalar mHalfHeight;
};

class CylinderHullImpl : public CylinderHull {
    GN_FIZ_DEFINE_A_BEING(CylinderHull);

public:
    CylinderHullImpl(Scalar radius, Scalar halfHeight)
        : CylinderHull(TYPE_INFO(), "CylinderHull"), mRadius(radius), mHalfHeight(halfHeight) {}

    HullType type() const override { return HullType::CYLINDER; }
    Box localAabb() const override {
        Vector3 ext(mRadius, mHalfHeight, mRadius);
        return Box(-ext, ext);
    }
    Scalar calculateVolume() const override {
        return PI * mRadius * mRadius * (2.0f * mHalfHeight);
    }
    Scalar radius() const override { return mRadius; }
    Scalar halfHeight() const override { return mHalfHeight; }

private:
    Scalar mRadius;
    Scalar mHalfHeight;
};

class ConvexHullImpl : public ConvexHull {
    GN_FIZ_DEFINE_A_BEING(ConvexHull);

public:
    ConvexHullImpl(const Vector3 * points, size_t count)
        : ConvexHull(TYPE_INFO(), "ConvexHull") {
        if (points && count > 0) {
            mVertices.assign(points, points + count);
            calculateBoundingBox(mAabb, points, sizeof(Vector3), count);
        }
    }

    HullType type() const override { return HullType::CONVEX; }
    Box localAabb() const override { return mAabb; }
    Scalar calculateVolume() const override {
        return mAabb.w * mAabb.h * mAabb.d * 0.5f;
    }
    const std::vector<Vector3> & vertices() const override { return mVertices; }

private:
    std::vector<Vector3> mVertices;
    Box mAabb = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
};

class MeshHullImpl : public MeshHull {
    GN_FIZ_DEFINE_A_BEING(MeshHull);

public:
    MeshHullImpl(const Vector3 * vertices, size_t vertexCount, const uint32_t * indices, size_t indexCount)
        : MeshHull(TYPE_INFO(), "MeshHull") {
        if (vertices && vertexCount > 0) {
            mVertices.assign(vertices, vertices + vertexCount);
            calculateBoundingBox(mAabb, vertices, sizeof(Vector3), vertexCount);
        }
        if (indices && indexCount > 0) {
            mIndices.assign(indices, indices + indexCount);
        }
    }

    HullType type() const override { return HullType::MESH; }
    Box localAabb() const override { return mAabb; }
    Scalar calculateVolume() const override {
        return mAabb.w * mAabb.h * mAabb.d;
    }
    const std::vector<Vector3> & vertices() const override { return mVertices; }
    const std::vector<uint32_t> & indices() const override { return mIndices; }

private:
    std::vector<Vector3>  mVertices;
    std::vector<uint32_t> mIndices;
    Box mAabb = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
};

AutoRef<Hull> Hull::createBox(const Vector3 & halfExtents) {
    return AutoRef<Hull>(new BoxHullImpl(halfExtents));
}

AutoRef<Hull> Hull::createSphere(Scalar radius) {
    return AutoRef<Hull>(new SphereHullImpl(radius));
}

AutoRef<Hull> Hull::createCapsule(Scalar radius, Scalar halfHeight) {
    return AutoRef<Hull>(new CapsuleHullImpl(radius, halfHeight));
}

AutoRef<Hull> Hull::createCylinder(Scalar radius, Scalar halfHeight) {
    return AutoRef<Hull>(new CylinderHullImpl(radius, halfHeight));
}

AutoRef<Hull> Hull::createConvex(const Vector3 * points, size_t count) {
    return AutoRef<Hull>(new ConvexHullImpl(points, count));
}

AutoRef<Hull> Hull::createMesh(const Vector3 * vertices, size_t vertexCount, const uint32_t * indices, size_t indexCount) {
    return AutoRef<Hull>(new MeshHullImpl(vertices, vertexCount, indices, indexCount));
}

} // namespace GN::fiz
