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
using Heap    = gpu2::bindless::DescriptorHeap;
auto * logger = getLogger("GN.fx2.bindless");

// Backend packing is independent of both user-facing material and draw parameters.
struct MaterialData {
    glm::vec4  color;
    glm::vec4  emissiveAndCutoff;
    glm::vec4  diffuseAndOpaque;
    glm::uvec4 textureIndices;
};
struct DrawConstants {
    glm::mat4  world;
    glm::uvec4 materialIndex;
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
static_assert(sizeof(MaterialData) == 64);
static_assert(offsetof(MaterialData, emissiveAndCutoff) == 16);
static_assert(offsetof(MaterialData, diffuseAndOpaque) == 32);
static_assert(offsetof(MaterialData, textureIndices) == 48);
static_assert(sizeof(DrawConstants) == 80);
static_assert(offsetof(DrawConstants, materialIndex) == 64);

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

template<class P>
bool validMaterial(const P & p) {
    bool valid = std::isfinite(p.alphaCutoff) && p.alphaCutoff >= 0 && p.alphaCutoff <= 1 && validMap(p.colorMap);
    for (int i = 0; i < 4; ++i) valid &= std::isfinite(p.color[i]);
    for (int i = 0; i < 3; ++i) valid &= std::isfinite(p.emissive[i]);
    return valid;
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
bool validDraw(const D & draw, const glm::mat4 & world, bool lit, bool textured) {
    const auto & g     = draw.geometry;
    bool         valid = attribute(g, 0) && (!lit || attribute(g, 1)) && (!textured || attribute(g, 2, 2)) &&
                         (g.indexCount ? g.indices.buffer && (g.indices.stride == 2 || g.indices.stride == 4) : g.vertexCount > 0);
    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r) valid &= std::isfinite(world[c][r]);
    if (lit) {
        const float  det   = glm::determinant(glm::mat3(world));
        valid &= std::isfinite(det) && std::abs(det) > 1e-20f && world[0][3] == 0 && world[1][3] == 0 && world[2][3] == 0 && world[3][3] == 1;
    }
    return valid;
}

struct Shaders {
    AutoRef<gpu2::GpuContext> gpu;
    AutoRef<gpu2::GpuShader>  vertex, texturedVertex, fragment;
    bool initialize(AutoRef<gpu2::GpuContext> context, const uint32_t * vs, size_t vsSize, const uint32_t * ps, size_t psSize, const uint32_t * texturedVs,
                    size_t texturedVsSize) {
        gpu = std::move(context);
        if (!gpu) return false;
        vertex         = gpu2::GpuShader::create({.context = gpu, .name = "bindless-simple.vert", .binary = vs, .size = vsSize});
        fragment       = gpu2::GpuShader::create({.context = gpu, .name = "bindless-simple.frag", .binary = ps, .size = psSize});
        texturedVertex = gpu2::GpuShader::create({.context = gpu, .name = "bindless-simple-texture.vert", .binary = texturedVs, .size = texturedVsSize});
        return vertex && texturedVertex && fragment;
    }
};

// One kernel owns this set for now. Slot sharing/caching can change without affecting
// the public defaults or material contract; retained kernels keep fallback slots alive.
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
    bool          initialize(AutoRef<Heap> heap, bool lit) {
        mHeap                 = std::move(heap);
        auto       gpu        = mHeap->gpu();
        const auto descriptor = gpu2::Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setLevels(1);
        auto       white      = gpu2::Texture::create("bindless.default-white", {.context = gpu, .descriptor = descriptor});
        sampler               = gpu2::Sampler::create("bindless.default-sampler", {.context = gpu});
        if (!white || !sampler) return false;
        color        = gpu2::GpuResourceView(white).setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        colorIndex   = mHeap->allocate(Heap::SAMPLED_TEXTURE, color);
        samplerIndex = mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView(sampler));
        if (colorIndex == Heap::INVALID_DESCRIPTOR_INDEX || samplerIndex == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        if (lit) {
            auto flat = gpu2::Texture::create("bindless.default-normal", {.context = gpu, .descriptor = descriptor});
            if (!flat) return false;
            normal      = gpu2::GpuResourceView(flat).setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
            normalIndex = mHeap->allocate(Heap::SAMPLED_TEXTURE, normal);
            if (normalIndex == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        }
        return true;
    }
    void upload(gpu2::bindless::CnC & producer) const {
        const uint8_t               white[] = {255, 255, 255, 255};
        const uint8_t               flat[]  = {128, 128, 255, 255};
        gpu2::bindless::CnC::Region region;
        region.imageExtent = {1, 1, 1};
        producer.recordUploadImage(color.texture(), {white, sizeof(white)}, {&region, 1});
        if (!normal.empty()) producer.recordUploadImage(normal.texture(), {flat, sizeof(flat)}, {&region, 1});
    }
};

// The owner frees custom slots/chunk only after all upload and draw references release.
// Default slots are borrowed from its retained kernel, never freed by an individual material.
class MaterialStorage : NoCopy {
    AutoRef<Heap>         mHeap;
    Heap::MaterialToken   mToken = Heap::INVALID_MATERIAL_TOKEN;
    Heap::DescriptorIndex mColor = {}, mNormal = {}, mSampler = {};
    bool                  mOwnColor = false, mOwnNormal = false, mOwnSampler = false, mNeedsUv = false;
    gpu2::GpuResourceView mView;
    MaterialData          mData  = {};
    uint32_t              mIndex = 0;

public:
    explicit MaterialStorage(AutoRef<Heap> heap): mHeap(std::move(heap)) {}
    ~MaterialStorage() {
        if (!mHeap) return;
        mHeap->freeMaterial(mToken);
        if (mOwnColor) mHeap->free(mColor);
        if (mOwnNormal) mHeap->free(mNormal);
        if (mOwnSampler) mHeap->free(mSampler);
    }
    bool initialize(MaterialData data, const gpu2::GpuResourceView & color, const gpu2::GpuResourceView & normal, AutoRef<gpu2::Sampler> sampler,
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
        mToken   = mHeap->allocateMaterial(sizeof(MaterialData), sizeof(MaterialData));
        if (mToken == Heap::INVALID_MATERIAL_TOKEN) return false;
        mView = mHeap->materialView(mToken);
        if (!mView.buffer() || mView.bufferView.size != sizeof(MaterialData) || mView.bufferView.offset % sizeof(MaterialData) ||
            mView.bufferView.offset / sizeof(MaterialData) > uint64_t(uint32_t(-1)))
            return false;
        mIndex = uint32_t(mView.bufferView.offset / sizeof(MaterialData));
        // Constant white/flat defaults work with zero UV; bypassing flat normal sampling
        // also avoids the small normal perturbation inherent in 8-bit UNORM encoding.
        data.textureIndices = glm::uvec4(mColor.slot, mNormal.slot, mSampler.slot, 1u | (mOwnNormal ? 2u : 0u));
        mData               = data;
        return true;
    }
    void upload(gpu2::bindless::CnC & producer) const {
        producer.recordUploadBuffer(mView.buffer(), mView.bufferView.offset, {reinterpret_cast<const uint8_t *>(&mData), sizeof(mData)});
    }
    uint32_t index() const { return mIndex; }
    bool     textured() const { return mNeedsUv; }
};

template<class D>
bool append(const Shaders & shaders, const MaterialStorage & storage, const D & inputs, bool lit, AutoRef<const RCRT64> owner) {
    auto &       raster = inputs.raster;
    auto         state  = inputs.ssc;
    if (!state || !validUniform(state->view()) || !validDraw(inputs, inputs.object2WorldTransform, lit, storage.textured())) GN_UNLIKELY {
            GN_ERROR(logger, "Material: invalid uniform state, geometry, or transform");
            return false;
        }
    const DrawConstants                    values {inputs.object2WorldTransform, glm::uvec4(storage.index(), 0, 0, 0)};
    gpu2::bindless::Raster::DrawParameters draw {.geometry = inputs.geometry};
    draw.vs         = storage.textured() ? shaders.texturedVertex : shaders.vertex;
    draw.ps         = shaders.fragment;
    draw.states     = inputs.states;
    draw.immediates = {reinterpret_cast<const uint8_t *>(&values), sizeof(values)};
    raster.retainResource(std::move(state));
    raster.retainResource(std::move(owner));
    raster.recordDraw(draw);
    return true;
}

class UnlitMaterialImpl : public UnlitMaterial {
    AutoRef<const UnlitKernel>            mKernel;
    const UnlitMaterial::Parameters        mParameters;
    const Shaders                         mShaders;
    MaterialStorage                       mStorage;

public:
    GN_REGISTER_RUNTIME_TYPE(UnlitMaterial);
    UnlitMaterialImpl(AutoRef<const UnlitKernel> kernel, const Shaders & shaders, AutoRef<Heap> heap, const UnlitMaterial::Parameters & p)
        : UnlitMaterial(TYPE_INFO(), "bindless.unlit.material"), mKernel(std::move(kernel)), mParameters(p), mShaders(shaders),
          mStorage(std::move(heap)) {}
    bool initialize(const Fallbacks & defaults) {
        return mStorage.initialize({mParameters.color, glm::vec4(mParameters.emissive, mParameters.alphaCutoff), glm::vec4(1, 0, 0, 0), {}},
                                   mParameters.colorMap, {}, mParameters.sampler, defaults);
    }
    void upload(gpu2::bindless::CnC & producer) const { mStorage.upload(producer); }
    bool record(const UnlitMaterial::DrawParameters & inputs) const override {
        return append(mShaders, mStorage, inputs, false, referenceTo(this));
    }
};

class LambertianMaterialImpl : public LambertianMaterial {
    AutoRef<const LambertianKernel>     mKernel;
    const LambertianMaterial::Parameters mParameters;
    const Shaders                        mShaders;
    MaterialStorage                      mStorage;

public:
    GN_REGISTER_RUNTIME_TYPE(LambertianMaterial);
    LambertianMaterialImpl(AutoRef<const LambertianKernel> kernel, const Shaders & shaders, AutoRef<Heap> heap, const LambertianMaterial::Parameters & p)
        : LambertianMaterial(TYPE_INFO(), "bindless.lambertian.material"), mKernel(std::move(kernel)), mParameters(p), mShaders(shaders),
          mStorage(std::move(heap)) {}
    bool initialize(const Fallbacks & defaults) {
        return mStorage.initialize({mParameters.color,
                                    glm::vec4(mParameters.emissive, mParameters.alphaCutoff),
                                    glm::vec4(mParameters.diffuseMultiplier, mParameters.opaque ? 1.f : 0.f, 0, 0),
                                    {}},
                                   mParameters.colorMap, mParameters.normalMap, mParameters.sampler, defaults);
    }
    void upload(gpu2::bindless::CnC & producer) const { mStorage.upload(producer); }
    bool record(const LambertianMaterial::DrawParameters & inputs) const override {
        return append(mShaders, mStorage, inputs, true, referenceTo(this));
    }
};

class UnlitImpl : public UnlitKernel {
    Shaders   mShaders;
    Fallbacks mDefaults;

public:
    GN_REGISTER_RUNTIME_TYPE(UnlitKernel);
    UnlitImpl(): UnlitKernel(TYPE_INFO(), "bindless.unlit") {}
    bool      initialize(AutoRef<Heap> heap) {
        if (!heap || heap->bindingIndex() != 0) return false;
        auto gpu = heap->gpu();
        return mShaders.initialize(gpu, kBindlessUnlitVertSpv, sizeof(kBindlessUnlitVertSpv), kBindlessUnlitFragSpv, sizeof(kBindlessUnlitFragSpv),
                                   kBindlessUnlitTextureVertSpv, sizeof(kBindlessUnlitTextureVertSpv)) &&
               mDefaults.initialize(std::move(heap), false);
    }
    void               uploadDefaults(gpu2::bindless::CnC & producer) const { mDefaults.upload(producer); }
    UnlitMaterial::Parameters defaultMaterialParameters() const override {
        UnlitMaterial::Parameters p;
        p.colorMap = mDefaults.color;
        p.sampler  = mDefaults.sampler;
        return p;
    }
    AutoRef<UnlitMaterial> createMaterial(gpu2::bindless::CnC & producer, const UnlitMaterial::Parameters & p) const override {
        if (!validMaterial(p)) return {};
        auto material = AutoRef<UnlitMaterialImpl>(new UnlitMaterialImpl(referenceTo(this), mShaders, mDefaults.heap(), p));
        if (!material->initialize(mDefaults)) return {};
        producer.retainResource(material);
        material->upload(producer);
        return material;
    }
};

class LambertianImpl : public LambertianKernel {
    Shaders   mShaders;
    Fallbacks mDefaults;

public:
    GN_REGISTER_RUNTIME_TYPE(LambertianKernel);
    LambertianImpl(): LambertianKernel(TYPE_INFO(), "bindless.lambertian") {}
    bool      initialize(AutoRef<Heap> heap) {
        if (!heap || heap->bindingIndex() != 0) return false;
        auto gpu = heap->gpu();
        return mShaders.initialize(gpu, kBindlessLambertianVertSpv, sizeof(kBindlessLambertianVertSpv), kBindlessLambertianFragSpv,
                                   sizeof(kBindlessLambertianFragSpv), kBindlessLambertianTextureVertSpv, sizeof(kBindlessLambertianTextureVertSpv)) &&
               mDefaults.initialize(std::move(heap), true);
    }
    void               uploadDefaults(gpu2::bindless::CnC & producer) const { mDefaults.upload(producer); }
    LambertianMaterial::Parameters defaultMaterialParameters() const override {
        LambertianMaterial::Parameters p;
        p.colorMap  = mDefaults.color;
        p.normalMap = mDefaults.normal;
        p.sampler   = mDefaults.sampler;
        return p;
    }
    AutoRef<LambertianMaterial> createMaterial(gpu2::bindless::CnC & producer, const LambertianMaterial::Parameters & p) const override {
        if (!validMaterial(p) || !validMap(p.normalMap) || !std::isfinite(p.diffuseMultiplier) || p.diffuseMultiplier < 0) return {};
        auto material = AutoRef<LambertianMaterialImpl>(new LambertianMaterialImpl(referenceTo(this), mShaders, mDefaults.heap(), p));
        if (!material->initialize(mDefaults)) return {};
        producer.retainResource(material);
        material->upload(producer);
        return material;
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

AutoRef<UnlitKernel> UnlitKernel::create(Heap & heap, gpu2::bindless::CnC & initialization) {
    auto impl = AutoRef<UnlitImpl>(new UnlitImpl());
    if (!impl->initialize(referenceTo(&heap))) return {};
    initialization.retainResource(impl);
    impl->uploadDefaults(initialization);
    return impl;
}
AutoRef<LambertianKernel> LambertianKernel::create(Heap & heap, gpu2::bindless::CnC & initialization) {
    auto impl = AutoRef<LambertianImpl>(new LambertianImpl());
    if (!impl->initialize(referenceTo(&heap))) return {};
    initialization.retainResource(impl);
    impl->uploadDefaults(initialization);
    return impl;
}

} // namespace GN::fx2::bindless
