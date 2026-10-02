// Visual tableaux keep snapshot organization private to E2. The domain prepares
// frame resources, then each moment records its own work into the shared raster.

#include "e2-internal.h"

#include <garnet/GNwin.h>

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

struct VisualRecordContext final : VisualMoment::RenderContext {
    GN_REGISTER_RUNTIME_TYPE(VisualMoment::RenderContext);

    GpuRaster &                                  target;
    const fx2::SharedShaderConstants::Snapshot & snapshot;
    std::function<void(AutoRef<GpuPayload>)>     emit;

    VisualRecordContext(GpuRaster & raster, const fx2::SharedShaderConstants::Snapshot & snapshot_, std::function<void(AutoRef<GpuPayload>)> emit_)
        : RenderContext(TYPE_INFO()), target(raster), snapshot(snapshot_), emit(std::move(emit_)) {}

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
    AutoRef<fx2::SkyboxKernel>          skybox;

    const Desc        description;
    std::atomic<bool> visible {true};

    explicit VisualEnvironmentImpl(const CreateParameters & cp)
        : VisualEnvironment(TYPE_INFO(), cp.universe.generateUniqueIdentifier(), "visual-environment"), gpu(cp.gpu), description(cp.description) {}

    void setVisible(bool value) override { visible.store(value, std::memory_order_relaxed); }

    bool record(RenderContext & context) const override {
        // Visibility only controls the background draw; scene lighting still uses this environment's resources.
        if (!visible.load(std::memory_order_relaxed)) return true;
        return skybox->record(context.raster(), context.ssc().set0Resources);
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
        mSsc.clear();
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
        mRenderTarget.states.setCullMode(RasterState::CULL_BACK);
        mRenderTarget.states.setDepthState(RasterState::DepthState {.func = RasterState::Compare::LESS, .write = true});

        mSsc = fx2::SharedShaderConstants::create({.gpu = mGpu});
        if (!mSsc) {
            GN_ERROR(sLogger, "Failed to create FX2 shared shader constants.");
            return false;
        }
        // An absent environment moment contributes no indirect lighting. FX2 retains
        // valid fallback bindings, but their radiance must not light the scene.
        mSsc->set0.envLighting.environmentLuminanceScale = 0.f;

        return initFrameGraph();
    }

    void renderFrame(const RenderFrameParameters & parameters) override {
        mFrameSucceeded = false;
        auto * tableau  = RuntimeType::cast<VisualTableauImpl>(parameters.tableau.get());
        if (!tableau) return;

        mRenderTarget.clearColor = parameters.clearColor;
        mRenderTarget.setClearDepth(parameters.clearDepth);

        // Pump GPU to retire/recycle completed GPU works from previous frame.
        mGpu->pump();

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
        if (mTableauArtifact->publish(AutoRef<rdg2::Entity>(new VisualTableauEntity(parameters.tableau, std::move(tasks)))) == rdg2::Artifact::Version::OOO()) {
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
            VisualEnvironmentImpl * environment      = nullptr;
            size_t                  environmentIndex = 0;
            ++mFrameCounter;
            DynaArray<fx2::SharedShaderConstants::Snapshot> snapshots;
            snapshots.resize(momentRelic->tasks.size());
            for (size_t i = 0; i < momentRelic->tasks.size(); ++i) {
                auto * env = RuntimeType::cast<VisualEnvironmentImpl>(momentRelic->tasks[i].get());
                if (!env) continue;
                auto & snapshot = snapshots[i];
                snapshot        = prepareSharedShaderConstants(*env->constants);
                if (snapshot.set0Resources.empty()) return rdg2::QuestResult::failed("environment snapshot has no resources");
                for (auto & payload : snapshot.set0Payloads) emitUpload(context, payload);
                // Scene shaders bind one IBL resource set. Use the last environment
                // in this frame's unspecified environment order; do not blend sets.
                environment      = env;
                environmentIndex = i;
            }
            auto prepareDefaultSnapshot = [&](fx2::SharedShaderConstants & constants) {
                constants.set0.envLighting.environmentLuminanceScale = environment ? environment->description.environmentLuminanceScale : 0.f;
                auto snapshot                                        = prepareSharedShaderConstants(constants);
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
            fx2::SharedShaderConstants::Snapshot defaultSnapshot;
            for (auto & snapshot : snapshots) {
                if (!snapshot.set0Resources.empty()) continue;
                // Custom moments share one unchanged default camera/light binding. An effect
                // needing its own view supplies a separately prepared SSC instance.
                if (defaultSnapshot.set0Resources.empty()) defaultSnapshot = prepareDefaultSnapshot(*mSsc);
                if (defaultSnapshot.set0Resources.empty()) return rdg2::QuestResult::failed("default snapshot has no resources");
                snapshot = defaultSnapshot;
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
                VisualRecordContext recording(*raster, sscRelic->snapshots[i], [this, &context](AutoRef<GpuPayload> upload) { emitUpload(context, upload); });
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

    fx2::SharedShaderConstants::Snapshot prepareSharedShaderConstants(fx2::SharedShaderConstants & constants) {
        constants.set0.frameConstants.frameCounter = (int) mFrameCounter;
        constants.set0.directLighting.clear();
        constants.set0.camera                   = {};
        constants.set0.camera.aspectRatio       = mHeight ? (float) mWidth / (float) mHeight : 1.f;
        constants.set0.camera.viewWidthInPixel  = mWidth;
        constants.set0.camera.viewHeightInPixel = mHeight;
        return constants.takeSnapshot();
    }

    void emitUpload(rdg2::QuestContext & context, const AutoRef<GpuPayload> & payload) {
        if (!payload) return;
        mPendingUploads.append(payload);
        context.emit(payload);
    }

    Universe &                          mUniverse;
    AutoRef<Texture>                    mLastFrameTexture;
    bool                                mFrameSucceeded = false;
    Ref<Platform>                       mOs;
    AutoRef<GpuContext>                 mGpu;
    intptr_t                            mSurface = 0; ///< owned; destroyed in ~VisualDomainImpl between swapchain and GPU context
    AutoRef<Swapchain>                  mSwapchain;
    AutoRef<Texture>                    mDepth;
    AutoRef<fx2::SharedShaderConstants> mSsc;
    DynaArray<AutoRef<GpuPayload>>      mPendingUploads;
    rdg2::ArtifactRef                   mTableauArtifact;
    rdg2::ArtifactRef                   mBackbufferArtifact;
    rdg2::ArtifactRef                   mSscArtifact;
    rdg2::QuestRef                      mFrameBeginQuest;
    rdg2::QuestRef                      mPrepareSscQuest;
    rdg2::QuestRef                      mRenderQuest;
    rdg2::QuestRef                      mFrameEndQuest;
    RasterTarget                        mRenderTarget;
    uint32_t                            mFrameCounter = 0;
    uint32_t                            mWidth        = 1280;
    uint32_t                            mHeight       = 720;
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

Ref<VisualTableau> VisualTableau::create(Universe & universe) { return referenceTo(new VisualTableauImpl(universe)); }

Ref<VisualEnvironment> VisualEnvironment::create(const CreateParameters & cp) {
    if (!cp.gpu) return {};
    auto moment       = referenceTo(new VisualEnvironmentImpl(cp));
    moment->constants = fx2::SharedShaderConstants::create({.gpu = cp.gpu});
    moment->skybox    = fx2::SkyboxKernel::create(cp.gpu);
    if (!moment->skybox) return {};
    if (!moment->constants) return {};
    moment->constants->set0.envLighting = {
        .skyboxPath                = cp.description.skyboxPath,
        .irradiancePath            = cp.description.irradiancePath,
        .prefilteredPath           = cp.description.prefilteredPath,
        .brdfLutPath               = cp.description.brdfLutPath,
        .environmentLuminanceScale = cp.description.environmentLuminanceScale,
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
