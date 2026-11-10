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
        const gpu2::RasterGeometry * geometry = nullptr; ///< Borrowed for this call; its buffers are retained by the resulting scene.
        PbrMaterial * material = nullptr; ///< Borrowed for this call; the resulting scene retains this material.
        glm::mat4 objectToWorld = glm::mat4(1); ///< Affine transform from object space to world space.
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

/// Compute ray-query kernel for direct and environment-lit PBR rendering with ray-traced shadows.
///
/// The caller creates its CnC with the bindless heap at set 0 and these pass resources at set 1:
/// ACS (binding 0), SharedUniforms (binding 1), scene geometry table (binding 2), and output
/// storage image (binding 3). `record()` appends one dispatch and does not submit or present it.
struct RayTraceKernel : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    struct RecordParameters {
        gpu2::CnC & recorder; ///< Caller-owned pass recorder with the documented set-1 resources.
        PbrRayTracingScene & scene; ///< Previously built scene whose ACS is bound at set 1, binding 0.
        AutoRef<SkyMaterial> skyMaterial; ///< Optional same-heap sky used for diffuse environment lighting and soft shadows.
        AutoRef<SharedShaderConstants::UniformState> ssc; ///< Camera, lighting, and exposure for this dispatch.
        AutoRef<gpu2::Texture> output; ///< Single-sample RGBA16F storage image, also suitable for later sampling.
        uint32_t width = 0; ///< Dispatch/output width in pixels; must equal the output image width.
        uint32_t height = 0; ///< Dispatch/output height in pixels; must equal the output image height.
    };

    /// Record one ray-generation dispatch into the caller's output texture.
    virtual bool record(const RecordParameters & parameters) const = 0;

    /// Create a kernel using the supplied PBR descriptor heap.
    GN_API static AutoRef<RayTraceKernel> create(gpu2::DescriptorHeap & heap, gpu2::CnC & initialization);

    /// Build the standard set-1 resource table for a caller-created ray-trace CnC. Pass the result
    /// as `CnC::CreateParameters::passResources`, then record with the same scene, state, and output.
    GN_API static gpu2::GpuResourceTable createPassResources(PbrRayTracingScene & scene,
                                                              const AutoRef<SharedShaderConstants::UniformState> & ssc,
                                                              const AutoRef<gpu2::Texture> & output);

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::fx2
