// Minimal standalone host for the Taixu world prototype.

#include <garnet/GNfx2.h>
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

struct Landmark {
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec4 color;
};

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
    AutoRef<UnlitKernel>           kernel;
    AutoRef<SharedShaderConstants> constants;
    RasterGeometry                 box;
    AutoRef<GpuPayload>            initialUpload;

    bool initialize(const AutoRef<GpuContext> & gpu) {
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
        for (const auto & landmark : landmarks()) {
            UnlitKernel::Inputs input;
            input.geometry        = box;
            input.color           = landmark.color;
            input.worldFromObject = glm::translate(glm::mat4(1.0f), landmark.position) * glm::scale(glm::mat4(1.0f), landmark.scale);
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
    static ArrayView<const Landmark> landmarks() {
        static constexpr Landmark scene[] = {
            {{0.0f, -0.15f, 0.0f}, {24.0f, 0.3f, 24.0f}, {0.25f, 0.42f, 0.28f, 1.0f}}, {{-3.5f, 0.8f, 0.0f}, {2.2f, 1.6f, 2.0f}, {0.72f, 0.34f, 0.18f, 1.0f}},
            {{-3.5f, 1.9f, 0.0f}, {2.5f, 0.35f, 2.3f}, {0.86f, 0.63f, 0.28f, 1.0f}},   {{3.5f, 1.1f, -1.0f}, {1.2f, 2.2f, 1.2f}, {0.30f, 0.54f, 0.78f, 1.0f}},
            {{6.0f, 0.6f, -4.0f}, {1.0f, 1.2f, 1.0f}, {0.78f, 0.32f, 0.25f, 1.0f}},    {{6.0f, 1.6f, -4.0f}, {1.8f, 0.35f, 1.8f}, {0.92f, 0.72f, 0.35f, 1.0f}},
            {{-7.0f, 0.7f, -5.0f}, {0.55f, 1.4f, 0.55f}, {0.38f, 0.28f, 0.18f, 1.0f}}, {{-7.0f, 1.8f, -5.0f}, {2.2f, 1.2f, 2.2f}, {0.20f, 0.52f, 0.30f, 1.0f}},
        };
        return {scene, 8};
    }

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
    bool         headless   = false;
    const char * outputPath = "taixu-headless.png";
    if (argc > 1 && std::string(argv[1]) == "--headless") {
        headless = true;
        if (argc > 2) outputPath = argv[2];
    } else if (argc > 1 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
        std::puts("Usage: GNtaixu [--headless [output.png]]");
        return 0;
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
    if (!scene.initialize(host.gpu)) return 1;

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
