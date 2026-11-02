#include "../../win/window-input.h"

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNfx2.h>

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

} // namespace

TEST_CASE("fx2 ImGui backend translates window input and records gpu2 draws", "[fx2][imgui][gpu]") {
    using namespace GN;
    auto gpu = gpu2::GpuContext::create("imgui-test", {});
    if (!gpu) SKIP("No Vulkan GPU context is available");
    TestWindow window;
    auto heap = ::GN::gpu2::DescriptorHeap::create("imgui-smoke-heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);
    auto backend = fx2::ImGuiBackend::create({.gpu = gpu, .window = window, .heap = heap});
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
    auto raster = ::GN::gpu2::Raster::create("imgui-test-raster", {.gpu = gpu, .target = &target, .heap = heap});
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

}

TEST_CASE("fx2 ImGui backend samples registered textures through bindless indices", "[fx2][imgui][gpu][bindless]") {
    using namespace GN;
    auto gpu = gpu2::GpuContext::create("imgui-bindless-test", {});
    if (!gpu) SKIP("No Vulkan GPU context is available");

    TestWindow window;
    auto heap = ::GN::gpu2::DescriptorHeap::create("imgui-test-heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);
    auto backend = fx2::ImGuiBackend::create({.gpu = gpu, .window = window, .heap = heap, .heapSetIndex = 2});
    REQUIRE(backend);

    backend->newFrame(1.0f / 60.0f);
    ImGui::GetBackgroundDrawList()->AddRectFilled({0, 0}, {320, 240}, IM_COL32(0, 255, 0, 255));
    auto sampledTexture = gpu2::Texture::create(
        "imgui-sampled-test-texture",
        {.context = gpu, .descriptor = gpu2::Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setLevels(1)});
    REQUIRE(sampledTexture);
    gfx::img::Extent3D textureExtent;
    textureExtent.set(1, 1, 1);
    const uint8_t          redPixel[4] = {255, 0, 0, 255};
    const auto             texturePlane = gfx::img::PlaneDesc::make(gfx::img::PixelFormat::RGBA8(), textureExtent);
    gfx::img::Image        textureImage(gfx::img::ImageDesc::make(texturePlane, 1, 1, 1), redPixel, sizeof(redPixel));
    REQUIRE(sampledTexture->setContent(textureImage));
    const ImTextureID sampledId = backend->registerTexture(sampledTexture);
    REQUIRE(sampledId != ImTextureID_Invalid);
    ImGui::GetBackgroundDrawList()->AddImage(sampledId, {0, 0}, {40, 40});
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

    auto raster = ::GN::gpu2::Raster::create("imgui-bindless-raster", {
                                                                              .gpu          = gpu,
                                                                              .target       = &target,
                                                                              .heap         = heap,
                                                                              .heapSetIndex = 2,
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
    const uint32_t centerPixel = pixels[120 * 320 + 160];
    CHECK((centerPixel & 0xFF) == 0);
    CHECK(((centerPixel >> 8) & 0xFF) == 255);
    CHECK(((centerPixel >> 16) & 0xFF) == 0);
    const uint32_t imagePixel = pixels[20 * 320 + 20];
    CHECK((imagePixel & 0xFF) == 255);
    CHECK(((imagePixel >> 8) & 0xFF) == 0);
    CHECK(((imagePixel >> 16) & 0xFF) == 0);
    backend->unregisterTexture(sampledId);
}
