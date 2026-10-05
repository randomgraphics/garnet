// visual.cpp — the official Visual service implementation.
// A single Lambertian raster pass presents each Tableau without a render graph.

#include <garnet/GNengine2.h>

#include <garnet/GNwin.h>

using namespace GN;
using namespace GN::e2;
using namespace GN::e2::basis;
using namespace GN::gpu2;

namespace {

GN::Logger * sLogger = GN::getLogger("GN.e2.visual");

#if GN_BUILD_HAS_VULKAN

// ---------------------------------------------------------------------------
// VisualImpl
// ---------------------------------------------------------------------------

struct VisualImpl : Visual {
    GN_REGISTER_RUNTIME_TYPE(Visual);

    explicit VisualImpl(Universe & u): Visual(TYPE_INFO(), u.generateUniqueIdentifier(), "visual"), mUniverse(u) {}

    ~VisualImpl() override {
        if (!mGpu) return;
        mGpu->waitForIdle();
        mRenderTarget.setColorTarget(0, {});
        mLastFrameTexture.clear();
        mPendingUploads.clear();
        mSsc.clear();
        // Destroy swapchain views before the surface, while the GPU instance remains alive.
        mSwapchain.clear();
        if (mSurface && mOs) {
            mOs->destroyRenderSurface(mGpu->getVulkanInstanceHandle(), mSurface);
            mSurface = 0;
        }
    }

    Universe & universe() const override { return mUniverse; }
    Platform & platform() const override { return *mOs; }
    Assets &   assets() const override { return *mAssets; }

    gpu2::GpuContext & gpu() const override { return *mGpu; }

    bool init(const CreateParameters & cp) {
        mOs = cp.platform;

        mGpu = GpuContext::create("e2-gpu", GpuContext::CreateParameters {});
        if (!mGpu) {
            GN_ERROR(sLogger, "Failed to create GPU context.");
            return false;
        }

        if (mOs) {
            mSurface      = mOs->createRenderSurface(mGpu->getVulkanInstanceHandle());
            auto clientSz = mOs->clientSize();
            if (clientSz.x && clientSz.y) {
                mWidth  = clientSz.x;
                mHeight = clientSz.y;
            }
        }

        Swapchain::CreateDesc scDesc;
        scDesc.setGpu(mGpu).setName("e2-swapchain").setDimensions(mWidth, mHeight);
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
        mSsc->set0.envLighting.environmentLuminanceScale = 0.f;

        if (cp.assets) {
            mAssets = cp.assets;
        } else {
            mAssets = Assets::create({.universe = mUniverse, .gpu = mGpu});
        }
        auto initialization = GpuCnC::create({.gpu = mGpu});
        if (!mAssets || !initialization) return false;
        mLambert     = fx2::LambertianKernel::create(mGpu, *initialization);
        auto payload = initialization->seal();
        if (!mLambert || !payload) return false;
        mPendingUploads.append(payload);
        // A small uniform fill keeps unlit faces readable in this white-model preview.
        mSsc->set0.envLighting.environmentAmbientFloor = 50.f;
        fx2::SharedShaderConstants::DirectLight light;
        light.type                    = fx2::SharedShaderConstants::DirectLight::DIRECTIONAL;
        light.directional.orientation = glm::quat(glm::vec3(0.6f, 0.8f, 0.f));
        light.directional.irradiance  = {1.f, 1.f, 1.f, {1500.f}};
        mSsc->set0.directLighting.append(light);
        return true;
    }

    void renderFrame(const Tableau & tableau) override {
        mFrameSucceeded          = false;
        mRenderTarget.clearColor = tableau.clearColor;
        mRenderTarget.setClearDepth(tableau.clearDepth);

        mGpu->pump();

        auto frame = mSwapchain->prepare();
        if (frame.view.empty()) return;
        mRenderTarget.setColorTarget(0, frame.view);
        auto raster  = GpuRaster::create("e2-white-model", {.gpu = mGpu, .target = &mRenderTarget});
        auto uploads = GpuCnC::create({.gpu = mGpu});
        if (!raster || !uploads) return;
        ++mFrameCounter;
        auto snapshot = prepareSharedShaderConstants(*mSsc, tableau);
        // SSC hands initialization out only once; retain it if recording is abandoned.
        for (const auto & payload : snapshot.set0Payloads) mPendingUploads.append(payload);
        if (snapshot.set0Resources.empty()) return;
        for (const auto & object : tableau.objects) {
            auto mesh = mAssets->findMesh(object.meshId ? object.meshId : Assets::MESH_BOX);
            if (!mesh) continue;
            fx2::LambertianKernel::Inputs draw;
            draw.geometry = mesh->geometry();
            auto position = tableau.scale.toMeters(spatial::toLocal(tableau.camera.position, object.transform.position));
            draw.worldFromObject =
                glm::translate(glm::mat4(1.f), position) * glm::mat4_cast(object.transform.orientation) * glm::scale(glm::mat4(1.f), object.transform.scale);
            if (!mLambert->record(*raster, *uploads, snapshot.set0Resources, draw)) return;
        }
        auto parameters = uploads->seal();
        auto draws      = raster->seal();
        if (!parameters || !draws) return;
        GpuContext::SubmitParameters submission("e2-white-model");
        for (const auto & payload : mPendingUploads) submission.appendWork(payload);
        submission.appendWork(parameters).appendWork(draws).waitFor(frame.ready);
        mGpu->submit(submission);
        mPendingUploads.clear();
        mSwapchain->present(*draws);
        // The preview deliberately serializes frames to bound resources and SSC reuse.
        mGpu->waitForIdle();
        mLastFrameTexture = frame.view.texture();
        mFrameSucceeded   = true;
    }

    gfx::img::Image readbackFrame() const override {
        if (mOs || !mFrameSucceeded || !mLastFrameTexture) return {};
        return mLastFrameTexture->readback();
    }

private:
    fx2::SharedShaderConstants::Snapshot prepareSharedShaderConstants(fx2::SharedShaderConstants & constants, const Tableau & tableau) {
        constants.set0.frameConstants.frameCounter = (int) mFrameCounter;
        const auto & cam                           = tableau.camera;
        constants.set0.camera                      = {};
        constants.set0.camera.cameraPosition       = glm::vec3(0.f);
        constants.set0.camera.cameraOrientation    = cam.orientation;
        constants.set0.camera.cameraFov            = ArcDegree(cam.fovYInDegree);
        constants.set0.camera.nearPlane            = tableau.scale.toMeters(spatial::toLocal(WorldCoordinate::ZERO(), cam.nearPlane));
        constants.set0.camera.farPlane             = tableau.scale.toMeters(spatial::toLocal(WorldCoordinate::ZERO(), cam.farPlane));
        constants.set0.camera.exposure             = cam.exposure;
        constants.set0.camera.aspectRatio          = mHeight ? (float) mWidth / (float) mHeight : 1.f;
        constants.set0.camera.viewWidthInPixel     = mWidth;
        constants.set0.camera.viewHeightInPixel    = mHeight;
        return constants.takeSnapshot();
    }

    Universe &                          mUniverse;
    AutoRef<Texture>                    mLastFrameTexture;
    bool                                mFrameSucceeded = false;
    Ref<Platform>                       mOs;
    AutoRef<GpuContext>                 mGpu;
    intptr_t                            mSurface = 0;
    AutoRef<Swapchain>                  mSwapchain;
    AutoRef<Texture>                    mDepth;
    AutoRef<fx2::SharedShaderConstants> mSsc;
    Ref<Assets>                         mAssets;
    AutoRef<fx2::LambertianKernel>      mLambert;
    DynaArray<AutoRef<GpuPayload>>      mPendingUploads;
    RasterTarget                        mRenderTarget;
    uint32_t                            mFrameCounter = 0;
    uint32_t                            mWidth        = 1280;
    uint32_t                            mHeight       = 720;
};

#endif // GN_BUILD_HAS_VULKAN

} // namespace

namespace GN::e2::basis {

Ref<Visual> Visual::create(const CreateParameters & cp) {
#if GN_BUILD_HAS_VULKAN
    auto d = referenceTo(new VisualImpl(cp.universe));
    if (!d->init(cp)) return {};
    return d;
#else
    (void) cp;
    GN_ERROR(GN::getLogger("GN.e2.visual"), "Visual requires a Vulkan-enabled build.");
    return {};
#endif
}

} // namespace GN::e2::basis
