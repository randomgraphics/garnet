// Tests for Buffer::loadTextureToStagingBuffer().
// Requires a Vulkan GPU context (staging buffer allocation) and the media filesystem.

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNgpu2.h>
#include <garnet/base/filesys.h>
#include <cstring>

using namespace GN;
using namespace GN::gpu2;

static AutoRef<GpuContext> makeGpu() {
    return GpuContext::create("stg-gpu", GpuContext::CreateParameters {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
}

// Resolve a virtual media path (e.g. "media::texture/red.png") to a native absolute path
// that loadTextureToStagingBuffer() can open. Returns empty string if not found.
static StrA resolveMp(const char * virtualPath) {
    StrA native = GN::fs::toNativeDiskFilePath(virtualPath);
    return GN::fs::isFile(native) ? native : StrA {};
}

TEST_CASE("gpu2::Buffer::loadTextureToStagingBuffer: returns valid StagedTexture for a 2D PNG", "[gpu2][buffer][staging][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No Vulkan GPU context available");

    StrA path = resolveMp("media::texture/red.png");
    if (path.empty()) SKIP("media::texture/red.png not found");

    auto staged = Buffer::loadTextureToStagingBuffer("red", gpu, path);
    REQUIRE_FALSE(staged.empty());

    CHECK(staged.staging);
    CHECK(staged.descriptor.width > 0);
    CHECK(staged.descriptor.height > 0);
    CHECK(staged.descriptor.faces == 1u);
    CHECK(staged.descriptor.levels >= 1u);
    CHECK_FALSE(staged.regions.empty());
}

TEST_CASE("gpu2::Buffer::loadTextureToStagingBuffer: staging buffer contains exact image bytes", "[gpu2][buffer][staging][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No Vulkan GPU context available");

    StrA path = resolveMp("media::texture/red.png");
    if (path.empty()) SKIP("media::texture/red.png not found");

    auto staged = Buffer::loadTextureToStagingBuffer("red", gpu, path);
    REQUIRE_FALSE(staged.empty());

    // Load the source image independently to obtain ground-truth pixel bytes.
    auto fp = GN::fs::openFile(path, std::ios::in | std::ios::binary);
    REQUIRE(fp);
    gfx::img::Image ref = gfx::img::Image::load(fp->input(), path.c_str());
    REQUIRE_FALSE(ref.empty());

    // The staging buffer must contain the raw decoded image bytes verbatim.
    auto m = staged.staging->map();
    REQUIRE(m.data());
    REQUIRE(m.size() >= ref.size());
    CHECK(std::memcmp(m.data(), ref.data(), ref.size()) == 0);
}

TEST_CASE("gpu2::Buffer::loadTextureToStagingBuffer: descriptor matches decoded image dimensions", "[gpu2][buffer][staging][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No Vulkan GPU context available");

    StrA path = resolveMp("media::texture/red.png");
    if (path.empty()) SKIP("media::texture/red.png not found");

    auto staged = Buffer::loadTextureToStagingBuffer("red", gpu, path);
    REQUIRE_FALSE(staged.empty());

    auto fp = GN::fs::openFile(path, std::ios::in | std::ios::binary);
    REQUIRE(fp);
    gfx::img::Image ref = gfx::img::Image::load(fp->input(), path.c_str());
    REQUIRE_FALSE(ref.empty());

    gfx::img::PlaneCoord p0 {};
    CHECK(staged.descriptor.width == ref.width(p0));
    CHECK(staged.descriptor.height == ref.height(p0));
}

TEST_CASE("gpu2::Buffer::loadTextureToStagingBuffer: regions cover all face x mip combinations", "[gpu2][buffer][staging][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No Vulkan GPU context available");

    StrA path = resolveMp("media::texture/red.png");
    if (path.empty()) SKIP("media::texture/red.png not found");

    auto staged = Buffer::loadTextureToStagingBuffer("red", gpu, path);
    REQUIRE_FALSE(staged.empty());

    const size_t expected = staged.descriptor.faces * staged.descriptor.levels;
    CHECK(staged.regions.size() == expected);

    for (size_t i = 0; i < staged.regions.size(); ++i) {
        const auto & r = staged.regions[i];
        CHECK(r.imageExtent.x > 0);
        CHECK(r.imageExtent.y > 0);
    }
}

TEST_CASE("gpu2::Buffer::loadTextureToStagingBuffer: returns empty on nonexistent path", "[gpu2][buffer][staging][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto staged = Buffer::loadTextureToStagingBuffer("bad", gpu, "/nonexistent/path/texture.png");
    CHECK(staged.empty());
}
