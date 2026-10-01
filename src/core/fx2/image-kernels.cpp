#include "pch.h"
#include <cmath>
#include "image-kernel-vert.spv.h"
#include "blur-kernel-frag.spv.h"
#include "blur-kernel-comp.spv.h"
#include "mipmap-kernel-frag.spv.h"
#include "mipmap-kernel-comp.spv.h"
namespace GN::fx2 {
using namespace gpu2;
namespace {
// Six vec4 slots stay below Vulkan's baseline 128-byte push-constant limit.
// Precomputed symmetric weights avoid per-pixel exponentials and extra UBO uploads.
struct Parameters {
    int32_t dimensions[4] = {};
    int32_t filter[4]     = {};
    float   weights[16]   = {};
};
bool supported(const AutoRef<Texture> & texture) {
    if (!texture) return false;
    const auto & d = texture->descriptor();
    return d.width && d.height && d.depth == 1 && d.faces == 1 && d.samples == 1 && d.levels && d.format == gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM();
}
GpuResourceView view(AutoRef<Texture> texture, uint32_t level) {
    GpuResourceView result;
    result.resource = texture;
    result.setSubresourceIndex({level, 0}).setSubresourceExtent({1, 1});
    return result;
}
struct ImageImplementation {
    AutoRef<GpuContext> gpu;
    AutoRef<GpuShader>  shader, vertex;
    bool                compute = false;
    bool                initialize(AutoRef<GpuContext> device, bool useCompute, bool blur) {
        if (!device) return false;
        gpu                     = device;
        compute                 = useCompute;
        const uint32_t * binary = blur ? (compute ? kBlurKernelCompSpv : kBlurKernelFragSpv) : (compute ? kMipmapKernelCompSpv : kMipmapKernelFragSpv);
        size_t           size   = blur ? (compute ? sizeof(kBlurKernelCompSpv) : sizeof(kBlurKernelFragSpv))
                                       : (compute ? sizeof(kMipmapKernelCompSpv) : sizeof(kMipmapKernelFragSpv));
        shader                  = GpuShader::create({.context = gpu, .name = "image-filter", .binary = binary, .size = size});
        if (!compute)
            vertex = GpuShader::create({.context = gpu, .name = "image-triangle", .binary = kImageKernelVertSpv, .size = sizeof(kImageKernelVertSpv)});
        return shader && (compute || vertex);
    }
    AutoRef<GpuPayload> pass(GpuResourceView source, GpuResourceView destination, const Parameters & values) const {
        auto             immediate = referenceTo(new SimpleBlob<uint8_t>(sizeof(values), reinterpret_cast<const uint8_t *>(&values)));
        GpuResourceTable resources;
        resources.resize(2);
        resources[1].resize(compute ? 2 : 1);
        resources[1][0].append(source);
        if (compute) {
            destination.setImageViewType(GpuResourceView::ImageView::STORAGE);
            resources[1][1].append(destination);
            auto cnc = GpuCnC::create({.gpu = gpu});
            if (!cnc) return {};
            GpuCnC::ComputeParameters p;
            p.cs         = shader;
            p.resources  = resources;
            p.immediates = immediate;
            p.x          = (values.dimensions[2] + 7) / 8;
            p.y          = (values.dimensions[3] + 7) / 8;
            cnc->compute(p);
            return cnc->seal();
        }
        RasterTarget target;
        target.setColorTarget(0, destination);
        target.states.cullMode = RasterState::CULL_NONE;
        auto raster            = GpuRaster::create("image-filter", {.gpu = gpu, .target = &target});
        if (!raster) return {};
        GpuRaster::DrawParameters p;
        p.vs                   = vertex;
        p.ps                   = shader;
        p.geometry.vertexCount = 3;
        p.resources            = resources;
        p.immediates           = immediate;
        raster->draw(p);
        return raster->seal();
    }
    bool record(const GaussianBlurInputs & input, DynaArray<AutoRef<GpuPayload>> & work) const {
        if (!supported(input.source) || !supported(input.destination) || !std::isfinite(input.sigma) || input.sigma <= 0 || input.radius < 1 ||
            input.radius > 15)
            GN_UNLIKELY return false;
        const auto & d   = input.source->descriptor();
        const auto & out = input.destination->descriptor();
        if (d.width != out.width || d.height != out.height) GN_UNLIKELY return false;
        auto descriptor   = d;
        descriptor.levels = 1;
        auto scratch      = Texture::create("blur-scratch", {.context = gpu, .descriptor = descriptor});
        if (!scratch) return false;
        Parameters p;
        p.dimensions[0] = p.dimensions[2] = d.width;
        p.dimensions[1] = p.dimensions[3] = d.height;
        p.filter[0]                       = input.radius;
        double sum                        = 1;
        p.weights[0]                      = 1;
        for (uint32_t i = 1; i <= input.radius; ++i) {
            const double ratio = double(i) / input.sigma;
            p.weights[i]       = float(std::exp(-0.5 * ratio * ratio));
            sum += 2 * p.weights[i];
        }
        for (auto & weight : p.weights) weight = float(weight / sum);
        p.filter[1]     = 1;
        auto horizontal = pass(view(input.source, 0), view(scratch, 0), p);
        if (!horizontal) return false;
        p.filter[1]   = 0;
        auto vertical = pass(view(scratch, 0), view(input.destination, 0), p);
        if (!vertical) return false;
        work.append(horizontal);
        work.append(vertical);
        return true;
    }
    bool record(const MipmapInputs & input, DynaArray<AutoRef<GpuPayload>> & work) const {
        if (!supported(input.texture)) GN_UNLIKELY return false;
        const auto & d    = input.texture->descriptor();
        uint32_t     last = input.lastLevel == uint32_t(-1) ? d.levels - 1 : input.lastLevel;
        if (input.baseLevel >= d.levels || last >= d.levels || last < input.baseLevel) GN_UNLIKELY return false;
        DynaArray<AutoRef<GpuPayload>> pending;
        for (uint32_t level = input.baseLevel + 1; level <= last; ++level) {
            Parameters p;
            p.dimensions[0] = std::max(1u, d.width >> (level - 1));
            p.dimensions[1] = std::max(1u, d.height >> (level - 1));
            p.dimensions[2] = std::max(1u, d.width >> level);
            p.dimensions[3] = std::max(1u, d.height >> level);
            auto payload    = pass(view(input.texture, level - 1), view(input.texture, level), p);
            if (!payload) return false;
            pending.append(payload);
        }
        for (const auto & payload : pending) work.append(payload);
        return true;
    }
};
} // namespace
namespace {
class RasterGaussianBlurKernelImpl final : public RasterGaussianBlurKernel {
    ImageImplementation implementation;

public:
    GN_REGISTER_RUNTIME_TYPE(RasterGaussianBlurKernel);
    RasterGaussianBlurKernelImpl(): RasterGaussianBlurKernel(TYPE_INFO(), "RasterGaussianBlurKernel") {}
    Execution execution() const override { return Execution::RASTER; }
    bool      initialize(AutoRef<GpuContext> gpu) { return implementation.initialize(gpu, false, true); }
    bool      record(const Inputs & input, DynaArray<AutoRef<GpuPayload>> & work) const override { return implementation.record(input, work); }
};
} // namespace
AutoRef<RasterGaussianBlurKernel> RasterGaussianBlurKernel::create(AutoRef<GpuContext> gpu) {
    AutoRef<RasterGaussianBlurKernelImpl> kernel(new RasterGaussianBlurKernelImpl);
    if (!kernel->initialize(gpu)) return {};
    return kernel;
}
namespace {
class ComputeGaussianBlurKernelImpl final : public ComputeGaussianBlurKernel {
    ImageImplementation implementation;

public:
    GN_REGISTER_RUNTIME_TYPE(ComputeGaussianBlurKernel);
    ComputeGaussianBlurKernelImpl(): ComputeGaussianBlurKernel(TYPE_INFO(), "ComputeGaussianBlurKernel") {}
    Execution execution() const override { return Execution::COMPUTE; }
    bool      initialize(AutoRef<GpuContext> gpu) { return implementation.initialize(gpu, true, true); }
    bool      record(const Inputs & input, DynaArray<AutoRef<GpuPayload>> & work) const override { return implementation.record(input, work); }
};
} // namespace
AutoRef<ComputeGaussianBlurKernel> ComputeGaussianBlurKernel::create(AutoRef<GpuContext> gpu) {
    AutoRef<ComputeGaussianBlurKernelImpl> kernel(new ComputeGaussianBlurKernelImpl);
    if (!kernel->initialize(gpu)) return {};
    return kernel;
}
namespace {
class RasterMipmapKernelImpl final : public RasterMipmapKernel {
    ImageImplementation implementation;

public:
    GN_REGISTER_RUNTIME_TYPE(RasterMipmapKernel);
    RasterMipmapKernelImpl(): RasterMipmapKernel(TYPE_INFO(), "RasterMipmapKernel") {}
    Execution execution() const override { return Execution::RASTER; }
    bool      initialize(AutoRef<GpuContext> gpu) { return implementation.initialize(gpu, false, false); }
    bool      record(const Inputs & input, DynaArray<AutoRef<GpuPayload>> & work) const override { return implementation.record(input, work); }
};
} // namespace
AutoRef<RasterMipmapKernel> RasterMipmapKernel::create(AutoRef<GpuContext> gpu) {
    AutoRef<RasterMipmapKernelImpl> kernel(new RasterMipmapKernelImpl);
    if (!kernel->initialize(gpu)) return {};
    return kernel;
}
namespace {
class ComputeMipmapKernelImpl final : public ComputeMipmapKernel {
    ImageImplementation implementation;

public:
    GN_REGISTER_RUNTIME_TYPE(ComputeMipmapKernel);
    ComputeMipmapKernelImpl(): ComputeMipmapKernel(TYPE_INFO(), "ComputeMipmapKernel") {}
    Execution execution() const override { return Execution::COMPUTE; }
    bool      initialize(AutoRef<GpuContext> gpu) { return implementation.initialize(gpu, true, false); }
    bool      record(const Inputs & input, DynaArray<AutoRef<GpuPayload>> & work) const override { return implementation.record(input, work); }
};
} // namespace
AutoRef<ComputeMipmapKernel> ComputeMipmapKernel::create(AutoRef<GpuContext> gpu) {
    AutoRef<ComputeMipmapKernelImpl> kernel(new ComputeMipmapKernelImpl);
    if (!kernel->initialize(gpu)) return {};
    return kernel;
}
} // namespace GN::fx2
