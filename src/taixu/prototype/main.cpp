// Minimal standalone host for the Taixu world prototype.

#include "world-model.h"

#include <garnet/GNfx2.h>
#include <garnet/GNengine2.h>
#include <garnet/GNwin.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <optional>
#include <string>

using namespace GN;
using namespace GN::fx2;
using namespace GN::gpu2;
namespace e2 = GN::e2;

namespace {

constexpr uint32_t kWidth  = 1280;
constexpr uint32_t kHeight = 720;

struct Host {
    AutoRef<GpuContext>          gpu;
    std::unique_ptr<win::Window> window;
    intptr_t                     surface = 0;
    AutoRef<Swapchain>           swapchain;

    ~Host() {
        if (gpu) gpu->waitForIdle();
        swapchain.clear();
        if (surface) window->destroyVulkanSurfaceHandle(gpu->getVulkanInstanceHandle(), surface);
    }
};

glm::vec3 toMeters(const e2::WorldVector3 & position, const e2::PhysicalScale & scale) {
    const auto local = e2::positionToMeters(position, scale);
    return {static_cast<float>(local.x), static_cast<float>(local.y), static_cast<float>(local.z)};
}

struct CameraPose {
    glm::vec3 position {0.0f, 1.8f, 12.0f};
    float     yaw   = 0.0f;
    float     pitch = -0.08f;
};

std::optional<glm::vec3> groundTarget(const CameraPose & pose) {
    const glm::vec3 ray(std::sin(pose.yaw) * std::cos(pose.pitch), std::sin(pose.pitch), -std::cos(pose.yaw) * std::cos(pose.pitch));
    if (ray.y >= -0.001f) return {};
    const float distance = -pose.position.y / ray.y;
    if (distance <= 0.0f || distance > 100.0f) return {};
    const glm::vec3 position = pose.position + ray * distance;
    return glm::vec3(position.x, 0.0f, position.z);
}

class FirstPersonController {
public:
    CameraPose pose;

    void update(win::Window & window, float elapsedSeconds) {
        const float dt          = std::clamp(elapsedSeconds, 0.0f, 0.1f);
        const float forwardAxis = static_cast<float>(window.getKeyStatus(win::KeyCode::W).down) - static_cast<float>(window.getKeyStatus(win::KeyCode::S).down);
        const float rightAxis   = static_cast<float>(window.getKeyStatus(win::KeyCode::D).down) - static_cast<float>(window.getKeyStatus(win::KeyCode::A).down);
        const glm::vec3 forward(std::sin(pose.yaw), 0.0f, -std::cos(pose.yaw));
        const glm::vec3 right(std::cos(pose.yaw), 0.0f, std::sin(pose.yaw));
        glm::vec3       movement = forward * forwardAxis + right * rightAxis;
        if (glm::dot(movement, movement) > 1.0f) movement = glm::normalize(movement);
        const float speed = window.getKeyStatus(win::KeyCode::LSHIFT).down ? 8.0f : 4.0f;
        pose.position += movement * speed * dt;

        if (!window.getKeyStatus(win::KeyCode::MOUSEBTN_0).down) {
            mHasPreviousMousePosition = false;
            return;
        }

        int x = 0, y = 0;
        window.getMousePosition(x, y);
        if (mHasPreviousMousePosition) {
            pose.yaw += static_cast<float>(x - mPreviousMouseX) * kMouseSensitivity;
            pose.pitch -= static_cast<float>(y - mPreviousMouseY) * kMouseSensitivity;
            pose.pitch = std::clamp(pose.pitch, -1.5f, 1.5f);
        }
        mPreviousMouseX           = x;
        mPreviousMouseY           = y;
        mHasPreviousMousePosition = true;
    }

private:
    static constexpr float kMouseSensitivity         = 0.0025f;
    int                    mPreviousMouseX           = 0;
    int                    mPreviousMouseY           = 0;
    bool                   mHasPreviousMousePosition = false;
};

class WorldScene {
public:
    e2::Universe                   universe;
    AutoRef<e2::World>             world;
    e2::PhysicalScale              scale = e2::PhysicalScale::MICROMETER();
    AutoRef<UnlitKernel>           kernel;
    AutoRef<SharedShaderConstants> constants;
    RasterGeometry                 box;
    AutoRef<GpuPayload>            initialUpload;

    bool initialize(const AutoRef<GpuContext> & gpu, bool addHouse) {
        world = e2::World::create(universe, "taixu-prototype");
        if (!world || !taixu::prototype::createWorld(universe, *world, scale)) return false;
        if (addHouse && !taixu::prototype::addWoodHouse(universe, *world, scale, {-3.5, 0.0, 0.0})) return false;
        const auto   initialPrime      = world->primeSnapshot();
        const size_t expectedFormCount = addHouse ? 12 : 6;
        if (!initialPrime || initialPrime->query<e2::TransformFacet, e2::VisualFacet>().size() != expectedFormCount) return false;

        kernel       = UnlitKernel::create(gpu);
        constants    = SharedShaderConstants::create({.gpu = gpu});
        auto uploads = GpuCnC::create({.gpu = gpu});
        if (!kernel || !constants || !uploads) return false;

        LitKernelInputs::CubeCreateOptions boxOptions;
        boxOptions.width  = 1.0f;
        boxOptions.height = 1.0f;
        boxOptions.depth  = 1.0f;
        box               = LitKernelInputs::createBox(gpu, *uploads, boxOptions);
        if (!box.indexCount) return false;

        initialUpload = uploads->seal();
        return !!initialUpload;
    }

    bool record(GpuRaster & raster, const RasterTarget & target, const CameraPose & pose, const std::optional<glm::vec3> & targetPosition) {
        const glm::vec3 forward(std::sin(pose.yaw) * std::cos(pose.pitch), std::sin(pose.pitch), -std::cos(pose.yaw) * std::cos(pose.pitch));
        const auto      rasterSize            = target.calcRasterSizeInPixel();
        constants->set0.camera.cameraPosition = pose.position;
        constants->set0.camera.cameraOrientation =
            glm::quat_cast(glm::mat3(glm::inverse(glm::lookAtRH(pose.position, pose.position + forward, glm::vec3(0, 1, 0)))));
        constants->set0.camera.aspectRatio       = static_cast<float>(rasterSize.x) / static_cast<float>(rasterSize.y);
        constants->set0.camera.viewWidthInPixel  = rasterSize.x;
        constants->set0.camera.viewHeightInPixel = rasterSize.y;
        constants->set0.camera.nearPlane         = 0.1f;
        constants->set0.camera.farPlane          = 100.0f;
        constants->set0.camera.exposure          = 1.0f;

        const auto snapshot = constants->takeSnapshot();
        auto       prime    = world->primeSnapshot();
        if (!prime) return false;
        for (const auto formId : prime->query<e2::TransformFacet, e2::VisualFacet>()) {
            e2::WorldTransform transform;
            const auto *       visual = prime->get<e2::VisualFacet>(formId);
            if (!visual || !e2::resolveWorldTransform(*prime, formId, transform)) return false;
            UnlitKernel::Inputs input;
            input.geometry      = box;
            input.color         = visual->color;
            const auto position = toMeters(transform.position, scale);
            input.worldFromObject =
                glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(transform.orientation) * glm::scale(glm::mat4(1.0f), visual->halfExtent * 2.0f);
            if (!kernel->record(raster, snapshot.set0Resources, input)) return false;
        }
        if (targetPosition) {
            UnlitKernel::Inputs marker;
            marker.geometry = box;
            marker.color    = {1.0f, 0.88f, 0.18f, 1.0f};
            marker.worldFromObject =
                glm::translate(glm::mat4(1.0f), *targetPosition + glm::vec3(0.0f, 0.06f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.7f, 0.12f, 0.7f));
            if (!kernel->record(raster, snapshot.set0Resources, marker)) return false;
        }

        mLatestSnapshot = snapshot;
        return true;
    }

    const SharedShaderConstants::Snapshot & latestSnapshot() const { return mLatestSnapshot; }

private:
    SharedShaderConstants::Snapshot mLatestSnapshot;
};

bool renderFrame(const AutoRef<GpuContext> & gpu, WorldScene & scene, const CameraPose & camera, const std::optional<glm::vec3> & targetPosition,
                 const AutoRef<Texture> & color, const AutoRef<Texture> & depth, const GpuResourceView * swapchainView = nullptr,
                 const AutoRef<GpuPayload> * ready = nullptr, fx2::ImGuiBackend * imgui = nullptr, float elapsedSeconds = 0.0f,
                 AutoRef<GpuPayload> * renderedOut = nullptr) {
    RasterTarget    target;
    GpuResourceView colorView;
    if (swapchainView) {
        colorView = *swapchainView;
    } else {
        colorView.resource = color;
    }
    GpuResourceView depthView;
    depthView.resource = depth;
    target.setColorTarget(0, colorView).setDepthStencilTarget(depthView).setClearColor(0.36f, 0.61f, 0.82f, 1.0f).setClearDepth(1.0f);
    target.states.depthState = RasterState::DepthState {RasterState::Compare::LESS, true};
    if (imgui) {
        auto & blend   = target.colorTargets[0].blendState;
        blend.colorSrc = RasterTarget::BlendState::SRC_ALPHA;
        blend.colorDst = RasterTarget::BlendState::INV_SRC_ALPHA;
    }

    auto raster = GpuRaster::create("taixu.world-frame", {.gpu = gpu, .target = &target});
    if (!raster || !scene.record(*raster, target, camera, targetPosition)) return false;
    AutoRef<GpuPayload> imguiUpload;
    if (imgui) {
        imgui->newFrame(elapsedSeconds);
        const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        ImGui::SetNextWindowBgAlpha(0.72f);
        ImGui::Begin("Taixu World");
        ImGui::TextUnformatted("WASD move | Shift sprint | hold left mouse to look | Esc quit");
        if (targetPosition) {
            ImGui::Text("Ground target: X %.2f  Y %.2f  Z %.2f", targetPosition->x, targetPosition->y, targetPosition->z);
        } else {
            ImGui::TextUnformatted("Ground target: none (look down at the ground)");
        }
        ImGui::End();
        const ImVec2 center(displaySize.x * 0.5f, displaySize.y * 0.5f);
        ImGui::GetForegroundDrawList()->AddLine(ImVec2(center.x - 8.0f, center.y), ImVec2(center.x + 8.0f, center.y), IM_COL32_WHITE, 2.0f);
        ImGui::GetForegroundDrawList()->AddLine(ImVec2(center.x, center.y - 8.0f), ImVec2(center.x, center.y + 8.0f), IM_COL32_WHITE, 2.0f);
        imgui->render();
        if (!imgui->record(*raster, imguiUpload)) return false;
    }
    auto frame = raster->seal();
    if (!frame) return false;

    GpuContext::SubmitParameters submission("taixu.world-frame");
    if (scene.initialUpload) submission.appendWork(scene.initialUpload);
    for (const auto & payload : scene.latestSnapshot().set0Payloads) submission.appendWork(payload);
    submission.appendWork(imguiUpload);
    submission.appendWork(frame);
    if (ready) submission.waitFor(*ready);
    gpu->submit(submission);
    scene.initialUpload.clear();
    if (renderedOut) *renderedOut = frame;
    return true;
}

AutoRef<Texture> createTexture(const AutoRef<GpuContext> & gpu, const char * name, gfx::img::PixelFormat format) {
    return Texture::create(name, {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(format).setDimensions(kWidth, kHeight).setLevels(1)});
}

bool renderHeadless(const AutoRef<GpuContext> & gpu, WorldScene & scene, const CameraPose & camera, const char * outputPath) {
    auto       color          = createTexture(gpu, "taixu.headless-color", gfx::img::PixelFormat::RGBA8());
    auto       depth          = createTexture(gpu, "taixu.headless-depth", gfx::img::PixelFormat::D_32_FLOAT());
    const auto targetPosition = groundTarget(camera);
    if (!color || !depth || !renderFrame(gpu, scene, camera, targetPosition, color, depth)) return false;
    if (targetPosition) std::printf("Ground target: X %.2f, Y %.2f, Z %.2f\n", targetPosition->x, targetPosition->y, targetPosition->z);

    // Texture readback waits for GPU completion; this path is intentionally for verification.
    const auto image = color->readback();
    if (image.empty()) return false;

    image.save(std::string(outputPath));
    return true;
}

} // namespace

int main(int argc, const char ** argv) {
    bool         headless        = false;
    bool         addHouse        = false;
    const char * outputPath      = "taixu-headless.png";
    bool         outputSpecified = false;
    for (int i = 1; i < argc; ++i) {
        const std::string argument(argv[i]);
        if (argument == "--headless") {
            headless = true;
        } else if (argument == "--add-house") {
            addHouse = true;
        } else if (argument == "--help" || argument == "-h") {
            std::puts("Usage: GNtaixu [--headless [output.png]] [--add-house]");
            return 0;
        } else if (headless && !outputSpecified && !argument.starts_with("-")) {
            outputPath      = argv[i];
            outputSpecified = true;
        } else {
            std::fprintf(stderr, "Unknown or misplaced argument: %s\n", argv[i]);
            return 1;
        }
    }

    if (headless) {
#if GN_BUILD_HAS_MSW
        _putenv_s("DISPLAY", "");
#else
        // Vulkan platform probing may try to connect to a stale X11 DISPLAY even for offscreen work.
        unsetenv("DISPLAY");
#endif
    }

    Host host;
    host.gpu = GpuContext::create("taixu-prototype", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!host.gpu) return 1;

    WorldScene scene;
    if (!scene.initialize(host.gpu, addHouse)) return 1;

    FirstPersonController controller;
    if (headless) return renderHeadless(host.gpu, scene, controller.pose, outputPath) ? 0 : 1;

    host.window.reset(win::createWindow({.caption = "Taixu | WASD move, hold left mouse to look, Esc quit", .clientWidth = kWidth, .clientHeight = kHeight}));
    if (!host.window) return 1;
    host.window->show();

    host.surface = host.window->createVulkanSurfaceHandle(host.gpu->getVulkanInstanceHandle());
    if (!host.surface) return 1;

    Swapchain::CreateDesc swapchainDesc {.gpu = host.gpu, .width = kWidth, .height = kHeight};
    swapchainDesc.setSurface(host.surface);
    host.swapchain = Swapchain::create(swapchainDesc);
    if (!host.swapchain) return 1;

    auto depth = createTexture(host.gpu, "taixu.window-depth", gfx::img::PixelFormat::D_32_FLOAT());
    if (!depth) return 1;
    auto imgui = ImGuiBackend::create({.gpu = host.gpu, .window = *host.window});
    if (!imgui) return 1;
    auto previousFrameTime = std::chrono::steady_clock::now();
    while (host.window->runUntilNoNewEvents()) {
        const auto  now            = std::chrono::steady_clock::now();
        const float elapsedSeconds = std::chrono::duration<float>(now - previousFrameTime).count();
        previousFrameTime          = now;
        if (host.window->getKeyStatus(win::KeyCode::ESCAPE).down) break;
        controller.update(*host.window, elapsedSeconds);

        auto acquired = host.swapchain->prepare();
        if (acquired.view.empty()) return 1;
        const auto          targetPosition = groundTarget(controller.pose);
        AutoRef<GpuPayload> rendered;
        if (!renderFrame(host.gpu, scene, controller.pose, targetPosition, {}, depth, &acquired.view, &acquired.ready, imgui.get(), elapsedSeconds, &rendered))
            return 1;
        host.swapchain->present(*rendered);
    }

    return 0;
}
