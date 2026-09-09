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

    auto   moment   = world->captureVisualMoment({});
    auto * captured = RuntimeType::cast<VisualMomentImpl>(moment.get());
    REQUIRE(captured);
    REQUIRE(captured->renderables.size() == 1);
    CHECK(captured->renderables[0].model == model);
    CHECK_FALSE(captured->renderables[0].mesh);
    CHECK(captured->renderables[0].translation.x == worldCoordinate(3));

    form->setPosition({worldCoordinate(30), worldCoordinate(40), worldCoordinate(50)});
    CHECK(captured->renderables[0].translation.x == worldCoordinate(3));
    CHECK(captured->renderables[0].translation.y == worldCoordinate(4));
    CHECK(captured->renderables[0].translation.z == worldCoordinate(5));
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

    auto first = world->captureVisualMoment({.domain = visual, .cameras = {cameras, 1}});
    REQUIRE(first);
    visual->render(first);
    auto second = world->captureVisualMoment({.domain = visual, .cameras = {cameras, 1}});
    REQUIRE(second);
    visual->render(second);
    SUCCEED("two frames reused the visual-domain model cache");
}
