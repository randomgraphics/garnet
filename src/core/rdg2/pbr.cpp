#include "pch.h"
#include "pbr-vert.spv.h"
#include "pbr-frag.spv.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <cstring>
#include <vector>

static GN::Logger * sLogger = GN::getLogger("GN.rdg2");

namespace GN::rdg2 {

/// Private payload for one renderable PBR object: shaders + material textures + mesh.
struct PbrAssetData {
    AutoRef<gpu2::GpuShader>  vs;
    AutoRef<gpu2::GpuShader>  ps;
    AutoRef<gpu2::Texture>    albedo;
    AutoRef<gpu2::Texture>    normal;
    AutoRef<gpu2::Texture>    emissive;
    AutoRef<gpu2::Texture>    occlusion;
    AutoRef<gpu2::Texture>    metalRough;
    gpu2::RasterGeometry      geometry;
    AutoRef<gpu2::GpuPayload> uploadPayload;
};

// ─────────────────────────────────────────────────────────────────────────────
// Internal helpers — CPU-only, no GPU submit
// ─────────────────────────────────────────────────────────────────────────────

/// Records a 1×1 solid-color texture copy into cnc. Caller seals the CnC for a combined payload.
/// Staging buffer is captured by cnc and kept alive inside the sealed payload.
static AutoRef<gpu2::Texture> make1x1Tex(AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & cnc, const StrA & name, uint8_t r, uint8_t g, uint8_t b, uint8_t a,
                                         uint32_t faces) {
    gpu2::Texture::Descriptor desc;
    desc.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setFaces(faces).setLevels(1);
    auto tex = gpu2::Texture::create(name, {.context = gpu, .descriptor = desc});
    if (!tex) GN_UNLIKELY return {};

    const uint64_t stgSz = 4ULL * faces;
    auto           stg   = gpu2::Buffer::create(name + "_stg", {.context = gpu, .size = stgSz, .mappable = true});
    if (!stg) GN_UNLIKELY return {};

    {
        auto m = stg->map();
        if (!m.data()) GN_UNLIKELY return {};
        auto * p = static_cast<uint8_t *>(m.data());
        for (uint32_t f = 0; f < faces; ++f) {
            p[f * 4 + 0] = r;
            p[f * 4 + 1] = g;
            p[f * 4 + 2] = b;
            p[f * 4 + 3] = a;
        }
    }

    gpu2::ArrayContainer<gpu2::GpuCnC::Region> regions;
    for (uint32_t f = 0; f < faces; ++f) {
        gpu2::GpuCnC::Region reg;
        reg.mip          = 0;
        reg.face         = f;
        reg.imageOffset  = {0, 0, 0};
        reg.imageExtent  = {1, 1, 1};
        reg.bufferOffset = f * 4ULL;
        regions.append(reg);
    }
    cnc.copyBufferToImage({.src = stg, .dst = tex, .regions = regions});
    return tex; // staging reference held by cnc until seal()
}

/// Records staging→device copies for a unit cube into cnc. Returns {vb, ib}. No GPU submit.
/// 24 verts (4 per face, interleaved pos+normal+uv = 32 bytes), 36 uint16 indices.
static std::pair<AutoRef<gpu2::Buffer>, AutoRef<gpu2::Buffer>> makeUnitCubeStaged(AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & cnc) {
    struct Vert {
        float px, py, pz, nx, ny, nz, u, v;
    };
    static const Vert verts[24] = {
        // +Z face (normal 0,0,1)
        {-0.5f, -0.5f, 0.5f, 0, 0, 1, 0, 1},
        {0.5f, -0.5f, 0.5f, 0, 0, 1, 1, 1},
        {0.5f, 0.5f, 0.5f, 0, 0, 1, 1, 0},
        {-0.5f, 0.5f, 0.5f, 0, 0, 1, 0, 0},
        // -Z face (normal 0,0,-1)
        {0.5f, -0.5f, -0.5f, 0, 0, -1, 0, 1},
        {-0.5f, -0.5f, -0.5f, 0, 0, -1, 1, 1},
        {-0.5f, 0.5f, -0.5f, 0, 0, -1, 1, 0},
        {0.5f, 0.5f, -0.5f, 0, 0, -1, 0, 0},
        // +X
        {0.5f, -0.5f, 0.5f, 1, 0, 0, 0, 1},
        {0.5f, -0.5f, -0.5f, 1, 0, 0, 1, 1},
        {0.5f, 0.5f, -0.5f, 1, 0, 0, 1, 0},
        {0.5f, 0.5f, 0.5f, 1, 0, 0, 0, 0},
        // -X
        {-0.5f, -0.5f, -0.5f, -1, 0, 0, 0, 1},
        {-0.5f, -0.5f, 0.5f, -1, 0, 0, 1, 1},
        {-0.5f, 0.5f, 0.5f, -1, 0, 0, 1, 0},
        {-0.5f, 0.5f, -0.5f, -1, 0, 0, 0, 0},
        // +Y
        {-0.5f, 0.5f, 0.5f, 0, 1, 0, 0, 1},
        {0.5f, 0.5f, 0.5f, 0, 1, 0, 1, 1},
        {0.5f, 0.5f, -0.5f, 0, 1, 0, 1, 0},
        {-0.5f, 0.5f, -0.5f, 0, 1, 0, 0, 0},
        // -Y
        {-0.5f, -0.5f, -0.5f, 0, -1, 0, 0, 1},
        {0.5f, -0.5f, -0.5f, 0, -1, 0, 1, 1},
        {0.5f, -0.5f, 0.5f, 0, -1, 0, 1, 0},
        {-0.5f, -0.5f, 0.5f, 0, -1, 0, 0, 0},
    };
    static const uint16_t idx[36] = {
        0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23,
    };

    const uint64_t vbSz = sizeof(verts), ibSz = sizeof(idx);
    auto           stgVB = gpu2::Buffer::create("cube_stg_vb", {.context = gpu, .size = vbSz, .mappable = true});
    auto           stgIB = gpu2::Buffer::create("cube_stg_ib", {.context = gpu, .size = ibSz, .mappable = true});
    if (!stgVB || !stgIB) GN_UNLIKELY return {};

    {
        auto m = stgVB->map();
        if (!m.data()) return {};
        memcpy(m.data(), verts, vbSz);
    }
    {
        auto m = stgIB->map();
        if (!m.data()) return {};
        memcpy(m.data(), idx, ibSz);
    }

    auto vb = gpu2::Buffer::create("cube_vb", {.context = gpu, .size = vbSz});
    auto ib = gpu2::Buffer::create("cube_ib", {.context = gpu, .size = ibSz});
    if (!vb || !ib) GN_UNLIKELY return {};

    cnc.copyBufferToBuffer({.src = stgVB, .dst = vb, .size = vbSz});
    cnc.copyBufferToBuffer({.src = stgIB, .dst = ib, .size = ibSz});
    return {vb, ib}; // stagings held by cnc until caller calls seal()
}

static gpu2::RasterGeometry::VertexFormat stdVertexFormat() {
    gpu2::RasterGeometry::VertexFormat fmt;
    fmt.attributes.append({.location = 0, .binding = 0, .offset = 0, .format = gpu2::RasterGeometry::AttributeFormat::F32_3});
    fmt.attributes.append({.location = 1, .binding = 0, .offset = 12, .format = gpu2::RasterGeometry::AttributeFormat::F32_3});
    fmt.attributes.append({.location = 2, .binding = 0, .offset = 24, .format = gpu2::RasterGeometry::AttributeFormat::F32_2});
    return fmt;
}

static gpu2::RasterGeometry loadGltfGeometryStaged(AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & cnc, const StrA & gltfPath) {
    if (gltfPath.empty()) return {};

    StrA nativePath = fs::toNativeDiskFilePath(gltfPath);
    if (nativePath.empty() || !fs::isFile(nativePath)) nativePath = fs::resolvePath(fs::getCurrentDir(), gltfPath);
    if (!fs::isFile(nativePath)) {
        GN_ERROR(sLogger)("PbrShading::load: cannot resolve GLTF '{}'", gltfPath);
        return {};
    }

    const aiScene * scene = aiImportFile(nativePath.data(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices |
                                                                aiProcess_ImproveCacheLocality | aiProcess_SortByPType | aiProcess_ValidateDataStructure);
    if (!scene || !scene->mRootNode || !scene->HasMeshes()) {
        GN_ERROR(sLogger)("PbrShading::load: Assimp failed to load GLTF '{}': {}", gltfPath, aiGetErrorString());
        if (scene) aiReleaseImport(scene);
        return {};
    }

    struct Vtx {
        float px, py, pz, nx, ny, nz, u, v;
    };
    std::vector<Vtx>      verts;
    std::vector<uint32_t> idxData;

    auto appendMesh = [&](const aiMesh & mesh, const aiMatrix4x4 & transform) {
        if (!mesh.HasPositions() || !mesh.HasFaces()) return;

        aiMatrix4x4 normalTransform = transform;
        normalTransform.Inverse();
        normalTransform.Transpose();

        const uint32_t baseVertex = static_cast<uint32_t>(verts.size());
        verts.reserve(verts.size() + mesh.mNumVertices);
        for (uint32_t i = 0; i < mesh.mNumVertices; ++i) {
            aiVector3D p = mesh.mVertices[i];
            aiTransformVecByMatrix4(&p, &transform);

            aiVector3D n(0.f, 1.f, 0.f);
            if (mesh.HasNormals()) {
                n = mesh.mNormals[i];
                aiTransformVecByMatrix4(&n, &normalTransform);
                n.Normalize();
            }

            aiVector3D uv(0.f, 0.f, 0.f);
            if (mesh.HasTextureCoords(0)) uv = mesh.mTextureCoords[0][i];

            verts.push_back({p.x, p.y, p.z, n.x, n.y, n.z, uv.x, uv.y});
        }

        idxData.reserve(idxData.size() + mesh.mNumFaces * 3);
        for (uint32_t f = 0; f < mesh.mNumFaces; ++f) {
            const aiFace & face = mesh.mFaces[f];
            if (face.mNumIndices != 3) continue;
            idxData.push_back(baseVertex + face.mIndices[0]);
            idxData.push_back(baseVertex + face.mIndices[1]);
            idxData.push_back(baseVertex + face.mIndices[2]);
        }
    };

    std::function<void(const aiNode *, const aiMatrix4x4 &)> walkNode = [&](const aiNode * node, const aiMatrix4x4 & parentTransform) {
        const aiMatrix4x4 transform = parentTransform * node->mTransformation;
        for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
            const uint32_t meshIndex = node->mMeshes[i];
            if (meshIndex < scene->mNumMeshes && scene->mMeshes[meshIndex]) appendMesh(*scene->mMeshes[meshIndex], transform);
        }
        for (uint32_t i = 0; i < node->mNumChildren; ++i) {
            if (node->mChildren[i]) walkNode(node->mChildren[i], transform);
        }
    };

    aiMatrix4x4 identity;
    aiIdentityMatrix4(&identity);
    walkNode(scene->mRootNode, identity);
    aiReleaseImport(scene);

    if (verts.empty() || idxData.empty()) {
        GN_ERROR(sLogger)("PbrShading::load: GLTF '{}' contains no renderable triangles", gltfPath);
        return {};
    }

    const uint64_t vertexBytes = static_cast<uint64_t>(verts.size() * sizeof(Vtx));
    const uint64_t indexBytes  = static_cast<uint64_t>(idxData.size() * sizeof(uint32_t));
    auto           stgVB       = gpu2::Buffer::create("helmet_stg_vb", {.context = gpu, .size = vertexBytes, .mappable = true});
    auto           stgIB       = gpu2::Buffer::create("helmet_stg_ib", {.context = gpu, .size = indexBytes, .mappable = true});
    auto           vbuf        = gpu2::Buffer::create("helmet_vb", {.context = gpu, .size = vertexBytes});
    auto           ibuf        = gpu2::Buffer::create("helmet_ib", {.context = gpu, .size = indexBytes});
    if (!stgVB || !stgIB || !vbuf || !ibuf) GN_UNLIKELY return {};

    {
        auto m = stgVB->map();
        if (!m.data()) GN_UNLIKELY return {};
        std::memcpy(m.data(), verts.data(), static_cast<size_t>(vertexBytes));
    }
    {
        auto m = stgIB->map();
        if (!m.data()) GN_UNLIKELY return {};
        std::memcpy(m.data(), idxData.data(), static_cast<size_t>(indexBytes));
    }

    cnc.copyBufferToBuffer({.src = stgVB, .dst = vbuf, .size = vertexBytes});
    cnc.copyBufferToBuffer({.src = stgIB, .dst = ibuf, .size = indexBytes});

    gpu2::RasterGeometry geom;
    geom.format = stdVertexFormat();
    geom.vertices.append({.buffer = vbuf, .offset = 0, .stride = sizeof(Vtx)});
    geom.vertexCount = static_cast<uint32_t>(verts.size());
    geom.indices     = {.buffer = ibuf, .offset = 0, .stride = sizeof(uint32_t)};
    geom.indexCount  = static_cast<uint32_t>(idxData.size());
    return geom;
}

struct PbrAssetImpl final : public PbrShading::Asset {
    GN_REGISTER_RUNTIME_TYPE(Asset);

    PbrAssetData asset;

    explicit PbrAssetImpl(PbrAssetData value): Asset(TYPE_INFO(), "PbrAssetImpl"), asset(std::move(value)) { gpuPayload = asset.uploadPayload; }
};

// ─────────────────────────────────────────────────────────────────────────────
// PbrShading static members
// ─────────────────────────────────────────────────────────────────────────────

GN_API AutoRef<PbrShading::Asset> PbrShading::load(AutoRef<gpu2::GpuContext> gpu, const LoadParameters & params) {
    if (!gpu) {
        GN_ERROR(sLogger)("PbrShading::load: null gpu");
        return {};
    }

    auto vs = gpu2::GpuShader::create({.context = gpu, .name = "pbr.vert", .binary = kPbrVertSpv, .size = sizeof(kPbrVertSpv)});
    auto ps = gpu2::GpuShader::create({.context = gpu, .name = "pbr.frag", .binary = kPbrFragSpv, .size = sizeof(kPbrFragSpv)});
    if (!vs || !ps) {
        GN_ERROR(sLogger)("loadPbrAsset: failed to compile PBR shaders");
        return {};
    }

    // v1: default 1×1 textures + unit cube, published synchronously.
    auto cnc = gpu2::GpuCnC::create({.gpu = gpu});
    if (!cnc) GN_UNLIKELY return {};

    auto albedo     = make1x1Tex(gpu, *cnc, "pbr.albedo_def", 255, 255, 255, 255, 1);
    auto normal     = make1x1Tex(gpu, *cnc, "pbr.normal_def", 128, 128, 255, 255, 1);
    auto emissive   = make1x1Tex(gpu, *cnc, "pbr.emissive_def", 0, 0, 0, 255, 1);
    auto occlusion  = make1x1Tex(gpu, *cnc, "pbr.occlusion_def", 255, 255, 255, 255, 1);
    auto metalRough = make1x1Tex(gpu, *cnc, "pbr.metalrough_def", 0, 128, 0, 255, 1);
    auto [vb, ib]   = makeUnitCubeStaged(gpu, *cnc);

    PbrAssetData asset;
    asset.vs         = vs;
    asset.ps         = ps;
    asset.albedo     = albedo;
    asset.normal     = normal;
    asset.emissive   = emissive;
    asset.occlusion  = occlusion;
    asset.metalRough = metalRough;
    if (vb && ib) {
        asset.geometry.format = stdVertexFormat();
        asset.geometry.vertices.append({.buffer = vb, .offset = 0, .stride = 32});
        asset.geometry.vertexCount = 24;
        asset.geometry.indices     = {.buffer = ib, .offset = 0, .stride = sizeof(uint16_t)};
        asset.geometry.indexCount  = 36;
    }

    auto geom = loadGltfGeometryStaged(gpu, *cnc, params.gltfPath);
    if (geom.vertexCount > 0 && geom.indexCount > 0) asset.geometry = std::move(geom);

    auto tryLoad = [&](const StrA & path, const StrA & name, AutoRef<gpu2::Texture> & outTex) {
        if (path.empty()) return;
        auto stg = gpu2::Buffer::loadTextureToStagingBuffer(name, gpu, path);
        if (stg.empty()) return;
        auto tex = gpu2::Texture::create(name, {.context = gpu, .descriptor = stg.descriptor});
        if (!tex) return;
        cnc->copyBufferToImage(stg, tex);
        outTex = tex;
    };
    tryLoad(params.albedoPath, "pbr.albedo", asset.albedo);
    tryLoad(params.normalPath, "pbr.normal", asset.normal);
    tryLoad(params.emissivePath, "pbr.emissive", asset.emissive);
    tryLoad(params.occlusionPath, "pbr.occlusion", asset.occlusion);
    tryLoad(params.metalRoughPath, "pbr.metalrough", asset.metalRough);

    asset.uploadPayload = cnc->seal();
    if (!asset.uploadPayload || asset.geometry.vertexCount == 0 || asset.geometry.indexCount == 0) GN_UNLIKELY return {};

    return AutoRef<PbrAssetImpl>(new PbrAssetImpl(std::move(asset)));
}

GN_API gpu2::GpuRaster::DrawParameters PbrShading::getDrawParams(const SharedShaderConstants::Snapshot & sscSnapshot, AutoRef<const Asset> pbrAsset,
                                                                 const glm::mat4 & worldTransform) {
    auto * content = RuntimeType::cast<PbrAssetImpl>(pbrAsset.get());
    if (!content) GN_UNLIKELY return {};
    const auto & asset = content->asset;

    gpu2::GpuRaster::DrawParameters dp;
    dp.vs                = asset.vs;
    dp.ps                = asset.ps;
    dp.states.cullMode   = gpu2::RasterState::CULL_BACK;
    dp.states.frontFace  = gpu2::RasterState::FRONT_CCW;
    dp.states.depthState = gpu2::RasterState::DepthState {gpu2::RasterState::Compare::LESS, true};

    // 128-byte push constant: world matrix | normal matrix (pre-computed on CPU)
    glm::mat4 normalTransform = glm::transpose(glm::inverse(worldTransform));
    struct PC {
        glm::mat4 world, normal;
    };
    static_assert(sizeof(PC) == 128);
    PC pc {worldTransform, normalTransform};
    dp.immediates = referenceTo(new SimpleBlob<uint8_t>(sizeof(pc), reinterpret_cast<const uint8_t *>(&pc)));

    dp.resources.resize(2);
    dp.resources[0] = sscSnapshot.set0Resources;

    auto & set1 = dp.resources[1];
    set1.resize(5);
    auto bindTex = [&](uint32_t slot, const AutoRef<gpu2::Texture> & tex) {
        set1[slot].resize(1);
        set1[slot][0].resource = tex;
        set1[slot][0].setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
    };
    bindTex(0, asset.albedo);
    bindTex(1, asset.normal);
    bindTex(2, asset.emissive);
    bindTex(3, asset.occlusion);
    bindTex(4, asset.metalRough);
    dp.geometry = asset.geometry;

    return dp;
}

} // namespace GN::rdg2
