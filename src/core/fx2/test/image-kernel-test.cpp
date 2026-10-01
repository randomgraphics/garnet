#include <catch2/catch_test_macros.hpp>
#include <garnet/GNfx2.h>
#include <cmath>
using namespace GN;
using namespace GN::gpu2;
using namespace GN::fx2;
namespace {
gfx::img::Image pattern(uint32_t w, uint32_t h, uint32_t levels = 1) {
    gfx::img::Extent3D extent;
    extent.set(w, h, 1);
    gfx::img::Image image(gfx::img::ImageDesc::make(gfx::img::PlaneDesc::make(gfx::img::PixelFormat::RGBA8(), extent), 1, 1, levels));
    memset(image.data(), 0, image.size());
    for (uint32_t y = 0; y < h; ++y)
        for (uint32_t x = 0; x < w; ++x) {
            auto p = image.at({}, x, y);
            p[0]   = uint8_t((x * 31 + y * 17) % 256);
            p[1]   = uint8_t(x == w / 2 && y == h / 2 ? 255 : 0);
            p[2]   = 71;
            p[3]   = 255;
        }
    return image;
}
AutoRef<Texture> texture(AutoRef<GpuContext> gpu, const gfx::img::Image & image, uint32_t levels = 1) {
    auto result = Texture::create(
        "image-test",
        {.context    = gpu,
         .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(image.width(), image.height()).setLevels(levels)});
    REQUIRE(result);
    REQUIRE(result->setContent(image));
    return result;
}
void submit(AutoRef<GpuContext> gpu, const DynaArray<AutoRef<GpuPayload>> & work) {
    GpuContext::SubmitParameters parameters("image-test");
    for (auto & p : work) parameters.appendWork(p);
    gpu->submit(parameters);
    gpu->waitForIdle();
}
} // namespace
TEST_CASE("fx2 raster and compute Gaussian blur match a CPU reference", "[fx2][image-kernel][gpu]") {
    auto gpu = GpuContext::create("image-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No gpu2 device available");
    auto original = pattern(9, 7);
    auto source   = texture(gpu, original);
    auto raster   = RasterGaussianBlurKernel::create(gpu);
    auto compute  = ComputeGaussianBlurKernel::create(gpu);
    REQUIRE(raster);
    REQUIRE(compute);
    double weights[3] = {1, std::exp(-0.5), std::exp(-2.)};
    double total      = weights[0] + 2 * (weights[1] + weights[2]);
    for (auto & w : weights) w /= total;
    auto intermediate = pattern(9, 7), expected = pattern(9, 7);
    for (int axis = 0; axis < 2; ++axis)
        for (int y = 0; y < 7; ++y)
            for (int x = 0; x < 9; ++x)
                for (int c = 0; c < 4; ++c) {
                    double value = 0;
                    for (int i = -2; i <= 2; ++i) {
                        int xx = std::clamp(x + (axis == 0 ? i : 0), 0, 8), yy = std::clamp(y + (axis == 1 ? i : 0), 0, 6);
                        value += (axis == 0 ? original : intermediate).at({}, xx, yy)[c] * weights[std::abs(i)];
                    }
                    (axis == 0 ? intermediate : expected).at({}, x, y)[c] = uint8_t(std::round(value));
                }
    for (bool useCompute : {false, true}) {
        auto                           output = texture(gpu, original);
        DynaArray<AutoRef<GpuPayload>> work;
        GaussianBlurInputs             inputs {.source = source, .destination = output, .sigma = 1, .radius = 2};
        REQUIRE((useCompute ? compute->record(inputs, work) : raster->record(inputs, work)));
        REQUIRE(work.size() == 2);
        auto constant = pattern(9, 7);
        for (uint32_t y = 0; y < 7; ++y)
            for (uint32_t x = 0; x < 9; ++x) {
                auto pixel = constant.at({}, x, y);
                pixel[0]   = 11;
                pixel[1]   = 22;
                pixel[2]   = 33;
                pixel[3]   = 255;
            }
        auto               second = texture(gpu, constant);
        GaussianBlurInputs other {.source = second, .destination = second, .sigma = 3, .radius = 4};
        REQUIRE((useCompute ? compute->record(other, work) : raster->record(other, work)));
        REQUIRE(work.size() == 4);
        other = {}; // Recorded parameters and temporary images must survive caller mutation.
        submit(gpu, work);
        auto secondImage = second->readback();
        REQUIRE_FALSE(secondImage.empty());
        CHECK(memcmp(secondImage.data(), constant.data(), constant.size()) == 0);
        auto actual = output->readback();
        REQUIRE_FALSE(actual.empty());
        for (int y = 0; y < 7; ++y)
            for (int x = 0; x < 9; ++x)
                for (int c = 0; c < 4; ++c) CHECK(std::abs(int(actual.at({}, x, y)[c]) - int(expected.at({}, x, y)[c])) <= 1);
        inputs.radius = 16;
        CHECK_FALSE((useCompute ? compute->record(inputs, work) : raster->record(inputs, work)));
        CHECK(work.size() == 4);
        inputs.radius = 2;
        inputs.source = output;
        work.clear();
        REQUIRE((useCompute ? compute->record(inputs, work) : raster->record(inputs, work)));
        submit(gpu, work);
    }
}
TEST_CASE("fx2 mipmap kernels cover odd edges and retain mip zero", "[fx2][image-kernel][gpu]") {
    auto gpu = GpuContext::create("image-test", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No gpu2 device available");
    auto raster  = RasterMipmapKernel::create(gpu);
    auto compute = ComputeMipmapKernel::create(gpu);
    REQUIRE(raster);
    REQUIRE(compute);
    auto original = pattern(7, 5, 3);
    auto expected = pattern(7, 5, 3);
    for (uint32_t level = 1; level < 3; ++level) {
        gfx::img::PlaneCoord previous {.level = level - 1}, current {.level = level};
        int                  w = expected.width(current), h = expected.height(current), sw = expected.width(previous), sh = expected.height(previous);
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                for (int c = 0; c < 4; ++c) {
                    double loX = double(x) * sw / w, hiX = double(x + 1) * sw / w, loY = double(y) * sh / h, hiY = double(y + 1) * sh / h, value = 0;
                    for (int yy = int(loY); yy < int(std::ceil(hiY)); ++yy)
                        for (int xx = int(loX); xx < int(std::ceil(hiX)); ++xx)
                            value += expected.at(previous, xx, yy)[c] * (std::min(hiX, double(xx + 1)) - std::max(loX, double(xx))) *
                                     (std::min(hiY, double(yy + 1)) - std::max(loY, double(yy)));
                    expected.at(current, x, y)[c] = uint8_t(std::round(value / ((hiX - loX) * (hiY - loY))));
                }
    }
    for (bool useCompute : {false, true}) {
        auto                           output = texture(gpu, original, 3);
        DynaArray<AutoRef<GpuPayload>> work;
        MipmapInputs                   inputs {.texture = output};
        REQUIRE((useCompute ? compute->record(inputs, work) : raster->record(inputs, work)));
        REQUIRE(work.size() == 2);
        submit(gpu, work);
        // Texture::readback currently returns only mip zero; download each level explicitly.
        for (uint32_t level = 0; level < 3; ++level) {
            gfx::img::PlaneCoord coord {.level = level};
            auto                 download = GpuCnC::create({.gpu = gpu});
            REQUIRE(download);
            GpuCnC::Region region;
            region.mip                            = level;
            region.imageExtent                    = {expected.width(coord), expected.height(coord), 1};
            auto                           future = download->downloadImage(output, {&region, 1});
            DynaArray<AutoRef<GpuPayload>> reads;
            reads.append(download->seal());
            submit(gpu, reads);
            auto content = future.get();
            REQUIRE(content.blob);
            const auto * bytes = static_cast<const uint8_t *>(content.blob->data());
            for (uint32_t y = 0; y < expected.height(coord); ++y)
                for (uint32_t x = 0; x < expected.width(coord); ++x)
                    for (int c = 0; c < 4; ++c) CHECK(std::abs(int(bytes[(y * expected.width(coord) + x) * 4 + c]) - int(expected.at(coord, x, y)[c])) <= 1);
        }
        inputs.baseLevel = 2;
        work.clear();
        REQUIRE((useCompute ? compute->record(inputs, work) : raster->record(inputs, work)));
        CHECK(work.empty());
        inputs.lastLevel = 3;
        CHECK_FALSE((useCompute ? compute->record(inputs, work) : raster->record(inputs, work)));
        CHECK(work.empty());
    }
}
