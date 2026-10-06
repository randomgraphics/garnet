#include "pch.h"
#include "bindless-pbr-vert.spv.h"
#include "bindless-pbr-frag.spv.h"
#include <glm/mat3x3.hpp>
#include <cmath>

namespace GN::fx2::bindless {
namespace {
using Heap    = gpu2::bindless::DescriptorHeap;
auto * logger = getLogger("GN.fx2.bindless.pbr");

struct PbrMaterialData {
    glm::vec4  baseColor;
    glm::vec4  emissive;
    glm::vec4  factors;
    glm::uvec4 textureIndices;
    glm::uvec4 extraIndices;
};
static_assert(sizeof(PbrMaterialData) == 80);

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
bool validDraw(const D & draw, const glm::mat4 & world) {
    const auto & g     = draw.geometry;
    bool         valid = attribute(g, PbrMaterial::POSITION_LOCATION) && attribute(g, PbrMaterial::NORMAL_LOCATION) &&
                         attribute(g, PbrMaterial::TEXCOORD_LOCATION, 2) &&
                         (g.indexCount ? g.indices.buffer && (g.indices.stride == 2 || g.indices.stride == 4) : g.vertexCount > 0);
    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r) valid &= std::isfinite(world[c][r]);
    const float det = glm::determinant(glm::mat3(world));
    valid &= std::isfinite(det) && std::abs(det) > 1e-20f && world[0][3] == 0 && world[1][3] == 0 && world[2][3] == 0 && world[3][3] == 1;
    return valid;
}

struct Shaders {
    AutoRef<gpu2::GpuContext> gpu;
    AutoRef<gpu2::GpuShader>  vertex, fragment;
    bool initialize(AutoRef<gpu2::GpuContext> context, const uint32_t * vs, size_t vsSize, const uint32_t * ps, size_t psSize) {
        gpu = std::move(context);
        if (!gpu) return false;
        vertex   = gpu2::GpuShader::create({.context = gpu, .name = "bindless-pbr.vert", .binary = vs, .size = vsSize});
        fragment = gpu2::GpuShader::create({.context = gpu, .name = "bindless-pbr.frag", .binary = ps, .size = psSize});
        return vertex && fragment;
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
        auto       white      = gpu2::Texture::create("bindless.pbr-default-white", {.context = gpu, .descriptor = descriptor});
        auto       flat       = gpu2::Texture::create("bindless.pbr-default-normal", {.context = gpu, .descriptor = descriptor});
        sampler               = gpu2::Sampler::create("bindless.pbr-default-sampler", {.context = gpu});
        if (!white || !flat || !sampler) return false;
        color        = gpu2::GpuResourceView(white).setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        normal       = gpu2::GpuResourceView(flat).setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        colorIndex   = mHeap->allocate(Heap::SAMPLED_TEXTURE, color);
        normalIndex  = mHeap->allocate(Heap::SAMPLED_TEXTURE, normal);
        samplerIndex = mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView(sampler));
        return colorIndex != Heap::INVALID_DESCRIPTOR_INDEX && normalIndex != Heap::INVALID_DESCRIPTOR_INDEX &&
               samplerIndex != Heap::INVALID_DESCRIPTOR_INDEX;
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

class PbrStorage : NoCopy {
    AutoRef<Heap>                        mHeap;
    Heap::MaterialToken                  mToken = Heap::INVALID_MATERIAL_TOKEN;
    std::array<Heap::DescriptorIndex, 5> mTextures {};
    Heap::DescriptorIndex                mSampler {};
    std::array<bool, 5>                  mOwnTextures {};
    bool                                 mOwnSampler = false;
    gpu2::GpuResourceView                mView;
    PbrMaterialData                      mData {};
    uint32_t                             mIndex = 0;

public:
    explicit PbrStorage(AutoRef<Heap> heap): mHeap(std::move(heap)) {}
    ~PbrStorage() {
        if (!mHeap) return;
        mHeap->freeMaterial(mToken);
        for (size_t i = 0; i < mTextures.size(); ++i)
            if (mOwnTextures[i]) mHeap->free(mTextures[i]);
        if (mOwnSampler) mHeap->free(mSampler);
    }
    bool initialize(const PbrMaterial::Parameters & p, const Fallbacks & defaults) {
        const gpu2::GpuResourceView maps[] = {p.baseColorMap, p.normalMap, p.emissiveMap, p.occlusionMap, p.metalRoughMap};
        for (size_t i = 0; i < mTextures.size(); ++i) {
            mOwnTextures[i] = !maps[i].empty() && maps[i] != (i == 1 ? defaults.normal : defaults.color);
            mTextures[i]    = mOwnTextures[i] ? mHeap->allocate(Heap::SAMPLED_TEXTURE, maps[i]) : (i == 1 ? defaults.normalIndex : defaults.colorIndex);
            if (mTextures[i] == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        }
        mOwnSampler = p.sampler && p.sampler.get() != defaults.sampler.get();
        mSampler    = mOwnSampler ? mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView(p.sampler)) : defaults.samplerIndex;
        if (mSampler == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        mToken = mHeap->allocateMaterial(sizeof(mData), alignof(PbrMaterialData));
        if (mToken == Heap::INVALID_MATERIAL_TOKEN) return false;
        mView = mHeap->materialView(mToken);
        if (!mView.buffer() || mView.bufferView.size != sizeof(mData) || mView.bufferView.offset % sizeof(mData) ||
            mView.bufferView.offset / sizeof(mData) > uint64_t(uint32_t(-1)))
            return false;
        mIndex         = uint32_t(mView.bufferView.offset / sizeof(mData));
        uint32_t flags = 0;
        for (uint32_t i = 0; i < 5; ++i)
            if (mOwnTextures[i]) flags |= 1u << i;
        mData = {p.baseColor, glm::vec4(p.emissive, 0.0f), glm::vec4(p.metallic, p.roughness, p.normalScale, p.occlusionStrength),
                 glm::uvec4(mTextures[0].slot, mTextures[1].slot, mTextures[2].slot, mTextures[3].slot),
                 glm::uvec4(mTextures[4].slot, mSampler.slot, flags, p.opaque ? 1u : 0u)};
        return true;
    }
    void upload(gpu2::bindless::CnC & producer) const {
        producer.recordUploadBuffer(mView.buffer(), mView.bufferView.offset, {reinterpret_cast<const uint8_t *>(&mData), sizeof(mData)});
    }
    uint32_t index() const { return mIndex; }
};

class PbrMaterialImpl final : public PbrMaterial {
    AutoRef<const PbrKernel> mKernel;
    Shaders                  mShaders;
    PbrStorage               mStorage;

public:
    GN_REGISTER_RUNTIME_TYPE(PbrMaterial);
    PbrMaterialImpl(AutoRef<const PbrKernel> kernel, const Shaders & shaders, AutoRef<Heap> heap)
        : PbrMaterial(TYPE_INFO(), "bindless.pbr.material"), mKernel(std::move(kernel)), mShaders(shaders), mStorage(std::move(heap)) {}
    bool initialize(const Parameters & p, const Fallbacks & defaults) { return mStorage.initialize(p, defaults); }
    void upload(gpu2::bindless::CnC & producer) const { mStorage.upload(producer); }
    bool record(const PbrMaterial::DrawParameters & input) const override {
        if (!input.ssc || !validUniform(input.ssc->view()) || !validDraw(input, input.object2WorldTransform)) {
            GN_ERROR(logger, "PbrMaterial: invalid uniform state, geometry, UVs, or transform");
            return false;
        }
        const DrawConstants                    values {input.object2WorldTransform, glm::uvec4(mStorage.index(), 0, 0, 0)};
        gpu2::bindless::Raster::DrawParameters draw {.geometry = input.geometry};
        draw.vs         = mShaders.vertex;
        draw.ps         = mShaders.fragment;
        if (input.states) draw.states = *input.states;
        draw.immediates = {reinterpret_cast<const uint8_t *>(&values), sizeof(values)};
        input.raster.retainResource(input.ssc);
        input.raster.retainResource(referenceTo(this));
        input.raster.recordDraw(draw);
        return true;
    }
};

class PbrImpl final : public PbrKernel {
    Shaders   mShaders;
    Fallbacks mDefaults;

public:
    GN_REGISTER_RUNTIME_TYPE(PbrKernel);
    PbrImpl(): PbrKernel(TYPE_INFO(), "bindless.pbr") {}
    bool initialize(AutoRef<Heap> heap) {
        if (!heap || heap->bindingIndex() != 0) return false;
        return mShaders.initialize(heap->gpu(), kBindlessPbrVertSpv, sizeof(kBindlessPbrVertSpv), kBindlessPbrFragSpv, sizeof(kBindlessPbrFragSpv)) &&
               mDefaults.initialize(std::move(heap));
    }
    void                    uploadDefaults(gpu2::bindless::CnC & producer) const { mDefaults.upload(producer); }
    PbrMaterial::Parameters defaultMaterialParameters() const override {
        PbrMaterial::Parameters p;
        p.sampler = mDefaults.sampler;
        return p;
    }
    AutoRef<PbrMaterial> createMaterial(gpu2::bindless::CnC & producer, const PbrMaterial::Parameters & p) const override {
        bool valid = std::isfinite(p.metallic) && p.metallic >= 0 && p.metallic <= 1 && std::isfinite(p.roughness) && p.roughness >= 0 && p.roughness <= 1 &&
                     std::isfinite(p.normalScale) && std::isfinite(p.occlusionStrength) && p.occlusionStrength >= 0 && p.occlusionStrength <= 1;
        for (int i = 0; i < 4; ++i) valid &= std::isfinite(p.baseColor[i]);
        for (int i = 0; i < 3; ++i) valid &= std::isfinite(p.emissive[i]);
        const gpu2::GpuResourceView maps[] = {p.baseColorMap, p.normalMap, p.emissiveMap, p.occlusionMap, p.metalRoughMap};
        for (auto & map : maps) valid &= validMap(map);
        if (!valid) return {};
        auto material = AutoRef<PbrMaterialImpl>(new PbrMaterialImpl(referenceTo(this), mShaders, mDefaults.heap()));
        if (!material->initialize(p, mDefaults)) return {};
        producer.retainResource(material);
        material->upload(producer);
        return material;
    }
};

} // namespace

AutoRef<PbrKernel> PbrKernel::create(gpu2::bindless::DescriptorHeap & heap, gpu2::bindless::CnC & initialization) {
    auto impl = AutoRef<PbrImpl>(new PbrImpl());
    if (!impl->initialize(referenceTo(&heap))) return {};
    initialization.retainResource(impl);
    impl->uploadDefaults(initialization);
    return impl;
}

} // namespace GN::fx2::bindless
