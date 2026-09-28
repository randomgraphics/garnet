#pragma once
#if !defined(__GN_INSIDE_FIZ_H__)
    #error "Do not include <garnet/fiz/gel.h> directly. Include <garnet/GNfiz.h> instead."
#endif

namespace GN::fiz {

struct Solid;
struct SolidDesc;

/// Single particle vertex within a soft-body tetrahedral mesh.
struct GelVertex {
    Vector3 position = {0.0f, 0.0f, 0.0f};
    Vector3 velocity = {0.0f, 0.0f, 0.0f};
    Scalar  invMass  = 1.0f;
};

/// Triangular surface face of a soft body, used for visual rendering and surface contacts.
struct GelFace {
    uint32_t indices[3] = {0, 0, 0};
};

/// Tetrahedral volume constraint enforcing hydrostatic incompressibility via XPBD.
struct GelTetrahedron {
    uint32_t indices[4] = {0, 0, 0, 0};
    Scalar   compliance = 0.0f; ///< XPBD volume inverse stiffness (0.0 = strictly incompressible).
};

/// Distance spring edge constraint restoring the rest length of adjacent particles.
struct GelEdge {
    uint32_t indices[2] = {0, 0};
    Scalar   compliance = 0.0f; ///< XPBD edge inverse stiffness (0.0 = rigid, >0.0 = elastic).
};

/// Static tetrahedral and surface mesh definition for instantiating soft volumetric bodies.
struct GelMesh : public RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

protected:
    using RCRT64::RCRT64;

public:
    /// Creates a tetrahedralized soft-body cube lattice.
    /// \param gridSize Number of vertices along each cube axis (minimum 2, typically 3 to 6).
    /// \param size Total side length of the cube.
    static GN_API AutoRef<GelMesh> createCube(uint32_t gridSize = 4, Scalar size = 1.0f);

    /// Creates a soft-body sphere with radial tetrahedral volume constraints.
    /// \param radius Radius of the sphere.
    /// \param slices Longitudinal segments around the Y axis.
    /// \param stacks Latitudinal segments from south pole to north pole.
    static GN_API AutoRef<GelMesh> createSphere(Scalar radius = 0.5f, uint32_t slices = 12, uint32_t stacks = 8);

    /// Creates a custom soft-body mesh from explicit vertices, surface faces, tetrahedra, and edges.
    static GN_API AutoRef<GelMesh> create(const DynaArray<GelVertex> & vertices, const DynaArray<GelFace> & faces, const DynaArray<GelTetrahedron> & tetrahedra,
                                          const DynaArray<GelEdge> & edges = {});

    virtual ArrayView<const GelVertex>      vertices() const   = 0;
    virtual ArrayView<const GelFace>        faces() const      = 0;
    virtual ArrayView<const GelTetrahedron> tetrahedra() const = 0;
    virtual ArrayView<const GelEdge>        edges() const      = 0;

    /// Computes the un-deformed rest volume of the tetrahedral mesh.
    virtual Scalar restVolume() const = 0;
};

/// Construction descriptor for instantiating a soft volumetric body.
struct GelDesc {
    AutoRef<GelMesh> mesh;
    Temper           temper;
    Transform        transform;
    Vector3          linearVelocity   = {0.0f, 0.0f, 0.0f};
    Vector3          angularVelocity  = {0.0f, 0.0f, 0.0f};
    CollisionLayer   layer            = CollisionLayer::MOVING;
    Scalar           edgeCompliance   = 1e-4f; ///< XPBD inverse stiffness for edge distance constraints.
    Scalar           volumeCompliance = 0.0f;  ///< XPBD inverse stiffness for volume preservation (0.0 = completely incompressible).
    Scalar           pressure         = 0.0f;  ///< Internal pneumatic overpressure factor for inflatables.
    bool             allowSleep       = false;
    uint32_t         solverIterations = 10;
    uint64_t         entityId         = 0;
};

/// Soft and bouncy volumetric body instance simulated via XPBD volume and edge constraints.
struct Gel : public RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

protected:
    using RCRT64::RCRT64;

public:
    /// Deformed surface vertex containing simulated position and computed normal.
    struct DeformedVertex {
        Vector3 position;
        Vector3 normal;
    };

    virtual Transform transform() const                         = 0;
    virtual void      setTransform(const Transform & transform) = 0;

    virtual Vector3 position() const                            = 0; ///< World-space center of mass position.
    virtual Vector3 linearVelocity() const                      = 0;
    virtual void    setLinearVelocity(const Vector3 & velocity) = 0;

    virtual const Temper & temper() const                   = 0;
    virtual void           setTemper(const Temper & temper) = 0;

    virtual Box aabb() const = 0;

    /// Returns a Blob holding an array of DeformedVertex (position and normal) in world or COM space.
    virtual AutoRef<Blob> deformedVertices(bool worldSpace = true) const = 0;

    /// View of the surface triangle index buffer (3 indices per face).
    virtual ArrayView<const uint32_t> surfaceIndices() const = 0;

    /// Total volume of the deformed soft body in its current simulated state.
    virtual Scalar currentVolume() const = 0;

    /// Un-deformed rest volume.
    virtual Scalar restVolume() const = 0;

    virtual bool isActive() const = 0;
    virtual void activate()       = 0;
    virtual void deactivate()     = 0;

    virtual uint64_t entityId() const = 0;
    virtual Scalar   mass() const     = 0;

    virtual void applyImpulse(const Vector3 & impulse) = 0;
    virtual void applyForce(const Vector3 & force)     = 0;
};

/// Construction descriptor for GelSolver.
struct GelSolverDesc {
    Vector3        gravity               = {0.0f, -9.81f, 0.0f};
    uint32_t       maxGels               = 1024;
    uint32_t       maxSolids             = 4096;
    uint32_t       maxBodyPairs          = 32768;
    uint32_t       maxContactConstraints = 32768;
    uint32_t       numWorkerThreads      = 0; ///< 0 = auto-detect hardware concurrency.
    SimulationMode simulationMode        = SimulationMode::DISSIPATIVE;
};

/// Simulation solver governing soft volumetric bodies (Gel) with XPBD constraints
/// and two-way interaction with rigid solid bodies.
struct GelSolver : public RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

protected:
    using RCRT64::RCRT64;

public:
    static GN_API AutoRef<GelSolver> create(const GelSolverDesc & desc = {});

    virtual AutoRef<Gel> createGel(const GelDesc & desc) = 0;
    virtual void         removeGel(Gel * gel)            = 0;

    /// Spawns a rigid solid within this solver for mutual collision with soft bodies.
    virtual AutoRef<Solid> createSolid(const SolidDesc & desc) = 0;
    virtual void           removeSolid(Solid * solid)          = 0;

    /// Advances simulation by discrete time dt.
    virtual void step(UnitOfTime dt) = 0;

    virtual void    setGravity(const Vector3 & gravity) = 0;
    virtual Vector3 gravity() const                     = 0;

    virtual size_t gelCount() const   = 0;
    virtual size_t solidCount() const = 0;

    virtual void           setSimulationMode(SimulationMode mode) = 0;
    virtual SimulationMode simulationMode() const                 = 0;
};

} // namespace GN::fiz
