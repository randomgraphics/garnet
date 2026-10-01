// Suspended: excluded from the build pending E2 Simple-world design review.
#include <catch2/catch_test_macros.hpp>
#include "e2/e2-internal.h"

#include <concepts>
#include <utility>

using namespace GN;
using namespace GN::e2;

static_assert(std::derived_from<MeshVisualFacet, VisualFacet>);
static_assert(std::same_as<decltype(MeshVisualFacet::create(std::declval<const MeshVisualFacet::CreateParameters &>())), Ref<MeshVisualFacet>>);
static_assert(std::same_as<decltype(createMeshForm(std::declval<Universe &>(), std::declval<const StrA &>(), std::declval<AutoRef<const fx2::Geometry>>(),
                                                   std::declval<AutoRef<const fx2::Surface>>())),
                           Ref<Form>>);

TEST_CASE("e2 mesh interface remains renderer independent", "[e2][model]") { CHECK(MeshVisualFacet::TYPE_INFO().isDerivedFrom(VisualFacet::TYPE_INFO())); }

static WorldCoordinate worldCoordinate(int64_t value) { return spatial::toWorld(LocalCoordinate(value)); }

TEST_CASE("e2 mesh facet captures immutable instance state", "[e2][model]") {
    const auto geometry = fx2::generateBoxGeometry();
    const auto surface  = fx2::UnlitSurface::create({});
    REQUIRE(geometry);
    REQUIRE(surface);

    Universe universe;
    auto     world = Simple::createWorld(universe, PhysicalScale::METER());
    auto     form  = createMeshForm(universe, "captured-mesh", geometry, surface);
    REQUIRE(world);
    REQUIRE(form);
    form->setPosition({worldCoordinate(3), worldCoordinate(4), worldCoordinate(5)});
    Ref<Form> forms[] = {form};
    world->populate({forms, 1});

    auto   moment   = world->snapshot({});
    auto * captured = RuntimeType::cast<VisualMomentImpl>(RuntimeType::cast<VisualTableauImpl>(moment.get())->moments[0].get());
    REQUIRE(captured);
    REQUIRE(captured->renderables.size() == 1);
    CHECK(captured->renderables[0].geometry.get() == geometry.get());
    CHECK(captured->renderables[0].surface.get() == surface.get());
    CHECK_FALSE(captured->renderables[0].mesh);
    CHECK(captured->renderables[0].translation.x == worldCoordinate(3));

    form->setPosition({worldCoordinate(30), worldCoordinate(40), worldCoordinate(50)});
    CHECK(captured->renderables[0].translation.x == worldCoordinate(3));
    CHECK(captured->renderables[0].translation.y == worldCoordinate(4));
    CHECK(captured->renderables[0].translation.z == worldCoordinate(5));

    auto * modelFacet = RuntimeType::cast<MeshVisualFacet>(form->facets()[0].get());
    REQUIRE(modelFacet);
    modelFacet->setVisible(false);
    CHECK_FALSE(modelFacet->visible());
    auto hidden       = world->snapshot({});
    auto hiddenMoment = RuntimeType::cast<VisualMomentImpl>(RuntimeType::cast<VisualTableauImpl>(hidden.get())->moments[0].get());
    REQUIRE(hiddenMoment);
    CHECK(hiddenMoment->renderables.empty());
}

TEST_CASE("e2 mesh facets reject missing or incompatible inputs", "[e2][model]") {
    Universe                universe;
    fx2::BoxGeometryOptions options;
    options.attributes  = 0;
    const auto geometry = fx2::generateBoxGeometry(options);
    const auto unlit    = fx2::UnlitSurface::create({});
    const auto pbr      = fx2::PbrSurface::create({});
    REQUIRE(geometry);
    REQUIRE(unlit);
    REQUIRE(pbr);
    CHECK_FALSE(MeshVisualFacet::create({.universe = universe, .geometry = {}, .surface = unlit}));
    CHECK_FALSE(MeshVisualFacet::create({.universe = universe, .geometry = geometry, .surface = {}}));
    CHECK_FALSE(MeshVisualFacet::create({.universe = universe, .geometry = geometry, .surface = pbr}));
    CHECK(MeshVisualFacet::create({.universe = universe, .geometry = geometry, .surface = unlit}));
}

TEST_CASE("e2 replacing a mesh surface preserves captured appearance", "[e2][model]") {
    Universe                universe;
    fx2::BoxGeometryOptions options;
    options.attributes                 = 0;
    auto                      geometry = fx2::generateBoxGeometry(options);
    auto                      original = fx2::UnlitSurface::create({});
    fx2::UnlitSurface::Config redConfig;
    redConfig.appearance.color = {1, 0, 0, 1};
    auto replacement           = fx2::UnlitSurface::create(redConfig);
    auto invalid               = fx2::PbrSurface::create({});
    REQUIRE(geometry);
    REQUIRE(original);
    REQUIRE(replacement);
    REQUIRE(invalid);
    auto world = Simple::createWorld(universe);
    auto form  = createMeshForm(universe, "adjustable-surface", geometry, original);
    REQUIRE(world);
    REQUIRE(form);
    auto * facet = RuntimeType::cast<MeshVisualFacet>(form->facets()[0].get());
    REQUIRE(facet);
    Ref<Form> forms[] = {form};
    world->populate({forms, 1});
    auto   before         = world->snapshot({});
    auto * capturedBefore = RuntimeType::cast<VisualMomentImpl>(RuntimeType::cast<VisualTableauImpl>(before.get())->moments[0].get());
    REQUIRE(capturedBefore);
    REQUIRE(capturedBefore->renderables.size() == 1);
    REQUIRE(facet->setSurface(replacement));
    CHECK(facet->surface()->id == replacement->id);
    CHECK_FALSE(facet->setSurface({}));
    CHECK_FALSE(facet->setSurface(invalid));
    CHECK(facet->surface()->id == replacement->id);
    auto   after         = world->snapshot({});
    auto * capturedAfter = RuntimeType::cast<VisualMomentImpl>(RuntimeType::cast<VisualTableauImpl>(after.get())->moments[0].get());
    REQUIRE(capturedAfter);
    REQUIRE(capturedAfter->renderables.size() == 1);
    CHECK(capturedBefore->renderables[0].surface->id == original->id);
    CHECK(capturedAfter->renderables[0].surface->id == replacement->id);
    CHECK(capturedBefore->renderables[0].geometry->id == geometry->id);
    CHECK(capturedAfter->renderables[0].geometry->id == geometry->id);
}

TEST_CASE("e2 mesh hierarchy is captured from forms", "[e2][model]") {
    Universe   universe;
    const auto geometry = fx2::generateBoxGeometry();
    const auto surface  = fx2::UnlitSurface::create({});
    auto       world    = Simple::createWorld(universe);
    auto       parent   = Form::create(universe, "parent");
    auto       child    = createMeshForm(universe, "child", geometry, surface);
    REQUIRE(child);
    parent->setPosition({worldCoordinate(10), worldCoordinate(20), worldCoordinate(30)});
    child->setPosition({worldCoordinate(1), worldCoordinate(2), worldCoordinate(3)});
    REQUIRE(parent->attach(child));
    Ref<Form> roots[] = {parent};
    world->populate({roots, 1});
    auto   tableau = world->snapshot({});
    auto * moment  = RuntimeType::cast<VisualMomentImpl>(RuntimeType::cast<VisualTableauImpl>(tableau.get())->moments[0].get());
    REQUIRE(moment);
    REQUIRE(moment->renderables.size() == 1);
    CHECK(moment->renderables[0].translation.x == worldCoordinate(11));
    CHECK(moment->renderables[0].translation.y == worldCoordinate(22));
    CHECK(moment->renderables[0].translation.z == worldCoordinate(33));
    parent->setPosition({worldCoordinate(100), worldCoordinate(200), worldCoordinate(300)});
    CHECK(moment->renderables[0].translation.x == worldCoordinate(11));
}

TEST_CASE("e2 headless readback contains display-encoded color", "[e2][model][gpu]") {
    Universe universe;
    auto     visual = VisualDomain::create({.universe = universe, .os = {}});
    if (!visual) SKIP("No headless Vulkan visual domain is available");
    auto world  = Simple::createWorld(universe);
    auto camera = Camera::create({.domain = visual});
    REQUIRE(world);
    REQUIRE(camera);
    Ref<Camera> cameras[] = {camera};
    auto        moment    = world->snapshot({.domain = visual, .cameras = {cameras, 1}});
    REQUIRE(moment);
    visual->renderFrame({.tableau = moment, .clearColor = {{0.05f, 0.06f, 0.09f, 1.f}}});
    auto image = visual->readbackFrame();
    REQUIRE_FALSE(image.empty());
    CHECK(image.format() == gfx::img::PixelFormat::RGBA_8_8_8_8_SRGB());
    // The requested clear color is linear (0.05, 0.06, 0.09), not these byte values.
    // Readback must contain the sRGB encoding that a PNG/JPEG viewer displays.
    const auto * rgba = static_cast<const uint8_t *>(image.data());
    CHECK(rgba[0] >= 62);
    CHECK(rgba[0] <= 64);
    CHECK(rgba[1] >= 68);
    CHECK(rgba[1] <= 70);
    CHECK(rgba[2] >= 84);
    CHECK(rgba[2] <= 86);
    CHECK(rgba[3] == 255);
}

TEST_CASE("e2 visual domain renders and caches captured mesh instances", "[e2][model][gpu]") {
    const auto geometry = fx2::generateBoxGeometry();
    const auto surface  = fx2::UnlitSurface::create({});
    REQUIRE(geometry);
    REQUIRE(surface);

    Universe universe;
    auto     visual = VisualDomain::create({.universe = universe, .os = {}});
    if (!visual) SKIP("No headless Vulkan visual domain is available");
    CHECK(visual->readbackFrame().empty());
    auto camera = Camera::create({.domain = visual});
    auto world  = Simple::createWorld(universe, PhysicalScale::METER());
    auto form   = createMeshForm(universe, "rendered-mesh", geometry, surface);
    REQUIRE(camera);
    REQUIRE(world);
    REQUIRE(form);
    camera->desc.position  = {worldCoordinate(0), worldCoordinate(0), worldCoordinate(5)};
    camera->desc.nearPlane = LocalCoordinate(1);
    camera->desc.farPlane  = LocalCoordinate(100);
    Ref<Form> forms[]      = {form};
    world->populate({forms, 1});
    Ref<Camera> cameras[] = {camera};

    auto first = world->snapshot({.domain = visual, .cameras = {cameras, 1}});
    REQUIRE(first);
    visual->renderFrame({.tableau = first});
    auto second = world->snapshot({.domain = visual, .cameras = {cameras, 1}});
    REQUIRE(second);
    visual->renderFrame({.tableau = second});
    auto image = visual->readbackFrame();
    REQUIRE_FALSE(image.empty());
    CHECK(image.width() == 1280);
    CHECK(image.height() == 720);
    // Unlit geometry must visibly render without an environment or direct light.
    const auto * center = static_cast<const uint8_t *>(image.data()) + (image.width() * (image.height() / 2) + image.width() / 2) * 4;
    CHECK(center[0] > 200);
    CHECK(center[1] > 200);
    CHECK(center[2] > 200);
    visual->renderFrame({.tableau = {}});
    CHECK(visual->readbackFrame().empty());
}

TEST_CASE("e2 renders every surface on generated full and half precision meshes", "[e2][model][gpu][surfaces]") {
    Universe universe;
    auto     visual = VisualDomain::create({.universe = universe, .os = {}});
    if (!visual) SKIP("No headless Vulkan visual domain is available");
    auto camera = Camera::create({.domain = visual});
    REQUIRE(camera);
    camera->desc.exposure  = 1.f;
    camera->desc.position  = {worldCoordinate(0), worldCoordinate(0), worldCoordinate(5)};
    camera->desc.nearPlane = LocalCoordinate(1);
    camera->desc.farPlane  = LocalCoordinate(100);
    Ref<Camera> cameras[]  = {camera};

    VisualEnvironment::Desc environmentDescription;
    environmentDescription.environmentLuminanceScale = 1.f;
    auto environment = VisualEnvironment::create({.universe = universe, .gpu = visual->gpu(), .description = environmentDescription});
    REQUIRE(environment);
    // PBR consumes environment lighting; hide only the skybox so the clear-color
    // assertions still distinguish mesh coverage from background pixels.
    environment->setVisible(false);

    fx2::PbrSurface::Config        pbr;
    fx2::CelSurface::Config        cel;
    fx2::UnlitSurface::Config      unlit;
    fx2::LambertianSurface::Config lambertian;
    const glm::vec4                red(0.8f, 0.02f, 0.01f, 1);
    pbr.appearance.color = cel.appearance.color = unlit.appearance.color = lambertian.appearance.color = red;
    AutoRef<const fx2::Surface> surfaces[] = {fx2::PbrSurface::create(pbr), fx2::CelSurface::create(cel), fx2::UnlitSurface::create(unlit),
                                              fx2::LambertianSurface::create(lambertian)};

    for (size_t surfaceIndex = 0; surfaceIndex < 4; ++surfaceIndex) {
        REQUIRE(surfaces[surfaceIndex]);
        for (bool sphere : {false, true}) {
            for (bool half : {false, true}) {
                CAPTURE(surfaceIndex, sphere, half);
                fx2::GeometryOptions options;
                // Exercise the minimal unlit contract through the real GPU vertex path.
                options.attributes   = surfaceIndex == 2 ? 0 : fx2::GeometryOptions::NORMAL | fx2::GeometryOptions::TEXCOORD | fx2::GeometryOptions::TANGENT;
                const auto precision = half ? fx2::Geometry::Precision::FLOAT16 : fx2::Geometry::Precision::FLOAT32;
                options.positionPrecision = options.normalPrecision = options.texcoordPrecision = options.tangentPrecision = precision;
                fx2::BoxGeometryOptions    box;
                fx2::SphereGeometryOptions ball;
                static_cast<fx2::GeometryOptions &>(box)  = options;
                static_cast<fx2::GeometryOptions &>(ball) = options;
                auto geometry                             = sphere ? fx2::generateSphereGeometry(ball) : fx2::generateBoxGeometry(box);
                REQUIRE(geometry);
                auto world = Simple::createWorld(universe, PhysicalScale::METER());
                auto form  = createMeshForm(universe, "surface-mesh", geometry, surfaces[surfaceIndex]);
                auto light =
                    Simple::createPointLight(universe, {worldCoordinate(0), worldCoordinate(0), worldCoordinate(4)}, IntensityRGB {1, 1, 1, Candela {1000}});
                REQUIRE(world);
                REQUIRE(form);
                REQUIRE(light);
                Ref<Form> forms[] = {form, light};
                world->populate({forms, 2});
                auto tableau = world->snapshot({.domain = visual, .cameras = {cameras, 1}});
                tableau->add(environment);
                visual->renderFrame({.tableau = tableau, .clearColor = {{0, 0, 0, 1}}});
                auto image = visual->readbackFrame();
                REQUIRE_FALSE(image.empty());
                const auto * center = static_cast<const uint8_t *>(image.data()) + (image.width() * (image.height() / 2) + image.width() / 2) * 4;
                CHECK(center[0] > 30);
                if (surfaceIndex == 0) {
                    // PBR includes the colored environment's specular reflection.
                    CHECK(center[0] < 250);
                } else {
                    CHECK(center[0] > center[1]);
                    CHECK(center[0] > center[2]);
                }
                const auto * corner = static_cast<const uint8_t *>(image.data());
                CHECK(corner[0] == 0);
                CHECK(corner[1] == 0);
                CHECK(corner[2] == 0);
            }
        }
    }
}

TEST_CASE("e2 unlit ignores light direction while Lambertian follows the surface normal", "[e2][model][gpu][surfaces]") {
    Universe universe;
    auto     visual = VisualDomain::create({.universe = universe, .os = {}});
    if (!visual) SKIP("No headless Vulkan visual domain is available");
    auto camera = Camera::create({.domain = visual});
    REQUIRE(camera);
    camera->desc.position  = {worldCoordinate(0), worldCoordinate(0), worldCoordinate(5)};
    camera->desc.nearPlane = LocalCoordinate(1);
    camera->desc.farPlane  = LocalCoordinate(100);
    Ref<Camera> cameras[]  = {camera};
    auto        geometry   = fx2::generateBoxGeometry();
    REQUIRE(geometry);
    fx2::LambertianSurface::Config diffuse;
    diffuse.ambientIntensity               = 0;
    AutoRef<const fx2::Surface> surfaces[] = {fx2::UnlitSurface::create({}), fx2::LambertianSurface::create(diffuse)};
    for (size_t surfaceIndex = 0; surfaceIndex < 2; ++surfaceIndex) {
        CAPTURE(surfaceIndex);
        auto world = Simple::createWorld(universe, PhysicalScale::METER());
        auto form  = createMeshForm(universe, "direction-test", geometry, surfaces[surfaceIndex]);
        auto light = Simple::createPointLight(universe, {worldCoordinate(0), worldCoordinate(0), worldCoordinate(4)}, IntensityRGB {1, 1, 1, Candela {1000}});
        REQUIRE(world);
        REQUIRE(form);
        REQUIRE(light);
        Ref<Form> forms[] = {form, light};
        world->populate({forms, 2});
        uint8_t center[2][3] {};
        for (size_t side = 0; side < 2; ++side) {
            light->setPosition({worldCoordinate(0), worldCoordinate(0), worldCoordinate(side == 0 ? 4 : -4)});
            auto tableau = world->snapshot({.domain = visual, .cameras = {cameras, 1}});
            visual->renderFrame({.tableau = tableau, .clearColor = {{1, 0, 1, 1}}});
            auto image = visual->readbackFrame();
            REQUIRE_FALSE(image.empty());
            memcpy(center[side], static_cast<const uint8_t *>(image.data()) + (image.width() * (image.height() / 2) + image.width() / 2) * 4, 3);
        }
        for (size_t channel = 0; channel < 3; ++channel) {
            CHECK(center[0][channel] > 30);
            if (surfaceIndex == 0) {
                CHECK(center[0][channel] == center[1][channel]);
            } else {
                CHECK(center[1][channel] <= 1);
            }
        }
    }
}

TEST_CASE("e2 mixes all surface descriptor layouts in one frame", "[e2][model][gpu][surfaces]") {
    Universe universe;
    auto     visual = VisualDomain::create({.universe = universe, .os = {}});
    if (!visual) SKIP("No headless Vulkan visual domain is available");
    auto camera = Camera::create({.domain = visual});
    REQUIRE(camera);
    camera->desc.position   = {worldCoordinate(0), worldCoordinate(0), worldCoordinate(10)};
    camera->desc.exposure   = 1.f;
    Ref<Camera> cameras[]   = {camera};
    auto        environment = VisualEnvironment::create({.universe = universe, .gpu = visual->gpu(), .description = {}});
    REQUIRE(environment);
    environment->setVisible(false);
    auto geometry = fx2::generateBoxGeometry();
    REQUIRE(geometry);
    AutoRef<const fx2::Surface> surfaces[] = {fx2::UnlitSurface::create({}), fx2::LambertianSurface::create({}), fx2::CelSurface::create({}),
                                              fx2::PbrSurface::create({})};
    auto                        world      = Simple::createWorld(universe, PhysicalScale::METER());
    REQUIRE(world);
    DynaArray<Ref<Form>> forms;
    for (size_t i = 0; i < 4; ++i) {
        auto form = createMeshForm(universe, "mixed-surface", geometry, surfaces[i]);
        REQUIRE(form);
        form->setPosition({worldCoordinate(static_cast<int64_t>(i) * 2 - 3), worldCoordinate(0), worldCoordinate(0)});
        forms.append(form);
    }
    auto light = Simple::createPointLight(universe, {worldCoordinate(0), worldCoordinate(0), worldCoordinate(4)}, IntensityRGB {1, 1, 1, Candela {50}});
    REQUIRE(light);
    forms.append(light);
    world->populate({forms.data(), forms.size()});
    // Draw twice so the second frame also exercises already-cached descriptor layouts.
    for (size_t frame = 0; frame < 2; ++frame) {
        auto tableau = world->snapshot({.domain = visual, .cameras = {cameras, 1}});
        tableau->add(environment);
        visual->renderFrame({.tableau = tableau, .clearColor = {{0, 0, 0, 1}}});
        auto image = visual->readbackFrame();
        REQUIRE_FALSE(image.empty());
        const auto * pixels = static_cast<const uint8_t *>(image.data());
        for (size_t i = 0; i < 4; ++i) {
            CAPTURE(frame, i);
            // Front faces lie at z=0.5, 9.5 meters from this perspective camera.
            const float  x      = static_cast<float>(i) * 2.f - 3.f;
            const auto   column = static_cast<uint32_t>(image.width() * 0.5f + x * image.height() / (2.f * std::tan(3.14159265359f / 6.f) * 9.5f));
            const auto * pixel  = pixels + (image.width() * (image.height() / 2) + column) * 4;
            CHECK(pixel[0] > 20);
            CHECK(pixel[1] > 20);
            CHECK(pixel[2] > 20);
        }
        CHECK(pixels[0] == 0);
        CHECK(pixels[1] == 0);
        CHECK(pixels[2] == 0);
    }
}
