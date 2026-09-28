#include "fiz-jolt-common.h"
#include "solid-impl.h"
#include <garnet/GNfiz.h>

#include <Jolt/Physics/SoftBody/SoftBodySharedSettings.h>
#include <Jolt/Physics/SoftBody/SoftBodyCreationSettings.h>
#include <Jolt/Physics/SoftBody/SoftBodyMotionProperties.h>
#include <Jolt/Physics/SoftBody/SoftBodyVertex.h>

#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <vector>

namespace GN::fiz {

static constexpr float PI = 3.14159265358979323846f;

// ─── GelMesh Implementation ──────────────────────────────────────────────────

class GelMeshImpl : public GelMesh {
    GN_REGISTER_RUNTIME_TYPE(GelMesh);
    using GelMesh::GelMesh;

public:
    DynaArray<GelVertex>      mVertices;
    DynaArray<GelFace>        mFaces;
    DynaArray<GelTetrahedron> mTetrahedra;
    DynaArray<GelEdge>        mEdges;
    Scalar                    mRestVolume = 0.0f;

    GelMeshImpl(): GelMesh(TYPE_INFO(), "GelMesh") {}

    ArrayView<const GelVertex>      vertices() const override { return mVertices; }
    ArrayView<const GelFace>        faces() const override { return mFaces; }
    ArrayView<const GelTetrahedron> tetrahedra() const override { return mTetrahedra; }
    ArrayView<const GelEdge>        edges() const override { return mEdges; }

    Scalar restVolume() const override { return mRestVolume; }
};

AutoRef<GelMesh> GelMesh::createCube(uint32_t gridSize, Scalar size) {
    uint32_t N    = std::max(2u, gridSize);
    auto     mesh = AutoRef<GelMeshImpl>(new GelMeshImpl);

    Scalar h      = size / static_cast<Scalar>(N - 1);
    Scalar offset = -0.5f * size;

    // 1. Grid vertices
    for (uint32_t z = 0; z < N; ++z) {
        for (uint32_t y = 0; y < N; ++y) {
            for (uint32_t x = 0; x < N; ++x) {
                GelVertex v;
                v.position = {offset + static_cast<Scalar>(x) * h, offset + static_cast<Scalar>(y) * h, offset + static_cast<Scalar>(z) * h};
                v.velocity = {0.0f, 0.0f, 0.0f};
                v.invMass  = 1.0f;
                mesh->mVertices.append(v);
            }
        }
    }

    auto vIdx = [N](uint32_t x, uint32_t y, uint32_t z) -> uint32_t { return x + y * N + z * N * N; };

    // Set of edges to prevent duplicates
    struct EdgeKey {
        uint32_t a, b;
        bool     operator==(const EdgeKey & o) const { return a == o.a && b == o.b; }
    };
    struct EdgeHash {
        size_t operator()(const EdgeKey & k) const { return (static_cast<size_t>(k.a) << 32) ^ static_cast<size_t>(k.b); }
    };
    std::unordered_set<EdgeKey, EdgeHash> edgeSet;
    auto                                  addEdge = [&](uint32_t u, uint32_t v) {
        if (u == v) return;
        uint32_t mn = std::min(u, v);
        uint32_t mx = std::max(u, v);
        if (edgeSet.insert({mn, mx}).second) { mesh->mEdges.append(GelEdge {{mn, mx}, 0.0f}); }
    };

    // 2. Axial and shear edges
    for (uint32_t z = 0; z < N; ++z) {
        for (uint32_t y = 0; y < N; ++y) {
            for (uint32_t x = 0; x < N; ++x) {
                uint32_t curr = vIdx(x, y, z);
                if (x + 1 < N) addEdge(curr, vIdx(x + 1, y, z));
                if (y + 1 < N) addEdge(curr, vIdx(x, y + 1, z));
                if (z + 1 < N) addEdge(curr, vIdx(x, y, z + 1));

                // Face diagonals for shear stability
                if (x + 1 < N && y + 1 < N) {
                    addEdge(curr, vIdx(x + 1, y + 1, z));
                    addEdge(vIdx(x + 1, y, z), vIdx(x, y + 1, z));
                }
                if (x + 1 < N && z + 1 < N) {
                    addEdge(curr, vIdx(x + 1, y, z + 1));
                    addEdge(vIdx(x + 1, y, z), vIdx(x, y, z + 1));
                }
                if (y + 1 < N && z + 1 < N) {
                    addEdge(curr, vIdx(x, y + 1, z + 1));
                    addEdge(vIdx(x, y + 1, z), vIdx(x, y, z + 1));
                }
                // Body diagonal
                if (x + 1 < N && y + 1 < N && z + 1 < N) {
                    addEdge(curr, vIdx(x + 1, y + 1, z + 1));
                    addEdge(vIdx(x + 1, y, z), vIdx(x, y + 1, z + 1));
                    addEdge(vIdx(x, y + 1, z), vIdx(x + 1, y, z + 1));
                    addEdge(vIdx(x, y, z + 1), vIdx(x + 1, y + 1, z));
                }
            }
        }
    }

    // 3. Tetrahedral volume decomposition
    static const int tetIndices[6][4][3] = {{{0, 0, 0}, {0, 1, 1}, {0, 0, 1}, {1, 1, 1}}, {{0, 0, 0}, {0, 1, 0}, {0, 1, 1}, {1, 1, 1}},
                                            {{0, 0, 0}, {0, 0, 1}, {1, 0, 1}, {1, 1, 1}}, {{0, 0, 0}, {1, 0, 1}, {1, 0, 0}, {1, 1, 1}},
                                            {{0, 0, 0}, {1, 1, 0}, {0, 1, 0}, {1, 1, 1}}, {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {1, 1, 1}}};

    for (uint32_t z = 0; z < N - 1; ++z) {
        for (uint32_t y = 0; y < N - 1; ++y) {
            for (uint32_t x = 0; x < N - 1; ++x) {
                for (int t = 0; t < 6; ++t) {
                    GelTetrahedron tet;
                    for (int i = 0; i < 4; ++i) { tet.indices[i] = vIdx(x + tetIndices[t][i][0], y + tetIndices[t][i][1], z + tetIndices[t][i][2]); }
                    tet.compliance = 0.0f;
                    mesh->mTetrahedra.append(tet);
                }
            }
        }
    }

    // 4. Surface boundary faces
    auto addFace = [&](uint32_t i0, uint32_t i1, uint32_t i2) { mesh->mFaces.append(GelFace {{i0, i1, i2}}); };

    for (uint32_t y = 0; y < N - 1; ++y) {
        for (uint32_t x = 0; x < N - 1; ++x) {
            // -Z face (z = 0, normal {0, 0, -1})
            addFace(vIdx(x, y, 0), vIdx(x, y + 1, 0), vIdx(x + 1, y + 1, 0));
            addFace(vIdx(x, y, 0), vIdx(x + 1, y + 1, 0), vIdx(x + 1, y, 0));

            // +Z face (z = N-1, normal {0, 0, 1})
            addFace(vIdx(x, y, N - 1), vIdx(x + 1, y + 1, N - 1), vIdx(x, y + 1, N - 1));
            addFace(vIdx(x, y, N - 1), vIdx(x + 1, y, N - 1), vIdx(x + 1, y + 1, N - 1));
        }
    }

    for (uint32_t z = 0; z < N - 1; ++z) {
        for (uint32_t x = 0; x < N - 1; ++x) {
            // -Y face (y = 0, normal {0, -1, 0})
            addFace(vIdx(x, 0, z), vIdx(x + 1, 0, z + 1), vIdx(x, 0, z + 1));
            addFace(vIdx(x, 0, z), vIdx(x + 1, 0, z), vIdx(x + 1, 0, z + 1));

            // +Y face (y = N-1, normal {0, 1, 0})
            addFace(vIdx(x, N - 1, z), vIdx(x, N - 1, z + 1), vIdx(x + 1, N - 1, z + 1));
            addFace(vIdx(x, N - 1, z), vIdx(x + 1, N - 1, z + 1), vIdx(x + 1, N - 1, z));
        }
    }

    for (uint32_t z = 0; z < N - 1; ++z) {
        for (uint32_t y = 0; y < N - 1; ++y) {
            // -X face (x = 0, normal {-1, 0, 0})
            addFace(vIdx(0, y, z), vIdx(0, y, z + 1), vIdx(0, y + 1, z + 1));
            addFace(vIdx(0, y, z), vIdx(0, y + 1, z + 1), vIdx(0, y + 1, z));

            // +X face (x = N-1, normal {1, 0, 0})
            addFace(vIdx(N - 1, y, z), vIdx(N - 1, y + 1, z + 1), vIdx(N - 1, y, z + 1));
            addFace(vIdx(N - 1, y, z), vIdx(N - 1, y + 1, z), vIdx(N - 1, y + 1, z + 1));
        }
    }

    mesh->mRestVolume = size * size * size;
    return mesh;
}

AutoRef<GelMesh> GelMesh::createSphere(Scalar radius, uint32_t slices, uint32_t stacks) {
    uint32_t nSlices = std::max(4u, slices);
    uint32_t nStacks = std::max(3u, stacks);
    auto     mesh    = AutoRef<GelMeshImpl>(new GelMeshImpl);

    // Vertex 0: Center vertex. In a star tetrahedral decomposition, the center vertex
    // is shared by all tetrahedra (25% of total volume), so its lumped mass is ~N/4 times
    // that of an individual surface vertex. Setting invMass inversely proportional to face
    // count prevents XPBD constraint over-correction and numerical explosion.
    mesh->mVertices.append(GelVertex {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 1.0f});

    // Vertex 1: North pole (single distinct vertex)
    const uint32_t northPole = static_cast<uint32_t>(mesh->mVertices.size());
    mesh->mVertices.append(GelVertex {{0.0f, radius, 0.0f}, {0.0f, 0.0f, 0.0f}, 1.0f});

    // Intermediate latitude rings (1 to nStacks - 1)
    std::vector<uint32_t> ringStart(nStacks - 1);
    for (uint32_t r = 1; r < nStacks; ++r) {
        float phi        = PI * static_cast<float>(r) / static_cast<float>(nStacks);
        float sinPhi     = std::sin(phi);
        float cosPhi     = std::cos(phi);
        ringStart[r - 1] = static_cast<uint32_t>(mesh->mVertices.size());

        for (uint32_t s = 0; s < nSlices; ++s) {
            float theta    = 2.0f * PI * static_cast<float>(s) / static_cast<float>(nSlices);
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            Vector3 pos(radius * sinPhi * cosTheta, radius * cosPhi, radius * sinPhi * sinTheta);
            mesh->mVertices.append(GelVertex {pos, {0.0f, 0.0f, 0.0f}, 1.0f});
        }
    }

    // South pole (single distinct vertex)
    const uint32_t southPole = static_cast<uint32_t>(mesh->mVertices.size());
    mesh->mVertices.append(GelVertex {{0.0f, -radius, 0.0f}, {0.0f, 0.0f, 0.0f}, 1.0f});

    auto addSurfaceTriangle = [&](uint32_t f0, uint32_t f1, uint32_t f2) {
        mesh->mFaces.append(GelFace {{f0, f1, f2}});

        // Tetrahedron (0, f0, f1, f2)
        // Ensure strictly positive signed volume determinant for Jolt's XPBD volume constraint solver
        const Vector3 & p0 = mesh->mVertices[f0].position;
        const Vector3 & p1 = mesh->mVertices[f1].position;
        const Vector3 & p2 = mesh->mVertices[f2].position;
        Vector3         c(p1.y * p2.z - p1.z * p2.y, p1.z * p2.x - p1.x * p2.z, p1.x * p2.y - p1.y * p2.x);
        Scalar          det = p0.x * c.x + p0.y * c.y + p0.z * c.z;
        if (det < 0.0f) {
            mesh->mTetrahedra.append(GelTetrahedron {{0, f1, f0, f2}, 0.0f});
        } else {
            mesh->mTetrahedra.append(GelTetrahedron {{0, f0, f1, f2}, 0.0f});
        }
    };

    // 1. North cap faces (outward pointing normals)
    for (uint32_t s = 0; s < nSlices; ++s) {
        uint32_t sNext = (s + 1) % nSlices;
        addSurfaceTriangle(northPole, ringStart[0] + sNext, ringStart[0] + s);
    }

    // 2. Intermediate quad bands
    for (uint32_t r = 0; r + 1 < nStacks - 1; ++r) {
        uint32_t r0 = ringStart[r];
        uint32_t r1 = ringStart[r + 1];
        for (uint32_t s = 0; s < nSlices; ++s) {
            uint32_t sNext = (s + 1) % nSlices;
            uint32_t i0    = r0 + s;
            uint32_t i1    = r1 + s;
            uint32_t i2    = r1 + sNext;
            uint32_t i3    = r0 + sNext;
            addSurfaceTriangle(i0, i2, i1);
            addSurfaceTriangle(i0, i3, i2);
        }
    }

    // 3. South cap faces
    uint32_t lastRing = ringStart[nStacks - 2];
    for (uint32_t s = 0; s < nSlices; ++s) {
        uint32_t sNext = (s + 1) % nSlices;
        addSurfaceTriangle(southPole, lastRing + s, lastRing + sNext);
    }

    // Assign appropriate lumped inverse mass to center vertex
    if (!mesh->mFaces.empty()) { mesh->mVertices[0].invMass = 4.0f / static_cast<float>(mesh->mFaces.size()); }

    struct EdgeKey {
        uint32_t a, b;
        bool     operator==(const EdgeKey & o) const { return a == o.a && b == o.b; }
    };
    struct EdgeHash {
        size_t operator()(const EdgeKey & k) const { return (static_cast<size_t>(k.a) << 32) ^ static_cast<size_t>(k.b); }
    };
    std::unordered_set<EdgeKey, EdgeHash> edgeSet;
    auto                                  addEdge = [&](uint32_t u, uint32_t v) {
        if (u == v) return;
        uint32_t mn = std::min(u, v);
        uint32_t mx = std::max(u, v);
        if (edgeSet.insert({mn, mx}).second) { mesh->mEdges.append(GelEdge {{mn, mx}, 0.0f}); }
    };

    // Radial spokes from center to all surface vertices
    for (uint32_t v = 1; v < mesh->mVertices.size(); ++v) { addEdge(0, v); }

    // Surface face boundary edges
    for (const auto & f : mesh->mFaces) {
        addEdge(f.indices[0], f.indices[1]);
        addEdge(f.indices[1], f.indices[2]);
        addEdge(f.indices[2], f.indices[0]);
    }

    // Quad cross-diagonals for shear stability
    for (uint32_t r = 0; r + 1 < nStacks - 1; ++r) {
        uint32_t r0 = ringStart[r];
        uint32_t r1 = ringStart[r + 1];
        for (uint32_t s = 0; s < nSlices; ++s) {
            uint32_t sNext = (s + 1) % nSlices;
            addEdge(r0 + s, r1 + sNext);
            addEdge(r1 + s, r0 + sNext);
        }
    }

    mesh->mRestVolume = (4.0f / 3.0f) * PI * radius * radius * radius;
    return mesh;
}

AutoRef<GelMesh> GelMesh::create(const DynaArray<GelVertex> & vertices, const DynaArray<GelFace> & faces, const DynaArray<GelTetrahedron> & tetrahedra,
                                 const DynaArray<GelEdge> & edges) {
    auto mesh         = AutoRef<GelMeshImpl>(new GelMeshImpl);
    mesh->mVertices   = vertices;
    mesh->mFaces      = faces;
    mesh->mTetrahedra = tetrahedra;
    mesh->mEdges      = edges;

    if (mesh->mEdges.empty()) {
        struct EdgeKey {
            uint32_t a, b;
            bool     operator==(const EdgeKey & o) const { return a == o.a && b == o.b; }
        };
        struct EdgeHash {
            size_t operator()(const EdgeKey & k) const { return (static_cast<size_t>(k.a) << 32) ^ static_cast<size_t>(k.b); }
        };
        std::unordered_set<EdgeKey, EdgeHash> edgeSet;
        auto                                  addEdge = [&](uint32_t u, uint32_t v) {
            if (u == v) return;
            uint32_t mn = std::min(u, v);
            uint32_t mx = std::max(u, v);
            if (edgeSet.insert({mn, mx}).second) { mesh->mEdges.append(GelEdge {{mn, mx}, 0.0f}); }
        };
        for (const auto & f : faces) {
            addEdge(f.indices[0], f.indices[1]);
            addEdge(f.indices[1], f.indices[2]);
            addEdge(f.indices[2], f.indices[0]);
        }
        for (const auto & t : tetrahedra) {
            addEdge(t.indices[0], t.indices[1]);
            addEdge(t.indices[0], t.indices[2]);
            addEdge(t.indices[0], t.indices[3]);
            addEdge(t.indices[1], t.indices[2]);
            addEdge(t.indices[1], t.indices[3]);
            addEdge(t.indices[2], t.indices[3]);
        }
    }

    Scalar totalVol = 0.0f;
    for (const auto & t : tetrahedra) {
        const auto & p0 = vertices[t.indices[0]].position;
        const auto & p1 = vertices[t.indices[1]].position;
        const auto & p2 = vertices[t.indices[2]].position;
        const auto & p3 = vertices[t.indices[3]].position;
        Vector3      v1 = p1 - p0;
        Vector3      v2 = p2 - p0;
        Vector3      v3 = p3 - p0;
        Vector3      c(v2.y * v3.z - v2.z * v3.y, v2.z * v3.x - v2.x * v3.z, v2.x * v3.y - v2.y * v3.x);
        Scalar       det = v1.x * c.x + v1.y * c.y + v1.z * c.z;
        totalVol += std::abs(det) / 6.0f;
    }
    mesh->mRestVolume = totalVol;
    return mesh;
}

// ─── Gel Implementation ──────────────────────────────────────────────────────

class GelSolverImpl;

class GelImpl : public Gel {
    GN_REGISTER_RUNTIME_TYPE(Gel);
    using Gel::Gel;

public:
    GelImpl(GelSolverImpl * solver, const GelDesc & desc, JPH::BodyID bodyId, Scalar mass, AutoRef<GelMesh> mesh)
        : Gel(TYPE_INFO(), "Gel"), mSolver(solver), mMesh(std::move(mesh)), mTemper(desc.temper), mBodyId(bodyId), mEntityId(desc.entityId), mMass(mass) {
        if (mMesh) {
            mSurfaceIndices.reserve(mMesh->faces().size() * 3);
            for (const auto & f : mMesh->faces()) {
                mSurfaceIndices.append(f.indices[0]);
                mSurfaceIndices.append(f.indices[1]);
                mSurfaceIndices.append(f.indices[2]);
            }
        }
    }

    ~GelImpl() override = default;

    Transform transform() const override;
    void      setTransform(const Transform & transform) override;

    Vector3 position() const override;
    Vector3 linearVelocity() const override;
    void    setLinearVelocity(const Vector3 & velocity) override;

    const Temper & temper() const override { return mTemper; }
    void           setTemper(const Temper & temper) override;

    Box aabb() const override;

    AutoRef<Blob> deformedVertices(bool worldSpace = true) const override;

    ArrayView<const uint32_t> surfaceIndices() const override { return mSurfaceIndices; }

    Scalar currentVolume() const override;
    Scalar restVolume() const override { return mMesh ? mMesh->restVolume() : 0.0f; }

    bool isActive() const override;
    void activate() override;
    void deactivate() override;

    uint64_t entityId() const override { return mEntityId; }
    Scalar   mass() const override { return mMass; }

    void applyImpulse(const Vector3 & impulse) override;
    void applyForce(const Vector3 & force) override;

    JPH::BodyID bodyId() const { return mBodyId; }
    void        invalidateBody() { mBodyId = JPH::BodyID(); }

private:
    GelSolverImpl *     mSolver = nullptr;
    AutoRef<GelMesh>    mMesh;
    Temper              mTemper;
    JPH::BodyID         mBodyId;
    uint64_t            mEntityId = 0;
    Scalar              mMass     = 1.0f;
    DynaArray<uint32_t> mSurfaceIndices;
};

// ─── GelSolver Implementation ────────────────────────────────────────────────

class GelSolverImpl : public GelSolver {
    GN_REGISTER_RUNTIME_TYPE(GelSolver);
    using GelSolver::GelSolver;

public:
    GelSolverImpl(const GelSolverDesc & desc): GelSolver(TYPE_INFO(), "GelSolver"), mGravity(desc.gravity), mSimulationMode(desc.simulationMode) {
        ensureJoltInitialized();

        mTempAllocator = std::make_unique<JPH::TempAllocatorImplWithMallocFallback>(32 * 1024 * 1024);

        int threads = desc.numWorkerThreads == 0 ? -1 : static_cast<int>(desc.numWorkerThreads);
        mJobSystem  = std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, threads);

        mPhysicsSystem = std::make_unique<JPH::PhysicsSystem>();
        mPhysicsSystem->Init(desc.maxSolids + desc.maxGels, 0, desc.maxBodyPairs, desc.maxContactConstraints, mBPLayerInterface, mObjectVsBroadPhaseFilter,
                             mObjectLayerPairFilter);

        mPhysicsSystem->SetGravity(JPH::Vec3(mGravity.x, mGravity.y, mGravity.z));
    }

    ~GelSolverImpl() override {
        auto & bi = mPhysicsSystem->GetBodyInterface();
        for (auto & g : mGels) {
            if (!g->bodyId().IsInvalid()) {
                bi.RemoveBody(g->bodyId());
                bi.DestroyBody(g->bodyId());
                g->invalidateBody();
            }
        }
        for (auto & s : mSolids) {
            if (!s->bodyId().IsInvalid()) {
                bi.RemoveBody(s->bodyId());
                bi.DestroyBody(s->bodyId());
                s->invalidateBody();
            }
        }
        mGels.clear();
        mSolids.clear();
    }

    AutoRef<Gel> createGel(const GelDesc & desc) override {
        if (!desc.mesh || desc.mesh->vertices().empty()) return {};

        auto sharedSettings = new JPH::SoftBodySharedSettings();

        // Populate vertices
        sharedSettings->mVertices.reserve(static_cast<JPH::uint>(desc.mesh->vertices().size()));
        for (const auto & v : desc.mesh->vertices()) {
            JPH::SoftBodySharedSettings::Vertex jv;
            jv.mPosition = JPH::Float3(v.position.x, v.position.y, v.position.z);
            jv.mVelocity = JPH::Float3(desc.linearVelocity.x, desc.linearVelocity.y, desc.linearVelocity.z);
            jv.mInvMass  = v.invMass;
            sharedSettings->mVertices.push_back(jv);
        }

        // Populate faces
        sharedSettings->mFaces.reserve(static_cast<JPH::uint>(desc.mesh->faces().size()));
        for (const auto & f : desc.mesh->faces()) { sharedSettings->AddFace(JPH::SoftBodySharedSettings::Face(f.indices[0], f.indices[1], f.indices[2])); }

        // Populate edge constraints
        sharedSettings->mEdgeConstraints.reserve(static_cast<JPH::uint>(desc.mesh->edges().size()));
        for (const auto & e : desc.mesh->edges()) {
            sharedSettings->mEdgeConstraints.push_back(JPH::SoftBodySharedSettings::Edge(e.indices[0], e.indices[1], desc.edgeCompliance));
        }

        // Populate volume constraints (only when pressure is not enabled, avoiding constraint conflicts)
        if (desc.pressure == 0.0f) {
            sharedSettings->mVolumeConstraints.reserve(static_cast<JPH::uint>(desc.mesh->tetrahedra().size()));
            const auto & verts = desc.mesh->vertices();
            for (const auto & t : desc.mesh->tetrahedra()) {
                uint32_t i0 = t.indices[0];
                uint32_t i1 = t.indices[1];
                uint32_t i2 = t.indices[2];
                uint32_t i3 = t.indices[3];

                // Jolt's XPBD volume solver assumes (x1 - x0) x (x2 - x0) . (x3 - x0) > 0.
                // If negative, swapping i1 and i2 flips the sign to strictly positive.
                if (i0 < verts.size() && i1 < verts.size() && i2 < verts.size() && i3 < verts.size()) {
                    const Vector3 & p0 = verts[i0].position;
                    const Vector3 & p1 = verts[i1].position;
                    const Vector3 & p2 = verts[i2].position;
                    const Vector3 & p3 = verts[i3].position;
                    Vector3         v1 = p1 - p0;
                    Vector3         v2 = p2 - p0;
                    Vector3         v3 = p3 - p0;
                    Vector3         c(v2.y * v3.z - v2.z * v3.y, v2.z * v3.x - v2.x * v3.z, v2.x * v3.y - v2.y * v3.x);
                    Scalar          det = v1.x * c.x + v1.y * c.y + v1.z * c.z;
                    if (det < 0.0f) { std::swap(i1, i2); }
                }

                sharedSettings->mVolumeConstraints.push_back(JPH::SoftBodySharedSettings::Volume(i0, i1, i2, i3, desc.volumeCompliance));
            }
        }

        sharedSettings->CalculateEdgeLengths();
        sharedSettings->CalculateVolumeConstraintVolumes();
        sharedSettings->Optimize();

        JPH::RVec3 pos(desc.transform.position.x, desc.transform.position.y, desc.transform.position.z);
        JPH::Quat  rot(desc.transform.orientation.v.x, desc.transform.orientation.v.y, desc.transform.orientation.v.z, desc.transform.orientation.w);

        JPH::SoftBodyCreationSettings sbSettings(sharedSettings, pos, rot, static_cast<JPH::ObjectLayer>(desc.layer));
        sbSettings.mLinearDamping  = desc.temper.linearDamping;
        sbSettings.mFriction       = desc.temper.friction;
        sbSettings.mRestitution    = desc.temper.restitution;
        sbSettings.mPressure       = desc.pressure;
        sbSettings.mNumIterations  = desc.solverIterations;
        sbSettings.mAllowSleeping  = desc.allowSleep;
        sbSettings.mUpdatePosition = true;

        auto &      bi   = mPhysicsSystem->GetBodyInterface();
        JPH::Body * body = bi.CreateSoftBody(sbSettings);
        if (!body) return {};

        Scalar totalMass = desc.mesh->restVolume() * desc.temper.density;
        if (totalMass <= 0.0f) totalMass = 1.0f;

        auto gel = AutoRef<GelImpl>(new GelImpl(this, desc, body->GetID(), totalMass, desc.mesh));
        body->SetUserData(reinterpret_cast<uint64_t>(gel.get()));

        bi.AddBody(body->GetID(), JPH::EActivation::Activate);

        mGels.push_back(gel);
        return gel;
    }

    void removeGel(Gel * gel) override {
        if (!gel) return;
        auto *      gImpl = static_cast<GelImpl *>(gel);
        JPH::BodyID bId   = gImpl->bodyId();
        if (!bId.IsInvalid()) {
            auto & bi = mPhysicsSystem->GetBodyInterface();
            bi.RemoveBody(bId);
            bi.DestroyBody(bId);
            gImpl->invalidateBody();
        }
        auto it = std::find_if(mGels.begin(), mGels.end(), [gel](const AutoRef<GelImpl> & g) { return g.get() == gel; });
        if (it != mGels.end()) { mGels.erase(it); }
    }

    AutoRef<Solid> createSolid(const SolidDesc & desc) override {
        JPH::ShapeRefC shape = createJoltShape(desc.hull.get());
        if (!shape) return {};

        JPH::EMotionType jmt = JPH::EMotionType::Dynamic;
        switch (desc.motionType) {
        case MotionType::STATIC:
            jmt = JPH::EMotionType::Static;
            break;
        case MotionType::KINEMATIC:
            jmt = JPH::EMotionType::Kinematic;
            break;
        case MotionType::DYNAMIC:
            jmt = JPH::EMotionType::Dynamic;
            break;
        }

        JPH::ObjectLayer layer = static_cast<JPH::ObjectLayer>(desc.layer);
        JPH::RVec3       pos(desc.transform.position.x, desc.transform.position.y, desc.transform.position.z);
        JPH::Quat        rot(desc.transform.orientation.v.x, desc.transform.orientation.v.y, desc.transform.orientation.v.z, desc.transform.orientation.w);

        JPH::BodyCreationSettings settings(shape, pos, rot, jmt, layer);
        Scalar                    mass = desc.massOverride > 0.0f ? desc.massOverride : desc.hull->calculateVolume() * desc.temper.density;
        if (mass <= 0.0f) mass = 1.0f;
        if (desc.motionType == MotionType::DYNAMIC) {
            settings.mOverrideMassProperties       = JPH::EOverrideMassProperties::CalculateInertia;
            settings.mMassPropertiesOverride.mMass = mass;
        }

        settings.mRestitution    = desc.temper.restitution;
        settings.mFriction       = desc.temper.friction;
        settings.mLinearDamping  = desc.temper.linearDamping;
        settings.mAngularDamping = desc.temper.angularDamping;
        settings.mAllowSleeping  = desc.allowSleep;
        settings.mMotionQuality  = desc.ccd ? JPH::EMotionQuality::LinearCast : JPH::EMotionQuality::Discrete;

        auto &      bi   = mPhysicsSystem->GetBodyInterface();
        JPH::Body * body = bi.CreateBody(settings);
        if (!body) return {};

        auto solid = AutoRef<SolidImpl>(new SolidImpl(&bi, desc, body->GetID(), mass));
        body->SetUserData(reinterpret_cast<uint64_t>(solid.get()));

        bi.AddBody(body->GetID(), desc.motionType == MotionType::DYNAMIC ? JPH::EActivation::Activate : JPH::EActivation::DontActivate);

        if (desc.motionType == MotionType::DYNAMIC) {
            bi.SetLinearVelocity(body->GetID(), JPH::Vec3(desc.linearVelocity.x, desc.linearVelocity.y, desc.linearVelocity.z));
            bi.SetAngularVelocity(body->GetID(), JPH::Vec3(desc.angularVelocity.x, desc.angularVelocity.y, desc.angularVelocity.z));
        }

        mSolids.push_back(solid);
        return solid;
    }

    void removeSolid(Solid * solid) override {
        if (!solid) return;
        auto *      sImpl = static_cast<SolidImpl *>(solid);
        JPH::BodyID bId   = sImpl->bodyId();
        if (!bId.IsInvalid()) {
            auto & bi = mPhysicsSystem->GetBodyInterface();
            bi.RemoveBody(bId);
            bi.DestroyBody(bId);
            sImpl->invalidateBody();
        }
        auto it = std::find_if(mSolids.begin(), mSolids.end(), [solid](const AutoRef<SolidImpl> & s) { return s.get() == solid; });
        if (it != mSolids.end()) { mSolids.erase(it); }
    }

    void step(UnitOfTime dt) override {
        int64_t ns = dt.count();
        if (ns == 0) return;

        if (ns > 0) {
            float seconds = static_cast<float>(ns) * 1e-9f;
            int   steps   = std::max(1, static_cast<int>(std::ceil(seconds / (1.0f / 60.0f))));
            mPhysicsSystem->Update(seconds, steps, mTempAllocator.get(), mJobSystem.get());
        }
    }

    void setGravity(const Vector3 & gravity) override {
        mGravity = gravity;
        mPhysicsSystem->SetGravity(JPH::Vec3(mGravity.x, mGravity.y, mGravity.z));
    }

    Vector3 gravity() const override { return mGravity; }

    size_t gelCount() const override { return mGels.size(); }
    size_t solidCount() const override { return mSolids.size(); }

    void           setSimulationMode(SimulationMode mode) override { mSimulationMode = mode; }
    SimulationMode simulationMode() const override { return mSimulationMode; }

    JPH::BodyInterface &           bodyInterface() { return mPhysicsSystem->GetBodyInterface(); }
    const JPH::BodyLockInterface & bodyLockInterface() const { return mPhysicsSystem->GetBodyLockInterface(); }

private:
    Vector3                             mGravity;
    SimulationMode                      mSimulationMode;
    BPLayerInterfaceImpl                mBPLayerInterface;
    ObjectVsBroadPhaseLayerFilterImpl   mObjectVsBroadPhaseFilter;
    ObjectLayerPairFilterImpl           mObjectLayerPairFilter;
    std::unique_ptr<JPH::TempAllocator> mTempAllocator;
    std::unique_ptr<JPH::JobSystem>     mJobSystem;
    std::unique_ptr<JPH::PhysicsSystem> mPhysicsSystem;
    std::vector<AutoRef<GelImpl>>       mGels;
    std::vector<AutoRef<SolidImpl>>     mSolids;
};

// ─── GelImpl Method Implementations ──────────────────────────────────────────

Transform GelImpl::transform() const {
    if (mBodyId.IsInvalid() || !mSolver) return {};
    auto &     bi = mSolver->bodyInterface();
    JPH::RVec3 p  = bi.GetPosition(mBodyId);
    JPH::Quat  q  = bi.GetRotation(mBodyId);
    return Transform(Vector3(static_cast<float>(p.GetX()), static_cast<float>(p.GetY()), static_cast<float>(p.GetZ())),
                     Quaternion(q.GetX(), q.GetY(), q.GetZ(), q.GetW()));
}

void GelImpl::setTransform(const Transform & transform) {
    if (mBodyId.IsInvalid() || !mSolver) return;
    auto & bi = mSolver->bodyInterface();
    bi.SetPositionAndRotation(mBodyId, JPH::RVec3(transform.position.x, transform.position.y, transform.position.z),
                              JPH::Quat(transform.orientation.v.x, transform.orientation.v.y, transform.orientation.v.z, transform.orientation.w),
                              JPH::EActivation::Activate);
}

Vector3 GelImpl::position() const {
    if (mBodyId.IsInvalid() || !mSolver) return {};
    auto &     bi = mSolver->bodyInterface();
    JPH::RVec3 p  = bi.GetPosition(mBodyId);
    return Vector3(static_cast<float>(p.GetX()), static_cast<float>(p.GetY()), static_cast<float>(p.GetZ()));
}

Vector3 GelImpl::linearVelocity() const {
    if (mBodyId.IsInvalid() || !mSolver) return {};
    auto &    bi = mSolver->bodyInterface();
    JPH::Vec3 v  = bi.GetLinearVelocity(mBodyId);
    return Vector3(v.GetX(), v.GetY(), v.GetZ());
}

void GelImpl::setLinearVelocity(const Vector3 & velocity) {
    if (mBodyId.IsInvalid() || !mSolver) return;
    auto & bi = mSolver->bodyInterface();
    bi.SetLinearVelocity(mBodyId, JPH::Vec3(velocity.x, velocity.y, velocity.z));
}

void GelImpl::setTemper(const Temper & temper) {
    mTemper = temper;
    if (mBodyId.IsInvalid() || !mSolver) return;
    auto & bi = mSolver->bodyInterface();
    bi.SetFriction(mBodyId, temper.friction);
    bi.SetRestitution(mBodyId, temper.restitution);
}

Box GelImpl::aabb() const {
    if (mBodyId.IsInvalid() || !mSolver) return {};
    auto &     bi = mSolver->bodyInterface();
    JPH::AABox b  = bi.GetTransformedShape(mBodyId).GetWorldSpaceBounds();
    return Box(b.mMin.GetX(), b.mMin.GetY(), b.mMin.GetZ(), b.mMax.GetX() - b.mMin.GetX(), b.mMax.GetY() - b.mMin.GetY(), b.mMax.GetZ() - b.mMin.GetZ());
}

AutoRef<Blob> GelImpl::deformedVertices(bool worldSpace) const {
    if (mBodyId.IsInvalid() || !mSolver || !mMesh) return {};

    JPH::BodyLockRead lock(mSolver->bodyLockInterface(), mBodyId);
    if (!lock.Succeeded()) return {};

    const JPH::Body & body = lock.GetBody();
    const auto *      mp   = static_cast<const JPH::SoftBodyMotionProperties *>(body.GetMotionProperties());
    if (!mp) return {};

    const auto & verts = mp->GetVertices();
    size_t       n     = verts.size();
    if (n == 0) return {};

    auto blob = referenceTo(new SimpleBlob<DeformedVertex>(n));
    auto span = blob->accessor<DeformedVertex>();

    JPH::RVec3 com = body.GetCenterOfMassPosition();

    // 1. Extract deformed positions and initialize normals
    for (size_t i = 0; i < n; ++i) {
        if (worldSpace) {
            JPH::RVec3 wp    = com + verts[i].mPosition;
            span[i].position = Vector3(static_cast<float>(wp.GetX()), static_cast<float>(wp.GetY()), static_cast<float>(wp.GetZ()));
        } else {
            span[i].position = Vector3(verts[i].mPosition.GetX(), verts[i].mPosition.GetY(), verts[i].mPosition.GetZ());
        }
        span[i].normal = Vector3(0.0f, 0.0f, 0.0f);
    }

    // 2. Accumulate deformed surface normals from face topology
    for (const auto & f : mMesh->faces()) {
        uint32_t i0 = f.indices[0];
        uint32_t i1 = f.indices[1];
        uint32_t i2 = f.indices[2];

        if (i0 < n && i1 < n && i2 < n) {
            Vector3 e1 = span[i1].position - span[i0].position;
            Vector3 e2 = span[i2].position - span[i0].position;
            Vector3 fn(e1.y * e2.z - e1.z * e2.y, e1.z * e2.x - e1.x * e2.z, e1.x * e2.y - e1.y * e2.x);

            span[i0].normal += fn;
            span[i1].normal += fn;
            span[i2].normal += fn;
        }
    }

    // 3. Normalize vertex normals
    for (size_t i = 0; i < n; ++i) {
        float lenSq = span[i].normal.x * span[i].normal.x + span[i].normal.y * span[i].normal.y + span[i].normal.z * span[i].normal.z;
        if (lenSq > 1e-8f) {
            float invLen = 1.0f / std::sqrt(lenSq);
            span[i].normal *= invLen;
        } else {
            span[i].normal = Vector3(0.0f, 1.0f, 0.0f);
        }
    }

    return blob;
}

Scalar GelImpl::currentVolume() const {
    if (mBodyId.IsInvalid() || !mSolver) return 0.0f;
    JPH::BodyLockRead lock(mSolver->bodyLockInterface(), mBodyId);
    if (!lock.Succeeded()) return 0.0f;

    const JPH::Body & body = lock.GetBody();
    const auto *      mp   = static_cast<const JPH::SoftBodyMotionProperties *>(body.GetMotionProperties());
    if (!mp) return 0.0f;

    float vol = mp->GetVolume();
    return std::abs(vol);
}

bool GelImpl::isActive() const {
    if (mBodyId.IsInvalid() || !mSolver) return false;
    return mSolver->bodyInterface().IsActive(mBodyId);
}

void GelImpl::activate() {
    if (!mBodyId.IsInvalid() && mSolver) mSolver->bodyInterface().ActivateBody(mBodyId);
}

void GelImpl::deactivate() {
    if (!mBodyId.IsInvalid() && mSolver) mSolver->bodyInterface().DeactivateBody(mBodyId);
}

void GelImpl::applyImpulse(const Vector3 & impulse) {
    if (mBodyId.IsInvalid() || !mSolver) return;
    mSolver->bodyInterface().AddImpulse(mBodyId, JPH::Vec3(impulse.x, impulse.y, impulse.z));
}

void GelImpl::applyForce(const Vector3 & force) {
    if (mBodyId.IsInvalid() || !mSolver) return;
    mSolver->bodyInterface().AddForce(mBodyId, JPH::Vec3(force.x, force.y, force.z));
}

AutoRef<GelSolver> GelSolver::create(const GelSolverDesc & desc) { return AutoRef<GelSolver>(new GelSolverImpl(desc)); }

} // namespace GN::fiz
