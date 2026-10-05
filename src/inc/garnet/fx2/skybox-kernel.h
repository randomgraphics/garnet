#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif

namespace GN::fx2 {

/// Fullscreen raster skybox using SSC's camera, environment map, and luminance scale.
/// Overrides culling to NONE and depth to LESS_EQUAL without writes: its vertices
/// lie at far depth so existing scene geometry remains visible. Other state is inherited.
struct SkyboxKernel : Kernel {
    GN_API                              GN_REGISTER_RUNTIME_TYPE(Kernel);
    GN_API static AutoRef<SkyboxKernel> create(AutoRef<gpu2::GpuContext>);
    /// Append a skybox draw using SSC set 0. Caller schedules its upload dependencies.
    /// All resources must belong to the creation device. False appends no draw.
    virtual bool record(gpu2::GpuRaster &, const gpu2::GpuResourceSet & shared) const = 0;

protected:
    using Kernel::Kernel;
};

} // namespace GN::fx2
