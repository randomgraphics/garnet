#include <garnet/GNengine2.h>
#include <garnet/GNfx2.h>

#include <glm/geometric.hpp>

#include <algorithm>
#include <cstdlib>
#include <string>

using namespace GN;
using namespace GN::e2;

namespace {

Logger * sLogger = getLogger("GN.tool.mesh-viewer");

struct Options {
    StrA path;
    bool headless = false;
    bool print    = false;
    int  frames   = 3;
};

bool parseOptions(int argc, const char * const * argv, Options & options) {
    for (int i = 1; i < argc; ++i) {
        const StrA argument = argv[i];
        if (argument == "--test") {
            options.headless = true;
        } else if (argument == "--print") {
            options.print = true;
        } else if (argument == "--frames" && i + 1 < argc) {
            options.frames = std::max(1, std::atoi(argv[++i]));
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
    GN_ERROR(sLogger, "Usage: GNtool-mesh-viewer [--print] [--test] [--frames N] <model.fbx|gltf|glb|stl>");
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

} // namespace

int main(int argc, const char * argv[]) {
    Options options;
    if (!parseOptions(argc, argv, options)) return EXIT_FAILURE;

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
    visual->setEnvironment({
        .skyboxPath      = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/skybox-cube.dds",
        .irradiancePath  = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/irradiance.dds",
        .prefilteredPath = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/prefiltered.dds",
        .brdfLutPath     = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/brdf_lut.dds",
        .radianceScale   = 1.0f,
    });

    auto world  = Simple::createWorld(universe);
    auto form   = createModelForm(universe, "model", model);
    auto camera = Camera::create({.domain = visual});
    if (!world || !form || !camera) return EXIT_FAILURE;

    const glm::vec3 center     = (model->bounds.minimum + model->bounds.maximum) * 0.5f;
    const float     radius     = std::max(glm::length(model->bounds.maximum - model->bounds.minimum) * 0.5f, 0.001f);
    const auto      debugScene = fx2::ModelScene::createDebugVisualization(model->bounds, radius * 0.004f);
    auto            debugForm  = createModelForm(universe, "model-debug-visualization", debugScene);
    if (!debugForm) return EXIT_FAILURE;
    form->setPosition(worldPosition(world->scale, -center));
    debugForm->setPosition(worldPosition(world->scale, -center));
    Ref<Form> forms[] = {form, debugForm};
    world->populate({forms, 2});

    camera->desc.position     = worldPosition(world->scale, {0, 0, radius * 2.5f});
    camera->desc.orientation  = glm::quat(1, 0, 0, 0);
    camera->desc.nearPlane    = localDistance(world->scale, std::max(radius / 1000.0f, 0.0001f));
    camera->desc.farPlane     = localDistance(world->scale, std::max(radius * 20.0f, 1.0f));
    camera->desc.fovYInDegree = 45.0f;
    Ref<Camera> cameras[]     = {camera};

    for (int frame = 0; options.headless ? frame < options.frames : true; ++frame) {
        if (os && !os->processEvents()) break;
        auto moment = world->captureVisualMoment({.domain = visual, .cameras = {cameras, 1}});
        if (!moment) return EXIT_FAILURE;
        visual->render(moment);
    }
    return EXIT_SUCCESS;
}
