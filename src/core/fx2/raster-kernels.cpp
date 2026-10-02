#include "pch.h"
#include "unlit-kernel-plain-vert.spv.h"
#include "unlit-kernel-color-vert.spv.h"
#include "unlit-kernel-texture-vert.spv.h"
#include "unlit-kernel-texture-color-vert.spv.h"
#include "unlit-kernel-plain-frag.spv.h"
#include "unlit-kernel-texture-frag.spv.h"
#include "skybox-vert.spv.h"
#include "skybox-frag.spv.h"
#include "vk-shaders/camera-ubo.h"
#include "vk-shaders/scene-ubo.h"
#include <array>
#include <cmath>

namespace GN::fx2 {
namespace {
using namespace gpu2;
auto * logger = getLogger("GN.fx2.kernel");

bool uniform(const GpuResourceSet & shared, size_t slot, size_t size) {
    if (shared.size() <= slot || shared[slot].size() != 1) return false;
    const auto & view = shared[slot][0];
    return view.buffer() && view.bufferView.type == GpuResourceView::BufferView::UNIFORM && view.bufferView.size >= size;
}

bool attribute(const RasterGeometry & geometry, uint32_t location, uint32_t components) {
    using F                                       = RasterGeometry::AttributeFormat;
    const RasterGeometry::VertexAttribute * found = nullptr;
    for (const auto & a : geometry.format.attributes) {
        if (a.location != location) continue;
        if (found) return false;
        found = &a;
    }
    if (!found || found->binding >= geometry.vertices.size()) return false;
    const auto & buffer = geometry.vertices[found->binding];
    if (!buffer.buffer) return false;
    const auto f     = static_cast<uint32_t>(found->format);
    uint32_t   bytes = 0;
    if (f == static_cast<uint32_t>(F::F32_1) + components - 1) bytes = components * 4;
    if (f == static_cast<uint32_t>(F::F16_1) + components - 1) bytes = components * 2;
    return bytes && found->offset <= buffer.stride && bytes <= buffer.stride - found->offset;
}

struct UnlitPush {
    glm::mat4 world;
    glm::vec4 color;
    glm::vec4 emissiveAndCutoff;
};
static_assert(sizeof(UnlitPush) == 96);
static_assert(offsetof(UnlitPush, color) == 64 && offsetof(UnlitPush, emissiveAndCutoff) == 80);

class UnlitKernelImpl final : public UnlitKernel {
    std::array<AutoRef<GpuShader>, 4> vertex;
    std::array<AutoRef<GpuShader>, 2> fragment;

public:
    GN_REGISTER_RUNTIME_TYPE(UnlitKernel);
    UnlitKernelImpl(): UnlitKernel(TYPE_INFO(), "unlit-kernel") {}
    Execution execution() const override { return Execution::RASTER; }
    bool      initialize(AutoRef<GpuContext> gpu) {
        if (!gpu) return false;
        const uint32_t * vertices[] = {kUnlitKernelPlainVertSpv, kUnlitKernelColorVertSpv, kUnlitKernelTextureVertSpv, kUnlitKernelTextureColorVertSpv};
        const size_t     sizes[]    = {sizeof(kUnlitKernelPlainVertSpv), sizeof(kUnlitKernelColorVertSpv), sizeof(kUnlitKernelTextureVertSpv),
                                       sizeof(kUnlitKernelTextureColorVertSpv)};
        for (size_t i = 0; i < vertex.size(); ++i) {
            vertex[i] = GpuShader::create({.context = gpu, .name = "unlit-kernel.vert", .binary = vertices[i], .size = sizes[i]});
            if (!vertex[i]) return false;
        }
        fragment[0] =
            GpuShader::create({.context = gpu, .name = "unlit-kernel.frag", .binary = kUnlitKernelPlainFragSpv, .size = sizeof(kUnlitKernelPlainFragSpv)});
        fragment[1] = GpuShader::create(
            {.context = gpu, .name = "unlit-kernel-texture.frag", .binary = kUnlitKernelTextureFragSpv, .size = sizeof(kUnlitKernelTextureFragSpv)});
        return fragment[0] && fragment[1];
    }
    bool record(GpuRaster & raster, const GpuResourceSet & shared, const Inputs & input) const override {
        const bool   textured = !input.colorMap.empty();
        const auto & g        = input.geometry;
        bool         valid    = uniform(shared, 1, sizeof(shader::CameraUBO)) && attribute(g, 0, 3) && (!input.useVertexColor || attribute(g, 4, 4)) &&
                                (!textured || attribute(g, 2, 2)) && g.instanceCount > 0 &&
                                (g.indexCount ? g.indices.buffer && (g.indices.stride == 2 || g.indices.stride == 4) : g.vertexCount > 0);
        for (int c = 0; c < 4; ++c) {
            valid &= std::isfinite(input.color[c]);
            for (int r = 0; r < 4; ++r) valid &= std::isfinite(input.worldFromObject[c][r]);
        }
        for (int i = 0; i < 3; ++i) valid &= std::isfinite(input.emissive[i]);
        valid &= std::isfinite(input.alphaCutoff) && input.alphaCutoff >= 0 && input.alphaCutoff <= 1;
        if (textured) {
            auto texture = input.colorMap.texture();
            valid &= texture && input.colorMap.imageView.type == GpuResourceView::ImageView::SAMPLED;
            if (texture) {
                const auto & d     = texture->descriptor();
                const auto & range = input.colorMap.imageView.range;
                valid &= d.faces == 1 && d.depth == 1 && range.i.face == 0 && range.i.mip < d.levels &&
                         (range.e.numArrayLayers == 1 || range.e.numArrayLayers == uint32_t(-1)) &&
                         (range.e.numMipLevels == uint32_t(-1) || (range.e.numMipLevels > 0 && range.e.numMipLevels <= d.levels - range.i.mip));
            }
        }
        if (!valid) GN_UNLIKELY {
                GN_ERROR(logger, "UnlitKernel: invalid camera binding, vertex layout, texture view, or immediate values");
                return false;
            }
        UnlitPush                 values {input.worldFromObject, input.color, glm::vec4(input.emissive, input.alphaCutoff)};
        GpuResourceTable          drawResources;
        GpuRaster::DrawParameters draw {.geometry = g, .resources = drawResources};
        draw.vs     = vertex[(textured ? 2 : 0) + (input.useVertexColor ? 1 : 0)];
        draw.ps     = fragment[textured ? 1 : 0];
        draw.states = input.states;
        drawResources.resize(textured ? 2 : 1);
        drawResources[0] = shared;
        if (textured) {
            drawResources[1].resize(1);
            drawResources[1][0].append(input.colorMap);
        }
        draw.immediates = referenceTo(new SimpleBlob<uint8_t>(sizeof(values), reinterpret_cast<const uint8_t *>(&values)));
        // Empty overrides intentionally preserve the caller's complete raster policy.
        raster.draw(draw);
        return true;
    }
};

class SkyboxKernelImpl final : public SkyboxKernel {
    AutoRef<GpuShader> vertex, fragment;

public:
    GN_REGISTER_RUNTIME_TYPE(SkyboxKernel);
    SkyboxKernelImpl(): SkyboxKernel(TYPE_INFO(), "skybox-kernel") {}
    Execution execution() const override { return Execution::RASTER; }
    bool      initialize(AutoRef<GpuContext> gpu) {
        if (!gpu) return false;
        vertex   = GpuShader::create({.context = gpu, .name = "skybox.vert", .binary = kSkyboxVertSpv, .size = sizeof(kSkyboxVertSpv)});
        fragment = GpuShader::create({.context = gpu, .name = "skybox.frag", .binary = kSkyboxFragSpv, .size = sizeof(kSkyboxFragSpv)});
        return vertex && fragment;
    }
    bool record(GpuRaster & raster, const GpuResourceSet & shared) const override {
        if (!uniform(shared, 0, sizeof(shader::SceneUBO)) || !uniform(shared, 1, sizeof(shader::CameraUBO)) || shared.size() <= 2 || shared[2].size() != 1 ||
            !shared[2][0].texture() || shared[2][0].texture()->descriptor().faces != 6)
            GN_UNLIKELY {
                GN_ERROR(logger, "SkyboxKernel: expected SSC scene/camera uniforms and a skybox cubemap");
                return false;
            }
        RasterGeometry            drawGeometry;
        GpuResourceTable          drawResources;
        GpuRaster::DrawParameters draw {.geometry = drawGeometry, .resources = drawResources};
        draw.vs                  = vertex;
        draw.ps                  = fragment;
        draw.states.depthState   = RasterState::DepthState {RasterState::Compare::LESS_EQUAL, false};
        draw.states.cullMode     = RasterState::CULL_NONE;
        drawGeometry.vertexCount = 3;
        drawResources.append(shared);
        raster.draw(draw);
        return true;
    }
};
} // namespace

AutoRef<UnlitKernel> UnlitKernel::create(AutoRef<gpu2::GpuContext> gpu) {
    AutoRef<UnlitKernelImpl> kernel(new UnlitKernelImpl);
    if (!kernel->initialize(std::move(gpu))) return {};
    return kernel;
}
AutoRef<SkyboxKernel> SkyboxKernel::create(AutoRef<gpu2::GpuContext> gpu) {
    AutoRef<SkyboxKernelImpl> kernel(new SkyboxKernelImpl);
    if (!kernel->initialize(std::move(gpu))) return {};
    return kernel;
}
} // namespace GN::fx2
