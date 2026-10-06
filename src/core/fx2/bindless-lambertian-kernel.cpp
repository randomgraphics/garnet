#include "pch.h"
#include "bindless-lambertian-vert.spv.h"
#include "bindless-lambertian-texture-vert.spv.h"
#include "bindless-lambertian-frag.spv.h"
#include <glm/mat3x3.hpp>
#include <cmath>

namespace GN::fx2::bindless {
namespace {
using Heap    = gpu2::bindless::DescriptorHeap;
auto * logger = getLogger("GN.fx2.bindless.lambertian");

// 64-byte std430 packing matching bindless-lambertian-material.h
struct LambertianMaterialData {
    glm::vec4  color;
    glm::vec4  emissive;
    glm::vec4  diffuseParameters;
    glm::uvec4 textureIndices; // color, normal, sampler, flags
};
static_assert(sizeof(LambertianMaterialData) == 64);

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
    bool         valid = attribute(g, LambertianMaterial::POSITION_LOCATION) && attribute(g, LambertianMaterial::NORMAL_LOCATION) &&
                         (!textured || attribute(g, LambertianMaterial::TEXCOORD_LOCATION, 2)) &&
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
    bool initialize(AutoRef<gpu2::GpuContext> context, const uint32_t * vs, size_t vsSize, const uint32_t * ps, size_t psSize, const uint32_t * texturedVs,
                    size_t texturedVsSize) {
        gpu = std::move(context);
        if (!gpu) return false;
        vertex         = gpu2::GpuShader::create({.context = gpu, .name = "bindless-lambertian.vert", .binary = vs, .size = vsSize});
        fragment       = gpu2::GpuShader::create({.context = gpu, .name = "bindless-lambertian.frag", .binary = ps, .size = psSize});
        texturedVertex = gpu2::GpuShader::create({.context = gpu, .name = "bindless-lambertian-texture.vert", .binary = texturedVs, .size = texturedVsSize});
        return vertex && texturedVertex && fragment;
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
    bool          initialize(AutoRef<Heap> heap) {
        mHeap                 = std::move(heap);
        auto       gpu        = mHeap->gpu();
        const auto descriptor = gpu2::Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setLevels(1);
        auto       white      = gpu2::Texture::create("bindless.lambertian-default-white", {.context = gpu, .descriptor = descriptor});
        auto       flat       = gpu2::Texture::create("bindless.lambertian-default-normal", {.context = gpu, .descriptor = descriptor});
        sampler               = gpu2::Sampler::create("bindless.lambertian-default-sampler", {.context = gpu});
        if (!white || !flat || !sampler) return false;
        color        = gpu2::GpuResourceView {white}.setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        normal       = gpu2::GpuResourceView {flat}.setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        colorIndex   = mHeap->allocate(Heap::SAMPLED_TEXTURE, color);
        normalIndex  = mHeap->allocate(Heap::SAMPLED_TEXTURE, normal);
        samplerIndex = mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView {sampler});
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

class LambertianStorage : NoCopy {
    AutoRef<Heap>          mHeap;
    Heap::MaterialToken    mToken = Heap::INVALID_MATERIAL_TOKEN;
    Heap::DescriptorIndex  mColor = {}, mNormal = {}, mSampler = {};
    bool                   mOwnColor = false, mOwnNormal = false, mOwnSampler = false, mNeedsUv = false;
    gpu2::GpuResourceView  mView;
    LambertianMaterialData mData  = {};
    uint32_t               mIndex = 0;

public:
    explicit LambertianStorage(AutoRef<Heap> heap): mHeap(std::move(heap)) {}
    ~LambertianStorage() {
        if (!mHeap) return;
        mHeap->freeMaterial(mToken);
        if (mOwnColor) mHeap->free(mColor);
        if (mOwnNormal) mHeap->free(mNormal);
        if (mOwnSampler) mHeap->free(mSampler);
    }
    bool initialize(LambertianMaterialData data, const gpu2::GpuResourceView & color, const gpu2::GpuResourceView & normal, AutoRef<gpu2::Sampler> sampler,
                    const Fallbacks & defaults) {
        mOwnColor   = !color.empty() && color != defaults.color;
        mOwnNormal  = !normal.empty() && normal != defaults.normal;
        mOwnSampler = sampler && sampler.get() != defaults.sampler.get();
        mColor      = mOwnColor ? mHeap->allocate(Heap::SAMPLED_TEXTURE, color) : defaults.colorIndex;
        if (mColor == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        mNormal = mOwnNormal ? mHeap->allocate(Heap::SAMPLED_TEXTURE, normal) : defaults.normalIndex;
        if (mOwnNormal && mNormal == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        mSampler = mOwnSampler ? mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView {sampler}) : defaults.samplerIndex;
        if (mSampler == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        mNeedsUv = mOwnColor || mOwnNormal;
        mToken   = mHeap->allocateMaterial(sizeof(LambertianMaterialData), sizeof(LambertianMaterialData));
        if (mToken == Heap::INVALID_MATERIAL_TOKEN) return false;
        mView = mHeap->materialView(mToken);
        if (!mView.buffer() || mView.bufferView.size != sizeof(LambertianMaterialData) || mView.bufferView.offset % sizeof(LambertianMaterialData) ||
            mView.bufferView.offset / sizeof(LambertianMaterialData) > uint64_t(uint32_t(-1)))
            return false;
        mIndex              = uint32_t(mView.bufferView.offset / sizeof(LambertianMaterialData));
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

class LambertianMaterialImpl : public LambertianMaterial {
    AutoRef<const LambertianKernel>      mKernel;
    const LambertianMaterial::Parameters mParameters;
    const Shaders                        mShaders;
    LambertianStorage                    mStorage;

public:
    GN_REGISTER_RUNTIME_TYPE(LambertianMaterial);
    LambertianMaterialImpl(AutoRef<const LambertianKernel> kernel, const Shaders & shaders, AutoRef<Heap> heap, const LambertianMaterial::Parameters & p)
        : LambertianMaterial(TYPE_INFO(), "bindless.lambertian.material"), mKernel(std::move(kernel)), mParameters(p), mShaders(shaders),
          mStorage(std::move(heap)) {}
    bool initialize(const Fallbacks & defaults) {
        return mStorage.initialize({mParameters.color, glm::vec4(mParameters.emissive, 0), glm::vec4(mParameters.diffuseMultiplier, 0, 0, 0), {}},
                                   mParameters.colorMap, mParameters.normalMap, mParameters.sampler, defaults);
    }
    void upload(gpu2::bindless::CnC & producer) const { mStorage.upload(producer); }
    bool record(const LambertianMaterial::DrawParameters & inputs) const override {
        auto & raster = inputs.raster;
        auto   state  = inputs.ssc;
        if (!state || !validUniform(state->view()) || !validDraw(inputs, inputs.object2WorldTransform, mStorage.textured())) GN_UNLIKELY {
                GN_ERROR(logger, "LambertianMaterial: invalid uniform state, geometry, or transform");
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
        return true;
    }
};

class LambertianImpl : public LambertianKernel {
    Shaders   mShaders;
    Fallbacks mDefaults;

public:
    GN_REGISTER_RUNTIME_TYPE(LambertianKernel);
    LambertianImpl(): LambertianKernel(TYPE_INFO(), "bindless.lambertian") {}
    bool initialize(AutoRef<Heap> heap) {
        if (!heap || heap->bindingIndex() != 0) return false;
        auto gpu = heap->gpu();
        return mShaders.initialize(gpu, kBindlessLambertianVertSpv, sizeof(kBindlessLambertianVertSpv), kBindlessLambertianFragSpv,
                                   sizeof(kBindlessLambertianFragSpv), kBindlessLambertianTextureVertSpv, sizeof(kBindlessLambertianTextureVertSpv)) &&
               mDefaults.initialize(std::move(heap));
    }
    void                           uploadDefaults(gpu2::bindless::CnC & producer) const { mDefaults.upload(producer); }
    LambertianMaterial::Parameters defaultMaterialParameters() const override {
        LambertianMaterial::Parameters p;
        p.colorMap  = mDefaults.color;
        p.normalMap = mDefaults.normal;
        p.sampler   = mDefaults.sampler;
        return p;
    }
    AutoRef<LambertianMaterial> createMaterial(gpu2::bindless::CnC & producer, const LambertianMaterial::Parameters & p) const override {
        if (!validMap(p.colorMap) || !validMap(p.normalMap) || !std::isfinite(p.diffuseMultiplier) || p.diffuseMultiplier < 0) return {};
        for (int i = 0; i < 4; ++i)
            if (!std::isfinite(p.color[i])) return {};
        for (int i = 0; i < 3; ++i)
            if (!std::isfinite(p.emissive[i])) return {};
        auto material = AutoRef<LambertianMaterialImpl>(new LambertianMaterialImpl(referenceTo(this), mShaders, mDefaults.heap(), p));
        if (!material->initialize(mDefaults)) return {};
        producer.retainResource(material);
        material->upload(producer);
        return material;
    }
};

} // namespace

AutoRef<LambertianKernel> LambertianKernel::create(Heap & heap, gpu2::bindless::CnC & initialization) {
    auto impl = AutoRef<LambertianImpl>(new LambertianImpl());
    if (!impl->initialize(referenceTo(&heap))) return {};
    initialization.retainResource(impl);
    impl->uploadDefaults(initialization);
    return impl;
}

} // namespace GN::fx2::bindless
