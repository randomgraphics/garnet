#include <catch2/catch_test_macros.hpp>
#include "e2/e2-internal.h"
#include <garnet/base/filesys.h>

#include <concepts>
#include <utility>

using namespace GN;
using namespace GN::e2;

static_assert(std::derived_from<ModelVisualFacet, VisualFacet>);
static_assert(std::same_as<decltype(ModelVisualFacet::create(std::declval<const ModelVisualFacet::CreateParameters &>())), Ref<ModelVisualFacet>>);
static_assert(std::same_as<decltype(createModelForm(std::declval<Universe &>(), std::declval<const StrA &>(), std::declval<AutoRef<const fx2::ModelScene>>())),
                           Ref<Form>>);

TEST_CASE("e2 model interface remains renderer independent", "[e2][model]") { CHECK(ModelVisualFacet::TYPE_INFO().isDerivedFrom(VisualFacet::TYPE_INFO())); }

static WorldCoordinate worldCoordinate(int64_t value) { return spatial::toWorld(LocalCoordinate(value)); }

TEST_CASE("e2 model facet captures immutable instance state", "[e2][model]") {
    const StrA root = getEnv("GARNET_ROOT");
    const StrA path = root.empty() ? StrA("media/boxes/boxes.fbx") : fs::joinPath(root, "media/boxes/boxes.fbx");
    if (!fs::isFile(path)) SKIP("Project media is not initialized");
    const auto model = fx2::ModelScene::load({.path = path});
    REQUIRE(model);

    Universe universe;
    auto     world = Simple::createWorld(universe, PhysicalScale::METER());
    auto     form  = createModelForm(universe, "captured-model", model);
    REQUIRE(world);
    REQUIRE(form);
    form->setPosition({worldCoordinate(3), worldCoordinate(4), worldCoordinate(5)});
    Ref<Form> forms[] = {form};
    world->populate({forms, 1});

    auto   moment   = world->snapshot({});
    auto * captured = RuntimeType::cast<VisualMomentImpl>(RuntimeType::cast<VisualTableauImpl>(moment.get())->moments[0].get());
    REQUIRE(captured);
    REQUIRE(captured->renderables.size() == 1);
    CHECK(captured->renderables[0].model.get() == model.get());
    CHECK_FALSE(captured->renderables[0].mesh);
    CHECK(captured->renderables[0].translation.x == worldCoordinate(3));

    form->setPosition({worldCoordinate(30), worldCoordinate(40), worldCoordinate(50)});
    CHECK(captured->renderables[0].translation.x == worldCoordinate(3));
    CHECK(captured->renderables[0].translation.y == worldCoordinate(4));
    CHECK(captured->renderables[0].translation.z == worldCoordinate(5));

    auto * modelFacet = RuntimeType::cast<ModelVisualFacet>(form->facets()[0].get());
    REQUIRE(modelFacet);
    modelFacet->setVisible(false);
    CHECK_FALSE(modelFacet->visible());
    auto hidden       = world->snapshot({});
    auto hiddenMoment = RuntimeType::cast<VisualMomentImpl>(RuntimeType::cast<VisualTableauImpl>(hidden.get())->moments[0].get());
    REQUIRE(hiddenMoment);
    CHECK(hiddenMoment->renderables.empty());
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
    visual->render(moment);
    auto image = visual->readbackFrame();
    REQUIRE_FALSE(image.empty());
    CHECK(image.format() == gfx::img::PixelFormat::RGBA_8_8_8_8_SRGB());
    // The domain clears to linear (0.05, 0.06, 0.09), not these byte values.
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

TEST_CASE("e2 visual domain renders and caches captured model instances", "[e2][model][gpu]") {
    const StrA root = getEnv("GARNET_ROOT");
    const StrA path = root.empty() ? StrA("media/boxes/boxes.fbx") : fs::joinPath(root, "media/boxes/boxes.fbx");
    if (!fs::isFile(path)) SKIP("Project media is not initialized");
    const auto model = fx2::ModelScene::load({.path = path});
    REQUIRE(model);

    Universe universe;
    auto     visual = VisualDomain::create({.universe = universe, .os = {}});
    if (!visual) SKIP("No headless Vulkan visual domain is available");
    CHECK(visual->readbackFrame().empty());
    auto camera = Camera::create({.domain = visual});
    auto world  = Simple::createWorld(universe, PhysicalScale::METER());
    auto form   = createModelForm(universe, "rendered-model", model);
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
    visual->render(first);
    auto second = world->snapshot({.domain = visual, .cameras = {cameras, 1}});
    REQUIRE(second);
    visual->render(second);
    auto image = visual->readbackFrame();
    REQUIRE_FALSE(image.empty());
    CHECK(image.width() == 1280);
    CHECK(image.height() == 720);
    visual->render({});
    CHECK(visual->readbackFrame().empty());
}
