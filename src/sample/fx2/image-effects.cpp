#include <garnet/GNfx2.h>
#include <fstream>

using namespace GN;
using namespace GN::gpu2;
using namespace GN::fx2;

// Headless example: the application selects explicit raster/compute kernels and
// submits their ordered work. No effect owns submission or presentation.
int main(int argc, const char * argv[]) {
    auto gpu = GpuContext::create("image-effects", {});
    if (!gpu) return 1;
    struct Drain {
        AutoRef<GpuContext> gpu;
        ~Drain() { gpu->waitForIdle(); }
    } drain {gpu};
    constexpr uint32_t width = 65, height = 49;
    auto               descriptor    = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(width, height);
    auto               source        = Texture::create("checker", {.context = gpu, .descriptor = Texture::Descriptor(descriptor).setLevels(1)});
    auto               rasterOutput  = Texture::create("raster.filtered-mips", {.context = gpu, .descriptor = descriptor});
    auto               computeOutput = Texture::create("compute.filtered-mips", {.context = gpu, .descriptor = descriptor});
    auto               rasterBlur    = RasterGaussianBlurKernel::create(gpu);
    auto               computeBlur   = ComputeGaussianBlurKernel::create(gpu);
    auto               rasterMips    = RasterMipmapKernel::create(gpu);
    auto               computeMips   = ComputeMipmapKernel::create(gpu);
    if (!source || !rasterOutput || !computeOutput || !rasterBlur || !computeBlur || !rasterMips || !computeMips) return 1;
    std::vector<uint8_t> pixels(width * height * 4);
    auto                 upload = GpuCnC::create({.gpu = gpu});
    if (!upload) return 1;
    for (uint32_t y = 0; y < height; ++y)
        for (uint32_t x = 0; x < width; ++x) {
            auto p = pixels.data() + (y * width + x) * 4;
            p[0]   = ((x / 8 + y / 8) & 1) ? 255 : 0;
            p[1]   = static_cast<uint8_t>(x * 255 / (width - 1));
            p[2]   = static_cast<uint8_t>(y * 255 / (height - 1));
            p[3]   = 255;
        }
    GpuCnC::Region region;
    region.imageExtent = {width, height, 1};
    upload->recordUploadImage(source, pixels, {&region, 1});
    DynaArray<AutoRef<GpuPayload>> work;
    auto                           initialization = upload->seal();
    if (!initialization) return 1;
    work.append(initialization);
    if (!rasterBlur->record({.source = source, .destination = rasterOutput}, work) || !rasterMips->record({.texture = rasterOutput}, work) ||
        !computeBlur->record({.source = source, .destination = computeOutput}, work) || !computeMips->record({.texture = computeOutput}, work))
        return 1;
    GpuContext::SubmitParameters submission("image-effects");
    for (const auto & payload : work) submission.appendWork(payload);
    gpu->submit(submission);
    // Readback is intentionally a final demonstration/check, never a per-frame path.
    auto rasterImage  = rasterOutput->readback();
    auto computeImage = computeOutput->readback();
    if (rasterImage.empty() || computeImage.empty()) return 1;
    if (argc > 1 && StrA(argv[1]) != "t") {
        auto save = [&](const char * suffix, const gfx::img::Image & image) {
            std::ofstream output(StrA::format("{}-{}.png", argv[1], suffix).data(), std::ios::binary);
            if (!output) return false;
            gfx::img::ImageDesc::SaveToStreamParameters parameters;
            parameters.format = gfx::img::ImageDesc::FileFormat::PNG;
            image.desc().save(parameters, output, image.data());
            const auto bytes = output.tellp();
            output.close();
            return !!output && bytes > 0;
        };
        if (!save("raster", rasterImage) || !save("compute", computeImage)) return 1;
    }
    return 0;
}
