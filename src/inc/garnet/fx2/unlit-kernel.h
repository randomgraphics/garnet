#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif

#include <glm/mat4x4.hpp>

namespace GN::fx2 {

/// Raster effect with optional texture/vertex color modulation; bypasses lighting/exposure.
/// Inherits caller depth, culling, blending, viewport, and scissor without overriding them.
struct UnlitKernel : Kernel {
    GN_API GN_REGISTER_RUNTIME_TYPE(Kernel);
    struct Inputs {
        /// Position at location 0; UV at 2 when textured; color at 4 when useVertexColor.
        /// Attributes use float16/float32 (position xyz, UV xy, color rgba).
        gpu2::RasterGeometry geometry;
        /// Optional caller overrides, applied only to this invocation.
        gpu2::RasterState states;
        glm::mat4         worldFromObject = glm::mat4(1);
        glm::vec4         color           = glm::vec4(1);
        glm::vec3         emissive        = glm::vec3(0);
        /// Reject fragments below this alpha after modulation; [0,1], zero disables rejection.
        float alphaCutoff = 0;
        /// Optional sampled 2D view. An empty view means constant white, without an upload.
        gpu2::GpuResourceView colorMap;
        bool                  useVertexColor = false;
    };
    /// Create shaders for this device. Returns empty on failure; performs no GPU submission.
    GN_API static AutoRef<UnlitKernel> create(AutoRef<gpu2::GpuContext>);
    /// Append one draw. shared uses SSC's set-0 contract (camera at binding 1).
    /// Caller schedules SSC/resource uploads before the raster. All resources must belong
    /// to the creation device. False appends no draw; validation failures are logged.
    virtual bool record(gpu2::GpuRaster &, const gpu2::GpuResourceSet & shared, const Inputs &) const = 0;

protected:
    using Kernel::Kernel;
};

} // namespace GN::fx2
