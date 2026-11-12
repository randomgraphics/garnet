#pragma once
#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif

namespace GN::fx2 {

/// Immutable scene data for the PBR ray-tracing kernel.
///
/// Creation retains the supplied geometry buffers and the same opaque PbrMaterial instances used by
/// raster kernels, and records the GPU acceleration-structure build into the supplied CnC.
/// The caller must submit that build work before recording a pass that traces this scene. The
/// kernel traces primary visibility and direct-light shadow rays, and samples the supplied sky
/// material for diffuse environment illumination and its soft visibility shadows. It does not
/// accumulate multiple bounces.
struct PbrRayTracingScene : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// One mesh instance included in the scene. Its vertex/index buffers must be created with
    /// `Buffer::CreateParameters::accelerationStructureBuildInput = true`. Geometry must expose
    /// float32 position, normal, and texcoord attributes using PbrMaterial's fixed locations.
    struct Mesh {
        const gpu2::RasterGeometry * geometry      = nullptr;      ///< Borrowed for this call; its buffers are retained by the resulting scene.
        PbrMaterial *                material      = nullptr;      ///< Borrowed for this call; the resulting scene retains this material.
        glm::mat4                    objectToWorld = glm::mat4(1); ///< Affine transform from object space to world space.
    };

    /// Build a scene and append its ACS build to initialization. The descriptor heap must be the
    /// same heap used to create every supplied material.
    GN_API static AutoRef<PbrRayTracingScene> create(gpu2::DescriptorHeap & heap, gpu2::CnC & initialization, ArrayView<const Mesh> meshes);

    /// Opaque gpu2 ACS resource to bind at set 1, binding 0 for the ray-tracing pass.
    virtual AutoRef<gpu2::AccelerationStructure> accelerationStructure() const = 0;

    /// Storage-buffer view mapping each ACS geometry index to its vertex/index addresses and material slot.
    virtual gpu2::GpuResourceView geometryTable() const = 0;

protected:
    using RCRT64::RCRT64;
};

/// Raster-primary PBR kernel that traces stochastic secondary rays from visible surfaces.
///
/// The caller creates the Raster with SharedShaderConstants resources at set 1 and the scene
/// acceleration structure and geometry table at set 2, bindings 0 and 1. Rasterization supplies
/// primary hits; the fragment shader shades those surfaces and traces secondary rays. `record()`
/// appends a draw to the caller's Raster; it does not submit or present it.
struct RayTraceKernel : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    struct DrawParameters : CommonDrawParameters {
        AutoRef<gpu2::AccelerationStructure> acs; ///< Previously built scene ACS bound at set 2, binding 0.
        glm::mat4                            object2WorldTransform = glm::mat4(1);
        AutoRef<SkyMaterial>                 skyMaterial           = {};
        PbrMaterial *                        material              = nullptr;
    };

    /// Record a rasterized primary hit shaded with ray-traced secondary rays.
    virtual bool record(const DrawParameters & parameters) const = 0;

    /// Create a kernel using the supplied PBR descriptor heap.
    GN_API static AutoRef<RayTraceKernel> create(gpu2::DescriptorHeap & heap, gpu2::CnC & initialization);

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::fx2
