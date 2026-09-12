// Visual tableaux keep snapshot organization private to E2. The domain prepares
// frame resources, then each moment records its own work into the shared raster.

#include "e2-internal.h"

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

struct SceneResources final : RCRT64 {
    GN_REGISTER_RUNTIME_TYPE(RCRT64);

    explicit SceneResources(AutoRef<GpuContext> gpu): RCRT64(TYPE_INFO(), "scene-resources"), mGpu(std::move(gpu)) {}

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

    AutoRef<fx2::ModelAsset> ensureGpuModel(VisualMoment::RenderContext & context, AutoRef<const fx2::ModelScene> scene) {
        auto found = mModelCache.find(scene->id);
        if (found != mModelCache.end()) return found->second;

        const auto sceneId = scene->id;
        auto       model   = fx2::ModelAsset::create(mGpu, std::move(scene));
        if (!model) {
            GN_ERROR(sLogger, "Failed to create GPU model asset.");
            return {};
        }
        context.upload(model->uploadPayload());
        mModelCache.emplace(sceneId, model);
        return model;
    }

    AutoRef<GpuContext>                                   mGpu;
    AutoRef<GpuShader>                                    mVs, mPs;
    AutoRef<fx2::ModelShading::Asset>                     mModelShading;
    std::unordered_map<int64_t, GpuMesh>                  mMeshCache;
    std::unordered_map<int64_t, AutoRef<fx2::ModelAsset>> mModelCache;
    bool                                                  mModelShadingPayloadEmitted = false;
};

/// Scene resource caches stay private. All moments can access prepared FX2 constants
/// through VisualMoment::RenderContext without learning about these caches or RDG2.
struct VisualRecordContext final : VisualMoment::RenderContext {
    GN_REGISTER_RUNTIME_TYPE(VisualMoment::RenderContext);

    GpuRaster &                                  target;
    SceneResources &                             resources;
    const fx2::SharedShaderConstants::Snapshot & snapshot;
    std::function<void(AutoRef<GpuPayload>)>     emit;

    VisualRecordContext(GpuRaster & raster, SceneResources & resources_, const fx2::SharedShaderConstants::Snapshot & snapshot_,
                        std::function<void(AutoRef<GpuPayload>)> emit_)
        : RenderContext(TYPE_INFO()), target(raster), resources(resources_), snapshot(snapshot_), emit(std::move(emit_)) {}

    const fx2::SharedShaderConstants::Snapshot & ssc() const override { return snapshot; }

    GpuRaster & raster() const override { return target; }
    void        upload(AutoRef<GpuPayload> payload) override { emit(std::move(payload)); }
};

/// Environment-owned FX2 state keeps texture caches tied to the supplying moment,
/// so dropping that moment cannot leak the previous frame's lighting into a new root.
struct VisualEnvironmentImpl final : VisualEnvironment {
    GN_REGISTER_RUNTIME_TYPE(VisualEnvironment);

    AutoRef<GpuContext>                 gpu;
    AutoRef<fx2::SharedShaderConstants> constants;

    const Desc description;

    explicit VisualEnvironmentImpl(const CreateParameters & cp)
        : VisualEnvironment(TYPE_INFO(), cp.universe.generateUniqueIdentifier(), "visual-environment"), gpu(cp.gpu), description(cp.description) {}

    bool record(RenderContext & context) const override {
        context.raster().draw(constants->getSkyboxDrawParams(context.ssc().set0Resources));
        return true;
    }
};

/// RDG2 relic adapter for E2's immutable, ref-counted visual snapshot. The wrapper is the
/// only cross-domain representation: RDG2 sees an Entity while E2 retains ownership and
/// type-checking of the underlying moment.
struct VisualTableauEntity final : rdg2::Entity {
    GN_REGISTER_RUNTIME_TYPE(rdg2::Entity);

    Ref<VisualTableau>           tableau;
    DynaArray<Ref<VisualMoment>> tasks;

    VisualTableauEntity(Ref<VisualTableau> tableau_, DynaArray<Ref<VisualMoment>> tasks_)
        : Entity(TYPE_INFO(), "visual-tableau"), tableau(std::move(tableau_)), tasks(std::move(tasks_)) {}
};

/// One immutable publication of FX2's frame-global GPU resources. The artifact carrying
/// these snapshots is stable for the lifetime of the visual domain; its relic version is
/// what changes from frame to frame.
struct SscSnapshotEntity final : rdg2::Entity {
    GN_REGISTER_RUNTIME_TYPE(rdg2::Entity);

    DynaArray<fx2::SharedShaderConstants::Snapshot> snapshots;

    explicit SscSnapshotEntity(DynaArray<fx2::SharedShaderConstants::Snapshot> snapshots_)
        : Entity(TYPE_INFO(), "ssc-snapshot"), snapshots(std::move(snapshots_)) {}
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
        mLastFrameTexture.clear();
        mFrameEndQuest.clear();
        mRenderQuest.clear();
        mPrepareSscQuest.clear();
        mFrameBeginQuest.clear();
        mSscArtifact.clear();
        mBackbufferArtifact.clear();
        mTableauArtifact.clear();
        mPendingUploads.clear();
        mSceneResources.clear();
        mSsc.clear();
        mAdditionalSceneConstants.clear();
        mSwapchain.clear();
        if (mSurface && mOs) {
            mOs->destroyRenderSurface(mGpu->getVulkanInstanceHandle(), mSurface);
            mSurface = 0;
        }
    }

    Universe & universe() const override { return mUniverse; }

    AutoRef<GpuContext> gpu() const override { return mGpu; }

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
        // FX2 shades in linear light. Let the attachment encode display values,
        // including headless readback, rather than saving linear bytes as an image.
        auto displayFormat = gfx::img::PixelFormat::RGBA_8_8_8_8_SRGB();
        if (mSurface) {
            displayFormat.swizzle0 = gfx::img::PixelFormat::SWIZZLE_Z;
            displayFormat.swizzle2 = gfx::img::PixelFormat::SWIZZLE_X;
        }
        scDesc.setFormat(displayFormat);
        if (mSurface) scDesc.setSurface(mSurface);
        mSwapchain = Swapchain::create(scDesc);
        if (!mSwapchain) {
            GN_ERROR(sLogger, "Failed to create swapchain.");
            return false;
        }

        mSceneResources  = referenceTo(new SceneResources(mGpu));
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

        mSceneResources->mVs = GpuShader::create({.context = mGpu, .name = "e2-box-vs", .binary = kBoxVertSpv, .size = sizeof(kBoxVertSpv), .entry = "main"});
        mSceneResources->mPs = GpuShader::create({.context = mGpu, .name = "e2-box-ps", .binary = kBoxFragSpv, .size = sizeof(kBoxFragSpv), .entry = "main"});
        if (!mSceneResources->mVs || !mSceneResources->mPs) {
            GN_ERROR(sLogger, "Failed to create box shaders.");
            return false;
        }

        mSsc = fx2::SharedShaderConstants::create({.gpu = mGpu});
        if (!mSsc) {
            GN_ERROR(sLogger, "Failed to create FX2 shared shader constants.");
            return false;
        }
        // An absent environment moment contributes no indirect lighting. FX2 retains
        // valid fallback bindings, but their radiance must not light the scene.
        mSsc->set0.envLighting.environmentRadianceScale = 0.f;
        mSceneResources->mModelShading                  = fx2::ModelShading::create(mGpu);
        if (!mSceneResources->mModelShading) {
            GN_ERROR(sLogger, "Failed to create FX2 model shading.");
            return false;
        }

        return initFrameGraph();
    }

    void render(Ref<VisualTableau> tableauBase) override {
        mFrameSucceeded = false;
        auto * tableau  = RuntimeType::cast<VisualTableauImpl>(tableauBase.get());
        if (!tableau) return;
        auto tasks = tableau->orderedMoments();
        for (const auto & task : tasks) {
            if (!task) GN_UNLIKELY return;
            if (auto * env = RuntimeType::cast<VisualEnvironmentImpl>(task.get())) {
                if (env->gpu != mGpu) GN_UNLIKELY {
                        GN_ERROR(sLogger, "A visual environment must be created for this domain's GPU.");
                        return;
                    }
            }
        }
        if (mTableauArtifact->publish(AutoRef<rdg2::Entity>(new VisualTableauEntity(std::move(tableauBase), std::move(tasks)))) ==
            rdg2::Artifact::Version::OOO()) {
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

        auto execution  = rdg2::Execution::run({.plan = plan, .gpu = mGpu, .name = "e2-frame"});
        mFrameSucceeded = execution && execution->status() == rdg2::Execution::Status::SUCCEEDED;
        if (mFrameSucceeded)
            mPendingUploads.clear();
        else
            GN_WARN(sLogger, "Visual frame execution failed.");
    }

    gfx::img::Image readbackFrame() const override {
        if (mOs || !mFrameSucceeded || !mLastFrameTexture) return {};
        return mLastFrameTexture->readback();
    }

private:
    bool initFrameGraph() {
        mTableauArtifact    = rdg2::Artifact::create("e2.visual-tableau");
        mBackbufferArtifact = rdg2::Artifact::create("e2.backbuffer");
        mSscArtifact        = rdg2::Artifact::create("e2.shared-shader-constants");
        if (!mTableauArtifact || !mBackbufferArtifact || !mSscArtifact) {
            GN_ERROR(sLogger, "Failed to create persistent visual-domain artifacts.");
            return false;
        }

        mFrameBeginQuest = rdg2::createFrameBeginQuest({.swapchain = mSwapchain, .backbuffer = mBackbufferArtifact});

        rdg2::Quest::CreateParameters prepareSscParameters;
        prepareSscParameters.name = "e2-prepare-ssc";
        prepareSscParameters.artifactUses.append({.name = "visual-tableau", .artifact = mTableauArtifact, .access = rdg2::ArtifactAccess::READ_ONLY});
        prepareSscParameters.artifactUses.append({.name = "ssc", .artifact = mSscArtifact, .access = rdg2::ArtifactAccess::DISCARD_WRITE});
        prepareSscParameters.execute = [this](rdg2::QuestContext & context) {
            auto momentRelic = context.read<VisualTableauEntity>(mTableauArtifact);
            if (!momentRelic) return rdg2::QuestResult::failed("visual moment is unavailable for SSC preparation");
            // FX2 consumes one-time uploads when taking a snapshot, before the graph
            // submits. Replay them if a prior frame failed during task recording.
            for (const auto & payload : mPendingUploads) context.emit(payload);
            VisualEnvironmentImpl *  environment      = nullptr;
            const VisualMomentImpl * firstScene       = nullptr;
            size_t                   environmentIndex = 0;
            size_t                   firstSceneIndex  = 0;
            for (size_t i = 0; i < momentRelic->tasks.size(); ++i) {
                firstScene = RuntimeType::cast<VisualMomentImpl>(momentRelic->tasks[i].get());
                if (firstScene) {
                    firstSceneIndex = i;
                    break;
                }
            }
            ++mFrameCounter;
            DynaArray<fx2::SharedShaderConstants::Snapshot> snapshots;
            snapshots.resize(momentRelic->tasks.size());
            for (size_t i = 0; i < momentRelic->tasks.size(); ++i) {
                auto * env = RuntimeType::cast<VisualEnvironmentImpl>(momentRelic->tasks[i].get());
                if (!env) continue;
                auto & snapshot = snapshots[i];
                snapshot        = prepareSharedShaderConstants(*env->constants, firstScene);
                if (snapshot.set0Resources.empty()) return rdg2::QuestResult::failed("environment snapshot has no resources");
                for (auto & payload : snapshot.set0Payloads) emitUpload(context, payload);
                // Scene shaders bind one IBL resource set. Use the last environment
                // in this frame's unspecified environment order; do not blend sets.
                environment      = env;
                environmentIndex = i;
            }
            auto prepareSceneSnapshot = [&](fx2::SharedShaderConstants & constants, const VisualMomentImpl * scene) {
                constants.set0.envLighting.environmentRadianceScale = environment ? environment->description.radianceScale : 0.f;
                auto snapshot                                       = prepareSharedShaderConstants(constants, scene);
                if (snapshot.set0Resources.empty()) return snapshot;
                if (environment) {
                    // FX2 set 0 reserves bindings 0/1 for scene/camera UBOs and 2..5
                    // for environment textures. Share only the environment-owned views.
                    const auto & resources = snapshots[environmentIndex].set0Resources;
                    for (size_t binding = 2; binding < resources.size(); ++binding) snapshot.set0Resources[binding] = resources[binding];
                }
                for (auto & payload : snapshot.set0Payloads) emitUpload(context, payload);
                return snapshot;
            };
            size_t sceneIndex = 0;
            for (size_t i = 0; i < momentRelic->tasks.size(); ++i) {
                auto * scene = RuntimeType::cast<VisualMomentImpl>(momentRelic->tasks[i].get());
                if (!scene) continue;
                auto constants = mSsc;
                if (sceneIndex > 0) {
                    if (mAdditionalSceneConstants.size() < sceneIndex) {
                        auto extra = fx2::SharedShaderConstants::create({.gpu = mGpu});
                        if (!extra) return rdg2::QuestResult::failed("failed to create scene constants");
                        mAdditionalSceneConstants.append(extra);
                    }
                    constants = mAdditionalSceneConstants[sceneIndex - 1];
                }
                ++sceneIndex;
                // FX2 snapshots reuse their effect's camera/light buffers. Give every
                // scene its own effect so all uploads can precede this frame's raster.
                snapshots[i] = prepareSceneSnapshot(*constants, scene);
                if (snapshots[i].set0Resources.empty()) return rdg2::QuestResult::failed("scene snapshot has no resources");
            }
            fx2::SharedShaderConstants::Snapshot defaultSnapshot;
            for (auto & snapshot : snapshots) {
                if (!snapshot.set0Resources.empty()) continue;
                // Extension moments have no E2 scene payload. Give them the first scene's
                // prepared constants, or a default camera/light snapshot for standalone use.
                if (firstScene) {
                    snapshot = snapshots[firstSceneIndex];
                } else {
                    if (defaultSnapshot.set0Resources.empty()) defaultSnapshot = prepareSceneSnapshot(*mSsc, nullptr);
                    if (defaultSnapshot.set0Resources.empty()) return rdg2::QuestResult::failed("default snapshot has no resources");
                    snapshot = defaultSnapshot;
                }
            }
            context.publish(mSscArtifact, AutoRef<rdg2::Entity>(new SscSnapshotEntity(std::move(snapshots))));
            return rdg2::QuestResult::succeeded();
        };
        mPrepareSscQuest = rdg2::Quest::create(prepareSscParameters);

        rdg2::Quest::CreateParameters renderParameters;
        renderParameters.name = "e2-visual-render";
        renderParameters.artifactUses.append({.name = "visual-tableau", .artifact = mTableauArtifact, .access = rdg2::ArtifactAccess::READ_ONLY});
        renderParameters.artifactUses.append({.name = "ssc", .artifact = mSscArtifact, .access = rdg2::ArtifactAccess::READ_ONLY});
        renderParameters.artifactUses.append({.name = "backbuffer", .artifact = mBackbufferArtifact, .access = rdg2::ArtifactAccess::READ_WRITE});
        renderParameters.execute = [this](rdg2::QuestContext & context) {
            auto momentRelic = context.read<VisualTableauEntity>(mTableauArtifact);
            auto sscRelic    = context.read<SscSnapshotEntity>(mSscArtifact);
            auto frameRelic  = context.read<rdg2::SwapchainFrameEntity>(mBackbufferArtifact);
            if (!momentRelic || !sscRelic || !frameRelic)
                return rdg2::QuestResult::failed("visual moment, SSC snapshot, or acquired backbuffer is unavailable");

            mRenderTarget.setColorTarget(0, frameRelic->frame.view);
            mLastFrameTexture = frameRelic->frame.view.texture();
            auto & blend      = mRenderTarget.colorTargets[0].blendState;
            blend.colorSrc    = RasterTarget::BlendState::SRC_ALPHA;
            blend.colorDst    = RasterTarget::BlendState::INV_SRC_ALPHA;
            blend.alphaSrc    = RasterTarget::BlendState::ONE;
            blend.alphaDst    = RasterTarget::BlendState::INV_SRC_ALPHA;
            auto raster       = GpuRaster::create("e2-visual", {.gpu = mGpu, .target = &mRenderTarget});
            if (!raster) return rdg2::QuestResult::failed("failed to create visual raster");
            for (size_t i = 0; i < momentRelic->tasks.size(); ++i) {
                VisualRecordContext recording(*raster, *mSceneResources, sscRelic->snapshots[i],
                                              [this, &context](AutoRef<GpuPayload> upload) { emitUpload(context, upload); });
                if (!momentRelic->tasks[i]->record(recording)) return rdg2::QuestResult::failed("failed to record visual moment");
            }
            auto payload = raster->seal();
            if (!payload) return rdg2::QuestResult::failed("failed to seal visual raster");
            context.emit(payload);

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

    fx2::SharedShaderConstants::Snapshot prepareSharedShaderConstants(fx2::SharedShaderConstants & constants, const VisualMomentImpl * moment) {
        const bool         haveCamera = moment && !moment->cameras.empty();
        const WorldVector3 eye =
            haveCamera ? moment->cameras[0].position : WorldVector3(WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO());

        constants.set0.frameConstants.frameCounter = (int) mFrameCounter;
        constants.set0.directLighting.clear();
        constants.set0.camera = {};
        if (moment)
            for (const auto & light : moment->lights) {
                fx2::SharedShaderConstants::DirectLight direct;
                direct.type            = fx2::SharedShaderConstants::DirectLight::POINT;
                direct.point.position  = moment->scale.toMeters(spatial::toLocal(eye, light.position));
                direct.point.intensity = IntensityRGB {light.color.r, light.color.g, light.color.b, Candela {1.f}};
                constants.set0.directLighting.append(direct);
            }

        constants.set0.camera.aspectRatio       = mHeight ? (float) mWidth / (float) mHeight : 1.f;
        constants.set0.camera.viewWidthInPixel  = mWidth;
        constants.set0.camera.viewHeightInPixel = mHeight;
        if (haveCamera) {
            const auto & camera                     = moment->cameras[0];
            constants.set0.camera.cameraPosition    = fx2::Location(0.f);
            constants.set0.camera.cameraOrientation = camera.orientation;
            constants.set0.camera.cameraFov         = ArcDegree(camera.fovYInDegree);
            constants.set0.camera.nearPlane         = moment->scale.toMeters(camera.nearPlane);
            constants.set0.camera.farPlane          = moment->scale.toMeters(camera.farPlane);
            if (constants.set0.camera.nearPlane <= 0.f) constants.set0.camera.nearPlane = 0.1f;
            if (constants.set0.camera.farPlane <= constants.set0.camera.nearPlane) constants.set0.camera.farPlane = constants.set0.camera.nearPlane + 1000.f;
        }

        return constants.takeSnapshot();
    }

    void emitUpload(rdg2::QuestContext & context, const AutoRef<GpuPayload> & payload) {
        if (!payload) return;
        mPendingUploads.append(payload);
        context.emit(payload);
    }

    AutoRef<SceneResources>                        mSceneResources;
    Universe &                                     mUniverse;
    AutoRef<Texture>                               mLastFrameTexture;
    bool                                           mFrameSucceeded = false;
    Ref<OperatingDomain>                           mOs;
    AutoRef<GpuContext>                            mGpu;
    intptr_t                                       mSurface = 0; ///< owned; destroyed in ~VisualDomainImpl between swapchain and GPU context
    AutoRef<Swapchain>                             mSwapchain;
    AutoRef<Texture>                               mDepth;
    AutoRef<fx2::SharedShaderConstants>            mSsc;
    DynaArray<AutoRef<fx2::SharedShaderConstants>> mAdditionalSceneConstants;
    DynaArray<AutoRef<GpuPayload>>                 mPendingUploads;
    rdg2::ArtifactRef                              mTableauArtifact;
    rdg2::ArtifactRef                              mBackbufferArtifact;
    rdg2::ArtifactRef                              mSscArtifact;
    rdg2::QuestRef                                 mFrameBeginQuest;
    rdg2::QuestRef                                 mPrepareSscQuest;
    rdg2::QuestRef                                 mRenderQuest;
    rdg2::QuestRef                                 mFrameEndQuest;
    RasterTarget                                   mRenderTarget;
    uint32_t                                       mFrameCounter = 0;
    uint32_t                                       mWidth        = 1280;
    uint32_t                                       mHeight       = 720;
};

#endif // GN_BUILD_HAS_VULKAN

} // namespace

namespace GN::e2 {

DynaArray<Ref<VisualMoment>> VisualTableauImpl::orderedMoments() const {
    auto result = moments;
    if (result.size() < 2) return result;
    // Stability preserves regular moments' insertion order. Equal-Z overlays and
    // environments intentionally have no public tie-breaking guarantee.
    std::stable_sort(result.begin(), result.end(), [](const auto & a, const auto & b) {
        auto * overlayA = RuntimeType::cast<VisualOverlay>(a.get());
        auto * overlayB = RuntimeType::cast<VisualOverlay>(b.get());
        int    phaseA   = overlayA ? 2 : (RuntimeType::cast<VisualEnvironment>(a.get()) ? 1 : 0);
        int    phaseB   = overlayB ? 2 : (RuntimeType::cast<VisualEnvironment>(b.get()) ? 1 : 0);
        if (phaseA != phaseB) return phaseA < phaseB;
        return overlayA && overlayB && overlayA->zOrder() > overlayB->zOrder();
    });
    return result;
}

bool VisualMomentImpl::record(RenderContext & base) const {
    auto * context = RuntimeType::cast<VisualRecordContext>(&base);
    if (!context) return false;
    auto &             resources   = context->resources;
    auto &             raster      = context->raster();
    const auto &       sscSnapshot = base.ssc();
    const auto &       moment      = *this;
    const bool         haveCamera  = !moment.cameras.empty();
    const WorldVector3 eye = haveCamera ? moment.cameras[0].position : WorldVector3(WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO());

    if (haveCamera) {
        for (auto & r : moment.renderables) {
            if (r.model) {
                const auto model = resources.ensureGpuModel(base, r.model);
                if (!model) continue;
                if (!resources.mModelShadingPayloadEmitted) {
                    base.upload(resources.mModelShading->uploadPayload());
                    resources.mModelShadingPayloadEmitted = true;
                }

                const glm::mat4 instanceTransform =
                    glm::translate(glm::mat4(1.f), moment.scale.toMeters(spatial::toLocal(eye, r.translation))) * glm::mat4_cast(glm::normalize(r.rotation));
                DynaArray<glm::mat4> nodeTransforms;
                nodeTransforms.resize(r.model->nodes.size());
                for (size_t nodeIndex = 0; nodeIndex < r.model->nodes.size(); ++nodeIndex) {
                    const auto &    node            = r.model->nodes[nodeIndex];
                    const glm::mat4 parentTransform = node.parent >= 0 ? nodeTransforms[static_cast<size_t>(node.parent)] : glm::mat4(1.f);
                    nodeTransforms[nodeIndex]       = parentTransform * node.transform;
                    for (uint32_t primitiveIndex : node.primitives) {
                        auto draw = fx2::ModelShading::getDrawParams(sscSnapshot, resources.mModelShading, model, primitiveIndex,
                                                                     instanceTransform * nodeTransforms[nodeIndex]);
                        if (draw.vs && draw.ps) raster.draw(draw);
                    }
                }
                continue;
            }
            if (!r.mesh) continue;
            const auto * gpuMesh = resources.ensureGpuMesh(*r.mesh);
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
            dp.vs       = resources.mVs;
            dp.ps       = resources.mPs;
            dp.geometry = geom;
            dp.resources.resize(1);
            dp.resources[0] = sscSnapshot.set0Resources;
            dp.immediates   = referenceTo(new SimpleBlob<uint8_t>(sizeof(dc), reinterpret_cast<const uint8_t *>(&dc)));
            raster.draw(dp);
        }
    }

    return true;
}

Ref<VisualTableau> VisualTableau::create(Universe & universe) { return referenceTo(new VisualTableauImpl(universe)); }

Ref<VisualEnvironment> VisualEnvironment::create(const CreateParameters & cp) {
    if (!cp.gpu) return {};
    auto moment       = referenceTo(new VisualEnvironmentImpl(cp));
    moment->constants = fx2::SharedShaderConstants::create({.gpu = cp.gpu});
    if (!moment->constants) return {};
    moment->constants->set0.envLighting = {
        .skyboxPath               = cp.description.skyboxPath,
        .irradiancePath           = cp.description.irradiancePath,
        .prefilteredPath          = cp.description.prefilteredPath,
        .brdfLutPath              = cp.description.brdfLutPath,
        .environmentRadianceScale = cp.description.radianceScale,
    };
    return moment;
}

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
