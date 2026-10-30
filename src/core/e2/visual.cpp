// visual.cpp — the official Visual service implementation.
// A single bindless Lambertian raster pass presents each Tableau without a render graph.

#include <garnet/GNengine2.h>
#include <algorithm>
#include <cmath>

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
        mWhiteMaterial.clear();
        mLambert.clear();
        mSsc.clear();
        mHeap.clear();
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

        mHeap = gpu2::bindless::DescriptorHeap::create("e2-heap", {.gpu = mGpu, .capacity = 256, .materialCapacity = 1024 * 1024});
        if (!mHeap) {
            GN_ERROR(sLogger, "Failed to create bindless descriptor heap.");
            return false;
        }

        mSsc = fx2::bindless::SharedShaderConstants::create({.gpu = mGpu, .uniformCapacity = 64 * 1024, .streamingCapacity = 64 * 1024});
        if (!mSsc) {
            GN_ERROR(sLogger, "Failed to create bindless shared shader constants.");
            return false;
        }

        if (cp.assets) {
            mAssets = cp.assets;
        } else {
            mAssets = Assets::create({.universe = mUniverse, .gpu = mGpu});
        }
        if (!mAssets) return false;

        auto initialization = gpu2::bindless::CnC::create("e2-init-uploads", {.gpu = mGpu, .heap = mHeap});
        if (!initialization) return false;
        mLambert = fx2::bindless::LambertianKernel::create(*mHeap, *initialization);
        if (!mLambert) return false;

        auto matParams = mLambert->defaultMaterialParameters();
        // A small uniform fill keeps unlit faces readable in this white-model preview.
        matParams.emissive = glm::vec3(50.f / 3.14159265359f);
        mWhiteMaterial     = mLambert->createMaterial(*initialization, matParams);
        if (!mWhiteMaterial) return false;

        auto payload = initialization->seal();
        if (!payload) return false;
        mGpu->submit(GpuContext::SubmitParameters("e2-visual-init").appendWork(payload));
        mGpu->waitForIdle();
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

        auto uploadCnc = gpu2::bindless::CnC::create(StrA::format("e2-frame-{}-uploads", mFrameCounter), {.gpu = mGpu, .heap = mHeap});
        if (!uploadCnc) return;

        ++mFrameCounter;
        auto uniformState = updateUniforms(*uploadCnc, tableau);
        if (!uniformState) return;

        gpu2::bindless::Raster::CreateParameters rcp;
        rcp.gpu               = mGpu;
        rcp.target            = &mRenderTarget;
        rcp.heap              = mHeap;
        rcp.heapSetIndex      = 0;
        rcp.passResources     = fx2::bindless::sharedUniformResources(uniformState);
        rcp.numberOfDrawsHint = tableau.objects.size();
        auto raster           = gpu2::bindless::Raster::create("e2-white-model", rcp);
        if (!raster) return;

        for (const auto & object : tableau.objects) {
            auto mesh = mAssets->findMesh(object.meshId ? object.meshId : Assets::MESH_BOX);
            if (!mesh) continue;
            auto      position = tableau.scale.toMeters(spatial::toLocal(tableau.camera.position, object.transform.position));
            glm::mat4 object2WorldTransform =
                glm::translate(glm::mat4(1.f), position) * glm::mat4_cast(object.transform.orientation) * glm::scale(glm::mat4(1.f), object.transform.scale);
            fx2::bindless::LambertianMaterial::DrawParameters draw {
                {*raster, uniformState, mesh->geometry(), &mRenderTarget.states},
                object2WorldTransform,
            };
            if (!mWhiteMaterial->record(draw)) return;
        }

        auto uniformWork = uploadCnc->seal();
        auto rasterWork  = raster->seal();
        if (!rasterWork) return;

        GpuContext::SubmitParameters submission("e2-white-model");
        if (uniformWork) submission.appendWork(uniformWork);
        submission.appendWork(rasterWork).waitFor(frame.ready);
        mGpu->submit(submission);
        mSwapchain->present(*rasterWork);
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
    AutoRef<fx2::bindless::SharedShaderConstants::UniformState> updateUniforms(gpu2::bindless::CnC & uploads, const Tableau & tableau) {
        fx2::bindless::SharedUniforms uniforms {};
        uniforms.frameCounter = mFrameCounter;

        const auto & cam = tableau.camera;
        // E2 rebases all positions against the observing camera before rendering, so
        // the camera sits at the origin and its view matrix carries orientation only.
        const glm::vec3 pos(0.f);
        glm::mat4       camToWorld = glm::translate(glm::mat4(1.f), pos) * glm::mat4_cast(cam.orientation);
        uniforms.viewMatrix        = glm::inverse(camToWorld);

        const float aspect    = mHeight ? (float) mWidth / (float) mHeight : 1.f;
        const float nearPlane = tableau.scale.toMeters(spatial::toLocal(WorldCoordinate::ZERO(), cam.nearPlane));
        const float farPlane  = tableau.scale.toMeters(spatial::toLocal(WorldCoordinate::ZERO(), cam.farPlane));
        uniforms.projMatrix   = glm::perspectiveRH_ZO(glm::radians(std::clamp(cam.fovYInDegree, 1.f, 179.f)), aspect, nearPlane, farPlane);
        uniforms.projMatrix[1][1] *= -1.f; // Vulkan clip space
        uniforms.projViewMatrix   = uniforms.projMatrix * uniforms.viewMatrix;
        uniforms.cameraPosition   = glm::vec4(pos, 1.f);
        uniforms.renderTargetSize = glm::vec2((float) mWidth, (float) mHeight);
        uniforms.nearPlane        = nearPlane;
        uniforms.farPlane         = farPlane;
        uniforms.exposure         = cam.exposure;

        // Fixed directional light matching the white-model preview illumination.
        glm::vec3 dir                    = glm::mat3_cast(glm::quat(glm::vec3(0.6f, 0.8f, 0.f))) * glm::vec3(0.f, 0.f, -1.f);
        uniforms.numLights               = 1;
        uniforms.lights[0].positionOrDir = glm::vec4(dir, float(fx2::bindless::DirectLightUniform::DIRECTIONAL));
        uniforms.lights[0].colorAndRange = glm::vec4(1500.f, 1500.f, 1500.f, 0.f);

        return mSsc->recordUniformUpdate(uploads, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
    }

    Universe &                                    mUniverse;
    AutoRef<Texture>                              mLastFrameTexture;
    bool                                          mFrameSucceeded = false;
    Ref<Platform>                                 mOs;
    AutoRef<GpuContext>                           mGpu;
    intptr_t                                      mSurface = 0;
    AutoRef<Swapchain>                            mSwapchain;
    AutoRef<Texture>                              mDepth;
    AutoRef<gpu2::bindless::DescriptorHeap>       mHeap;
    AutoRef<fx2::bindless::SharedShaderConstants> mSsc;
    Ref<Assets>                                   mAssets;
    AutoRef<fx2::bindless::LambertianKernel>      mLambert;
    AutoRef<fx2::bindless::LambertianMaterial>    mWhiteMaterial;
    RasterTarget                                  mRenderTarget;
    uint32_t                                      mFrameCounter = 0;
    uint32_t                                      mWidth        = 1280;
    uint32_t                                      mHeight       = 720;
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
