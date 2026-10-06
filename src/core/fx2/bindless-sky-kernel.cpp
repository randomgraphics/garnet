#include "pch.h"
#include "bindless-sky-vert.spv.h"
#include "bindless-sky-frag.spv.h"
#include "bindless-sky-material-impl.h"
#include <cmath>

namespace GN::fx2::bindless {
namespace {
using Heap    = gpu2::bindless::DescriptorHeap;
auto * logger = getLogger("GN.fx2.bindless.sky");

struct SkyMaterialData {
    glm::vec4  factors;
    glm::uvec4 textureIndices;
    glm::uvec4 samplerIndices;
    glm::uvec4 unused;
};
static_assert(sizeof(SkyMaterialData) == 64);

struct SkyDrawConstants {
    glm::uvec4 materialIndex;
};
static_assert(sizeof(SkyDrawConstants) == 16);

bool validUniform(const gpu2::GpuResourceView & view) {
    return view.buffer() && view.bufferView.type == gpu2::GpuResourceView::BufferView::UNIFORM && view.bufferView.size >= sizeof(SharedUniforms);
}

bool validCubeMap(const gpu2::GpuResourceView & view) {
    if (view.empty()) return true;
    auto texture = view.texture();
    if (!texture || view.combinedTextureSampler || view.imageView.type != gpu2::GpuResourceView::ImageView::SAMPLED) return false;
    const auto & d = texture->descriptor();
    const auto & r = view.imageView.range;
    return d.faces == 6 && d.depth == 1 && r.i.mip < d.levels && (r.e.numArrayLayers == 6 || r.e.numArrayLayers == uint32_t(-1)) &&
           (r.e.numMipLevels == uint32_t(-1) || (r.e.numMipLevels > 0 && r.e.numMipLevels <= d.levels - r.i.mip));
}

bool valid2DMap(const gpu2::GpuResourceView & view) {
    if (view.empty()) return true;
    auto texture = view.texture();
    if (!texture || view.combinedTextureSampler || view.imageView.type != gpu2::GpuResourceView::ImageView::SAMPLED) return false;
    const auto & d = texture->descriptor();
    const auto & r = view.imageView.range;
    return d.faces == 1 && d.depth == 1 && r.i.face == 0 && r.i.mip < d.levels && (r.e.numArrayLayers == 1 || r.e.numArrayLayers == uint32_t(-1)) &&
           (r.e.numMipLevels == uint32_t(-1) || (r.e.numMipLevels > 0 && r.e.numMipLevels <= d.levels - r.i.mip));
}

struct Shaders {
    AutoRef<gpu2::GpuContext> gpu;
    AutoRef<gpu2::GpuShader>  vertex, fragment;
    bool                      initialize(AutoRef<gpu2::GpuContext> context, const uint32_t * vs, size_t vsSize, const uint32_t * ps, size_t psSize) {
        gpu = std::move(context);
        if (!gpu) return false;
        vertex   = gpu2::GpuShader::create({.context = gpu, .name = "bindless-sky.vert", .binary = vs, .size = vsSize});
        fragment = gpu2::GpuShader::create({.context = gpu, .name = "bindless-sky.frag", .binary = ps, .size = psSize});
        return vertex && fragment;
    }
};

class Fallbacks : NoCopy {
    AutoRef<Heap> mHeap;

public:
    gpu2::GpuResourceView  cubemap, brdfLut;
    AutoRef<gpu2::Sampler> sampler, lutSampler;
    Heap::DescriptorIndex  cubemapIndex = {}, brdfLutIndex = {}, samplerIndex = {}, lutSamplerIndex = {};

    ~Fallbacks() {
        if (!mHeap) return;
        mHeap->free(cubemapIndex);
        mHeap->free(brdfLutIndex);
        mHeap->free(samplerIndex);
        mHeap->free(lutSamplerIndex);
    }
    AutoRef<Heap> heap() const { return mHeap; }

    bool initialize(AutoRef<Heap> heap) {
        mHeap    = std::move(heap);
        auto gpu = mHeap->gpu();

        const auto cubeDesc = gpu2::Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setFaces(6).setLevels(1);
        auto       defaultCube = gpu2::Texture::create("bindless.sky-default-cube", {.context = gpu, .descriptor = cubeDesc});

        const auto lutDesc    = gpu2::Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setLevels(1);
        auto       defaultLut = gpu2::Texture::create("bindless.sky-default-lut", {.context = gpu, .descriptor = lutDesc});

        gpu2::Sampler::Descriptor sDesc;
        sDesc.minFilter = gpu2::Sampler::Descriptor::Filter::LINEAR;
        sDesc.magFilter = gpu2::Sampler::Descriptor::Filter::LINEAR;
        sDesc.mipFilter = gpu2::Sampler::Descriptor::Filter::LINEAR;
        sDesc.addressU  = gpu2::Sampler::Descriptor::Address::CLAMP_TO_EDGE;
        sDesc.addressV  = gpu2::Sampler::Descriptor::Address::CLAMP_TO_EDGE;
        sDesc.addressW  = gpu2::Sampler::Descriptor::Address::CLAMP_TO_EDGE;
        sampler         = gpu2::Sampler::create("bindless.sky-default-sampler", {.context = gpu, .descriptor = sDesc});
        lutSampler      = gpu2::Sampler::create("bindless.sky-default-lut-sampler", {.context = gpu, .descriptor = sDesc});

        if (!defaultCube || !defaultLut || !sampler || !lutSampler) return false;
        cubemap         = gpu2::GpuResourceView {defaultCube}.setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        brdfLut         = gpu2::GpuResourceView {defaultLut}.setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        cubemapIndex    = mHeap->allocate(Heap::SAMPLED_TEXTURE, cubemap);
        brdfLutIndex    = mHeap->allocate(Heap::SAMPLED_TEXTURE, brdfLut);
        samplerIndex    = mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView {sampler});
        lutSamplerIndex = mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView {lutSampler});

        return cubemapIndex != Heap::INVALID_DESCRIPTOR_INDEX && brdfLutIndex != Heap::INVALID_DESCRIPTOR_INDEX &&
               samplerIndex != Heap::INVALID_DESCRIPTOR_INDEX && lutSamplerIndex != Heap::INVALID_DESCRIPTOR_INDEX;
    }

    void upload(gpu2::bindless::CnC & producer) const {
        const uint8_t skyBlue[6 * 4] = {
            135, 206, 235, 255, 135, 206, 235, 255, 135, 206, 235, 255, 135, 206, 235, 255, 135, 206, 235, 255, 135, 206, 235, 255,
        };
        gpu2::bindless::CnC::Region cubeRegions[6];
        for (uint32_t f = 0; f < 6; ++f) {
            cubeRegions[f].face        = f;
            cubeRegions[f].dataOffset  = f * 4;
            cubeRegions[f].imageExtent = {1, 1, 1};
        }
        producer.recordUploadImage(cubemap.texture(), {skyBlue, sizeof(skyBlue)}, {cubeRegions, 6});

        const uint8_t               white[4] = {255, 255, 255, 255};
        gpu2::bindless::CnC::Region lutRegion;
        lutRegion.imageExtent = {1, 1, 1};
        producer.recordUploadImage(brdfLut.texture(), {white, sizeof(white)}, {&lutRegion, 1});
    }
};

class SkyStorage : NoCopy {
    AutoRef<Heap>                        mHeap;
    Heap::MaterialToken                  mToken = Heap::INVALID_MATERIAL_TOKEN;
    std::array<Heap::DescriptorIndex, 4> mTextures {};
    std::array<Heap::DescriptorIndex, 2> mSamplers {};
    std::array<bool, 4>                  mOwnTextures {};
    std::array<bool, 2>                  mOwnSamplers {};
    gpu2::GpuResourceView                mView;
    SkyMaterialData                      mData {};
    uint32_t                             mIndex = 0;

public:
    explicit SkyStorage(AutoRef<Heap> heap): mHeap(std::move(heap)) {}

    ~SkyStorage() {
        if (!mHeap) return;
        mHeap->freeMaterial(mToken);
        for (size_t i = 0; i < mTextures.size(); ++i) {
            if (mOwnTextures[i]) mHeap->free(mTextures[i]);
        }
        for (size_t i = 0; i < mSamplers.size(); ++i) {
            if (mOwnSamplers[i]) mHeap->free(mSamplers[i]);
        }
    }

    bool initialize(const SkyMaterial::Parameters & p, const Fallbacks & defaults) {
        const gpu2::GpuResourceView maps[] = {p.skyboxMap, p.irradianceMap, p.prefilteredMap, p.brdfLut};
        for (size_t i = 0; i < mTextures.size(); ++i) {
            const auto & defaultView = (i == 3) ? defaults.brdfLut : defaults.cubemap;
            const auto & defaultIdx  = (i == 3) ? defaults.brdfLutIndex : defaults.cubemapIndex;
            mOwnTextures[i]          = !maps[i].empty() && maps[i] != defaultView;
            mTextures[i]             = mOwnTextures[i] ? mHeap->allocate(Heap::SAMPLED_TEXTURE, maps[i]) : defaultIdx;
            if (mTextures[i] == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        }

        const AutoRef<gpu2::Sampler> samplers[] = {p.sampler, p.lutSampler};
        for (size_t i = 0; i < mSamplers.size(); ++i) {
            const auto & defaultSamp = (i == 1) ? defaults.lutSampler : defaults.sampler;
            const auto & defaultIdx  = (i == 1) ? defaults.lutSamplerIndex : defaults.samplerIndex;
            mOwnSamplers[i]          = samplers[i] && samplers[i].get() != defaultSamp.get();
            mSamplers[i]             = mOwnSamplers[i] ? mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView {samplers[i]}) : defaultIdx;
            if (mSamplers[i] == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        }

        mToken = mHeap->allocateMaterial(sizeof(mData), sizeof(SkyMaterialData));
        if (mToken == Heap::INVALID_MATERIAL_TOKEN) return false;
        mView = mHeap->materialView(mToken);
        if (!mView.buffer() || mView.bufferView.size != sizeof(mData) || mView.bufferView.offset % sizeof(mData) ||
            mView.bufferView.offset / sizeof(mData) > uint64_t(uint32_t(-1))) {
            return false;
        }
        mIndex = uint32_t(mView.bufferView.offset / sizeof(mData));

        uint32_t flags = 0;
        for (uint32_t i = 0; i < 4; ++i) {
            if (mOwnTextures[i]) flags |= 1u << i;
        }

        mData = {
            glm::vec4(p.luminanceScale, p.ambientFloor, 0.0f, 0.0f),
            glm::uvec4(mTextures[0].slot, mTextures[1].slot, mTextures[2].slot, mTextures[3].slot),
            glm::uvec4(mSamplers[0].slot, mSamplers[1].slot, flags, 0u),
            glm::uvec4(0),
        };
        return true;
    }

    void upload(gpu2::bindless::CnC & producer) const {
        producer.recordUploadBuffer(mView.buffer(), mView.bufferView.offset, {reinterpret_cast<const uint8_t *>(&mData), sizeof(mData)});
    }

    uint32_t index() const { return mIndex; }
};

class SkyMaterialImpl final : public SkyMaterial {
    AutoRef<const SkyKernel> mKernel;
    Shaders                  mShaders;
    SkyStorage               mStorage;

public:
    GN_REGISTER_RUNTIME_TYPE(SkyMaterial);

    SkyMaterialImpl(AutoRef<const SkyKernel> kernel, const Shaders & shaders, AutoRef<Heap> heap)
        : SkyMaterial(TYPE_INFO(), "bindless.sky.material"), mKernel(std::move(kernel)), mShaders(shaders), mStorage(std::move(heap)) {}

    bool initialize(const Parameters & p, const Fallbacks & defaults) { return mStorage.initialize(p, defaults); }
    void upload(gpu2::bindless::CnC & producer) const { mStorage.upload(producer); }

    bool record(const SkyMaterial::DrawParameters & input) const override {
        if (!input.ssc || !validUniform(input.ssc->view())) {
            GN_ERROR(logger, "SkyMaterial: invalid uniform state");
            return false;
        }
        const SkyDrawConstants values {glm::uvec4(mStorage.index(), 0, 0, 0)};
        gpu2::RasterGeometry   geometry;
        geometry.vertexCount = 3;
        gpu2::bindless::Raster::DrawParameters draw {.geometry = geometry};
        draw.vs = mShaders.vertex;
        draw.ps = mShaders.fragment;
        if (input.states) {
            draw.states = *input.states;
            if (!draw.states.depthState) { draw.states.depthState = gpu2::RasterState::DepthState {gpu2::RasterState::Compare::LESS_EQUAL, false}; }
            if (!draw.states.cullMode) { draw.states.cullMode = gpu2::RasterState::CULL_NONE; }
        } else {
            draw.states.depthState = gpu2::RasterState::DepthState {gpu2::RasterState::Compare::LESS_EQUAL, false};
            draw.states.cullMode   = gpu2::RasterState::CULL_NONE;
        }
        draw.immediates = {reinterpret_cast<const uint8_t *>(&values), sizeof(values)};
        input.raster.retainResource(input.ssc);
        input.raster.retainResource(referenceTo(this));
        input.raster.recordDraw(draw);
        return true;
    }

    uint32_t index() const { return mStorage.index(); }
};

class SkyImpl final : public SkyKernel {
    Shaders   mShaders;
    Fallbacks mDefaults;

public:
    GN_REGISTER_RUNTIME_TYPE(SkyKernel);

    SkyImpl(): SkyKernel(TYPE_INFO(), "bindless.sky") {}

    bool initialize(AutoRef<Heap> heap) {
        if (!heap || heap->bindingIndex() != 0) return false;
        return mShaders.initialize(heap->gpu(), kBindlessSkyVertSpv, sizeof(kBindlessSkyVertSpv), kBindlessSkyFragSpv, sizeof(kBindlessSkyFragSpv)) &&
               mDefaults.initialize(std::move(heap));
    }

    void uploadDefaults(gpu2::bindless::CnC & producer) const { mDefaults.upload(producer); }

    SkyMaterial::Parameters defaultMaterialParameters() const override {
        SkyMaterial::Parameters p;
        p.sampler        = mDefaults.sampler;
        p.lutSampler     = mDefaults.lutSampler;
        p.luminanceScale = 1.0f;
        p.ambientFloor   = 0.0f;
        return p;
    }

    AutoRef<SkyMaterial> createMaterial(gpu2::bindless::CnC & producer, const SkyMaterial::Parameters & p) const override {
        bool valid = std::isfinite(p.luminanceScale) && p.luminanceScale >= 0.0f && std::isfinite(p.ambientFloor) && p.ambientFloor >= 0.0f &&
                     validCubeMap(p.skyboxMap) && validCubeMap(p.irradianceMap) && validCubeMap(p.prefilteredMap) && valid2DMap(p.brdfLut);
        if (!valid) return {};

        auto material = AutoRef<SkyMaterialImpl>(new SkyMaterialImpl(referenceTo(this), mShaders, mDefaults.heap()));
        if (!material->initialize(p, mDefaults)) return {};
        producer.retainResource(material);
        material->upload(producer);
        return material;
    }
};

} // namespace

uint32_t getSkyMaterialIndex(const SkyMaterial * material) {
    if (!material) return uint32_t(-1);
    auto * impl = RuntimeType::cast<SkyMaterialImpl>(material);
    return impl ? impl->index() : uint32_t(-1);
}

AutoRef<SkyKernel> SkyKernel::create(gpu2::bindless::DescriptorHeap & heap, gpu2::bindless::CnC & initialization) {
    auto impl = AutoRef<SkyImpl>(new SkyImpl());
    if (!impl->initialize(referenceTo(&heap))) return {};
    initialization.retainResource(impl);
    impl->uploadDefaults(initialization);
    return impl;
}

} // namespace GN::fx2::bindless
