#include "pch.h"
#include "bindless-unlit-vert.spv.h"
#include "bindless-unlit-texture-vert.spv.h"
#include "bindless-unlit-frag.spv.h"
#include "bindless-lambertian-vert.spv.h"
#include "bindless-lambertian-texture-vert.spv.h"
#include "bindless-lambertian-frag.spv.h"
#include <glm/mat3x3.hpp>
#include <cmath>

namespace GN::fx2::bindless {
namespace {

auto * logger = getLogger("GN.fx2.bindless");

struct DrawConstants {
    glm::mat4  world;
    glm::vec4  color;
    glm::vec4  emissiveAndCutoff;
    glm::vec4  diffuseAndOpaque;
    glm::uvec4 textureIndices;
};
static_assert(sizeof(DirectLightUniform) == 48);
static_assert(offsetof(DirectLightUniform, positionOrDir) == 0);
static_assert(offsetof(DirectLightUniform, colorAndRange) == 16);
static_assert(offsetof(DirectLightUniform, coneAngles) == 32);
static_assert(sizeof(SharedUniforms) == 1024);
static_assert(offsetof(SharedUniforms, frameCounter) == 0);
static_assert(offsetof(SharedUniforms, frameDurationMs) == 4);
static_assert(offsetof(SharedUniforms, numLights) == 244);
static_assert(offsetof(SharedUniforms, activeSkyMaterialIndex) == 8);
static_assert(offsetof(SharedUniforms, lights) == 256);
static_assert(offsetof(SharedUniforms, viewMatrix) == 16);
static_assert(offsetof(SharedUniforms, projMatrix) == 80);
static_assert(offsetof(SharedUniforms, projViewMatrix) == 144);
static_assert(offsetof(SharedUniforms, cameraPosition) == 208);
static_assert(offsetof(SharedUniforms, renderTargetSize) == 224);
static_assert(offsetof(SharedUniforms, nearPlane) == 232);
static_assert(offsetof(SharedUniforms, farPlane) == 236);
static_assert(offsetof(SharedUniforms, exposure) == 240);
static_assert(sizeof(DrawConstants) == 128);
static_assert(offsetof(DrawConstants, color) == 64);
static_assert(offsetof(DrawConstants, emissiveAndCutoff) == 80);
static_assert(offsetof(DrawConstants, diffuseAndOpaque) == 96);
static_assert(offsetof(DrawConstants, textureIndices) == 112);

bool validUniform(const gpu2::GpuResourceView & view) {
    return view.buffer() && view.bufferView.type == gpu2::GpuResourceView::BufferView::UNIFORM && view.bufferView.size >= sizeof(SharedUniforms);
}

bool attribute(const gpu2::RasterGeometry & geometry, uint32_t location, uint32_t components = 3) {
    using F                                             = gpu2::RasterGeometry::AttributeFormat;
    const gpu2::RasterGeometry::VertexAttribute * found = nullptr;
    for (const auto & a : geometry.format.attributes) {
        if (a.location != location) continue;
        if (found) return false;
        found = &a;
    }
    if (!found || found->binding >= geometry.vertices.size()) return false;
    const auto &   v     = geometry.vertices[found->binding];
    const uint32_t bytes = uint32_t(found->format) == uint32_t(F::F32_1) + components - 1
                               ? components * 4
                               : (uint32_t(found->format) == uint32_t(F::F16_1) + components - 1 ? components * 2 : 0);
    return v.buffer && bytes && found->offset <= v.stride && bytes <= v.stride - found->offset;
}

bool validInputs(const SimpleMeshInputs & inputs, bool lit) {
    const auto & g     = inputs.geometry;
    bool         valid = attribute(g, 0) && (!lit || attribute(g, 1)) &&
                         (g.indexCount ? g.indices.buffer && (g.indices.stride == 2 || g.indices.stride == 4) : g.vertexCount > 0);
    for (int c = 0; c < 4; ++c) {
        valid &= std::isfinite(inputs.color[c]);
        for (int r = 0; r < 4; ++r) valid &= std::isfinite(inputs.worldFromObject[c][r]);
    }
    for (int c = 0; c < 3; ++c) valid &= std::isfinite(inputs.emissive[c]);
    valid &= std::isfinite(inputs.alphaCutoff) && inputs.alphaCutoff >= 0 && inputs.alphaCutoff <= 1;
    if (lit) {
        const float det = glm::determinant(glm::mat3(inputs.worldFromObject));
        valid &= std::isfinite(det) && std::abs(det) > 1e-20f;
    }
    return valid;
}

struct Shaders {
    AutoRef<gpu2::GpuShader> vertex, texturedVertex, fragment;
    bool initialize(AutoRef<gpu2::GpuContext> gpu, const uint32_t * vs, size_t vsSize, const uint32_t * ps, size_t psSize, const uint32_t * texturedVs,
                    size_t texturedVsSize) {
        if (!gpu) return false;
        vertex         = gpu2::GpuShader::create({.context = gpu, .name = "bindless-simple.vert", .binary = vs, .size = vsSize});
        fragment       = gpu2::GpuShader::create({.context = gpu, .name = "bindless-simple.frag", .binary = ps, .size = psSize});
        texturedVertex = gpu2::GpuShader::create({.context = gpu, .name = "bindless-simple-texture.vert", .binary = texturedVs, .size = texturedVsSize});
        return vertex && texturedVertex && fragment;
    }
};

bool append(const Shaders & shaders, gpu2::bindless::Raster & raster, AutoRef<SharedShaderConstants::UniformState> state, const SimpleMeshInputs & inputs,
            bool lit, float diffuse, bool opaque, gpu2::bindless::DescriptorHeap::DescriptorIndex normalMap = {}) {
    using Heap                 = gpu2::bindless::DescriptorHeap;
    const auto colorMap        = inputs.colorMap;
    const bool textured        = colorMap.u32 != 0 || normalMap.u32 != 0;
    const auto validDescriptor = [](Heap::DescriptorIndex index, Heap::DescriptorType type) { return index.tag == 1 && index.type == uint32_t(type); };
    const bool mapsValid       = (!colorMap.u32 || validDescriptor(colorMap, Heap::SAMPLED_TEXTURE)) &&
                                 (!normalMap.u32 || validDescriptor(normalMap, Heap::SAMPLED_TEXTURE)) &&
                                 (!textured || (validDescriptor(inputs.sampler, Heap::SAMPLER) && attribute(inputs.geometry, 2, 2)));
    if (!mapsValid || !state || !validUniform(state->view()) || !validInputs(inputs, lit) || !std::isfinite(diffuse) || diffuse < 0) GN_UNLIKELY {
            GN_ERROR(logger, "Simple kernel: invalid uniform state, geometry, transform, or immediate values");
            return false;
        }
    const DrawConstants values {inputs.worldFromObject, inputs.color, glm::vec4(inputs.emissive, inputs.alphaCutoff),
                                glm::vec4(diffuse, opaque ? 1.f : 0.f, 0, 0),
                                glm::uvec4(colorMap.slot, normalMap.slot, inputs.sampler.slot, (colorMap.u32 ? 1u : 0u) | (normalMap.u32 ? 2u : 0u))};
    gpu2::bindless::Raster::DrawParameters draw {.geometry = inputs.geometry};
    draw.vs         = textured ? shaders.texturedVertex : shaders.vertex;
    draw.ps         = shaders.fragment;
    draw.states     = inputs.states;
    draw.immediates = {reinterpret_cast<const uint8_t *>(&values), sizeof(values)};
    // The pass table retains only a buffer, so retain the uniform range lease separately.
    raster.retainResource(std::move(state));
    raster.recordDraw(draw);
    return true;
}

class UnlitImpl : public UnlitKernel {
    Shaders mShaders;

public:
    GN_REGISTER_RUNTIME_TYPE(UnlitKernel);
    UnlitImpl(): UnlitKernel(TYPE_INFO(), "bindless.unlit") {}
    Execution execution() const override { return Execution::RASTER; }
    bool      initialize(AutoRef<gpu2::GpuContext> gpu) {
        return mShaders.initialize(gpu, kBindlessUnlitVertSpv, sizeof(kBindlessUnlitVertSpv), kBindlessUnlitFragSpv, sizeof(kBindlessUnlitFragSpv),
                                   kBindlessUnlitTextureVertSpv, sizeof(kBindlessUnlitTextureVertSpv));
    }
    bool record(gpu2::bindless::Raster & raster, AutoRef<SharedShaderConstants::UniformState> state, const Inputs & inputs) const override {
        return append(mShaders, raster, std::move(state), inputs, false, 1, false);
    }
};

class LambertianImpl : public LambertianKernel {
    Shaders mShaders;

public:
    GN_REGISTER_RUNTIME_TYPE(LambertianKernel);
    LambertianImpl(): LambertianKernel(TYPE_INFO(), "bindless.lambertian") {}
    Execution execution() const override { return Execution::RASTER; }
    bool      initialize(AutoRef<gpu2::GpuContext> gpu) {
        return mShaders.initialize(gpu, kBindlessLambertianVertSpv, sizeof(kBindlessLambertianVertSpv), kBindlessLambertianFragSpv,
                                   sizeof(kBindlessLambertianFragSpv), kBindlessLambertianTextureVertSpv, sizeof(kBindlessLambertianTextureVertSpv));
    }
    bool record(gpu2::bindless::Raster & raster, AutoRef<SharedShaderConstants::UniformState> state, const Inputs & inputs) const override {
        return append(mShaders, raster, std::move(state), inputs, true, inputs.diffuseMultiplier, inputs.opaque, inputs.normalMap);
    }
};

} // namespace

gpu2::GpuResourceTable sharedUniformResources(const AutoRef<SharedShaderConstants::UniformState> & state) {
    if (!state) return {};
    auto view = state->view();
    if (!validUniform(view)) return {};
    gpu2::GpuResourceTable resources;
    resources.resize(2);
    resources[1].resize(2);
    resources[1][1].append(std::move(view));
    return resources;
}

AutoRef<UnlitKernel> UnlitKernel::create(AutoRef<gpu2::GpuContext> gpu) {
    auto impl = AutoRef<UnlitImpl>(new UnlitImpl());
    return impl->initialize(std::move(gpu)) ? AutoRef<UnlitKernel>(impl) : AutoRef<UnlitKernel>();
}

AutoRef<LambertianKernel> LambertianKernel::create(AutoRef<gpu2::GpuContext> gpu) {
    auto impl = AutoRef<LambertianImpl>(new LambertianImpl());
    return impl->initialize(std::move(gpu)) ? AutoRef<LambertianKernel>(impl) : AutoRef<LambertianKernel>();
}

} // namespace GN::fx2::bindless
