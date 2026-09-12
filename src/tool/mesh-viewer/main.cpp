#include <garnet/GNengine2.h>
#include <garnet/GNfx2.h>
#include <garnet/GNui2.h>
#include <garnet/GNwin.h>

#include <glm/geometric.hpp>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace GN;
using namespace GN::e2;

namespace {

Logger * sLogger = getLogger("GN.tool.mesh-viewer");

struct Options {
    StrA snapshot;
    StrA path;
    bool headless     = false;
    bool print        = false;
    bool finiteFrames = false;
    int  frames       = 3;
};

bool parseOptions(int argc, const char * const * argv, Options & options) {
    for (int i = 1; i < argc; ++i) {
        const StrA argument = argv[i];
        if (argument == "--test") {
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
    if (!options.path.empty()) return true;
    GN_ERROR(sLogger, "Usage: GNtool-mesh-viewer [--print] [--test] [--snapshot <image.png|jpg|bmp>] [--frames N] <model.fbx|gltf|glb|stl|ase>");
    return false;
}

WorldVector3 worldPosition(const PhysicalScale & scale, const glm::vec3 & meters) {
    return {spatial::toWorld(scale.fromMeters(meters.x)), spatial::toWorld(scale.fromMeters(meters.y)), spatial::toWorld(scale.fromMeters(meters.z))};
}

LocalCoordinate localDistance(const PhysicalScale & scale, float meters) { return scale.fromMeters(meters); }

void printScene(const fx2::ModelScene & scene) {
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

Ref<ModelVisualFacet> modelFacet(const Ref<Form> & form) {
    for (const auto & facet : form->facets()) {
        if (auto * model = RuntimeType::cast<ModelVisualFacet>(facet.get())) return referenceTo(model);
    }
    return {};
}

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

    auto model = fx2::ModelScene::load({.path = options.path});
    if (!model) return EXIT_FAILURE;
    if (options.print) {
        printScene(*model);
        if (!options.headless) return EXIT_SUCCESS;
    }

    Universe             universe;
    Ref<OperatingDomain> os;
    if (!options.headless) {
        os = OperatingDomain::create({.universe = universe, .caption = "Garnet Mesh Viewer", .width = 1280, .height = 720});
        if (!os) return EXIT_FAILURE;
    }
    auto visual = VisualDomain::create({.universe = universe, .os = os});
    if (!visual) return EXIT_FAILURE;
    VisualEnvironment::Desc environment {
        .skyboxPath      = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/skybox-cube.dds",
        .irradiancePath  = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/irradiance.dds",
        .prefilteredPath = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/prefiltered.dds",
        .brdfLutPath     = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/brdf_lut.dds",
        .radianceScale   = 1.0f,
    };
    auto environmentMoment = VisualEnvironment::create({.universe = universe, .gpu = visual->gpu(), .description = environment});
    if (!environmentMoment) return EXIT_FAILURE;

    AutoRef<ui2::ImGuiBackend> ui;
    if (os) {
        ui = ui2::ImGuiBackend::create({.gpu = visual->gpu(), .window = *os->window()});
        if (!ui) return EXIT_FAILURE;
    }
    // Camera fitting and orbit positions need sub-meter precision; world units are integers.

    auto world  = Simple::createWorld(universe, PhysicalScale::MICROMETER());
    auto form   = createModelForm(universe, "model", model);
    auto camera = Camera::create({.domain = visual});
    if (!world || !form || !camera) return EXIT_FAILURE;

    const glm::vec3 center      = (model->bounds.minimum + model->bounds.maximum) * 0.5f;
    const float     radius      = std::max(glm::length(model->bounds.maximum - model->bounds.minimum) * 0.5f, 0.001f);
    const auto      boundsScene = fx2::ModelScene::createDebugVisualization(model->bounds, radius * 0.004f, true, false);
    const auto      axesScene   = fx2::ModelScene::createDebugVisualization(model->bounds, radius * 0.004f, false, true);
    auto            boundsForm  = createModelForm(universe, "model-bounds", boundsScene);
    auto            axesForm    = createModelForm(universe, "model-axes", axesScene);
    const auto      boundsFacet = modelFacet(boundsForm);
    const auto      axesFacet   = modelFacet(axesForm);
    if (!boundsForm || !axesForm || !boundsFacet || !axesFacet) return EXIT_FAILURE;
    form->setPosition(worldPosition(world->scale, -center));
    boundsForm->setPosition(worldPosition(world->scale, -center));
    axesForm->setPosition(worldPosition(world->scale, -center));
    Ref<Form> forms[] = {form, boundsForm, axesForm};
    world->populate({forms, 3});

    camera->desc.position     = worldPosition(world->scale, {0, 0, radius * 2.5f});
    camera->desc.orientation  = glm::quat(1, 0, 0, 0);
    camera->desc.nearPlane    = localDistance(world->scale, std::max(radius / 1000.0f, 0.0001f));
    camera->desc.farPlane     = localDistance(world->scale, std::max(radius * 20.0f, 1.0f));
    camera->desc.fovYInDegree = 45.0f;
    Ref<Camera> cameras[]     = {camera};

    ArcballCameraController arcball;
    arcball.resetToFit(model->bounds.minimum - center, model->bounds.maximum - center, camera->desc.fovYInDegree);
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
        arcball.resetToFit(model->bounds.minimum - center, model->bounds.maximum - center, camera->desc.fovYInDegree);
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
        if (os && !os->processEvents()) break;
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
            if (ImGui::Checkbox("Bounds", &showBounds)) boundsFacet->setVisible(showBounds);
            ImGui::SameLine();
            if (ImGui::Checkbox("Axes", &showAxes)) axesFacet->setVisible(showAxes);
            if (ImGui::SliderFloat("Environment exposure", &environment.radianceScale, 0.0f, 4.0f, "%.2f")) {
                environmentMoment = VisualEnvironment::create({.universe = universe, .gpu = visual->gpu(), .description = environment});
                if (!environmentMoment) return EXIT_FAILURE;
            }

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

        if (os) {
            win::Window & window = *os->window();
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
                const float     viewportHeight = static_cast<float>(std::max(os->clientSize().y, 1u));
                if (!captureMouse && navigationMode == NavigationMode::ARCBALL) {
                    if (keyDown(window, win::KeyCode::MOUSEBTN_0)) arcball.rotate(delta, viewportHeight);
                    if (keyDown(window, win::KeyCode::MOUSEBTN_1)) arcball.pan(delta, viewportHeight, camera->desc.fovYInDegree);
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

        const glm::vec3 eye      = navigationMode == NavigationMode::ARCBALL ? arcball.eyePosition() : fly.position;
        camera->desc.position    = worldPosition(world->scale, eye);
        camera->desc.orientation = navigationMode == NavigationMode::ARCBALL ? arcball.orientation : fly.orientation;
        auto tableau             = world->snapshot({.domain = visual, .cameras = {cameras, 1}});
        if (!tableau) return EXIT_FAILURE;
        tableau->add(environmentMoment);
        if (ui) tableau->add(ui);
        visual->render(tableau);
    }
    if (!options.snapshot.empty()) {
        auto image = visual->readbackFrame();
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
