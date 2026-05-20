#include <catch2/catch_test_macros.hpp>
#include <garnet/GNgpu2.h>

using namespace GN;
using namespace GN::gpu2;

static AutoRef<GpuContext> makeGpu() {
    return GpuContext::create("gpu", GpuContext::CreateParameters {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
}

static gfx::img::Image makeSolidImage(uint32_t w, uint32_t h, uint32_t depth, uint32_t faces, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
    gfx::img::Extent3D extent;
    extent.set(w, h, depth);
    gfx::img::PlaneDesc planeDesc = gfx::img::PlaneDesc::make(gfx::img::PixelFormat::RGBA8(), extent);
    gfx::img::ImageDesc imageDesc = gfx::img::ImageDesc::make(planeDesc, faces, 1, 1);
    gfx::img::Image     image(imageDesc);
    auto *              p = (uint8_t *) image.data();
    for (size_t i = 0; i < image.size(); i += 4) {
        p[i + 0] = r;
        p[i + 1] = g;
        p[i + 2] = b;
        p[i + 3] = a;
    }
    return image;
}

static void checkPixels(const gfx::img::Image & image, uint8_t er, uint8_t eg, uint8_t eb, uint8_t ea = 255) {
    REQUIRE_FALSE(image.empty());
    auto pixels = image.plane().toRGBA8(image.data());
    REQUIRE_FALSE(pixels.empty());
    for (const auto & px : pixels) {
        CHECK(px.r == er);
        CHECK(px.g == eg);
        CHECK(px.b == eb);
        CHECK(px.a == ea);
    }
}

TEST_CASE("GPU2: Texture2D setContent and readback", "[gpu2][texture][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 4, H = 4;
    auto tex = Texture::create("tex2d", Texture::CreateParameters {
                                            .context    = gpu,
                                            .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(W, H).setLevels(1),
                                        });
    if (!tex) SKIP("RGBA8 2D texture unavailable");

    REQUIRE(tex->setContent(makeSolidImage(W, H, 1, 1, 255, 0, 0)));
    checkPixels(tex->readback(), 255, 0, 0);
}

TEST_CASE("GPU2: TextureCube setContent and readback", "[gpu2][texture][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 4;
    auto tex = Texture::create("texcube",
                               Texture::CreateParameters {
                                   .context    = gpu,
                                   .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(W, W).setFaces(6).setLevels(1),
                               });
    if (!tex) SKIP("RGBA8 cubemap texture unavailable");

    REQUIRE(tex->setContent(makeSolidImage(W, W, 1, 6, 0, 255, 0)));
    checkPixels(tex->readback(), 0, 255, 0);
}

TEST_CASE("GPU2: Texture3D setContent and readback", "[gpu2][texture][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 4, H = 4, D = 4;
    auto tex = Texture::create("tex3d", Texture::CreateParameters {
                                            .context    = gpu,
                                            .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(W, H, D).setLevels(1),
                                        });
    if (!tex) SKIP("RGBA8 3D texture unavailable");

    REQUIRE(tex->setContent(makeSolidImage(W, H, D, 1, 0, 0, 255)));
    checkPixels(tex->readback(), 0, 0, 255);
}
