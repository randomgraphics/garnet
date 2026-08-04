// visual.cpp — the official engine2 visual layer: the concrete Camera and the GPU-backed
// VisualDomain. The VisualDomain consumes a self-contained VisualMomentImpl snapshot and
// renders its renderables (lit by its point lights, observed by its first camera) to a
// window via the gpu2 layer. It is the real visual domain implementation, independent of
// the Simple world that happens to be exercising it.

#include "e2-internal.h"

#include <garnet/GNfx2.h>
#include <garnet/GNwin.h>

#include <glm/gtc/matrix_transform.hpp>

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

/// RDG2 relic adapter for E2's immutable, ref-counted visual snapshot. The wrapper is the
/// only cross-domain representation: RDG2 sees an Entity while E2 retains ownership and
/// type-checking of the underlying moment.
struct VisualMomentEntity final : rdg2::Entity {
    GN_REGISTER_RUNTIME_TYPE(rdg2::Entity);

    Ref<VisualMoment> moment;

    explicit VisualMomentEntity(Ref<VisualMoment> moment_): Entity(TYPE_INFO(), "visual-moment"), moment(std::move(moment_)) {}
};

/// One immutable publication of FX2's frame-global GPU resources. The artifact carrying
/// these snapshots is stable for the lifetime of the visual domain; its relic version is
/// what changes from frame to frame.
struct SscSnapshotEntity final : rdg2::Entity {
    GN_REGISTER_RUNTIME_TYPE(rdg2::Entity);

    fx2::SharedShaderConstants::Snapshot snapshot;

    explicit SscSnapshotEntity(fx2::SharedShaderConstants::Snapshot snapshot_): Entity(TYPE_INFO(), "ssc-snapshot"), snapshot(std::move(snapshot_)) {}
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
        mFrameEndQuest.clear();
        mRenderQuest.clear();
        mPrepareSscQuest.clear();
        mFrameBeginQuest.clear();
        mSscArtifact.clear();
        mBackbufferArtifact.clear();
        mMomentArtifact.clear();
        mSsc.clear();
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
            GN_ERROR(sLogger, "Failed to create GPU context.");
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
            GN_ERROR(sLogger, "Failed to create swapchain.");
            return false;
        }

        auto depthFormat = mGpu->caps().defaultDepthFormat;
        if (depthFormat == gfx::img::PixelFormat::UNKNOWN()) {
            GN_ERROR(sLogger, "GPU reports no usable depth format.");
            return false;
        }
        mDepth = Texture::create("e2-depth", Texture::CreateParameters {
                                                 .context    = mGpu,
                                                 .descriptor = Texture::Descriptor {}.setFormat(depthFormat).setDimensions(mWidth, mHeight).setLevels(1),
                                             });
        if (!mDepth) {
            GN_ERROR(sLogger, "Failed to create depth texture.");
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
            GN_ERROR(sLogger, "Failed to create box shaders.");
            return false;
        }

        mSsc = fx2::SharedShaderConstants::create({.gpu = mGpu});
        if (!mSsc) {
            GN_ERROR(sLogger, "Failed to create FX2 shared shader constants.");
            return false;
        }

        return initFrameGraph();
    }

    void render(Ref<VisualMoment> momentBase) override {
        auto * moment = RuntimeType::cast<VisualMomentImpl>(momentBase.get());
        if (!moment) return;

        if (mMomentArtifact->publish(AutoRef<rdg2::Entity>(new VisualMomentEntity(std::move(momentBase)))) == rdg2::Artifact::Version::OOO()) {
            GN_WARN(sLogger, "Failed to seal visual moment into a render-graph artifact; skipping frame.");
            return;
        }

        // Artifacts and quests are stable domain-owned identities, while the plan is a
        // frame-local compilation over the relics and workload available for this moment.
        auto plan = compileVisualFramePlan(mFrameBeginQuest, mPrepareSscQuest, mRenderQuest, mFrameEndQuest);
        if (!plan) {
            GN_WARN(sLogger, "Failed to compile visual frame plan; skipping frame.");
            return;
        }

        auto execution = rdg2::Execution::run({.plan = plan, .gpu = mGpu, .name = "e2-frame"});
        if (!execution || execution->status() != rdg2::Execution::Status::SUCCEEDED) GN_WARN(sLogger, "Visual frame execution failed.");
    }

private:
    struct GpuMesh {
        AutoRef<Buffer> vb;
        AutoRef<Buffer> ib;
        uint32_t        indexCount = 0;
    };

    bool initFrameGraph() {
        mMomentArtifact     = rdg2::Artifact::create("e2.visual-moment");
        mBackbufferArtifact = rdg2::Artifact::create("e2.backbuffer");
        mSscArtifact        = rdg2::Artifact::create("e2.shared-shader-constants");
        if (!mMomentArtifact || !mBackbufferArtifact || !mSscArtifact) {
            GN_ERROR(sLogger, "Failed to create persistent visual-domain artifacts.");
            return false;
        }

        mFrameBeginQuest = rdg2::createFrameBeginQuest({.swapchain = mSwapchain, .backbuffer = mBackbufferArtifact});

        rdg2::Quest::CreateParameters prepareSscParameters;
        prepareSscParameters.name = "e2-prepare-ssc";
        prepareSscParameters.artifactUses.append({.name = "visual-moment", .artifact = mMomentArtifact, .access = rdg2::ArtifactAccess::READ_ONLY});
        prepareSscParameters.artifactUses.append({.name = "ssc", .artifact = mSscArtifact, .access = rdg2::ArtifactAccess::DISCARD_WRITE});
        prepareSscParameters.execute = [this](rdg2::QuestContext & context) {
            auto momentRelic = context.read<VisualMomentEntity>(mMomentArtifact);
            if (!momentRelic) return rdg2::QuestResult::failed("visual moment is unavailable for SSC preparation");
            auto * visualMoment = RuntimeType::cast<VisualMomentImpl>(momentRelic->moment.get());
            if (!visualMoment) return rdg2::QuestResult::failed("unsupported visual moment type");

            auto snapshot = prepareSharedShaderConstants(*visualMoment);
            if (snapshot.set0Resources.empty()) return rdg2::QuestResult::failed("FX2 SSC snapshot has no resources");
            for (auto & payload : snapshot.set0Payloads) context.emit(payload);
            context.publish(mSscArtifact, AutoRef<rdg2::Entity>(new SscSnapshotEntity(std::move(snapshot))));
            return rdg2::QuestResult::succeeded();
        };
        mPrepareSscQuest = rdg2::Quest::create(prepareSscParameters);

        rdg2::Quest::CreateParameters renderParameters;
        renderParameters.name = "e2-visual-render";
        renderParameters.artifactUses.append({.name = "visual-moment", .artifact = mMomentArtifact, .access = rdg2::ArtifactAccess::READ_ONLY});
        renderParameters.artifactUses.append({.name = "ssc", .artifact = mSscArtifact, .access = rdg2::ArtifactAccess::READ_ONLY});
        renderParameters.artifactUses.append({.name = "backbuffer", .artifact = mBackbufferArtifact, .access = rdg2::ArtifactAccess::READ_WRITE});
        renderParameters.execute = [this](rdg2::QuestContext & context) {
            auto momentRelic = context.read<VisualMomentEntity>(mMomentArtifact);
            auto sscRelic    = context.read<SscSnapshotEntity>(mSscArtifact);
            auto frameRelic  = context.read<rdg2::SwapchainFrameEntity>(mBackbufferArtifact);
            if (!momentRelic || !sscRelic || !frameRelic)
                return rdg2::QuestResult::failed("visual moment, SSC snapshot, or acquired backbuffer is unavailable");

            auto * visualMoment = RuntimeType::cast<VisualMomentImpl>(momentRelic->moment.get());
            if (!visualMoment) return rdg2::QuestResult::failed("unsupported visual moment type");

            mRenderTarget.setColorTarget(0, frameRelic->frame.view);
            if (!recordVisualMoment(context, *visualMoment, sscRelic->snapshot)) return rdg2::QuestResult::failed("failed to record visual moment");

            // The physical swapchain frame is unchanged; publishing a new relic records the
            // semantic transition from acquired to rendered for downstream graph ordering.
            context.publish(mBackbufferArtifact, frameRelic.value);
            return rdg2::QuestResult::succeeded();
        };
        mRenderQuest   = rdg2::Quest::create(renderParameters);
        mFrameEndQuest = rdg2::createFrameEndQuest({.swapchain = mSwapchain, .backbuffer = mBackbufferArtifact});

        if (!mFrameBeginQuest || !mPrepareSscQuest || !mRenderQuest || !mFrameEndQuest) {
            GN_ERROR(sLogger, "Failed to create persistent visual-domain quests.");
            return false;
        }
        return true;
    }

    fx2::SharedShaderConstants::Snapshot prepareSharedShaderConstants(const VisualMomentImpl & moment) {
        const bool         haveCamera = !moment.cameras.empty();
        const WorldVector3 eye =
            haveCamera ? moment.cameras[0].position : WorldVector3(WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO());

        mSsc->set0.frameConstants.frameCounter = (int) ++mFrameCounter;
        mSsc->set0.directLighting.clear();
        for (const auto & light : moment.lights) {
            fx2::SharedShaderConstants::DirectLight direct;
            direct.type            = fx2::SharedShaderConstants::DirectLight::POINT;
            direct.point.position  = moment.scale.toMeters(spatial::toLocal(eye, light.position));
            direct.point.intensity = IntensityRGB {light.color.r, light.color.g, light.color.b, Candela {1.f}};
            mSsc->set0.directLighting.append(direct);
        }

        if (haveCamera) {
            const auto & camera                 = moment.cameras[0];
            mSsc->set0.camera.cameraPosition    = fx2::Location(0.f);
            mSsc->set0.camera.cameraOrientation = camera.orientation;
            mSsc->set0.camera.cameraFov         = ArcDegree(camera.fovYInDegree);
            mSsc->set0.camera.aspectRatio       = mHeight ? (float) mWidth / (float) mHeight : 1.f;
            mSsc->set0.camera.nearPlane         = moment.scale.toMeters(camera.nearPlane);
            mSsc->set0.camera.farPlane          = moment.scale.toMeters(camera.farPlane);
            mSsc->set0.camera.viewWidthInPixel  = mWidth;
            mSsc->set0.camera.viewHeightInPixel = mHeight;
            if (mSsc->set0.camera.nearPlane <= 0.f) mSsc->set0.camera.nearPlane = 0.1f;
            if (mSsc->set0.camera.farPlane <= mSsc->set0.camera.nearPlane) mSsc->set0.camera.farPlane = mSsc->set0.camera.nearPlane + 1000.f;
        }

        return mSsc->takeSnapshot();
    }

    bool recordVisualMoment(rdg2::QuestContext & context, const VisualMomentImpl & moment, const fx2::SharedShaderConstants::Snapshot & sscSnapshot) {
        const bool         haveCamera = !moment.cameras.empty();
        const WorldVector3 eye =
            haveCamera ? moment.cameras[0].position : WorldVector3(WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO());
        auto raster = GpuRaster::create("e2-visual", {.gpu = mGpu, .target = &mRenderTarget});
        if (!raster) return false;

        if (haveCamera) {
            for (auto & r : moment.renderables) {
                if (!r.mesh) continue;
                const GpuMesh * gpuMesh = ensureGpuMesh(*r.mesh);
                if (!gpuMesh) continue;

                RasterGeometry geom;
                geom.format.attributes.append(RasterGeometry::VertexAttribute {.location = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
                geom.format.attributes.append(RasterGeometry::VertexAttribute {.location = 1, .offset = 12, .format = RasterGeometry::AttributeFormat::F32_3});
                geom.vertices.append(RasterGeometry::GeometryBuffer {.buffer = gpuMesh->vb, .offset = 0, .stride = (uint32_t) sizeof(MeshData::Vertex)});
                geom.indices    = RasterGeometry::GeometryBuffer {.buffer = gpuMesh->ib, .offset = 0, .stride = sizeof(uint16_t)};
                geom.indexCount = gpuMesh->indexCount;

                DrawConstants dc;
                dc.model     = glm::translate(glm::mat4(1.f), moment.scale.toMeters(spatial::toLocal(eye, r.translation))) *
                               glm::mat4_cast(glm::normalize(r.rotation)) * glm::scale(glm::mat4(1.f), moment.scale.toMeters(r.scaling));
                dc.baseColor = glm::vec4(r.baseColor, 1.f);

                GpuRaster::DrawParameters dp;
                dp.vs       = mVs;
                dp.ps       = mPs;
                dp.geometry = geom;
                dp.resources.resize(1);
                dp.resources[0] = sscSnapshot.set0Resources;
                dp.immediates   = referenceTo(new SimpleBlob<uint8_t>(sizeof(dc), reinterpret_cast<const uint8_t *>(&dc)));
                raster->draw(dp);
            }
        }

        auto payload = raster->seal();
        if (!payload) return false;
        context.emit(payload);
        return true;
    }

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
            GN_ERROR(sLogger, "Failed to create mesh buffers.");
            return nullptr;
        }
        if (!gm.vb->setContent(ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(mesh.vertices.data()), vbSize)) ||
            !gm.ib->setContent(ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(mesh.indices.data()), ibSize))) {
            GN_ERROR(sLogger, "Failed to upload mesh buffers.");
            return nullptr;
        }
        gm.indexCount = (uint32_t) mesh.indices.size();
        auto inserted = mMeshCache.emplace(mesh.id, std::move(gm));
        return &inserted.first->second;
    }

    Universe &                           mUniverse;
    Ref<OperatingDomain>                 mOs;
    AutoRef<GpuContext>                  mGpu;
    intptr_t                             mSurface = 0; ///< owned; destroyed in ~VisualDomainImpl between swapchain and GPU context
    AutoRef<Swapchain>                   mSwapchain;
    AutoRef<Texture>                     mDepth;
    AutoRef<GpuShader>                   mVs, mPs;
    AutoRef<fx2::SharedShaderConstants>  mSsc;
    rdg2::ArtifactRef                    mMomentArtifact;
    rdg2::ArtifactRef                    mBackbufferArtifact;
    rdg2::ArtifactRef                    mSscArtifact;
    rdg2::QuestRef                       mFrameBeginQuest;
    rdg2::QuestRef                       mPrepareSscQuest;
    rdg2::QuestRef                       mRenderQuest;
    rdg2::QuestRef                       mFrameEndQuest;
    RasterTarget                         mRenderTarget;
    std::unordered_map<int64_t, GpuMesh> mMeshCache;
    uint32_t                             mFrameCounter = 0;
    uint32_t                             mWidth        = 1280;
    uint32_t                             mHeight       = 720;
};

#endif // GN_BUILD_HAS_VULKAN

} // namespace

namespace GN::e2 {

rdg2::PlanRef compileVisualFramePlan(rdg2::QuestRef frameBegin, rdg2::QuestRef prepareSsc, rdg2::QuestRef visualRender, rdg2::QuestRef frameEnd) {
    rdg2::Plan::CompileParameters parameters;
    parameters.quests.append(std::move(frameBegin));
    parameters.quests.append(std::move(prepareSsc));
    parameters.quests.append(std::move(visualRender));
    parameters.quests.append(std::move(frameEnd));
    return rdg2::Plan::compile(parameters);
}

Ref<Camera> Camera::create(const CreateParameters & cp) {
    if (!cp.domain) {
        GN_ERROR(GN::getLogger("GN.e2.visual"), "Camera::create requires a non-null visual domain.");
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
    GN_ERROR(GN::getLogger("GN.e2.visual"), "VisualDomain requires a Vulkan-enabled build.");
    return {};
#endif
}

} // namespace GN::e2
