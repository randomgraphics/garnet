#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg.h>
#include <memory>

using namespace GN;
using namespace GN::rdg;

TEST_CASE("CopyBuffer: TYPE_ID is non-zero and distinct", "[rdg][copy-buffer]") {
    CHECK(GpuCopy::TYPE_ID != 0);
    CHECK(GpuCopy::BufferToBuffer::TYPE_ID != 0);
    CHECK(GpuCopy::TYPE_ID != GpuCopy::BufferToBuffer::TYPE_ID);
}

TEST_CASE("CopyBuffer::A: addToReadWriteList marks src read, dst write", "[rdg][copy-buffer]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto gpu = GpuContext::create(*db, "gpu", {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto src = PersistentBuffer::create(*db, "src", {.context = gpu, .size = 64, .usage = BufferUsageFlags(static_cast<uint32_t>(BufferUsageBits::UNIFORM))});
    auto dst = PersistentBuffer::create(*db, "dst", {.context = gpu, .size = 64, .usage = BufferUsageFlags(static_cast<uint32_t>(BufferUsageBits::UNIFORM))});
    REQUIRE(src);
    REQUIRE(dst);

    GpuCopy::BufferToBuffer args;
    args.src  = src;
    args.dst  = dst;
    args.size = 64;

    std::unordered_set<const Artifact *> readList, writeList;
    Arguments::ArtifactReadWriteList     list {readList, writeList};
    args.addToReadWriteList(list);
    CHECK(readList.size() == 1);
    CHECK(readList.count(src.get()) == 1);
    CHECK(writeList.size() == 1);
    CHECK(writeList.count(dst.get()) == 1);
}

TEST_CASE("CopyBuffer::create: returns non-null with valid gpu", "[rdg][copy-buffer][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto gpu = GpuContext::create(*db, "gpu_cb", {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto copy = GpuCopy::create(*db, "test_copy", {.gpu = gpu});
    REQUIRE(copy);
    CHECK(copy->typeId() == GpuCopy::TYPE_ID);
}

TEST_CASE("CopyBuffer: transient→buffer copy via submission", "[rdg][copy-buffer][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create(*db, "gpu_tcopy", {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    constexpr uint64_t kSize = 64;

    auto dst =
        PersistentBuffer::create(*db, "dst_buf", {.context = gpu, .size = kSize, .usage = BufferUsageFlags(static_cast<uint32_t>(BufferUsageBits::UNIFORM))});
    REQUIRE(dst);

    auto arena = TransientArena::create(*db, "arena_tcopy", TransientArena::CreateParameters {.context = gpu});
    REQUIRE(arena);
    auto tb = arena->allocate(kSize, "tb");
    REQUIRE(tb);
    {
        auto m = tb->map();
        REQUIRE(m.size() >= kSize);
        memset(m.data(), 0xAB, kSize);
    }

    auto copy = GpuCopy::create(*db, "copy_action", {.gpu = gpu});
    REQUIRE(copy);

    auto args  = AutoRef<GpuCopy::BufferToBuffer>::make();
    args->src  = tb;
    args->dst  = dst;
    args->size = kSize;

    auto * wf = rg->createWorkflow("copy-wf");
    wf->appendTask("copy-task", copy, args);

    auto sub = rg->submit({.workflows = {&wf, 1}});
    REQUIRE(sub);
    auto result = sub->result();
    CHECK(result.executionResult == Action::PASSED);
}
