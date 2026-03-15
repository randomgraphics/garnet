/**
 * Task 3.3 — CopyBufferToImage action API stub verification.
 *
 * Verifies the action type registers correctly and that the stubbed create()
 * returns null with an appropriate log message.
 */

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg.h>

using namespace GN;
using namespace GN::rdg;

TEST_CASE("CopyBufferToImage: TYPE_ID is non-zero and distinct from CopyBuffer", "[rdg][copy-buffer-to-image]") {
    CHECK(GpuCopy::TYPE_ID != 0);
    CHECK(GpuCopy::BufferToImage::TYPE_ID != 0);
    CHECK(GpuCopy::TYPE_ID != GpuCopy::BufferToImage::TYPE_ID);
    CHECK(GpuCopy::BufferToImage::TYPE_ID != GpuCopy::BufferToBuffer::TYPE_ID);
}

TEST_CASE("CopyBufferToImage::A: addToReadWriteList marks src read, dst write", "[rdg][copy-buffer-to-image]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto gpu = GpuContext::create(*db, "gpu_b2i", {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto arena = TransientArena::create(*db, "arena_b2i", TransientArena::CreateParameters {.context = gpu});
    if (!arena) SKIP("TransientArena not available");
    auto src = arena->allocate(256, "tb");
    auto dst =
        Texture::create(*db, "dst_b2i", {.context = gpu, .descriptor = {.format = GN::gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM(), .width = 4, .height = 4}});
    if (!src) SKIP("TransientArena::allocate not available");
    REQUIRE(dst);

    GpuCopy::BufferToImage args;
    args.src = src;
    args.dst = dst;

    std::unordered_set<const Artifact *> readList, writeList;
    Arguments::ArtifactReadWriteList     list {readList, writeList};
    args.addToReadWriteList(list);
    CHECK(readList.size() == 1);
    CHECK(readList.count(src.get()) == 1);
    CHECK(writeList.size() == 1);
    CHECK(writeList.count(dst.get()) == 1);
}

TEST_CASE("CopyBufferToImage::create: GpuCopy::create returns non-null", "[rdg][copy-buffer-to-image][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto gpu = GpuContext::create(*db, "gpu_b2i_create", {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto action = GpuCopy::create(*db, "b2i_stub", {.gpu = gpu});
    CHECK(action);
}
