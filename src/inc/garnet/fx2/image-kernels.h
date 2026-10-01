#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif
namespace GN::fx2 {
/// Separable Gaussian filtering of a linear RGBA8 2D image. Alpha is filtered
/// independently with clamp-to-edge addressing; use premultiplied colors when
/// transparent edges require it.
/// Images must be single-layer, single-sample 2D RGBA8_UNORM textures.
/// Filtering replaces the destination; scene blending/depth state is irrelevant.
struct GaussianBlurInputs {
    AutoRef<gpu2::Texture> source, destination;
    /// Standard deviation in pixels, finite and positive. Radius is [1,15].
    float    sigma  = 2;
    uint32_t radius = 6;
};
/// Rebuild preallocated mip levels from baseLevel through lastLevel (inclusive).
/// Linear RGBA8 2D images only. Odd edges use area-weighted box filtering.
struct MipmapInputs {
    AutoRef<gpu2::Texture> texture;
    uint32_t               baseLevel = 0;
    /// UINT32_MAX selects the final allocated mip.
    uint32_t lastLevel = uint32_t(-1);
};
/// Explicit raster implementation; no backend fallback or implicit submission.
struct RasterGaussianBlurKernel : Kernel {
    GN_API GN_REGISTER_RUNTIME_TYPE(Kernel);
    using Inputs = GaussianBlurInputs;
    GN_API static AutoRef<RasterGaussianBlurKernel> create(AutoRef<gpu2::GpuContext>);
    /// Append ordered payloads on success; false leaves work unchanged. Inputs
    /// belong to the creation device. Payloads retain resources and copied data.
    /// Blur uses mip 0 of equally sized images; in-place blur is supported.
    virtual bool record(const Inputs &, DynaArray<AutoRef<gpu2::GpuPayload>> & work) const = 0;

protected:
    using Kernel::Kernel;
};
/// Explicit compute implementation; no backend fallback or implicit submission.
struct ComputeGaussianBlurKernel : Kernel {
    GN_API GN_REGISTER_RUNTIME_TYPE(Kernel);
    using Inputs = GaussianBlurInputs;
    GN_API static AutoRef<ComputeGaussianBlurKernel> create(AutoRef<gpu2::GpuContext>);
    /// Append ordered payloads on success; false leaves work unchanged. Inputs
    /// belong to the creation device. Payloads retain resources and copied data.
    /// Blur uses mip 0 of equally sized images; in-place blur is supported.
    virtual bool record(const Inputs &, DynaArray<AutoRef<gpu2::GpuPayload>> & work) const = 0;

protected:
    using Kernel::Kernel;
};
/// Explicit raster implementation; no backend fallback or implicit submission.
struct RasterMipmapKernel : Kernel {
    GN_API GN_REGISTER_RUNTIME_TYPE(Kernel);
    using Inputs = MipmapInputs;
    GN_API static AutoRef<RasterMipmapKernel> create(AutoRef<gpu2::GpuContext>);
    /// Append ordered payloads on success; false leaves work unchanged. Inputs
    /// belong to the creation device. Payloads retain resources and copied data.
    /// Each destination mip samples only its predecessor. A one-level range is a no-op.
    virtual bool record(const Inputs &, DynaArray<AutoRef<gpu2::GpuPayload>> & work) const = 0;

protected:
    using Kernel::Kernel;
};
/// Explicit compute implementation; no backend fallback or implicit submission.
struct ComputeMipmapKernel : Kernel {
    GN_API GN_REGISTER_RUNTIME_TYPE(Kernel);
    using Inputs = MipmapInputs;
    GN_API static AutoRef<ComputeMipmapKernel> create(AutoRef<gpu2::GpuContext>);
    /// Append ordered payloads on success; false leaves work unchanged. Inputs
    /// belong to the creation device. Payloads retain resources and copied data.
    /// Each destination mip samples only its predecessor. A one-level range is a no-op.
    virtual bool record(const Inputs &, DynaArray<AutoRef<gpu2::GpuPayload>> & work) const = 0;

protected:
    using Kernel::Kernel;
};
} // namespace GN::fx2
