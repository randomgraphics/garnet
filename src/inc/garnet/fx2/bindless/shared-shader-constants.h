#if !defined(__GN_INSIDE_FX2_H__)
    #error "Do not include <garnet/fx2/bindless/shared-shader-constants.h> directly. Include <garnet/GNfx2.h> instead."
#endif

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace GN::fx2::bindless {

/// One std140-compatible direct-light record, matching the legacy FX2 scene uniform.
struct DirectLightUniform {
    enum Type : uint32_t { POINT, SPOT, DIRECTIONAL };
    /// xyz is world-space position (point/spot) or ray travel direction (directional); w is Type as float.
    glm::vec4 positionOrDir = {0, 0, 0, float(POINT)};
    /// Linear RGB luminous intensity (point/spot) or irradiance (directional); w is range, zero means unlimited.
    glm::vec4 colorAndRange = {0, 0, 0, 0};
    glm::vec4 coneAngles    = {1, 1, 0, 0}; ///< Inner/outer cone cosines in xy; zw reserved, matching the legacy record.
};

/// Combined frame, direct-light, camera, and sky-selection shader ABI, stored in one UBO.
/// This is the standard FX2 bindless schema; SSC storage itself still accepts opaque bytes.
/// Upload the complete struct through recordUniformUpdate(). All floating-point inputs must be finite.
/// Matrices are column-major. Environment textures/calibration belong to sky materials.
struct SharedUniforms {
    static constexpr uint32_t MAX_LIGHTS      = 16;
    static constexpr uint32_t NO_SKY_MATERIAL = uint32_t(-1);

    uint32_t frameCounter    = 0;
    float    frameDurationMs = 0;
    /// Index interpreted by the sky kernel; NO_SKY_MATERIAL disables sky selection. Zero is a valid index.
    uint32_t activeSkyMaterialIndex = NO_SKY_MATERIAL;
    uint32_t framePadding           = 0; ///< Align the camera matrices to a std140 16-byte boundary.

    glm::mat4 viewMatrix = glm::mat4(1); ///< World to view.
    /// View to clip; the caller applies the backend's clip-space convention, including Vulkan Y inversion.
    glm::mat4          projMatrix         = glm::mat4(1);
    glm::mat4          projViewMatrix     = glm::mat4(1); ///< projMatrix * viewMatrix, precomputed by the caller.
    glm::vec4          cameraPosition     = {0, 0, 0, 1}; ///< World-space xyz, w = 1.
    glm::vec2          renderTargetSize   = {1, 1};       ///< Width and height in pixels.
    float              nearPlane          = 0.01f;
    float              farPlane           = 10000.f;
    float              exposure           = 0.002f; ///< Nonnegative camera exposure before tone mapping; zero gives black.
    uint32_t           numLights          = 0;      ///< Active entries in lights, at most MAX_LIGHTS.
    float              padding[2]         = {};     ///< Align the final light array to a std140 16-byte boundary.
    DirectLightUniform lights[MAX_LIGHTS] = {};
};

/// Long-lived shared shader storage for bindless kernels. Uniform formats and material
/// schemas belong to callers; SSC interprets neither. Work is recorded without submission.
class SharedShaderConstants : public RCRT64 {
public:
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Retained lease on one independent uniform version. Keeping this object alive prevents
    /// its range from being reused, even after the upload completes. Every recorded/in-flight
    /// consumer must retain the state until completion or discard, not merely its buffer view.
    class UniformState : public RCRT64 {
    public:
        GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

        /// Uniform buffer range for this version. The view retains the buffer, not the range lease.
        virtual gpu2::GpuResourceView view() const = 0;

    protected:
        using RCRT64::RCRT64;
    };

    /// Opaque streaming allocation identifier. Zero is invalid; stale/foreign IDs are rejected.
    using StreamingId                                 = uint64_t;
    static constexpr StreamingId INVALID_STREAMING_ID = 0;

    /// CPU-writable allocation from a long-lived mapped buffer. No upload is needed.
    /// bytes remains valid until releaseStreaming(id); finish writing before submitting consumers.
    struct StreamingBlock {
        StreamingId        id = INVALID_STREAMING_ID;
        ArrayView<uint8_t> bytes;
        /// Storage-buffer range; its offset identifies the bytes read by the shader.
        /// Retains the buffer, but does not prevent releaseStreaming(id) from recycling the range.
        gpu2::GpuResourceView view;
    };

    /// Caller-supplied storage budgets; capacities are bytes, include alignment overhead, and
    /// must be nonzero. This contract uses fixed budgets: exhaustion fails without relocating
    /// outstanding ranges or waiting for GPU work.
    struct CreateParameters {
        AutoRef<gpu2::GpuContext> gpu;                   ///< Required owning GPU context.
        uint64_t                  uniformCapacity   = 0; ///< Budget for independently retained uniform versions.
        uint64_t                  streamingCapacity = 0; ///< Budget for CPU-to-GPU streaming allocations.
    };

    /// Create shared storage on the supplied GPU context. Returns an empty ref on invalid
    /// parameters, unsupported storage, or allocation failure. Does not submit GPU work.
    GN_API static AutoRef<SharedShaderConstants> create(const CreateParameters & params);

    /// Copy opaque uniform-buffer bytes during recording and append their upload to the caller's CnC.
    /// Returns an independent uniform state, or an empty ref for empty/unsupported input,
    /// insufficient capacity, or a recorder from another GPU context. On failure no upload
    /// is appended. The recorder and its sealed payload retain the range until completion
    /// or discard; each consumer must additionally retain the returned state.
    /// Submit the producer once before all consumers, with the required GPU dependencies.
    virtual AutoRef<UniformState> recordUniformUpdate(gpu2::bindless::CnC & producer, ArrayView<const uint8_t> bytes) = 0;

    /// Allocate exactly size writable bytes, with shader-compatible alignment chosen internally.
    /// Returns an invalid ID, empty bytes, and an empty view for zero size, insufficient capacity, or failure.
    /// Buffers are reused and persistently mapped where supported; allocations do not record
    /// transfers. Host writes must be visible to GPU readers before their submission.
    virtual StreamingBlock allocateStreaming(uint64_t size) = 0;

    /// Immediately recycle a streaming range. Invalid/stale IDs are ignored. The consuming
    /// payload must retain SSC and call this only after its final GPU use completes or its
    /// recording is discarded. Shared use requires waiting for every consumer; a block's
    /// destruction alone does not release it. No CPU writes or reads through bytes may follow.
    virtual void releaseStreaming(StreamingId id) = 0;

protected:
    using RCRT64::RCRT64;
};

/// Build pass bindings from a captured SharedUniforms state; invalid views yield an empty table.
/// Supply the table when creating the caller's raster. It retains the buffer, not the uniform
/// lease; each material draw retains that lease until payload cleanup.
GN_API gpu2::GpuResourceTable sharedUniformResources(const AutoRef<SharedShaderConstants::UniformState> & state);

} // namespace GN::fx2::bindless
