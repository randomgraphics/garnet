#include <catch2/catch_test_macros.hpp>
#include <garnet/GNengine2.h>
#include "e2/e2-internal.h"

using namespace GN;
using namespace GN::e2;

static_assert(std::derived_from<VisualMoment::RenderContext, RuntimeType>);
static_assert(!std::derived_from<VisualMoment::RenderContext, RefCounter>);

namespace {

struct RecordingMoment final : VisualMoment {
    GN_REGISTER_RUNTIME_TYPE(VisualMoment);

    DynaArray<int> & order;
    int              label;
    bool             fail = false;

    RecordingMoment(Universe & universe, DynaArray<int> & order_, int label_)
        : VisualMoment(TYPE_INFO(), universe.generateUniqueIdentifier(), "test-moment"), order(order_), label(label_) {}

    bool record(RenderContext &) const override {
        order.append(label);
        return !fail;
    }
};

struct RecordingOverlay final : VisualOverlay {
    GN_REGISTER_RUNTIME_TYPE(VisualOverlay);

    DynaArray<int> & order;
    int              label;
    int32_t          z;
    bool             fail = false;

    RecordingOverlay(Universe & universe, DynaArray<int> & order_, int label_, int32_t z_ = 0)
        : VisualOverlay(TYPE_INFO(), universe.generateUniqueIdentifier(), "test-overlay"), order(order_), label(label_), z(z_) {}

    int32_t zOrder() const override { return z; }
    void    setZOrder(int32_t value) override { z = value; }

    bool record(RenderContext &) const override {
        order.append(label);
        return !fail;
    }
};

struct RecordingEnvironment final : VisualEnvironment {
    GN_REGISTER_RUNTIME_TYPE(VisualEnvironment);

    DynaArray<int> & order;
    int              label;

    RecordingEnvironment(Universe & universe, DynaArray<int> & order_, int label_)
        : VisualEnvironment(TYPE_INFO(), universe.generateUniqueIdentifier(), "test-environment"), order(order_), label(label_) {}

    bool record(RenderContext &) const override {
        order.append(label);
        return true;
    }
};

struct CustomVisualFacet final : VisualFacet {
    GN_REGISTER_RUNTIME_TYPE(VisualFacet);

    Ref<VisualMoment> contribution;

    CustomVisualFacet(Universe & universe, Ref<VisualMoment> moment)
        : VisualFacet(TYPE_INFO(), universe.generateUniqueIdentifier(), "custom-visual"), contribution(std::move(moment)) {}

    Ref<VisualMoment> snapshot(const VisualTableau::SnapshotParameters &) override { return contribution; }
};

struct TestModelScene final : fx2::ModelScene {
    GN_REGISTER_RUNTIME_TYPE(fx2::ModelScene);

    TestModelScene(): ModelScene(TYPE_INFO(), "test-lit-triangle") {
        materials.append(Material {});
        materials[0].doubleSided = true;
        Primitive triangle;
        for (const auto & position : {glm::vec3(-2, -2, 0), glm::vec3(2, -2, 0), glm::vec3(0, 2, 0)}) {
            Vertex vertex;
            vertex.position = position;
            vertex.normal   = {0, 0, 1};
            triangle.vertices.append(vertex);
        }
        triangle.indices.append(0);
        triangle.indices.append(1);
        triangle.indices.append(2);
        primitives.append(std::move(triangle));
        Node node;
        node.primitives.append(0);
        nodes.append(std::move(node));
    }
};

/// An extension renderer using only the public context, without E2's scene cache.
struct ShadedMoment final : VisualMoment {
    GN_REGISTER_RUNTIME_TYPE(VisualMoment);

    AutoRef<fx2::ModelShading::Asset> shading;
    AutoRef<fx2::ModelAsset>          model;

    ShadedMoment(Universe & universe, AutoRef<gpu2::GpuContext> gpu, AutoRef<const fx2::ModelScene> scene)
        : VisualMoment(TYPE_INFO(), universe.generateUniqueIdentifier(), "test-shared-constants"), shading(fx2::ModelShading::create(gpu)),
          model(fx2::ModelAsset::create(gpu, scene)) {}

    bool record(RenderContext & context) const override {
        if (!shading || !model || context.ssc().set0Resources.empty()) return false;
        context.upload(shading->uploadPayload());
        context.upload(model->uploadPayload());
        glm::mat4 transform(1.f);
        transform[3].z = -4.f;
        auto draw      = fx2::ModelShading::getDrawParams(context.ssc(), shading, model, 0, transform);
        if (!draw.vs || !draw.ps) return false;
        context.raster().draw(draw);
        return true;
    }
};

} // namespace

TEST_CASE("e2 extension moments render using public shared shader constants", "[e2][visual-moment][gpu]") {
    Universe universe;
    auto     visual = VisualDomain::create({.universe = universe, .os = {}});
    if (!visual) SKIP("No headless Vulkan visual domain is available");
    auto custom = referenceTo(new ShadedMoment(universe, visual->gpu(), referenceTo(new TestModelScene)));
    REQUIRE(custom->shading);
    REQUIRE(custom->model);
    auto environment = VisualEnvironment::create({.universe = universe, .gpu = visual->gpu(), .description = {}});
    REQUIRE(environment);
    auto world  = Simple::createWorld(universe);
    auto camera = Camera::create({.domain = visual});
    REQUIRE(camera);
    camera->desc.nearPlane = LocalCoordinate(1);
    camera->desc.farPlane  = LocalCoordinate(100);
    Ref<Camera> cameras[]  = {camera};

    auto renderCenter = [&](Ref<VisualTableau> tableau, bool withEnvironment) {
        tableau->add(custom);
        if (withEnvironment) tableau->add(environment);
        visual->render(tableau);
        auto image = visual->readbackFrame();
        REQUIRE_FALSE(image.empty());
        const size_t center = (image.width() * (image.height() / 2) + image.width() / 2) * 4;
        return static_cast<const uint8_t *>(image.data())[center];
    };

    CHECK(renderCenter(world->snapshot({.domain = visual, .cameras = {cameras, 1}}), true) > 20);
    // Extension moments must inherit the observing scene's camera, including clipping.
    camera->desc.farPlane = LocalCoordinate(2);
    CHECK(renderCenter(world->snapshot({.domain = visual, .cameras = {cameras, 1}}), true) < 20);
    // No scene payload: the public context still supplies valid default camera/IBL bindings.
    CHECK(renderCenter(VisualTableau::create(universe), true) > 20);
    CHECK(renderCenter(VisualTableau::create(universe), false) == 0);
}

TEST_CASE("e2 tableaux retain custom snapshot contributions and render each moment", "[e2][visual-moment][gpu]") {
    Universe universe;
    auto     visual = VisualDomain::create({.universe = universe, .os = {}});
    if (!visual) SKIP("No headless Vulkan visual domain is available");
    DynaArray<int> order;
    auto           first  = referenceTo(new RecordingMoment(universe, order, 1));
    auto           second = referenceTo(new RecordingMoment(universe, order, 2));
    auto           facet  = referenceTo(new CustomVisualFacet(universe, first));
    auto           form   = Form::create(universe, "custom-visual");
    REQUIRE(form);
    REQUIRE(form->addFacet(facet));
    auto      world   = Simple::createWorld(universe);
    Ref<Form> forms[] = {form};
    world->populate({forms, 1});
    auto tableau = world->snapshot({});
    REQUIRE(tableau);
    tableau->add(second);
    visual->render(tableau);
    REQUIRE_FALSE(visual->readbackFrame().empty());
    REQUIRE(order.size() == 2);
    CHECK(order[0] == 1);
    CHECK(order[1] == 2);

    first->fail = true;
    order.clear();
    visual->render(tableau);
    CHECK(visual->readbackFrame().empty());
    REQUIRE(order.size() == 1);
    CHECK(order[0] == 1);

    // A new opaque tableau carries no moment state from the prior snapshot.
    visual->render(VisualTableau::create(universe));
    CHECK_FALSE(visual->readbackFrame().empty());
    CHECK(order.size() == 1);
}

TEST_CASE("e2 tableaux order regular moments before environments and overlays by descending Z", "[e2][visual-moment][gpu]") {
    Universe universe;
    auto     visual = VisualDomain::create({.universe = universe, .os = {}});
    if (!visual) SKIP("No headless Vulkan visual domain is available");
    DynaArray<int> order;
    auto           tableau     = VisualTableau::create(universe);
    auto           nearOverlay = referenceTo(new RecordingOverlay(universe, order, 43, -5));
    // Interleave all categories so insertion order cannot accidentally satisfy the policy.
    tableau->add(nearOverlay);
    tableau->add(referenceTo(new RecordingEnvironment(universe, order, 30)));
    tableau->add(referenceTo(new RecordingMoment(universe, order, 1)));
    tableau->add(referenceTo(new RecordingOverlay(universe, order, 41, 0)));
    tableau->add(referenceTo(new RecordingOverlay(universe, order, 40, 10)));
    tableau->add(referenceTo(new RecordingMoment(universe, order, 2)));
    tableau->add(referenceTo(new RecordingEnvironment(universe, order, 31)));
    tableau->add(referenceTo(new RecordingOverlay(universe, order, 42, 0)));
    visual->render(tableau);
    REQUIRE_FALSE(visual->readbackFrame().empty());
    REQUIRE(order.size() == 8);
    CHECK(order[0] == 1);
    CHECK(order[1] == 2);
    CHECK((order[2] == 30 || order[2] == 31));
    CHECK((order[3] == 30 || order[3] == 31));
    CHECK(order[2] != order[3]);
    CHECK(order[4] == 40);
    CHECK((order[5] == 41 || order[5] == 42));
    CHECK((order[6] == 41 || order[6] == 42));
    CHECK(order[5] != order[6]);
    CHECK(order[7] == 43);

    // Z changes between frames must invalidate the prior overlay order.
    nearOverlay->setZOrder(100);
    order.clear();
    visual->render(tableau);
    REQUIRE_FALSE(visual->readbackFrame().empty());
    REQUIRE(order.size() == 8);
    CHECK(order[0] == 1);
    CHECK(order[1] == 2);
    CHECK(order[4] == 43);
    CHECK(order[5] == 40);
    CHECK((order[6] == 41 || order[6] == 42));
    CHECK((order[7] == 41 || order[7] == 42));
    CHECK(order[6] != order[7]);
}

TEST_CASE("e2 environment moments draw a background without persisting in the domain", "[e2][visual-moment][gpu]") {
    Universe universe;
    auto     visual = VisualDomain::create({.universe = universe, .os = {}});
    if (!visual) SKIP("No headless Vulkan visual domain is available");
    VisualEnvironment::Desc description;
    description.environmentLuminanceScale = 1000.f;
    auto environment          = VisualEnvironment::create({.universe = universe, .gpu = visual->gpu(), .description = description});
    REQUIRE(environment);
    REQUIRE(RuntimeType::cast<VisualMoment>(environment.get()));
    auto empty = VisualTableau::create(universe);
    visual->render(empty);
    auto clear = visual->readbackFrame();
    REQUIRE_FALSE(clear.empty());
    const auto * clearColor = static_cast<const uint8_t *>(clear.data());

    auto skyTableau = VisualTableau::create(universe);
    skyTableau->add(environment);
    visual->render(skyTableau);
    auto sky = visual->readbackFrame();
    REQUIRE_FALSE(sky.empty());
    const auto * skyColor = static_cast<const uint8_t *>(sky.data());
    CHECK(skyColor[0] > clearColor[0]);
    CHECK(skyColor[1] > clearColor[1]);
    CHECK(skyColor[2] > clearColor[2]);

    visual->render(empty);
    auto restored = visual->readbackFrame();
    REQUIRE_FALSE(restored.empty());
    const auto * restoredColor = static_cast<const uint8_t *>(restored.data());
    for (size_t i = 0; i < 4; ++i) CHECK(restoredColor[i] == clearColor[i]);

    auto another = VisualEnvironment::create({.universe = universe, .gpu = visual->gpu(), .description = description});
    REQUIRE(another);
    auto multiple = VisualTableau::create(universe);
    multiple->add(environment);
    multiple->add(another);
    multiple->add(environment);
    visual->render(multiple);
    auto skies = visual->readbackFrame();
    REQUIRE_FALSE(skies.empty());
    const auto * skiesColor = static_cast<const uint8_t *>(skies.data());
    for (size_t i = 0; i < 3; ++i) CHECK(skiesColor[i] > clearColor[i]);
}

TEST_CASE("e2 scene moments share environment textures but retain independent cameras", "[e2][visual-moment][gpu]") {
    Universe universe;
    auto     visual = VisualDomain::create({.universe = universe, .os = {}});
    if (!visual) SKIP("No headless Vulkan visual domain is available");
    auto world = Simple::createWorld(universe);
    auto model = referenceTo(new TestModelScene);
    auto form  = createModelForm(universe, "triangle", model);
    REQUIRE(form);
    Ref<Form> forms[] = {form};
    world->populate({forms, 1});
    auto camera = Camera::create({.domain = visual});
    REQUIRE(camera);
    camera->desc.position   = {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), spatial::toWorld(LocalCoordinate(4))};
    camera->desc.nearPlane  = LocalCoordinate(1);
    camera->desc.farPlane   = LocalCoordinate(100);
    Ref<Camera> cameras[]   = {camera};
    auto        scene       = world->snapshot({.domain = visual, .cameras = {cameras, 1}});
    auto        environment = VisualEnvironment::create({.universe = universe, .gpu = visual->gpu(), .description = {}});
    REQUIRE(environment);
    auto root = scene;
    root->add(environment);
    DynaArray<int> order;
    auto           overlay = referenceTo(new RecordingOverlay(universe, order, 1));
    overlay->fail          = true;
    root->add(overlay);
    visual->render(root);
    REQUIRE(visual->readbackFrame().empty());
    overlay->fail = false;
    visual->render(root);
    auto lit = visual->readbackFrame();
    REQUIRE_FALSE(lit.empty());
    const size_t center   = (lit.width() * (lit.height() / 2) + lit.width() / 2) * 4;
    const auto * litPixel = static_cast<const uint8_t *>(lit.data()) + center;
    CHECK(litPixel[0] > 20);
    CHECK(litPixel[1] > 20);
    CHECK(litPixel[2] > 20);

    // This empty scene's clip range excludes the first scene's triangle. Its UBO
    // upload must not replace the first scene's camera before the shared raster runs.
    auto emptyWorld       = Simple::createWorld(universe);
    camera->desc.farPlane = LocalCoordinate(2);
    auto   otherScene     = emptyWorld->snapshot({.domain = visual, .cameras = {cameras, 1}});
    auto * other          = RuntimeType::cast<VisualTableauImpl>(otherScene.get());
    REQUIRE(other);
    for (auto & moment : other->moments) root->add(moment);
    visual->render(root);
    auto combined = visual->readbackFrame();
    REQUIRE_FALSE(combined.empty());
    const auto * combinedPixel = static_cast<const uint8_t *>(combined.data()) + center;
    for (size_t i = 0; i < 4; ++i) CHECK(combinedPixel[i] == litPixel[i]);

    camera->desc.farPlane = LocalCoordinate(100);
    visual->render(world->snapshot({.domain = visual, .cameras = {cameras, 1}}));
    auto unlit = visual->readbackFrame();
    REQUIRE_FALSE(unlit.empty());
    const auto * unlitPixel = static_cast<const uint8_t *>(unlit.data()) + center;
    for (size_t i = 0; i < 3; ++i) CHECK(unlitPixel[i] == 0);
}
