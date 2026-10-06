#include "pch.h"
#include "bindless-cel-vert.spv.h"
#include "bindless-cel-texture-vert.spv.h"
#include "bindless-cel-frag.spv.h"
#include "bindless-cel-outline-vert.spv.h"
#include "bindless-cel-outline-texture-vert.spv.h"
#include "bindless-cel-outline-frag.spv.h"
#include <glm/mat3x3.hpp>
#include <cmath>

namespace GN::fx2::bindless {
namespace {
using Heap    = gpu2::bindless::DescriptorHeap;
auto * logger = getLogger("GN.fx2.bindless.cel");

// 176-byte std430 packing matching bindless-cel-material.h
struct CelMaterialData {
    glm::vec4  baseColor;
    glm::vec4  emissive;
    glm::vec4  shadowParams;   // x = shadowThreshold, y = shadowFeather, z = deepShadowThreshold, w = deepShadowFeather
    glm::vec4  shadowTint;     // rgb = shadowTint, a = unused
    glm::vec4  deepShadowTint; // rgb = deepShadowTint, a = unused
    glm::vec4  specularParams; // x = specularThreshold, y = specularShininess, z = specularIntensity, w = unused
    glm::vec4  rimParams;      // x = rimThreshold, y = rimFeather, z = rimIntensity, w = unused
    glm::vec4  rimTint;        // rgb = rimTint, a = unused
    glm::vec4  outlineParams;  // x = outlineWidth, yzw = unused
    glm::vec4  outlineColor;   // rgba = outlineColor
    glm::uvec4 textureIndices; // x = colorIndex, y = normalIndex, z = samplerIndex, w = flags (1: color, 2: normal)
};
static_assert(sizeof(CelMaterialData) == 176);

struct DrawConstants {
    glm::mat4  world;
    glm::uvec4 materialIndex;
};
static_assert(sizeof(DrawConstants) == 80);

bool validUniform(const gpu2::GpuResourceView & view) {
    return view.buffer() && view.bufferView.type == gpu2::GpuResourceView::BufferView::UNIFORM && view.bufferView.size >= sizeof(SharedUniforms);
}

bool validMap(const gpu2::GpuResourceView & view) {
    if (view.empty()) return true;
    auto texture = view.texture();
    if (!texture || view.combinedTextureSampler || view.imageView.type != gpu2::GpuResourceView::ImageView::SAMPLED) return false;
    const auto & d = texture->descriptor();
    const auto & r = view.imageView.range;
    return d.faces == 1 && d.depth == 1 && r.i.face == 0 && r.i.mip < d.levels && (r.e.numArrayLayers == 1 || r.e.numArrayLayers == uint32_t(-1)) &&
           (r.e.numMipLevels == uint32_t(-1) || (r.e.numMipLevels > 0 && r.e.numMipLevels <= d.levels - r.i.mip));
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

template<class D>
bool validDraw(const D & draw, const glm::mat4 & world, bool textured) {
    const auto & g     = draw.geometry;
    bool         valid = attribute(g, CelMaterial::POSITION_LOCATION) && attribute(g, CelMaterial::NORMAL_LOCATION) &&
                         (!textured || attribute(g, CelMaterial::TEXCOORD_LOCATION, 2)) &&
                         (g.indexCount ? g.indices.buffer && (g.indices.stride == 2 || g.indices.stride == 4) : g.vertexCount > 0);
    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r) valid &= std::isfinite(world[c][r]);
    const float det = glm::determinant(glm::mat3(world));
    valid &= std::isfinite(det) && std::abs(det) > 1e-20f && world[0][3] == 0 && world[1][3] == 0 && world[2][3] == 0 && world[3][3] == 1;
    return valid;
}

struct Shaders {
    AutoRef<gpu2::GpuContext> gpu;
    AutoRef<gpu2::GpuShader>  vertex, texturedVertex, fragment;
    AutoRef<gpu2::GpuShader>  outlineVertex, texturedOutlineVertex, outlineFragment;

    bool initialize(AutoRef<gpu2::GpuContext> context, const uint32_t * vs, size_t vsSize, const uint32_t * ps, size_t psSize, const uint32_t * texturedVs,
                    size_t texturedVsSize, const uint32_t * ovs, size_t ovsSize, const uint32_t * ops, size_t opsSize, const uint32_t * texturedOvs,
                    size_t texturedOvsSize) {
        gpu = std::move(context);
        if (!gpu) return false;
        vertex                = gpu2::GpuShader::create({.context = gpu, .name = "bindless-cel.vert", .binary = vs, .size = vsSize});
        fragment              = gpu2::GpuShader::create({.context = gpu, .name = "bindless-cel.frag", .binary = ps, .size = psSize});
        texturedVertex        = gpu2::GpuShader::create({.context = gpu, .name = "bindless-cel-texture.vert", .binary = texturedVs, .size = texturedVsSize});
        outlineVertex         = gpu2::GpuShader::create({.context = gpu, .name = "bindless-cel-outline.vert", .binary = ovs, .size = ovsSize});
        outlineFragment       = gpu2::GpuShader::create({.context = gpu, .name = "bindless-cel-outline.frag", .binary = ops, .size = opsSize});
        texturedOutlineVertex = gpu2::GpuShader::create({.context = gpu, .name = "bindless-cel-outline-texture.vert", .binary = texturedOvs, .size = texturedOvsSize});
        return vertex && texturedVertex && fragment && outlineVertex && texturedOutlineVertex && outlineFragment;
    }
};

class Fallbacks : NoCopy {
    AutoRef<Heap> mHeap;

public:
    gpu2::GpuResourceView  color, normal;
    AutoRef<gpu2::Sampler> sampler;
    Heap::DescriptorIndex  colorIndex = {}, normalIndex = {}, samplerIndex = {};

    ~Fallbacks() {
        if (!mHeap) return;
        mHeap->free(colorIndex);
        mHeap->free(normalIndex);
        mHeap->free(samplerIndex);
    }

    AutoRef<Heap> heap() const { return mHeap; }

    bool initialize(AutoRef<Heap> heap) {
        mHeap                 = std::move(heap);
        auto       gpu        = mHeap->gpu();
        const auto descriptor = gpu2::Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setLevels(1);
        auto       white      = gpu2::Texture::create("bindless.cel-default-white", {.context = gpu, .descriptor = descriptor});
        auto       flat       = gpu2::Texture::create("bindless.cel-default-normal", {.context = gpu, .descriptor = descriptor});
        sampler               = gpu2::Sampler::create("bindless.cel-default-sampler", {.context = gpu});
        if (!white || !flat || !sampler) return false;
        color        = gpu2::GpuResourceView(white).setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        normal       = gpu2::GpuResourceView(flat).setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        colorIndex   = mHeap->allocate(Heap::SAMPLED_TEXTURE, color);
        normalIndex  = mHeap->allocate(Heap::SAMPLED_TEXTURE, normal);
        samplerIndex = mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView(sampler));
        return colorIndex != Heap::INVALID_DESCRIPTOR_INDEX && normalIndex != Heap::INVALID_DESCRIPTOR_INDEX && samplerIndex != Heap::INVALID_DESCRIPTOR_INDEX;
    }

    void upload(gpu2::bindless::CnC & producer) const {
        const uint8_t               white[] = {255, 255, 255, 255};
        const uint8_t               flat[]  = {128, 128, 255, 255};
        gpu2::bindless::CnC::Region region;
        region.imageExtent = {1, 1, 1};
        producer.recordUploadImage(color.texture(), {white, sizeof(white)}, {&region, 1});
        producer.recordUploadImage(normal.texture(), {flat, sizeof(flat)}, {&region, 1});
    }
};

class CelStorage : NoCopy {
    AutoRef<Heap>         mHeap;
    Heap::MaterialToken   mToken = Heap::INVALID_MATERIAL_TOKEN;
    Heap::DescriptorIndex mColor = {}, mNormal = {}, mSampler = {};
    bool                  mOwnColor = false, mOwnNormal = false, mOwnSampler = false, mNeedsUv = false;
    gpu2::GpuResourceView mView;
    CelMaterialData       mData  = {};
    uint32_t              mIndex = 0;

public:
    explicit CelStorage(AutoRef<Heap> heap): mHeap(std::move(heap)) {}

    ~CelStorage() {
        if (!mHeap) return;
        mHeap->freeMaterial(mToken);
        if (mOwnColor) mHeap->free(mColor);
        if (mOwnNormal) mHeap->free(mNormal);
        if (mOwnSampler) mHeap->free(mSampler);
    }

    bool initialize(CelMaterialData data, const gpu2::GpuResourceView & color, const gpu2::GpuResourceView & normal, AutoRef<gpu2::Sampler> sampler,
                    const Fallbacks & defaults) {
        mOwnColor   = !color.empty() && color != defaults.color;
        mOwnNormal  = !normal.empty() && normal != defaults.normal;
        mOwnSampler = sampler && sampler.get() != defaults.sampler.get();
        mColor      = mOwnColor ? mHeap->allocate(Heap::SAMPLED_TEXTURE, color) : defaults.colorIndex;
        if (mColor == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        mNormal = mOwnNormal ? mHeap->allocate(Heap::SAMPLED_TEXTURE, normal) : defaults.normalIndex;
        if (mOwnNormal && mNormal == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        mSampler = mOwnSampler ? mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView(sampler)) : defaults.samplerIndex;
        if (mSampler == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        mNeedsUv = mOwnColor || mOwnNormal;
        mToken   = mHeap->allocateMaterial(sizeof(CelMaterialData), sizeof(CelMaterialData));
        if (mToken == Heap::INVALID_MATERIAL_TOKEN) return false;
        mView = mHeap->materialView(mToken);
        if (!mView.buffer() || mView.bufferView.size != sizeof(CelMaterialData) || mView.bufferView.offset % sizeof(CelMaterialData) ||
            mView.bufferView.offset / sizeof(CelMaterialData) > uint64_t(uint32_t(-1)))
            return false;
        mIndex              = uint32_t(mView.bufferView.offset / sizeof(CelMaterialData));
        data.textureIndices = glm::uvec4(mColor.slot, mNormal.slot, mSampler.slot, (mOwnColor ? 1u : 0u) | (mOwnNormal ? 2u : 0u));
        mData               = data;
        return true;
    }

    void upload(gpu2::bindless::CnC & producer) const {
        producer.recordUploadBuffer(mView.buffer(), mView.bufferView.offset, {reinterpret_cast<const uint8_t *>(&mData), sizeof(mData)});
    }

    uint32_t index() const { return mIndex; }
    bool     textured() const { return mNeedsUv; }
};

class CelMaterialImpl : public CelMaterial {
    AutoRef<const CelKernel>  mKernel;
    const CelMaterial::Parameters mParameters;
    const Shaders             mShaders;
    CelStorage                mStorage;

public:
    GN_REGISTER_RUNTIME_TYPE(CelMaterial);

    CelMaterialImpl(AutoRef<const CelKernel> kernel, const Shaders & shaders, AutoRef<Heap> heap, const CelMaterial::Parameters & p)
        : CelMaterial(TYPE_INFO(), "bindless.cel.material"), mKernel(std::move(kernel)), mParameters(p), mShaders(shaders),
          mStorage(std::move(heap)) {}

    bool initialize(const Fallbacks & defaults) {
        CelMaterialData data {};
        data.baseColor      = mParameters.color;
        data.emissive       = glm::vec4(mParameters.emissive, 0.0f);
        data.shadowParams   = glm::vec4(mParameters.shadowThreshold, mParameters.shadowFeather, mParameters.deepShadowThreshold, mParameters.deepShadowFeather);
        data.shadowTint     = glm::vec4(mParameters.shadowTint, 0.0f);
        data.deepShadowTint = glm::vec4(mParameters.deepShadowTint, 0.0f);
        data.specularParams = glm::vec4(mParameters.specularThreshold, mParameters.specularShininess, mParameters.specularIntensity, 0.0f);
        data.rimParams      = glm::vec4(mParameters.rimThreshold, mParameters.rimFeather, mParameters.rimIntensity, 0.0f);
        data.rimTint        = glm::vec4(mParameters.rimTint, 0.0f);
        data.outlineParams  = glm::vec4(mParameters.outlineWidth, 0.0f, 0.0f, 0.0f);
        data.outlineColor   = mParameters.outlineColor;

        return mStorage.initialize(data, mParameters.colorMap, mParameters.normalMap, mParameters.sampler, defaults);
    }

    void upload(gpu2::bindless::CnC & producer) const { mStorage.upload(producer); }

    bool record(const CelMaterial::DrawParameters & inputs) const override {
        auto & raster = inputs.raster;
        auto   state  = inputs.ssc;
        if (!state || !validUniform(state->view()) || !validDraw(inputs, inputs.object2WorldTransform, mStorage.textured())) GN_UNLIKELY {
            GN_ERROR(logger, "CelMaterial: invalid uniform state, geometry, or transform");
            return false;
        }

        const DrawConstants                    values {inputs.object2WorldTransform, glm::uvec4(mStorage.index(), 0, 0, 0)};
        gpu2::bindless::Raster::DrawParameters draw {.geometry = inputs.geometry};
        draw.vs = mStorage.textured() ? mShaders.texturedVertex : mShaders.vertex;
        draw.ps = mShaders.fragment;
        if (inputs.states) draw.states = *inputs.states;
        draw.immediates = {reinterpret_cast<const uint8_t *>(&values), sizeof(values)};
        raster.retainResource(std::move(state));
        raster.retainResource(referenceTo(this));
        raster.recordDraw(draw);

        if (inputs.renderOutline && mParameters.outlineWidth > 0.0f) {
            gpu2::bindless::Raster::DrawParameters outlineDraw {.geometry = inputs.geometry};
            outlineDraw.vs = mStorage.textured() ? mShaders.texturedOutlineVertex : mShaders.outlineVertex;
            outlineDraw.ps = mShaders.outlineFragment;
            if (inputs.states) outlineDraw.states = *inputs.states;
            outlineDraw.states.cullMode   = gpu2::RasterState::CULL_FRONT;
            outlineDraw.states.depthState = gpu2::RasterState::DepthState {gpu2::RasterState::Compare::LESS_EQUAL, true};
            outlineDraw.immediates        = {reinterpret_cast<const uint8_t *>(&values), sizeof(values)};
            raster.recordDraw(outlineDraw);
        }

        return true;
    }
};

class CelImpl : public CelKernel {
    Shaders   mShaders;
    Fallbacks mDefaults;

public:
    GN_REGISTER_RUNTIME_TYPE(CelKernel);

    CelImpl(): CelKernel(TYPE_INFO(), "bindless.cel") {}

    bool initialize(AutoRef<Heap> heap) {
        if (!heap || heap->bindingIndex() != 0) return false;
        auto gpu = heap->gpu();
        return mShaders.initialize(gpu, kBindlessCelVertSpv, sizeof(kBindlessCelVertSpv), kBindlessCelFragSpv, sizeof(kBindlessCelFragSpv),
                                   kBindlessCelTextureVertSpv, sizeof(kBindlessCelTextureVertSpv), kBindlessCelOutlineVertSpv,
                                   sizeof(kBindlessCelOutlineVertSpv), kBindlessCelOutlineFragSpv, sizeof(kBindlessCelOutlineFragSpv),
                                   kBindlessCelOutlineTextureVertSpv, sizeof(kBindlessCelOutlineTextureVertSpv)) &&
               mDefaults.initialize(std::move(heap));
    }

    void uploadDefaults(gpu2::bindless::CnC & producer) const { mDefaults.upload(producer); }

    CelMaterial::Parameters defaultMaterialParameters() const override {
        CelMaterial::Parameters p;
        p.colorMap  = mDefaults.color;
        p.normalMap = mDefaults.normal;
        p.sampler   = mDefaults.sampler;
        return p;
    }

    AutoRef<CelMaterial> createMaterial(gpu2::bindless::CnC & producer, const CelMaterial::Parameters & p) const override {
        if (!validMap(p.colorMap) || !validMap(p.normalMap)) return {};
        for (int i = 0; i < 4; ++i) {
            if (!std::isfinite(p.color[i]) || !std::isfinite(p.outlineColor[i])) return {};
        }
        for (int i = 0; i < 3; ++i) {
            if (!std::isfinite(p.emissive[i]) || !std::isfinite(p.shadowTint[i]) || !std::isfinite(p.deepShadowTint[i]) || !std::isfinite(p.rimTint[i]))
                return {};
        }
        if (!std::isfinite(p.shadowThreshold) || !std::isfinite(p.shadowFeather) || !std::isfinite(p.deepShadowThreshold) ||
            !std::isfinite(p.deepShadowFeather) || !std::isfinite(p.specularThreshold) || !std::isfinite(p.specularShininess) ||
            !std::isfinite(p.specularIntensity) || !std::isfinite(p.rimThreshold) || !std::isfinite(p.rimFeather) || !std::isfinite(p.rimIntensity) ||
            !std::isfinite(p.outlineWidth))
            return {};

        auto material = AutoRef<CelMaterialImpl>(new CelMaterialImpl(referenceTo(this), mShaders, mDefaults.heap(), p));
        if (!material->initialize(mDefaults)) return {};
        producer.retainResource(material);
        material->upload(producer);
        return material;
    }
};

} // namespace

AutoRef<CelKernel> CelKernel::create(Heap & heap, gpu2::bindless::CnC & initialization) {
    auto impl = AutoRef<CelImpl>(new CelImpl());
    if (!impl->initialize(referenceTo(&heap))) return {};
    initialization.retainResource(impl);
    impl->uploadDefaults(initialization);
    return impl;
}

} // namespace GN::fx2::bindless
