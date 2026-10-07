#include "../../win/window-input.h"

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNfx2.h>
#if 0 // Only needed by the suspended Simple-world integration case below.
    #include <garnet/GNengine2.h>
#endif

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

#if 0 // Suspended with E2 Simple world; standalone FX2 coverage remains active.
// E2 integration is an explicit client adapter; the backend itself only knows gpu2.
struct ImGuiOverlay final : GN::e2::VisualOverlay {
    GN_REGISTER_RUNTIME_TYPE(GN::e2::VisualOverlay);
    GN::AutoRef<GN::fx2::ImGuiBackend> backend;
    explicit ImGuiOverlay(GN::AutoRef<GN::fx2::ImGuiBackend> value): VisualOverlay(TYPE_INFO(), "imgui-test-overlay"), backend(std::move(value)) {}
    int32_t zOrder() const override { return z; }
    void    setZOrder(int32_t value) override { z = value; }
    bool    record(RenderContext & context) const override {
        GN::AutoRef<GN::gpu2::GpuPayload> upload;
        if (!backend->record(context.raster(), upload)) return false;
        if (upload) context.upload(upload);
        return true;
    }
    int32_t z = 0;
};

#endif

} // namespace

TEST_CASE("fx2 ImGui backend translates window input and records gpu2 draws", "[fx2][imgui][gpu]") {
    using namespace GN;
    auto gpu = gpu2::GpuContext::create("imgui-test", {});
    if (!gpu) SKIP("No Vulkan GPU context is available");
#if 0 // Retain the old Simple-world setup with its disabled integration case.
    e2::Universe universe;
    auto         visual = e2::VisualDomain::create({.universe = universe, .os = {}});
    if (!visual) SKIP("No headless Vulkan visual domain is available");
    const auto gpu = visual->gpu();

#endif

    TestWindow window;
    auto       backend = fx2::ImGuiBackend::create({.gpu = gpu, .window = window});
    REQUIRE(backend);
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
        "imgui-test-target",
        {
            .context    = gpu,
            .descriptor = gpu2::Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(320, 240).setLevels(1),
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

    AutoRef<gpu2::GpuPayload> upload;
    REQUIRE(backend->record(*raster, upload));
    REQUIRE(upload);
    auto draws = raster->seal();
    REQUIRE(draws);
    CAPTURE("submitting upload");
    gpu->submit(gpu2::GpuContext::SubmitParameters("imgui-test-upload").appendWork(upload));
    gpu->waitForIdle();
    gpu->submit(gpu2::GpuContext::SubmitParameters("imgui-test-draw").appendWork(draws));
    gpu->waitForIdle();

    auto image = targetTexture->readback();
    REQUIRE(!image.empty());
    const auto * pixels    = reinterpret_cast<const uint32_t *>(image.data());
    uint32_t     rectPixel = pixels[10 * 320 + 10];
    uint8_t      r         = rectPixel & 0xFF;
    uint8_t      g         = (rectPixel >> 8) & 0xFF;
    uint8_t      b         = (rectPixel >> 16) & 0xFF;
    CHECK(r == 255);
    CHECK(g == 255);
    CHECK(b == 255);

#if 0 // Simple-world integration is not a prerequisite for FX2 rendering.
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
    moment->add(referenceTo(new ImGuiOverlay(backend)));
    visual->renderFrame({.tableau = moment});
    REQUIRE(!visual->readbackFrame().empty());
#endif
}

TEST_CASE("fx2 ImGui backend records into bindless::Raster with configurable resourceSetIndex", "[fx2][imgui][gpu][bindless]") {
    using namespace GN;
    auto gpu = gpu2::GpuContext::create("imgui-bindless-test", {});
    if (!gpu) SKIP("No Vulkan GPU context is available");

    TestWindow window;
    auto       backend = fx2::ImGuiBackend::create({.gpu = gpu, .window = window, .resourceSetIndex = 2});
    REQUIRE(backend);

    backend->newFrame(1.0f / 60.0f);
    ImGui::GetBackgroundDrawList()->AddRectFilled({0, 0}, {320, 240}, IM_COL32(0, 255, 0, 255));
    backend->render();
    REQUIRE(ImGui::GetDrawData());
    REQUIRE(ImGui::GetDrawData()->TotalVtxCount > 0);

    auto targetTexture = gpu2::Texture::create(
        "imgui-bindless-target",
        {
            .context    = gpu,
            .descriptor = gpu2::Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(320, 240).setLevels(1),
        });
    REQUIRE(targetTexture);
    gpu2::GpuResourceView targetView;
    targetView.resource = targetTexture;
    gpu2::RasterTarget target;
    target.setColorTarget(0, targetView);
    auto & blend   = target.colorTargets[0].blendState;
    blend.colorSrc = gpu2::RasterTarget::BlendState::SRC_ALPHA;
    blend.colorDst = gpu2::RasterTarget::BlendState::INV_SRC_ALPHA;

    auto heap = gpu2::bindless::DescriptorHeap::create("imgui-test-heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);

    auto raster = gpu2::bindless::Raster::create("imgui-bindless-raster", {
                                                                              .gpu          = gpu,
                                                                              .target       = &target,
                                                                              .heap         = heap,
                                                                              .heapSetIndex = 0,
                                                                          });
    REQUIRE(raster);

    AutoRef<gpu2::GpuPayload> upload;
    REQUIRE(backend->record(*raster, upload));
    REQUIRE(upload);
    auto draws = raster->seal();
    REQUIRE(draws);

    gpu->submit(gpu2::GpuContext::SubmitParameters("imgui-bindless-upload").appendWork(upload));
    gpu->waitForIdle();
    gpu->submit(gpu2::GpuContext::SubmitParameters("imgui-bindless-draw").appendWork(draws));
    gpu->waitForIdle();

    auto image = targetTexture->readback();
    REQUIRE(!image.empty());
    const auto * pixels      = reinterpret_cast<const uint32_t *>(image.data());
    uint32_t     centerPixel = pixels[120 * 320 + 160];
    uint8_t      r           = centerPixel & 0xFF;
    uint8_t      g           = (centerPixel >> 8) & 0xFF;
    uint8_t      b           = (centerPixel >> 16) & 0xFF;
    CHECK(r == 0);
    CHECK(g == 255);
    CHECK(b == 0);
}
