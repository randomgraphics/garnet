#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg2.h>

using namespace GN;
using namespace GN::rdg2;

namespace {

class TestTexture final : public gpu2::Texture {
public:
    TestTexture(): Texture(Texture::TYPE_INFO(), "repo test texture") {}

    const Descriptor & descriptor() const override { return mDesc; }
    gfx::img::Image    readback() const override { return {}; }
    bool               setContent(const gfx::img::Image &) override { return false; }

private:
    Descriptor mDesc;
};

} // namespace

TEST_CASE("rdg2::TextureResource::Repo stores resources by key", "[rdg2][repo]") {
    auto graph = Graph::create();
    REQUIRE(graph);

    auto repo = TextureResource::Repo::create("test texture repo", {.graph = graph});
    REQUIRE(repo);
    CHECK(repo->size() == 0);

    TextureResource::LoadDesc desc;
    desc.key.uri         = "checker";
    desc.key.variantHash = 7;
    desc.deferred        = false;
    desc.fallback        = AutoRef<TextureResource::Content>(new TextureResource::Content("checker fallback", AutoRef<gpu2::Texture>(new TestTexture)));

    auto first = repo->load(desc);
    REQUIRE_FALSE(first.handle.empty());
    REQUIRE(first.artifact);
    CHECK(repo->size() == 1);

    auto content = repo->getContent(first);
    REQUIRE(content);
    CHECK(content == desc.fallback);

    auto second = repo->load(desc);
    CHECK(second.handle == first.handle);
    CHECK(second.artifact == first.artifact);
    CHECK(repo->size() == 1);

    auto found = repo->find(desc.key);
    CHECK(found.handle == first.handle);
    CHECK(found.artifact == first.artifact);

    CHECK(repo->remove(first.handle));
    CHECK(repo->size() == 0);
    CHECK_FALSE(repo->reload(first.handle));
    CHECK(repo->find(desc.key).handle.empty());

    auto third = repo->load(desc);
    REQUIRE_FALSE(third.handle.empty());
    CHECK(third.handle != first.handle);
    CHECK(repo->size() == 1);
}

TEST_CASE("rdg2::geometry and shader repositories are placeholder factories", "[rdg2][repo]") {
    auto graph = Graph::create();
    REQUIRE(graph);

    CHECK(!GpuGeometryResource::Repo::create({.graph = graph}));
    CHECK(!ShaderResource::Repo::create({.graph = graph}));
}
