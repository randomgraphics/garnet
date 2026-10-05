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

    bool init() {
        if (!mGpu) return true; // Headless / GPU-less support

        auto uploads = GpuCnC::create({.gpu = mGpu});
        if (!uploads) return false;
        fx2::LitKernelInputs::CubeCreateOptions options;
        options.width = options.height = options.depth = 2.f;
        options.uv = options.tangent = false;
        auto geometry                = fx2::LitKernelInputs::createBox(mGpu, *uploads, options);
        if (geometry.vertices.empty()) return false;
        auto payload = uploads->seal();
        if (!payload) return false;
        // Built-in geometry is a one-shot initialization, completed before assets are exposed.
        mGpu->submit(GpuContext::SubmitParameters("e2-box-init").appendWork(payload));
        mGpu->waitForIdle();
        mMeshes[MESH_BOX] = referenceTo(new MeshImpl(MESH_BOX, "box", std::move(geometry)));

        return true;
    }

    Ref<Mesh> registerMesh(const StrA & name, const gpu2::RasterGeometry & geom) override {
        MeshId id   = reinterpret_cast<MeshId>((uintptr_t) mNextMeshId++);
        auto   mesh = referenceTo(new MeshImpl(id, name, geom));
        mMeshes[id] = mesh;
        return mesh;
    }

    Ref<Mesh> findMesh(MeshId id) const override {
        auto * ptr = mMeshes.find(id);
        return ptr ? *ptr : Ref<Mesh> {};
    }

    Ref<Texture> registerTexture(const StrA & name, AutoRef<gpu2::Texture> tex) override {
        TextureId id      = reinterpret_cast<TextureId>((uintptr_t) mNextTextureId++);
        auto      texture = referenceTo(new TextureImpl(id, name, std::move(tex)));
        mTextures[id]     = texture;
        return texture;
    }

    Ref<Texture> findTexture(TextureId id) const override {
        auto * ptr = mTextures.find(id);
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
