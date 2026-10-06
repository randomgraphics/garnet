#include "pch.h"
#include "bindless-unlit-vert.spv.h"
#include "bindless-unlit-texture-vert.spv.h"
#include "bindless-unlit-frag.spv.h"
#include <glm/mat3x3.hpp>
#include <cmath>

namespace GN::fx2::bindless {
namespace {
using Heap    = gpu2::bindless::DescriptorHeap;
auto * logger = getLogger("GN.fx2.bindless.unlit");

// 64-byte std430 packing matching bindless-unlit-material.h
struct UnlitMaterialData {
    glm::vec4  color;
    glm::vec4  emissive;
    glm::vec4  unused;
    glm::uvec4 textureIndices; // color, unused, sampler, hasTexture
};
static_assert(sizeof(UnlitMaterialData) == 64);

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
    bool         valid = attribute(g, UnlitMaterial::POSITION_LOCATION) && (!textured || attribute(g, UnlitMaterial::TEXCOORD_LOCATION, 2)) &&
                         (g.indexCount ? g.indices.buffer && (g.indices.stride == 2 || g.indices.stride == 4) : g.vertexCount > 0);
    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r) valid &= std::isfinite(world[c][r]);
    return valid;
}

struct Shaders {
    AutoRef<gpu2::GpuContext> gpu;
    AutoRef<gpu2::GpuShader>  vertex, texturedVertex, fragment;
    bool initialize(AutoRef<gpu2::GpuContext> context, const uint32_t * vs, size_t vsSize, const uint32_t * ps, size_t psSize, const uint32_t * texturedVs,
                    size_t texturedVsSize) {
        gpu = std::move(context);
        if (!gpu) return false;
        vertex         = gpu2::GpuShader::create({.context = gpu, .name = "bindless-unlit.vert", .binary = vs, .size = vsSize});
        fragment       = gpu2::GpuShader::create({.context = gpu, .name = "bindless-unlit.frag", .binary = ps, .size = psSize});
        texturedVertex = gpu2::GpuShader::create({.context = gpu, .name = "bindless-unlit-texture.vert", .binary = texturedVs, .size = texturedVsSize});
        return vertex && texturedVertex && fragment;
    }
};

class Fallbacks : NoCopy {
    AutoRef<Heap> mHeap;

public:
    gpu2::GpuResourceView  color;
    AutoRef<gpu2::Sampler> sampler;
    Heap::DescriptorIndex  colorIndex = {}, samplerIndex = {};
    ~Fallbacks() {
        if (!mHeap) return;
        mHeap->free(colorIndex);
        mHeap->free(samplerIndex);
    }
    AutoRef<Heap> heap() const { return mHeap; }
    bool          initialize(AutoRef<Heap> heap) {
        mHeap                 = std::move(heap);
        auto       gpu        = mHeap->gpu();
        const auto descriptor = gpu2::Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setLevels(1);
        auto       white      = gpu2::Texture::create("bindless.unlit-default-white", {.context = gpu, .descriptor = descriptor});
        sampler               = gpu2::Sampler::create("bindless.unlit-default-sampler", {.context = gpu});
        if (!white || !sampler) return false;
        color        = gpu2::GpuResourceView(white).setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        colorIndex   = mHeap->allocate(Heap::SAMPLED_TEXTURE, color);
        samplerIndex = mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView(sampler));
        return colorIndex != Heap::INVALID_DESCRIPTOR_INDEX && samplerIndex != Heap::INVALID_DESCRIPTOR_INDEX;
    }
    void upload(gpu2::bindless::CnC & producer) const {
        const uint8_t               white[] = {255, 255, 255, 255};
        gpu2::bindless::CnC::Region region;
        region.imageExtent = {1, 1, 1};
        producer.recordUploadImage(color.texture(), {white, sizeof(white)}, {&region, 1});
    }
};

class UnlitStorage : NoCopy {
    AutoRef<Heap>         mHeap;
    Heap::MaterialToken   mToken = Heap::INVALID_MATERIAL_TOKEN;
    Heap::DescriptorIndex mColor = {}, mSampler = {};
    bool                  mOwnColor = false, mOwnSampler = false, mNeedsUv = false;
    gpu2::GpuResourceView mView;
    UnlitMaterialData     mData  = {};
    uint32_t              mIndex = 0;

public:
    explicit UnlitStorage(AutoRef<Heap> heap): mHeap(std::move(heap)) {}
    ~UnlitStorage() {
        if (!mHeap) return;
        mHeap->freeMaterial(mToken);
        if (mOwnColor) mHeap->free(mColor);
        if (mOwnSampler) mHeap->free(mSampler);
    }
    bool initialize(UnlitMaterialData data, const gpu2::GpuResourceView & color, AutoRef<gpu2::Sampler> sampler, const Fallbacks & defaults) {
        mOwnColor   = !color.empty() && color != defaults.color;
        mOwnSampler = sampler && sampler.get() != defaults.sampler.get();
        mColor      = mOwnColor ? mHeap->allocate(Heap::SAMPLED_TEXTURE, color) : defaults.colorIndex;
        if (mColor == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        mSampler = mOwnSampler ? mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView(sampler)) : defaults.samplerIndex;
        if (mSampler == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        mNeedsUv = mOwnColor;
        mToken   = mHeap->allocateMaterial(sizeof(UnlitMaterialData), sizeof(UnlitMaterialData));
        if (mToken == Heap::INVALID_MATERIAL_TOKEN) return false;
        mView = mHeap->materialView(mToken);
        if (!mView.buffer() || mView.bufferView.size != sizeof(UnlitMaterialData) || mView.bufferView.offset % sizeof(UnlitMaterialData) ||
            mView.bufferView.offset / sizeof(UnlitMaterialData) > uint64_t(uint32_t(-1)))
            return false;
        mIndex              = uint32_t(mView.bufferView.offset / sizeof(UnlitMaterialData));
        data.textureIndices = glm::uvec4(mColor.slot, 0, mSampler.slot, mOwnColor ? 1u : 0u);
        mData               = data;
        return true;
    }
    void upload(gpu2::bindless::CnC & producer) const {
        producer.recordUploadBuffer(mView.buffer(), mView.bufferView.offset, {reinterpret_cast<const uint8_t *>(&mData), sizeof(mData)});
    }
    uint32_t index() const { return mIndex; }
    bool     textured() const { return mNeedsUv; }
};

class UnlitMaterialImpl : public UnlitMaterial {
    AutoRef<const UnlitKernel>      mKernel;
    const UnlitMaterial::Parameters mParameters;
    const Shaders                   mShaders;
    UnlitStorage                    mStorage;

public:
    GN_REGISTER_RUNTIME_TYPE(UnlitMaterial);
    UnlitMaterialImpl(AutoRef<const UnlitKernel> kernel, const Shaders & shaders, AutoRef<Heap> heap, const UnlitMaterial::Parameters & p)
        : UnlitMaterial(TYPE_INFO(), "bindless.unlit.material"), mKernel(std::move(kernel)), mParameters(p), mShaders(shaders), mStorage(std::move(heap)) {}
    bool initialize(const Fallbacks & defaults) {
        return mStorage.initialize({mParameters.color, glm::vec4(mParameters.emissive, 0), {}, {}}, mParameters.colorMap, mParameters.sampler, defaults);
    }
    void upload(gpu2::bindless::CnC & producer) const { mStorage.upload(producer); }
    bool record(const UnlitMaterial::DrawParameters & inputs) const override {
        auto & raster = inputs.raster;
        auto   state  = inputs.ssc;
        if (!state || !validUniform(state->view()) || !validDraw(inputs, inputs.object2WorldTransform, mStorage.textured())) GN_UNLIKELY {
                GN_ERROR(logger, "UnlitMaterial: invalid uniform state, geometry, or transform");
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

class UnlitImpl : public UnlitKernel {
    Shaders   mShaders;
    Fallbacks mDefaults;

public:
    GN_REGISTER_RUNTIME_TYPE(UnlitKernel);
    UnlitImpl(): UnlitKernel(TYPE_INFO(), "bindless.unlit") {}
    bool initialize(AutoRef<Heap> heap) {
        if (!heap || heap->bindingIndex() != 0) return false;
        auto gpu = heap->gpu();
        return mShaders.initialize(gpu, kBindlessUnlitVertSpv, sizeof(kBindlessUnlitVertSpv), kBindlessUnlitFragSpv, sizeof(kBindlessUnlitFragSpv),
                                   kBindlessUnlitTextureVertSpv, sizeof(kBindlessUnlitTextureVertSpv)) &&
               mDefaults.initialize(std::move(heap));
    }
    void                      uploadDefaults(gpu2::bindless::CnC & producer) const { mDefaults.upload(producer); }
    UnlitMaterial::Parameters defaultMaterialParameters() const override {
        UnlitMaterial::Parameters p;
        p.colorMap = mDefaults.color;
        p.sampler  = mDefaults.sampler;
        return p;
    }
    AutoRef<UnlitMaterial> createMaterial(gpu2::bindless::CnC & producer, const UnlitMaterial::Parameters & p) const override {
        if (!validMap(p.colorMap)) return {};
        for (int i = 0; i < 4; ++i)
            if (!std::isfinite(p.color[i])) return {};
        for (int i = 0; i < 3; ++i)
            if (!std::isfinite(p.emissive[i])) return {};
        auto material = AutoRef<UnlitMaterialImpl>(new UnlitMaterialImpl(referenceTo(this), mShaders, mDefaults.heap(), p));
        if (!material->initialize(mDefaults)) return {};
        producer.retainResource(material);
        material->upload(producer);
        return material;
    }
};

} // namespace

AutoRef<UnlitKernel> UnlitKernel::create(Heap & heap, gpu2::bindless::CnC & initialization) {
    auto impl = AutoRef<UnlitImpl>(new UnlitImpl());
    if (!impl->initialize(referenceTo(&heap))) return {};
    initialization.retainResource(impl);
    impl->uploadDefaults(initialization);
    return impl;
}

} // namespace GN::fx2::bindless
