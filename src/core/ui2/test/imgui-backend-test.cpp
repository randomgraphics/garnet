#include "../../win/window-input.h"

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNui2.h>

namespace {

struct TestWindow final : GN::win::WindowInput {
    using WindowInput::notifyAxisMove;
    using WindowInput::notifyCharPress;
    using WindowInput::notifyKeyPress;
    using WindowInput::updateMousePosition;

    intptr_t              getDisplayHandle() const override { return 0; }
    intptr_t              getMonitorHandle() const override { return 0; }
    intptr_t              getWindowHandle() const override { return 0; }
    intptr_t              getModuleHandle() const override { return 0; }
    GN::Vector2<uint32_t> getClientSize() const override { return {320, 240}; }
    intptr_t              createVulkanSurfaceHandle(intptr_t) const override { return 0; }
    void                  destroyVulkanSurfaceHandle(intptr_t, intptr_t) const override {}
    void                  show() override {}
    void                  hide() override {}
    void                  minimize() override {}
    void                  moveTo(int, int) override {}
    void                  setClientSize(size_t, size_t) override {}
    bool                  runUntilNoNewEvents(bool) override { return true; }
};

struct RecordingContext final : GN::e2::VisualMoment::RenderContext {
    GN_REGISTER_RUNTIME_TYPE(GN::e2::VisualMoment::RenderContext);

    GN::gpu2::GpuRaster &             target;
    GN::AutoRef<GN::gpu2::GpuPayload> payload;

    // This standalone UI renderer does not use FX2 bindings.
    GN::fx2::SharedShaderConstants::Snapshot constants;

    explicit RecordingContext(GN::gpu2::GpuRaster & raster): RenderContext(TYPE_INFO()), target(raster) {}
    GN::gpu2::GpuRaster &                            raster() const override { return target; }
    const GN::fx2::SharedShaderConstants::Snapshot & ssc() const override { return constants; }
    void                                             upload(GN::AutoRef<GN::gpu2::GpuPayload> value) override { payload = std::move(value); }
};

} // namespace

TEST_CASE("ui2 ImGui backend translates window input and records gpu2 draws", "[ui2][imgui][gpu]") {
    using namespace GN;
    e2::Universe universe;
    auto         visual = e2::VisualDomain::create({.universe = universe, .os = {}});
    if (!visual) SKIP("No headless Vulkan visual domain is available");
    const auto gpu = visual->gpu();

    TestWindow window;
    auto       backend = ui2::ImGuiBackend::create({.gpu = gpu, .window = window});
    REQUIRE(backend);
    CHECK(backend->zOrder() == 0);
    backend->setZOrder(-5);
    CHECK(backend->zOrder() == -5);
    window.updateMousePosition(20, 20);
    window.notifyKeyPress(win::KeyCode::A, true);
    window.notifyCharPress(L'A');
    window.notifyAxisMove(win::Axis::MOUSE_WHEEL_0, 120);

    backend->newFrame(1.0f / 60.0f);
    ImGui::GetBackgroundDrawList()->AddRectFilled({5, 5}, {50, 50}, IM_COL32_WHITE);
    ImGui::Begin("gpu2 backend smoke");
    ImGui::TextUnformatted("Dear ImGui through Garnet gpu2");
    ImGui::End();
    backend->render();
    REQUIRE(ImGui::GetDrawData());
    INFO("vertices=" << ImGui::GetDrawData()->TotalVtxCount << " indices=" << ImGui::GetDrawData()->TotalIdxCount);
    REQUIRE(ImGui::GetDrawData()->TotalVtxCount > 0);

    auto targetTexture = gpu2::Texture::create(
        "imgui-test-target", {
                                 .context    = gpu,
                                 .descriptor = gpu2::Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(320, 240),
                             });
    REQUIRE(targetTexture);
    gpu2::GpuResourceView targetView;
    targetView.resource = targetTexture;
    gpu2::RasterTarget target;
    target.setColorTarget(0, targetView);
    auto & blend   = target.colorTargets[0].blendState;
    blend.colorSrc = gpu2::RasterTarget::BlendState::SRC_ALPHA;
    blend.colorDst = gpu2::RasterTarget::BlendState::INV_SRC_ALPHA;
    auto raster    = gpu2::GpuRaster::create("imgui-test-raster", {.gpu = gpu, .target = &target});
    REQUIRE(raster);

    RecordingContext recording(*raster);
    REQUIRE(backend->record(recording));
    auto upload = recording.payload;
    REQUIRE(upload);
    auto draws = raster->seal();
    REQUIRE(draws);
    CAPTURE("submitting upload");
    gpu->submit(gpu2::GpuContext::SubmitParameters("imgui-test-upload").appendWork(upload));
    gpu->waitForIdle();
    gpu->submit(gpu2::GpuContext::SubmitParameters("imgui-test-draw").appendWork(draws));
    gpu->waitForIdle();

    backend->newFrame(1.0f / 60.0f);
    ImGui::Begin("E2 overlay smoke");
    ImGui::TextUnformatted("overlay draw appended after world geometry");
    ImGui::End();
    backend->render();

    auto world      = e2::Simple::createWorld(universe, e2::PhysicalScale::METER());
    auto meters     = [&](float value) { return world->scale.fromMeters(value); };
    auto coordinate = [&](float value) { return e2::spatial::toWorld(meters(value)); };
    auto box        = e2::Simple::createBox(universe, {coordinate(0), coordinate(0), coordinate(0)}, {meters(1), meters(1), meters(1)});
    auto camera     = e2::Camera::create({.domain = visual});
    REQUIRE(world);
    REQUIRE(box);
    REQUIRE(camera);
    e2::Ref<e2::Form> forms[] = {box};
    world->populate({forms, 1});
    camera->desc.position         = {coordinate(0), coordinate(0), coordinate(4)};
    camera->desc.nearPlane        = meters(0.1f);
    camera->desc.farPlane         = meters(100.0f);
    e2::Ref<e2::Camera> cameras[] = {camera};
    auto                moment    = world->snapshot({.domain = visual, .cameras = {cameras, 1}});
    REQUIRE(moment);
    moment->add(backend);
    visual->render(moment);
    REQUIRE(!visual->readbackFrame().empty());
}
