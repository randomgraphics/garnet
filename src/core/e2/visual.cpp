// visual.cpp — the official engine2 visual layer: the concrete Camera and the GPU-backed
// VisualDomain. The VisualDomain consumes a self-contained VisualMomentImpl snapshot and
// renders its renderables (lit by its point lights, observed by its first camera) to a
// window via the gpu2 layer. It is the real visual domain implementation, independent of
// the Simple world that happens to be exercising it.

#include "e2-internal.h"

#include <garnet/GNwin.h>

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>
#include <unordered_map>

#if GN_BUILD_HAS_VULKAN
    #include "box-vert.spv.h"
    #include "box-frag.spv.h"
#endif

using namespace GN;
using namespace GN::e2;
using namespace GN::gpu2;

namespace {

GN::Logger * sLogger = GN::getLogger("GN.e2.visual");

// ---------------------------------------------------------------------------
// Camera
// ---------------------------------------------------------------------------

struct CameraImpl : Camera {
    GN_REGISTER_RUNTIME_TYPE(Camera);

    explicit CameraImpl(Universe & u): Camera(TYPE_INFO(), u.generateUniqueIdentifier(), "camera") {}
};

#if GN_BUILD_HAS_VULKAN

// ---------------------------------------------------------------------------
// VisualDomain
// ---------------------------------------------------------------------------

struct VisualDomainImpl : VisualDomain {
    GN_REGISTER_RUNTIME_TYPE(VisualDomain);

    explicit VisualDomainImpl(Universe & u): VisualDomain(TYPE_INFO(), u.generateUniqueIdentifier(), "visual-domain"), mUniverse(u) {}

    ~VisualDomainImpl() override {
        if (!mGpu) return;
        mGpu->waitForIdle();
        // Vulkan teardown must nest: swapchain-image views → swapchain → surface → instance.
        // Member destruction alone can't order the surface (a raw handle we own) between the
        // swapchain and the instance, so unwind explicitly while mGpu keeps the instance alive.
        mRenderTarget.setColorTarget(0, {});
        mFrameUbo.clear();
        mSwapchain.clear();
        if (mSurface && mOs) {
            mOs->destroyRenderSurface(mGpu->getVulkanInstanceHandle(), mSurface);
            mSurface = 0;
        }
    }

    Universe & universe() const override { return mUniverse; }

    bool init(const CreateParameters & cp) {
        mOs = cp.os;

        mGpu = GpuContext::create("e2-gpu", GpuContext::CreateParameters {});
        if (!mGpu) {
            GN_ERROR(sLogger)("Failed to create GPU context.");
            return false;
        }

        if (mOs) {
            // The domain owns this surface from here on; the destructor destroys it between
            // the swapchain and the GPU context (i.e. the Vulkan instance).
            mSurface      = mOs->createRenderSurface(mGpu->getVulkanInstanceHandle());
            auto clientSz = mOs->clientSize();
            if (clientSz.x && clientSz.y) {
                mWidth  = clientSz.x;
                mHeight = clientSz.y;
            }
        }

        Swapchain::CreateDesc scDesc;
        scDesc.setGpu(mGpu).setName("e2-swapchain").setDimensions(mWidth, mHeight);
        if (mSurface) scDesc.setSurface(mSurface);
        mSwapchain = Swapchain::create(scDesc);
        if (!mSwapchain) {
            GN_ERROR(sLogger)("Failed to create swapchain.");
            return false;
        }

        auto depthFormat = mGpu->caps().defaultDepthFormat;
        if (depthFormat == gfx::img::PixelFormat::UNKNOWN()) {
            GN_ERROR(sLogger)("GPU reports no usable depth format.");
            return false;
        }
        mDepth = Texture::create("e2-depth", Texture::CreateParameters {
                                                 .context    = mGpu,
                                                 .descriptor = Texture::Descriptor {}.setFormat(depthFormat).setDimensions(mWidth, mHeight).setLevels(1),
                                             });
        if (!mDepth) {
            GN_ERROR(sLogger)("Failed to create depth texture.");
            return false;
        }

        GpuResourceView depthView;
        depthView.resource = mDepth;
        mRenderTarget.colorTargets.append(RasterTarget::ColorTarget {});
        mRenderTarget.setDepthStencilTarget(depthView);
        mRenderTarget.setClearColor(0.05f, 0.06f, 0.09f, 1.0f);
        mRenderTarget.setClearDepth(1.0f);
        mRenderTarget.states.setCullMode(RasterState::CULL_BACK);
        mRenderTarget.states.setDepthState(RasterState::DepthState {.func = RasterState::Compare::LESS, .write = true});

        mVs = GpuShader::create({.context = mGpu, .name = "e2-box-vs", .binary = kBoxVertSpv, .size = sizeof(kBoxVertSpv), .entry = "main"});
        mPs = GpuShader::create({.context = mGpu, .name = "e2-box-ps", .binary = kBoxFragSpv, .size = sizeof(kBoxFragSpv), .entry = "main"});
        if (!mVs || !mPs) {
            GN_ERROR(sLogger)("Failed to create box shaders.");
            return false;
        }

        mFrameUbo = Buffer::create("e2-frame-ubo", {.context = mGpu, .size = sizeof(FrameConstants)});
        if (!mFrameUbo) {
            GN_ERROR(sLogger)("Failed to create frame uniform buffer.");
            return false;
        }

        return true;
    }

    void render(Ref<VisualMoment> momentBase) override {
        auto * moment = RuntimeType::cast<VisualMomentImpl>(momentBase.get());
        if (!moment) return;

        Swapchain::Frame frame = mSwapchain->prepare();
        if (frame.view.empty()) {
            GN_WARN(sLogger)("Swapchain prepare() failed; skipping frame.");
            return;
        }
        mRenderTarget.setColorTarget(0, frame.view);

        // Fill the per-frame constants from the first camera and the lights. Rendering is
        // camera-relative: every absolute position is rebased against the primary camera in
        // exact integer space, and only the resulting local delta converts to float meters.
        FrameConstants fc             = {};
        fc.ambient                    = glm::vec4(0.04f, 0.04f, 0.05f, 0.f);
        const bool         haveCamera = moment->cameras.size() > 0;
        const WorldVector3 eye =
            haveCamera ? moment->cameras[0].position : WorldVector3(WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO());
        if (haveCamera) fc.viewProj = buildViewProj(moment->cameras[0], moment->scale);
        int lightCount = (int) std::min<size_t>(moment->lights.size(), kMaxLights);
        for (int i = 0; i < lightCount; ++i) {
            fc.lightPosition[i] = glm::vec4(moment->scale.toMeters(spatial::toLocal(eye, moment->lights[i].position)), 1.f);
            fc.lightColor[i]    = glm::vec4(moment->lights[i].color, 0.f);
        }
        fc.lightCount = lightCount;

        auto cnc = GpuCnC::create({.gpu = mGpu});
        if (!cnc) {
            GN_WARN(sLogger)("Failed to create per-frame upload command list; skipping frame.");
            return;
        }
        cnc->uploadBuffer(mFrameUbo, 0, ArrayProxy<const uint8_t>(reinterpret_cast<const uint8_t *>(&fc), sizeof(fc)));
        AutoRef<GpuPayload> constantsUpload = cnc->seal();
        if (!constantsUpload) {
            GN_WARN(sLogger)("Failed to seal per-frame constants upload; skipping frame.");
            return;
        }

        GpuResourceView uboView;
        uboView.resource = mFrameUbo;
        uboView.setBufferViewType(GpuResourceView::BufferView::UNIFORM).setBufferViewOffset(0).setBufferViewSize(sizeof(FrameConstants));
        GpuResourceTable resources;
        resources.resize(1);
        resources[0].resize(1);
        resources[0][0].resize(1);
        resources[0][0][0] = uboView;

        auto raster = GpuRaster::create("e2-visual", {.gpu = mGpu, .target = &mRenderTarget});
        if (!raster) {
            GN_WARN(sLogger)("Failed to create raster pass; skipping frame.");
            return;
        }

        // Only draw geometry when there is a camera to view it through.
        if (haveCamera) {
            for (auto & r : moment->renderables) {
                if (!r.mesh) continue;
                const GpuMesh * gpuMesh = ensureGpuMesh(*r.mesh);
                if (!gpuMesh) continue;

                RasterGeometry geom;
                geom.format.attributes.append(RasterGeometry::VertexAttribute {.location = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
                geom.format.attributes.append(RasterGeometry::VertexAttribute {.location = 1, .offset = 12, .format = RasterGeometry::AttributeFormat::F32_3});
                geom.vertices.append(RasterGeometry::GeometryBuffer {.buffer = gpuMesh->vb, .offset = 0, .stride = (uint32_t) sizeof(MeshData::Vertex)});
                geom.indices    = RasterGeometry::GeometryBuffer {.buffer = gpuMesh->ib, .offset = 0, .stride = sizeof(uint16_t)};
                geom.indexCount = gpuMesh->indexCount;

                // Moment transforms stay in world units; the camera-relative delta converts to
                // float meters only here.
                DrawConstants dc;
                dc.model     = glm::translate(glm::mat4(1.f), moment->scale.toMeters(spatial::toLocal(eye, r.translation))) *
                               glm::mat4_cast(glm::normalize(r.rotation)) * glm::scale(glm::mat4(1.f), moment->scale.toMeters(r.scaling));
                dc.baseColor = glm::vec4(r.baseColor, 1.f);

                GpuRaster::DrawParameters dp;
                dp.vs         = mVs;
                dp.ps         = mPs;
                dp.geometry   = geom;
                dp.resources  = resources;
                dp.immediates = referenceTo(new SimpleBlob<uint8_t>(sizeof(dc), reinterpret_cast<const uint8_t *>(&dc)));
                raster->draw(dp);
            }
        }

        AutoRef<GpuPayload> payload = raster->seal();
        if (!payload) {
            GN_WARN(sLogger)("Failed to seal raster pass; skipping frame.");
            return;
        }

        GpuContext::SubmitParameters submit("e2-frame");
        submit.waitFor(frame.ready).appendWork(constantsUpload).appendWork(payload);
        mGpu->submit(submit);
        mSwapchain->present(*payload);
    }

private:
    struct GpuMesh {
        AutoRef<Buffer> vb;
        AutoRef<Buffer> ib;
        uint32_t        indexCount = 0;
    };

    // Upload a mesh's GPU buffers on first sight; reuse the cached buffers afterwards.
    const GpuMesh * ensureGpuMesh(const MeshData & mesh) {
        auto it = mMeshCache.find(mesh.id);
        if (it != mMeshCache.end()) return &it->second;

        GpuMesh        gm;
        const uint64_t vbSize = mesh.vertices.size() * sizeof(MeshData::Vertex);
        const uint64_t ibSize = mesh.indices.size() * sizeof(uint16_t);
        gm.vb                 = Buffer::create("e2-mesh-vb", {.context = mGpu, .size = vbSize});
        gm.ib                 = Buffer::create("e2-mesh-ib", {.context = mGpu, .size = ibSize});
        if (!gm.vb || !gm.ib) {
            GN_ERROR(sLogger)("Failed to create mesh buffers.");
            return nullptr;
        }
        if (!gm.vb->setContent(ArrayProxy<const uint8_t>(reinterpret_cast<const uint8_t *>(mesh.vertices.data()), vbSize)) ||
            !gm.ib->setContent(ArrayProxy<const uint8_t>(reinterpret_cast<const uint8_t *>(mesh.indices.data()), ibSize))) {
            GN_ERROR(sLogger)("Failed to upload mesh buffers.");
            return nullptr;
        }
        gm.indexCount = (uint32_t) mesh.indices.size();
        auto inserted = mMeshCache.emplace(mesh.id, std::move(gm));
        return &inserted.first->second;
    }

    glm::mat4 buildViewProj(const Camera::Desc & cam, PhysicalScale scale) const {
        // Rendering is camera-relative: positions are rebased against the camera before any
        // float conversion, so the camera sits at the origin and the view carries no translation.
        glm::quat orientation = cam.orientation;
        if (glm::dot(orientation, orientation) < 1e-8f) orientation = glm::quat(1.f, 0.f, 0.f, 0.f);
        orientation = glm::normalize(orientation);

        glm::mat4 view = glm::inverse(glm::mat4_cast(orientation));

        float aspect = (mHeight > 0) ? (float) mWidth / (float) mHeight : 1.f;
        float fovY   = glm::radians(std::clamp(cam.fovYInDegree, 1.f, 179.f));
        float nearP  = scale.toMeters(cam.nearPlane);
        float farP   = scale.toMeters(cam.farPlane);
        if (nearP <= 0.f) nearP = 0.1f;
        if (farP <= nearP) farP = nearP + 1000.f;

        glm::mat4 proj = glm::perspectiveRH_ZO(fovY, aspect, nearP, farP);
        proj[1][1] *= -1.f; // flip Y for Vulkan clip space
        return proj * view;
    }

    Universe &                                    mUniverse;
    Ref<OperatingDomain>                          mOs;
    AutoRef<GpuContext>                           mGpu;
    intptr_t                                      mSurface = 0; ///< owned; destroyed in ~VisualDomainImpl between swapchain and GPU context
    AutoRef<Swapchain>                            mSwapchain;
    AutoRef<Texture>                              mDepth;
    AutoRef<GpuShader>                            mVs, mPs;
    AutoRef<Buffer>                               mFrameUbo;
    RasterTarget                                  mRenderTarget;
    std::unordered_map<UniqueIdentifier, GpuMesh> mMeshCache;
    uint32_t                                      mWidth  = 1280;
    uint32_t                                      mHeight = 720;
};

#endif // GN_BUILD_HAS_VULKAN

} // namespace

namespace GN::e2 {

Ref<Camera> Camera::create(const CreateParameters & cp) {
    if (!cp.domain) {
        GN_ERROR(GN::getLogger("GN.e2.visual"))("Camera::create requires a non-null visual domain.");
        return {};
    }
    return referenceTo(new CameraImpl(cp.domain->universe()));
}

Ref<VisualDomain> VisualDomain::create(const CreateParameters & cp) {
#if GN_BUILD_HAS_VULKAN
    auto d = referenceTo(new VisualDomainImpl(cp.universe));
    if (!d->init(cp)) return {};
    return d;
#else
    (void) cp;
    GN_ERROR(GN::getLogger("GN.e2.visual"))("VisualDomain requires a Vulkan-enabled build.");
    return {};
#endif
}

} // namespace GN::e2
