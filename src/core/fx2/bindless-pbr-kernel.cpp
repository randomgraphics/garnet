#include "pch.h"
#include "bindless-pbr-vert.spv.h"
#include "bindless-pbr-frag.spv.h"
#include "bindless-pbr-raytrace-frag.spv.h"
#include "bindless-sky-material-impl.h"
#include <glm/mat3x3.hpp>
#include <cmath>

namespace GN::fx2 {
namespace {
using Heap    = ::GN::gpu2::DescriptorHeap;
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
    bool                      initialize(AutoRef<gpu2::GpuContext> context, const uint32_t * vs, size_t vsSize, const uint32_t * ps, size_t psSize) {
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
        color        = gpu2::GpuResourceView {white}.setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        normal       = gpu2::GpuResourceView {flat}.setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
        colorIndex   = mHeap->allocate(Heap::SAMPLED_TEXTURE, color);
        normalIndex  = mHeap->allocate(Heap::SAMPLED_TEXTURE, normal);
        samplerIndex = mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView {sampler});
        return colorIndex != Heap::INVALID_DESCRIPTOR_INDEX && normalIndex != Heap::INVALID_DESCRIPTOR_INDEX && samplerIndex != Heap::INVALID_DESCRIPTOR_INDEX;
    }
    void upload(::GN::gpu2::CnC & producer) const {
        const uint8_t           white[] = {255, 255, 255, 255};
        const uint8_t           flat[]  = {128, 128, 255, 255};
        ::GN::gpu2::CnC::Region region;
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
        mSampler    = mOwnSampler ? mHeap->allocate(Heap::SAMPLER, gpu2::GpuResourceView {p.sampler}) : defaults.samplerIndex;
        if (mSampler == Heap::INVALID_DESCRIPTOR_INDEX) return false;
        mToken = mHeap->allocateMaterial(sizeof(mData), sizeof(mData));
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
    void upload(::GN::gpu2::CnC & producer) const {
        producer.recordUploadBuffer(mView.buffer(), mView.bufferView.offset, {reinterpret_cast<const uint8_t *>(&mData), sizeof(mData)});
    }
    uint32_t index() const { return mIndex; }
    bool     usesHeap(const AutoRef<Heap> & heap) const { return mHeap == heap; }
    bool     opaque() const { return mData.extraIndices.w != 0; }
};

class PbrMaterialImpl final : public PbrMaterial {
    Fallbacks  mDefaults;
    PbrStorage mStorage;

public:
    GN_REGISTER_RUNTIME_TYPE(PbrMaterial);
    explicit PbrMaterialImpl(AutoRef<Heap> heap): PbrMaterial(TYPE_INFO(), "bindless.pbr.material"), mStorage(std::move(heap)) {}
    bool initialize(const Parameters & p, AutoRef<Heap> heap) { return mDefaults.initialize(std::move(heap)) && mStorage.initialize(p, mDefaults); }
    void upload(::GN::gpu2::CnC & producer) const {
        mDefaults.upload(producer);
        mStorage.upload(producer);
    }
    uint32_t index() const { return mStorage.index(); }
    bool     usesHeap(const AutoRef<Heap> & heap) const { return mStorage.usesHeap(heap); }
    bool     opaque() const { return mStorage.opaque(); }
};

class PbrImpl final : public PbrKernel {
    Shaders       mShaders;
    AutoRef<Heap> mHeap;

public:
    GN_REGISTER_RUNTIME_TYPE(PbrKernel);
    PbrImpl(): PbrKernel(TYPE_INFO(), "bindless.pbr") {}
    bool initialize(AutoRef<Heap> heap) {
        if (!heap || heap->bindingIndex() != 0 ||
            !mShaders.initialize(heap->gpu(), kBindlessPbrVertSpv, sizeof(kBindlessPbrVertSpv), kBindlessPbrFragSpv, sizeof(kBindlessPbrFragSpv)))
            return false;
        mHeap = std::move(heap);
        return true;
    }
    bool record(PbrMaterial & material, const PbrMaterial::DrawParameters & input) const override {
        auto * implementation = RuntimeType::cast<PbrMaterialImpl>(&material);
        if (!implementation || !implementation->usesHeap(mHeap)) return false;
        if (!input.ssc || !validUniform(input.ssc->view()) || !validDraw(input, input.object2WorldTransform)) {
            GN_ERROR(logger, "PbrKernel: invalid uniform state, geometry, UVs, or transform");
            return false;
        }
        const uint32_t                     skyIndex = getSkyMaterialIndex(input.skyMaterial.get());
        const DrawConstants                values {input.object2WorldTransform, glm::uvec4(implementation->index(), skyIndex, 0, 0)};
        ::GN::gpu2::Raster::DrawParameters draw {.geometry = input.geometry};
        draw.vs = mShaders.vertex;
        draw.ps = mShaders.fragment;
        if (input.states) draw.states = *input.states;
        draw.immediates = {reinterpret_cast<const uint8_t *>(&values), sizeof(values)};
        input.raster.retainResource(input.ssc);
        input.raster.retainResource(referenceTo(&material));
        if (input.skyMaterial) input.raster.retainResource(input.skyMaterial);
        input.raster.recordDraw(draw);
        return true;
    }
};

struct RayTracingGeometryData {
    uint64_t  vertexAddress  = 0;
    uint64_t  indexAddress   = 0;
    uint32_t  vertexStride   = 0;
    uint32_t  positionOffset = 0;
    uint32_t  normalOffset   = 0;
    uint32_t  texcoordOffset = 0;
    uint32_t  materialIndex  = 0;
    uint32_t  vertexCount    = 0;
    uint32_t  indexCount     = 0;
    uint32_t  indexFormat    = 0;
    glm::mat4 objectToWorld {1};
};
static_assert(offsetof(RayTracingGeometryData, objectToWorld) == 48);
static_assert(sizeof(RayTracingGeometryData) == 112);

const gpu2::RasterGeometry::VertexAttribute * findAttribute(const gpu2::RasterGeometry & geometry, uint32_t location) {
    const gpu2::RasterGeometry::VertexAttribute * found = nullptr;
    for (const auto & attribute : geometry.format.attributes) {
        if (attribute.location != location) continue;
        if (found) return nullptr;
        found = &attribute;
    }
    return found;
}

bool validRayTransform(const glm::mat4 & transform) {
    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            if (!std::isfinite(transform[c][r])) return false;
    const float determinant = glm::determinant(glm::mat3(transform));
    return std::isfinite(determinant) && std::abs(determinant) > 1e-20f && transform[0][3] == 0 && transform[1][3] == 0 && transform[2][3] == 0 &&
           transform[3][3] == 1;
}

bool packRayGeometry(const PbrRayTracingScene::Mesh & mesh, const PbrMaterialImpl & material, RayTracingGeometryData & data,
                     gpu2::AccelerationStructure::TriangleGeometry & triangle) {
    if (!mesh.geometry || !validRayTransform(mesh.objectToWorld)) return false;
    const auto & geometry = *mesh.geometry;
    const auto * position = findAttribute(geometry, PbrMaterial::POSITION_LOCATION);
    const auto * normal   = findAttribute(geometry, PbrMaterial::NORMAL_LOCATION);
    const auto * texcoord = findAttribute(geometry, PbrMaterial::TEXCOORD_LOCATION);
    if (!position || !normal || !texcoord || position->format != gpu2::RasterGeometry::AttributeFormat::F32_3 ||
        normal->format != gpu2::RasterGeometry::AttributeFormat::F32_3 || texcoord->format != gpu2::RasterGeometry::AttributeFormat::F32_2 ||
        position->binding >= geometry.vertices.size() || normal->binding != position->binding || texcoord->binding != position->binding)
        return false;
    const auto & vertex           = geometry.vertices[position->binding];
    const bool   vertexLayoutFits = vertex.buffer && vertex.buffer->gpuAddress() && vertex.stride && position->offset <= vertex.stride &&
                                    vertex.stride - position->offset >= 12 && normal->offset <= vertex.stride && vertex.stride - normal->offset >= 12 &&
                                    texcoord->offset <= vertex.stride && vertex.stride - texcoord->offset >= 8;
    const bool   indexLayoutFits  = (geometry.indexCount != 0) == bool(geometry.indices.buffer) &&
                                    (!geometry.indexCount || ((geometry.indices.stride == 2 || geometry.indices.stride == 4) && geometry.indexCount % 3 == 0)) &&
                                    (!geometry.indices.buffer || geometry.indices.buffer->gpuAddress());
    if (!vertexLayoutFits || !indexLayoutFits || !geometry.vertexCount || !material.opaque()) return false;

    data.vertexAddress  = vertex.buffer->gpuAddress() + vertex.offset;
    data.indexAddress   = geometry.indices.buffer ? geometry.indices.buffer->gpuAddress() + geometry.indices.offset : 0;
    data.vertexStride   = vertex.stride;
    data.positionOffset = position->offset;
    data.normalOffset   = normal->offset;
    data.texcoordOffset = texcoord->offset;
    data.materialIndex  = material.index();
    data.vertexCount    = geometry.vertexCount;
    data.indexCount     = geometry.indexCount;
    data.indexFormat    = geometry.indices.stride == 2 ? 1u : (geometry.indices.stride == 4 ? 2u : 0u);
    data.objectToWorld  = mesh.objectToWorld;

    triangle.vertexBuffer   = vertex.buffer;
    triangle.vertexOffset   = vertex.offset;
    triangle.vertexStride   = vertex.stride;
    triangle.positionOffset = position->offset;
    triangle.vertexCount    = geometry.vertexCount;
    triangle.indexBuffer    = geometry.indices.buffer;
    triangle.indexOffset    = geometry.indices.offset;
    triangle.indexCount     = geometry.indexCount;
    triangle.indexFormat    = geometry.indexCount ? (geometry.indices.stride == 2 ? gpu2::AccelerationStructure::IndexFormat::UINT16
                                                                                  : gpu2::AccelerationStructure::IndexFormat::UINT32)
                                                  : gpu2::AccelerationStructure::IndexFormat::NONE;
    for (uint32_t row = 0; row < 4; ++row)
        triangle.objectToWorld.rows[row] = {mesh.objectToWorld[0][row], mesh.objectToWorld[1][row], mesh.objectToWorld[2][row], mesh.objectToWorld[3][row]};
    return true;
}

class PbrRayTracingSceneImpl final : public PbrRayTracingScene {
    AutoRef<gpu2::AccelerationStructure> mAccelerationStructure;
    AutoRef<gpu2::Buffer>                mGeometryBuffer;
    DynaArray<AutoRef<gpu2::Buffer>>     mGeometryBuffers;
    DynaArray<AutoRef<PbrMaterial>>      mMaterials;

public:
    GN_REGISTER_RUNTIME_TYPE(PbrRayTracingScene);
    PbrRayTracingSceneImpl(): PbrRayTracingScene(TYPE_INFO(), "fx2.pbr-ray-tracing-scene") {}
    bool initialize(gpu2::DescriptorHeap & heap, gpu2::CnC & initialization, ArrayView<const Mesh> meshes) {
        if (meshes.empty()) return false;
        DynaArray<RayTracingGeometryData>                        geometryData;
        DynaArray<gpu2::AccelerationStructure::TriangleGeometry> acsGeometry;
        geometryData.reserve(meshes.size());
        acsGeometry.reserve(meshes.size());
        for (const Mesh & mesh : meshes) {
            if (!mesh.geometry || !mesh.material) return false;
            auto * material = RuntimeType::cast<PbrMaterialImpl>(mesh.material);
            if (!material || !material->usesHeap(referenceTo(&heap))) return false;
            RayTracingGeometryData                        data;
            gpu2::AccelerationStructure::TriangleGeometry triangle;
            if (!packRayGeometry(mesh, *material, data, triangle)) return false;
            geometryData.append(data);
            acsGeometry.append(triangle);
            mGeometryBuffers.append(triangle.vertexBuffer);
            if (triangle.indexBuffer) mGeometryBuffers.append(triangle.indexBuffer);
            mMaterials.append(referenceTo(mesh.material));
        }
        auto         gpu       = heap.gpu();
        const size_t tableSize = geometryData.size() * sizeof(RayTracingGeometryData);
        mGeometryBuffer        = gpu2::Buffer::create("fx2.pbr-ray-tracing-geometry-table", {.context = gpu, .size = tableSize});
        if (!mGeometryBuffer) return false;
        mAccelerationStructure = initialization.recordBuildAccelerationStructure("fx2.pbr-ray-tracing-acs", {.geometries = acsGeometry});
        if (!mAccelerationStructure) return false;
        initialization.recordUploadBuffer(mGeometryBuffer, 0, {reinterpret_cast<const uint8_t *>(geometryData.data()), tableSize});
        initialization.retainResource(referenceTo(this));
        return true;
    }
    AutoRef<gpu2::AccelerationStructure> accelerationStructure() const override { return mAccelerationStructure; }
    gpu2::GpuResourceView                geometryTable() const override {
        gpu2::GpuResourceView view {mGeometryBuffer};
        view.setBufferViewType(gpu2::GpuResourceView::BufferView::STORAGE);
        return view;
    }
};

class RayTraceImpl final : public RayTraceKernel {
    AutoRef<Heap> mHeap;
    Shaders       mShaders;

public:
    GN_REGISTER_RUNTIME_TYPE(RayTraceKernel);

    RayTraceImpl(): RayTraceKernel(TYPE_INFO(), "bindless.pbr-ray-tracing") {}

    bool initialize(AutoRef<Heap> heap) {
        if (!heap || heap->bindingIndex() != 0 || !heap->gpu()->caps().rayQueries) return false;
        if (!mShaders.initialize(heap->gpu(), kBindlessPbrVertSpv, sizeof(kBindlessPbrVertSpv), kBindlessPbrRaytraceFragSpv,
                                 sizeof(kBindlessPbrRaytraceFragSpv))) {
            return false;
        }
        mHeap = std::move(heap);
        return true;
    }

    bool record(const DrawParameters & input) const override {
        auto * material = RuntimeType::cast<PbrMaterialImpl>(input.material);
        if (!material || !material->usesHeap(mHeap) || !input.acs || input.acs->gpu() != mHeap->gpu()) return false;
        if (!input.ssc || !validUniform(input.ssc->view()) || !validDraw(input, input.object2WorldTransform)) return false;
        if (input.skyMaterial && !skyMaterialUsesHeap(input.skyMaterial.get(), *mHeap)) return false;
        const uint32_t skyMaterialIndex = getSkyMaterialIndex(input.skyMaterial.get());
        if (input.skyMaterial && skyMaterialIndex == uint32_t(-1)) return false;

        DrawConstants                      constants {input.object2WorldTransform, {material->index(), skyMaterialIndex, 0, 0}};
        ::GN::gpu2::Raster::DrawParameters draw {.vs = mShaders.vertex, .ps = mShaders.fragment, .geometry = input.geometry};
        if (input.states) draw.states = *input.states;
        draw.immediates = {reinterpret_cast<const uint8_t *>(&constants), sizeof(constants)};

        input.raster.retainResource(input.ssc);
        input.raster.retainResource(referenceTo(input.material));
        input.raster.retainResource(input.acs);
        if (input.skyMaterial) input.raster.retainResource(input.skyMaterial);
        input.raster.recordDraw(draw);
        return true;
    }
};

} // namespace

AutoRef<PbrRayTracingScene> PbrRayTracingScene::create(gpu2::DescriptorHeap & heap, gpu2::CnC & initialization, ArrayView<const Mesh> meshes) {
    auto scene = AutoRef<PbrRayTracingSceneImpl>(new PbrRayTracingSceneImpl());
    if (!scene->initialize(heap, initialization, meshes)) return {};
    return scene;
}

AutoRef<RayTraceKernel> RayTraceKernel::create(gpu2::DescriptorHeap & heap, gpu2::CnC & initialization) {
    auto kernel = AutoRef<RayTraceImpl>(new RayTraceImpl());
    if (!kernel->initialize(referenceTo(&heap))) return {};
    initialization.retainResource(kernel);
    return kernel;
}

AutoRef<PbrKernel> PbrKernel::create(::GN::gpu2::DescriptorHeap & heap, ::GN::gpu2::CnC & initialization) {
    auto impl = AutoRef<PbrImpl>(new PbrImpl());
    if (!impl->initialize(referenceTo(&heap))) return {};
    initialization.retainResource(impl);
    return impl;
}

AutoRef<PbrMaterial> PbrMaterial::create(::GN::gpu2::DescriptorHeap & heap, ::GN::gpu2::CnC & initialization, const Parameters & p) {
    bool valid = std::isfinite(p.metallic) && p.metallic >= 0 && p.metallic <= 1 && std::isfinite(p.roughness) && p.roughness >= 0 && p.roughness <= 1 &&
                 std::isfinite(p.normalScale) && std::isfinite(p.occlusionStrength) && p.occlusionStrength >= 0 && p.occlusionStrength <= 1;
    for (int i = 0; i < 4; ++i) valid &= std::isfinite(p.baseColor[i]);
    for (int i = 0; i < 3; ++i) valid &= std::isfinite(p.emissive[i]);
    const gpu2::GpuResourceView maps[] = {p.baseColorMap, p.normalMap, p.emissiveMap, p.occlusionMap, p.metalRoughMap};
    for (auto & map : maps) valid &= validMap(map);
    if (!valid || heap.bindingIndex() != 0) return {};
    auto material = AutoRef<PbrMaterialImpl>(new PbrMaterialImpl(referenceTo(&heap)));
    if (!material->initialize(p, referenceTo(&heap))) return {};
    initialization.retainResource(material);
    material->upload(initialization);
    return material;
}

} // namespace GN::fx2
