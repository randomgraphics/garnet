#include "model-scene.h"
#include "scene-renderer.h"
#include "navigation.h"
#include <garnet/GNfx2.h>
#include <garnet/GNwin.h>

#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace GN;
using namespace GN::gpu2;
using namespace GN::viewer;

namespace {

Logger * sLogger = getLogger("GN.tool.mesh-viewer");

struct Options {
    viewer::Shading surface = viewer::Shading::IMPORTED;
    StrA            snapshot;
    StrA            path;
    bool            headless             = false;
    bool            print                = false;
    bool            finiteFrames         = false;
    int             frames               = 3;
    float           environmentLuminance = 250.f;
    float           exposure             = 0;
};

bool parseOptions(int argc, const char * const * argv, Options & options) {
    for (int i = 1; i < argc; ++i) {
        const StrA argument = argv[i];
        if (argument == "--box" || argument == "--sphere") {
            options.path = argument;
        } else if (argument == "--surface" && i + 1 < argc) {
            const StrA choice = argv[++i];
            if (choice == "pbr")
                options.surface = viewer::Shading::PBR;
            else if (choice == "cel") {
                GN_ERROR(sLogger, "Cel shading is temporarily disabled in the bindless mesh viewer");
                return false;
            } else if (choice == "unlit")
                options.surface = viewer::Shading::UNLIT;
            else if (choice == "lambertian")
                options.surface = viewer::Shading::LAMBERTIAN;
            else {
                GN_ERROR(sLogger, "Unknown surface '{}'", choice);
                return false;
            }
        } else if (argument == "--exposure" && i + 1 < argc) {
            const char * text = argv[++i];
            char *       end  = nullptr;
            options.exposure  = std::strtof(text, &end);
            if (end == text || *end != '\0' || !std::isfinite(options.exposure) || options.exposure <= 0) {
                GN_ERROR(sLogger, "--exposure requires a positive finite number");
                return false;
            }
        } else if (argument == "--test") {
            options.headless = true;
        } else if (argument == "--snapshot") {
            if (i + 1 == argc || !argv[i + 1][0] || argv[i + 1][0] == '-' || !options.snapshot.empty()) {
                GN_ERROR(sLogger, "--snapshot requires one output image filename");
                return false;
            }
            options.snapshot = argv[++i];
            options.headless = true;
        } else if (argument == "--print") {
            options.print = true;
        } else if (argument == "--frames" && i + 1 < argc) {
            options.frames       = std::max(1, std::atoi(argv[++i]));
            options.finiteFrames = true;
        } else if ((argument == "--env-luminance" || argument == "--environment-luminance") && i + 1 < argc) {
            char *       end             = nullptr;
            const char * text            = argv[++i];
            options.environmentLuminance = std::strtof(text, &end);
            if (end == text || !end || *end != '\0' || std::isnan(options.environmentLuminance) || std::isinf(options.environmentLuminance)) {
                GN_ERROR(sLogger, "Unable to parse '--env-luminance' value '{}'", text);
                return false;
            }
            options.environmentLuminance = std::max(0.0f, options.environmentLuminance);
        } else if (!argument.empty() && argument[0] == '-') {
            GN_ERROR(sLogger, "Unknown option '{}'", argument);
            return false;
        } else if (options.path.empty()) {
            options.path = argument;
        } else {
            GN_ERROR(sLogger, "Only one model path may be specified");
            return false;
        }
    }
    if (options.exposure == 0) options.exposure = 0.002f;
    if (!options.path.empty()) return true;
    GN_ERROR(sLogger, "Usage: GNtool-mesh-viewer [--print] [--test] [--snapshot <image.png|jpg|bmp>] [--frames N] [--environment-luminance <nits>] "
                      "[--surface pbr|unlit|lambertian] [--exposure <positive value>] <model.fbx|gltf|glb|stl|ase|--box|--sphere>");
    return false;
}

// The viewer owns presentation directly; destruction must drain GPU work before the
// swapchain/surface/window disappear, including early error returns.
struct FrameHost {
    AutoRef<GpuContext>          gpu;
    std::unique_ptr<win::Window> window;
    intptr_t                     surface = 0;
    AutoRef<Swapchain>           swapchain;
    AutoRef<Texture>             depth, lastFrame;
    uint32_t                     width = 1280, height = 720;

    ~FrameHost() {
        if (gpu) gpu->waitForIdle();
        swapchain.clear();
        if (surface) window->destroyVulkanSurfaceHandle(gpu->getVulkanInstanceHandle(), surface);
    }
    bool resize(uint32_t w, uint32_t h) {
        gpu->waitForIdle();
        swapchain.clear();
        auto format = gfx::img::PixelFormat::RGBA_8_8_8_8_SRGB();
        if (surface) {
            format.swizzle0 = gfx::img::PixelFormat::SWIZZLE_Z;
            format.swizzle2 = gfx::img::PixelFormat::SWIZZLE_X;
        }
        Swapchain::CreateDesc desc {.gpu = gpu, .width = w, .height = h};
        desc.setFormat(format).setSurface(surface);
        swapchain = Swapchain::create(desc);
        depth     = Texture::create("viewer.depth",
                                    {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gpu->caps().defaultDepthFormat).setDimensions(w, h)});
        width     = w;
        height    = h;
        return swapchain && depth;
    }
    bool initialize(bool headless) {
        gpu = GpuContext::create("mesh-viewer", {});
        if (!gpu) return false;
        if (!headless) {
            window.reset(win::createWindow({.caption = "Garnet Mesh Viewer", .clientWidth = width, .clientHeight = height}));
            if (!window) return false;
            window->show();
            surface = window->createVulkanSurfaceHandle(gpu->getVulkanInstanceHandle());
            if (!surface) return false;
            const auto size = window->getClientSize();
            width           = size.x;
            height          = size.y;
        }
        return resize(width, height);
    }
};

void printScene(const viewer::ModelScene & scene) {
    GN_INFO(sLogger, "Model: {}", scene.sourcePath);
    GN_INFO(sLogger, "Nodes: {}, primitives: {}, materials: {}, textures: {}", scene.nodes.size(), scene.primitives.size(), scene.materials.size(),
            scene.textures.size());
    GN_INFO(sLogger, "Bounds: ({}, {}, {}) - ({}, {}, {})", scene.bounds.minimum.x, scene.bounds.minimum.y, scene.bounds.minimum.z, scene.bounds.maximum.x,
            scene.bounds.maximum.y, scene.bounds.maximum.z);
    for (size_t nodeIndex = 0; nodeIndex < scene.nodes.size(); ++nodeIndex) {
        const auto & node  = scene.nodes[nodeIndex];
        size_t       depth = 0;
        for (int32_t parent = node.parent; parent >= 0 && static_cast<size_t>(parent) < nodeIndex; parent = scene.nodes[parent].parent) ++depth;
        GN_INFO(sLogger, "{}{} [{} primitive(s)]", std::string(depth * 2, ' '), node.name, node.primitives.size());
    }
    for (const StrA & warning : scene.warnings) GN_WARN(sLogger, "Import warning: {}", warning);
}

enum class NavigationMode { ARCBALL, FLY_BY };

bool keyDown(const win::Window & window, win::KeyCode key) { return window.getKeyStatus(key).down; }

} // namespace

int main(int argc, const char * argv[]) {
    Options options;
    if (!parseOptions(argc, argv, options)) return EXIT_FAILURE;

    gfx::img::ImageDesc::SaveToStreamParameters imageParameters;
    if (!options.snapshot.empty()) {
        StrA extension = fs::extName(options.snapshot);
        extension.toLower();
        if (extension == ".png") {
            imageParameters.format = gfx::img::ImageDesc::FileFormat::PNG;
        } else if (extension == ".jpg" || extension == ".jpeg") {
            imageParameters.format = gfx::img::ImageDesc::FileFormat::JPG;
        } else if (extension == ".bmp") {
            imageParameters.format = gfx::img::ImageDesc::FileFormat::BMP;
        } else {
            GN_ERROR(sLogger, "Snapshot output must use .png, .jpg, .jpeg, or .bmp");
            return EXIT_FAILURE;
        }
    }

    auto model = options.path == "--box" || options.path == "--sphere" ? viewer::ModelScene::createProcedural(options.path == "--sphere")
                                                                       : viewer::ModelScene::load({.path = options.path});
    if (!model) return EXIT_FAILURE;
    if (options.print) {
        printScene(*model);
        if (!options.headless) return EXIT_SUCCESS;
    }

    FrameHost host;
    if (!host.initialize(options.headless)) return EXIT_FAILURE;
    SceneRenderer renderer;
    if (!renderer.prepare(host.gpu, *model, options.surface, options.environmentLuminance)) return EXIT_FAILURE;
    AutoRef<fx2::ImGuiBackend> ui;
    if (host.window) {
        ui = fx2::ImGuiBackend::create({.gpu = host.gpu, .window = *host.window});
        if (!ui) return EXIT_FAILURE;
    }
    const glm::vec3 center     = (model->bounds.minimum + model->bounds.maximum) * 0.5f;
    const float     radius     = std::max(glm::length(model->bounds.maximum - model->bounds.minimum) * 0.5f, 0.001f);
    const float     nearPlane  = std::max(radius / 1000.0f, 0.0001f);
    const float     farPlane   = std::max(radius * 20.0f, 1.0f);
    constexpr float fovDegrees = 45.f;
    float           exposure   = options.exposure;

    ArcballCameraController arcball;
    arcball.resetToFit(model->bounds.minimum - center, model->bounds.maximum - center, fovDegrees);
    if (options.headless) arcball.orientation = glm::quat(glm::vec3(glm::radians(-15.0f), glm::radians(25.0f), 0.0f));
    FlyCameraController fly {.position = arcball.eyePosition(), .orientation = arcball.orientation, .movementSpeed = radius};
    NavigationMode      navigationMode    = NavigationMode::ARCBALL;
    int                 previousMouseX    = 0;
    int                 previousMouseY    = 0;
    int                 previousWheel     = 0;
    bool                havePointerSample = false;
    bool                showBounds        = true;
    bool                showAxes          = true;
    int                 selectedNode      = model->nodes.empty() ? -1 : 0;
    auto                previousFrameTime = std::chrono::steady_clock::now();

    auto resetToFit = [&] {
        arcball.resetToFit(model->bounds.minimum - center, model->bounds.maximum - center, fovDegrees);
        fly = {.position = arcball.eyePosition(), .orientation = arcball.orientation, .movementSpeed = radius};
    };
    auto setNavigationMode = [&](NavigationMode mode) {
        if (mode == navigationMode) return;
        if (mode == NavigationMode::FLY_BY) {
            fly.position    = arcball.eyePosition();
            fly.orientation = arcball.orientation;
        } else {
            arcball.pivot       = fly.position + fly.orientation * glm::vec3(0, 0, -arcball.distance);
            arcball.orientation = fly.orientation;
        }
        navigationMode = mode;
    };

    for (int frame = 0; options.headless || options.finiteFrames ? frame < options.frames : true; ++frame) {
        if (host.window) {
            if (!host.window->runUntilNoNewEvents()) break;
            const auto size = host.window->getClientSize();
            if (!size.x || !size.y) continue;
            if ((size.x != host.width || size.y != host.height) && !host.resize(size.x, size.y)) return EXIT_FAILURE;
        }
        const auto  now            = std::chrono::steady_clock::now();
        const float elapsedSeconds = std::min(std::chrono::duration<float>(now - previousFrameTime).count(), 0.1f);
        previousFrameTime          = now;

        bool captureMouse = false, captureKeyboard = false;
        if (ui) {
            ui->newFrame(elapsedSeconds);
            ImGui::SetNextWindowSize({360, 620}, ImGuiCond_FirstUseEver);
            ImGui::Begin("Mesh Viewer");
            ImGui::TextUnformatted(model->sourcePath.data());
            ImGui::Text("%zu nodes, %zu primitives", model->nodes.size(), model->primitives.size());
            ImGui::Text("%zu materials, %zu textures", model->materials.size(), model->textures.size());
            ImGui::SeparatorText("Navigation");
            int mode = navigationMode == NavigationMode::ARCBALL ? 0 : 1;
            if (ImGui::RadioButton("Arcball", &mode, 0)) setNavigationMode(NavigationMode::ARCBALL);
            ImGui::SameLine();
            if (ImGui::RadioButton("Fly-by", &mode, 1)) setNavigationMode(NavigationMode::FLY_BY);
            if (ImGui::Button("Reset to fit")) resetToFit();
            ImGui::SameLine();
            ImGui::TextDisabled("F / R");
            ImGui::Checkbox("Bounds", &showBounds);
            ImGui::SameLine();
            ImGui::Checkbox("Axes", &showAxes);
            ImGui::SliderFloat("Camera exposure", &exposure, 0.0001f, 10.f, "%.4f", ImGuiSliderFlags_Logarithmic);
            // The bindless sky material is immutable, so environment calibration is fixed at
            // prepare() time by --environment-luminance and can only be reported here.
            ImGui::TextDisabled("Environment luminance: %.1f nits", options.environmentLuminance);

            ImGui::SeparatorText("Hierarchy");
            if (ImGui::BeginChild("hierarchy", {0, 190}, ImGuiChildFlags_Borders)) {
                for (size_t nodeIndex = 0; nodeIndex < model->nodes.size(); ++nodeIndex) {
                    const auto & node  = model->nodes[nodeIndex];
                    size_t       depth = 0;
                    for (int32_t parent = node.parent; parent >= 0 && static_cast<size_t>(parent) < nodeIndex; parent = model->nodes[parent].parent) ++depth;
                    ImGui::Indent(static_cast<float>(depth) * 12.0f);
                    ImGui::PushID(static_cast<int>(nodeIndex));
                    if (ImGui::Selectable(node.name.empty() ? "<unnamed>" : node.name.data(), selectedNode == static_cast<int>(nodeIndex))) {
                        selectedNode = static_cast<int>(nodeIndex);
                    }
                    ImGui::PopID();
                    ImGui::Unindent(static_cast<float>(depth) * 12.0f);
                }
            }
            ImGui::EndChild();

            ImGui::SeparatorText("Selection");
            if (selectedNode >= 0 && static_cast<size_t>(selectedNode) < model->nodes.size()) {
                const auto & node = model->nodes[static_cast<size_t>(selectedNode)];
                ImGui::Text("%s — %zu primitive(s)", node.name.data(), node.primitives.size());
                for (uint32_t primitiveIndex : node.primitives) {
                    if (primitiveIndex >= model->primitives.size()) continue;
                    const auto & primitive = model->primitives[primitiveIndex];
                    ImGui::BulletText("%s: %zu vertices, %zu triangles", primitive.name.data(), primitive.vertices.size(), primitive.indices.size() / 3);
                    if (primitive.material < model->materials.size()) {
                        const auto & material = model->materials[primitive.material];
                        ImGui::Indent();
                        ImGui::Text("Material: %s  roughness %.2f  metallic %.2f", material.name.data(), material.roughness, material.metallic);
                        ImGui::Unindent();
                    }
                }
            }
            if (!model->warnings.empty() && ImGui::CollapsingHeader("Import warnings")) {
                for (const StrA & warning : model->warnings) ImGui::BulletText("%s", warning.data());
            }
            ImGui::SeparatorText("Diagnostics");
            ImGui::Text("%.1f FPS (%.2f ms)", ImGui::GetIO().Framerate, 1000.0f / std::max(ImGui::GetIO().Framerate, 0.001f));
            ImGui::End();
            captureMouse    = ui->wantsMouse();
            captureKeyboard = ui->wantsKeyboard();
            ui->render();
        }

        if (host.window) {
            win::Window & window = *host.window;
            for (win::KeyEvent event = window.popLastKeyEvent(); event; event = window.popLastKeyEvent()) {
                if (!event.status.down && event.key == win::KeyCode::ESCAPE) return EXIT_SUCCESS;
                if (!captureKeyboard && !event.status.down && event.key == win::KeyCode::R) resetToFit();
                if (!captureKeyboard && !event.status.down && event.key == win::KeyCode::F) {
                    setNavigationMode(navigationMode == NavigationMode::ARCBALL ? NavigationMode::FLY_BY : NavigationMode::ARCBALL);
                    GN_INFO(sLogger, "Navigation mode: {}", navigationMode == NavigationMode::ARCBALL ? "arcball" : "fly-by");
                }
            }

            int mouseX = 0, mouseY = 0;
            window.getMousePosition(mouseX, mouseY);
            const int wheel = window.getAxisStatus()[static_cast<size_t>(win::Axis::MOUSE_WHEEL_0)];
            if (havePointerSample) {
                const glm::vec2 delta(mouseX - previousMouseX, mouseY - previousMouseY);
                const float     viewportHeight = static_cast<float>(std::max(host.window->getClientSize().y, 1u));
                if (!captureMouse && navigationMode == NavigationMode::ARCBALL) {
                    if (keyDown(window, win::KeyCode::MOUSEBTN_0)) arcball.rotate(delta, viewportHeight);
                    if (keyDown(window, win::KeyCode::MOUSEBTN_1)) arcball.pan(delta, viewportHeight, fovDegrees);
                    arcball.zoom(static_cast<float>(wheel - previousWheel) / 120.0f);
                } else if (!captureMouse && navigationMode == NavigationMode::FLY_BY) {
                    if (keyDown(window, win::KeyCode::MOUSEBTN_1)) fly.rotate(delta, viewportHeight);
                }
            }
            if (!captureKeyboard && navigationMode == NavigationMode::FLY_BY) {
                const glm::vec3 motion(static_cast<float>(keyDown(window, win::KeyCode::D)) - static_cast<float>(keyDown(window, win::KeyCode::A)), 0,
                                       static_cast<float>(keyDown(window, win::KeyCode::W)) - static_cast<float>(keyDown(window, win::KeyCode::S)));
                fly.move(motion, elapsedSeconds);
            }
            previousMouseX    = mouseX;
            previousMouseY    = mouseY;
            previousWheel     = wheel;
            havePointerSample = true;
        }

        auto acquired = host.swapchain->prepare();
        if (acquired.view.empty()) return EXIT_FAILURE;
        host.lastFrame = acquired.view.texture();

        auto uniformUploads = bindless::CnC::create("viewer.uniforms", {.gpu = host.gpu, .heap = renderer.heap});
        if (!uniformUploads) return EXIT_FAILURE;
        const bool useArcball = navigationMode == NavigationMode::ARCBALL;
        const auto uniforms   = renderer.updateUniforms(*uniformUploads, {.eye             = useArcball ? arcball.eyePosition() : fly.position,
                                                                          .orientation     = useArcball ? arcball.orientation : fly.orientation,
                                                                          .width           = host.width,
                                                                          .height          = host.height,
                                                                          .nearPlane       = nearPlane,
                                                                          .farPlane        = farPlane,
                                                                          .fovDegrees      = fovDegrees,
                                                                          .exposure        = exposure,
                                                                          .frameDurationMs = elapsedSeconds * 1000.f,
                                                                          .frame           = static_cast<uint32_t>(frame)});
        if (!uniforms) return EXIT_FAILURE;
        auto uniformWork = uniformUploads->seal();
        if (!uniformWork) return EXIT_FAILURE;

        RasterTarget    target;
        GpuResourceView depth;
        depth.resource = host.depth;
        target.setColorTarget(0, acquired.view).setDepthStencilTarget(depth).setClearDepth(1.f).setClearColor(0.05f, 0.06f, 0.09f, 1.f);
        auto & blend   = target.colorTargets[0].blendState;
        blend.colorSrc = RasterTarget::BlendState::SRC_ALPHA;
        blend.colorDst = RasterTarget::BlendState::INV_SRC_ALPHA;
        blend.alphaSrc = RasterTarget::BlendState::ONE;
        blend.alphaDst = RasterTarget::BlendState::INV_SRC_ALPHA;

        auto raster = bindless::Raster::create("viewer.frame", {.gpu               = host.gpu,
                                                                .target            = &target,
                                                                .heap              = renderer.heap,
                                                                .passResources     = fx2::bindless::sharedUniformResources(uniforms),
                                                                .numberOfDrawsHint = renderer.drawCount(showBounds, showAxes)});
        if (!raster || !renderer.record(*raster, uniforms, showBounds, showAxes)) return EXIT_FAILURE;
        auto rendered = raster->seal();
        if (!rendered) return EXIT_FAILURE;

        // The ImGui backend still records into a legacy raster, so the overlay is a second pass
        // over the same backbuffer. It loads rather than clears the scene's color and needs no depth.
        AutoRef<GpuPayload> uiUpload, uiPass;
        if (ui) {
            RasterTarget uiTarget;
            uiTarget.setColorTarget(0, acquired.view);
            uiTarget.loadColor = true;
            auto & uiBlend     = uiTarget.colorTargets[0].blendState;
            uiBlend.colorSrc   = RasterTarget::BlendState::SRC_ALPHA;
            uiBlend.colorDst   = RasterTarget::BlendState::INV_SRC_ALPHA;
            uiBlend.alphaSrc   = RasterTarget::BlendState::ONE;
            uiBlend.alphaDst   = RasterTarget::BlendState::INV_SRC_ALPHA;
            auto uiRaster      = GpuRaster::create("viewer.ui", {.gpu = host.gpu, .target = &uiTarget});
            if (!uiRaster || !ui->record(*uiRaster, uiUpload)) return EXIT_FAILURE;
            uiPass = uiRaster->seal();
            if (!uiPass) return EXIT_FAILURE;
        }

        GpuContext::SubmitParameters submit("mesh-viewer.frame");
        submit.appendWork(uniformWork);
        if (uiUpload) submit.appendWork(uiUpload);
        submit.appendWork(rendered).waitFor(acquired.ready);
        submit.appendWork(uiPass);
        host.gpu->submit(submit);
        // Present must wait on the last payload touching the backbuffer, not just the scene pass.
        host.swapchain->present(uiPass ? *uiPass : *rendered);
    }
    host.gpu->waitForIdle();
    if (!options.snapshot.empty()) {
        auto image = host.lastFrame ? host.lastFrame->readback() : gfx::img::Image {};
        if (image.empty()) {
            GN_ERROR(sLogger, "Snapshot failed: no successfully rendered frame available");
            return EXIT_FAILURE;
        }
        std::ofstream output(options.snapshot.data(), std::ios::binary | std::ios::trunc);
        if (!output) {
            GN_ERROR(sLogger, "Cannot open snapshot output '{}'", options.snapshot);
            return EXIT_FAILURE;
        }
        // Readback already contains display-encoded bytes. The image writer's
        // numeric conversion must preserve those bytes, not decode them again.
        auto savedDescriptor = image.desc();
        for (auto & plane : savedDescriptor.planes) {
            if (plane.desc.format.sign0 == gfx::img::PixelFormat::SIGN_GNORM) plane.desc.format.sign0 = gfx::img::PixelFormat::SIGN_UNORM;
            if (plane.desc.format.sign12 == gfx::img::PixelFormat::SIGN_GNORM) plane.desc.format.sign12 = gfx::img::PixelFormat::SIGN_UNORM;
        }
        savedDescriptor.save(imageParameters, output, image.data());
        const auto bytes = output.tellp();
        output.close();
        if (!output || bytes <= 0) {
            GN_ERROR(sLogger, "Failed to write snapshot '{}'", options.snapshot);
            return EXIT_FAILURE;
        }
        GN_INFO(sLogger, "Saved snapshot: {}", options.snapshot);
    }
    return EXIT_SUCCESS;
}
