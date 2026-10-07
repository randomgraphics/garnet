// asset.cpp — Assets service implementation for engine2 basis.
// Manages in-game visual assets such as meshes and textures on the GPU.

#include <garnet/GNengine2.h>

using namespace GN;
using namespace GN::e2;
using namespace GN::e2::basis;
using namespace GN::gpu2;

namespace {

using MeshId    = Assets::MeshId;
using TextureId = Assets::TextureId;

struct MeshImpl : Assets::Mesh {
    GN_REGISTER_RUNTIME_TYPE(Assets::Mesh);

    RasterGeometry mGeometry;

    MeshImpl(MeshId id, const StrA & name, RasterGeometry geom): Assets::Mesh(TYPE_INFO(), id, name), mGeometry(std::move(geom)) {}

    const RasterGeometry & geometry() const override { return mGeometry; }
};

struct TextureImpl : Assets::Texture {
    GN_REGISTER_RUNTIME_TYPE(Assets::Texture);

    AutoRef<gpu2::Texture> mTexture;

    TextureImpl(TextureId id, const StrA & name, AutoRef<gpu2::Texture> tex): Assets::Texture(TYPE_INFO(), id, name), mTexture(std::move(tex)) {}

    AutoRef<gpu2::Texture> texture() const override { return mTexture; }
};

struct AssetsImpl : Assets {
    GN_REGISTER_RUNTIME_TYPE(Assets);

    AutoRef<GpuContext>                 mGpu;
    Dictionary<MeshId, Ref<Mesh>>       mMeshes;
    Dictionary<TextureId, Ref<Texture>> mTextures;
    uint64_t                            mNextMeshId    = 100;
    uint64_t                            mNextTextureId = 100;

    AssetsImpl(Universe & u, AutoRef<GpuContext> gpu): Assets(TYPE_INFO(), u.generateUniqueIdentifier(), "assets"), mGpu(std::move(gpu)) {}

    struct BoxVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    static gpu2::RasterGeometry createBoxGeometry(AutoRef<GpuContext> gpu, GpuCnC & uploads, float halfExtent) {
        using AF = gpu2::RasterGeometry::AttributeFormat;
        gpu2::RasterGeometry geometry;
        geometry.format.attributes.push_back({.location = 0, .binding = 0, .offset = 0, .format = AF::F32_3});
        geometry.format.attributes.push_back({.location = 1, .binding = 0, .offset = 12, .format = AF::F32_3});
        geometry.format.attributes.push_back({.location = 2, .binding = 0, .offset = 24, .format = AF::F32_2});

        const float     h          = halfExtent;
        const glm::vec3 corners[8] = {
            {-h, -h, -h}, {h, -h, -h}, {h, h, -h}, {-h, h, -h}, {-h, -h, h}, {h, -h, h}, {h, h, h}, {-h, h, h},
        };
        static const glm::vec3 normals[6] = {
            {0, 0, -1}, {0, -1, 0}, {1, 0, 0}, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1},
        };
        static const int faceCorners[6][4] = {
            {0, 1, 2, 3}, {0, 4, 5, 1}, {1, 5, 6, 2}, {2, 6, 7, 3}, {3, 7, 4, 0}, {7, 6, 5, 4},
        };
        static const glm::vec2 uvs[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

        DynaArray<BoxVertex> verts;
        DynaArray<uint16_t>  indices;
        for (int f = 0; f < 6; ++f) {
            uint16_t base = static_cast<uint16_t>(verts.size());
            for (int v = 0; v < 4; ++v) { verts.append({corners[faceCorners[f][v]], normals[f], uvs[v]}); }
            indices.append(base + 2);
            indices.append(base + 1);
            indices.append(base + 0);
            indices.append(base + 3);
            indices.append(base + 2);
            indices.append(base + 0);
        }

        const uint32_t stride = sizeof(BoxVertex);
        auto           vb     = gpu2::Buffer::create("e2.box.vb", {.context = gpu, .size = verts.size() * stride});
        auto           ib     = gpu2::Buffer::create("e2.box.ib", {.context = gpu, .size = indices.size() * sizeof(uint16_t)});
        if (!vb || !ib) return {};

        uploads.recordUploadBuffer(vb, 0, {reinterpret_cast<const uint8_t *>(verts.data()), verts.size() * stride});
        uploads.recordUploadBuffer(ib, 0, {reinterpret_cast<const uint8_t *>(indices.data()), indices.size() * sizeof(uint16_t)});

        geometry.vertices.push_back({.buffer = vb, .offset = 0, .stride = stride});
        geometry.indices     = {.buffer = ib, .offset = 0, .stride = sizeof(uint16_t)};
        geometry.vertexCount = static_cast<uint32_t>(verts.size());
        geometry.indexCount  = static_cast<uint32_t>(indices.size());
        return geometry;
    }

    bool init() {
        if (!mGpu) return true; // Headless / GPU-less support

        auto uploads = GpuCnC::create({.gpu = mGpu});
        if (!uploads) return false;
        auto geometry = createBoxGeometry(mGpu, *uploads, 1.0f);
        if (geometry.vertices.empty()) return false;
        auto payload = uploads->seal();
        if (!payload) return false;
        // Built-in geometry is a one-shot initialization, completed before assets are exposed.
        mGpu->submit(GpuContext::SubmitParameters("e2-box-init").appendWork(payload));
        mGpu->waitForIdle();
        mMeshes[MESH_BOX] = referenceTo(new MeshImpl(MESH_BOX, "box", std::move(geometry)));

        return true;
    }

    Ref<Mesh> registerMesh(const StrA & assetName, const gpu2::RasterGeometry & geom) override {
        MeshId assetId   = reinterpret_cast<MeshId>((uintptr_t) mNextMeshId++);
        auto   mesh      = referenceTo(new MeshImpl(assetId, assetName, geom));
        mMeshes[assetId] = mesh;
        return mesh;
    }

    Ref<Mesh> findMesh(MeshId assetId) const override {
        auto * ptr = mMeshes.find(assetId);
        return ptr ? *ptr : Ref<Mesh> {};
    }

    Ref<Texture> registerTexture(const StrA & assetName, AutoRef<gpu2::Texture> tex) override {
        TextureId assetId  = reinterpret_cast<TextureId>((uintptr_t) mNextTextureId++);
        auto      texture  = referenceTo(new TextureImpl(assetId, assetName, std::move(tex)));
        mTextures[assetId] = texture;
        return texture;
    }

    Ref<Texture> findTexture(TextureId assetId) const override {
        auto * ptr = mTextures.find(assetId);
        return ptr ? *ptr : Ref<Texture> {};
    }
};

} // namespace

namespace GN::e2::basis {

Ref<Assets> Assets::create(const CreateParameters & cp) {
    auto mgr = referenceTo(new AssetsImpl(cp.universe, cp.gpu));
    if (!mgr->init()) return {};
    return mgr;
}

} // namespace GN::e2::basis
