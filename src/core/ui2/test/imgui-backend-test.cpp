#include "../../win/window-input.h"

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNui2.h>

namespace {

struct TestWindow final : GN::win::WindowInput {
    using WindowInput::notifyAxisMove;
    using WindowInput::notifyCharPress;
    using WindowInput::notifyKeyPress;
    using WindowInput::updateMousePosition;

    intptr_t getDisplayHandle() const override { return 0; }
    intptr_t getMonitorHandle() const override { return 0; }
    intptr_t getWindowHandle() const override { return 0; }
    intptr_t getModuleHandle() const override { return 0; }
    GN::Vector2<uint32_t> getClientSize() const override { return {320, 240}; }
    intptr_t createVulkanSurfaceHandle(intptr_t) const override { return 0; }
    void destroyVulkanSurfaceHandle(intptr_t, intptr_t) const override {}
    void show() override {}
    void hide() override {}
    void minimize() override {}
    void moveTo(int, int) override {}
    void setClientSize(size_t, size_t) override {}
    bool runUntilNoNewEvents(bool) override { return true; }
};

} // namespace

TEST_CASE("ui2 ImGui backend translates window input and records gpu2 draws", "[ui2][imgui][gpu]") {
    using namespace GN;
    const auto gpu = gpu2::GpuContext::create("imgui-backend-test", {.howToPrintDeviceCaps = gpu2::GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No gpu2 context is available");

    TestWindow window;
    auto backend = ui2::ImGuiBackend::create({.gpu = gpu, .window = window});
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

    auto targetTexture = gpu2::Texture::create("imgui-test-target", {
                                                                         .context = gpu,
                                                                         .descriptor = gpu2::Texture::Descriptor {}
                                                                                           .setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM())
                                                                                           .setDimensions(320, 240),
                                                                     });
    REQUIRE(targetTexture);
    gpu2::GpuResourceView targetView;
    targetView.resource = targetTexture;
    gpu2::RasterTarget target;
    target.setColorTarget(0, targetView);
    auto & blend   = target.colorTargets[0].blendState;
    blend.colorSrc = gpu2::RasterTarget::BlendState::SRC_ALPHA;
    blend.colorDst = gpu2::RasterTarget::BlendState::INV_SRC_ALPHA;
    auto raster = gpu2::GpuRaster::create("imgui-test-raster", {.gpu = gpu, .target = &target});
    REQUIRE(raster);

    bool ok = false;
    auto upload = backend->record(*raster, ok);
    REQUIRE(ok);
    REQUIRE(upload);
    auto draws = raster->seal();
    REQUIRE(draws);
    CAPTURE("submitting upload");
    gpu->submit(gpu2::GpuContext::SubmitParameters("imgui-test-upload").appendWork(upload));
    gpu->waitForIdle();
    gpu->submit(gpu2::GpuContext::SubmitParameters("imgui-test-draw").appendWork(draws));
    gpu->waitForIdle();
}
