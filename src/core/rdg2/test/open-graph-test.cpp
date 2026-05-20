#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg2.h>

#include <atomic>

using namespace GN;
using namespace GN::rdg2;

namespace {

struct TestContent final : public Entity {
    GN_REGISTER_RUNTIME_TYPE(Entity);

    static std::atomic<int> alive;

    int value = 0;

    explicit TestContent(int value_): Entity(TYPE_INFO(), "TestContent"), value(value_) { ++alive; }

    ~TestContent() override { --alive; }
};

std::atomic<int> TestContent::alive = 0;

} // namespace

TEST_CASE("rdg2::OpenGraph: artifact keeps only latest published content", "[rdg2][open-graph]") {
    TestContent::alive = 0;

    auto graph = Graph::create();
    REQUIRE(graph);

    auto artifact = graph->createArtifact("content");
    REQUIRE(artifact);

    graph->publishArtifact(artifact, AutoRef<Entity>(new TestContent(1)));
    CHECK(TestContent::alive.load() == 1);
    auto first = graph->getTypedArtifactContent<TestContent>(artifact);
    REQUIRE(first);
    CHECK(first->value == 1);

    graph->publishArtifact(artifact, AutoRef<Entity>(new TestContent(2)));
    CHECK(TestContent::alive.load() == 2);
    auto second = graph->getTypedArtifactContent<TestContent>(artifact);
    REQUIRE(second);
    CHECK(second->value == 2);

    first.clear();
    CHECK(TestContent::alive.load() == 1);
    second.clear();
    CHECK(TestContent::alive.load() == 1);

    graph.clear();
    CHECK(TestContent::alive.load() == 1);

    artifact.clear();
    CHECK(TestContent::alive.load() == 0);
}

TEST_CASE("rdg2::OpenGraph: cross-graph handles are rejected", "[rdg2][open-graph]") {
    auto graph1 = Graph::create();
    auto graph2 = Graph::create();
    REQUIRE(graph1);
    REQUIRE(graph2);

    auto artifact = graph1->createArtifact("artifact from graph1");
    REQUIRE(artifact);
    CHECK(!graph2->getArtifactVersionToken(artifact, NeverOverflowingCounter::ONE()));
    CHECK(!graph2->getArtifactContent(artifact));

    auto token = graph1->getArtifactVersionToken(artifact, NeverOverflowingCounter::ONE());
    REQUIRE(token);
    CHECK(graph2->waitForToken(token) == Graph::WaitResult::FAILED);

    auto node = graph1->addNode(NodeDesc("node from graph1"));
    REQUIRE(node);
    CHECK(!graph2->getNodeCompletionToken(node));
    graph2->satisfyNode(node);
}

TEST_CASE("rdg2::OpenGraph: handles survive graph destruction but graph operations reject them", "[rdg2][open-graph]") {
    ArtifactPtr artifact;
    TokenPtr    token;
    NodePtr     node;

    {
        auto graph = Graph::create();
        REQUIRE(graph);

        artifact = graph->createArtifact("artifact");
        REQUIRE(artifact);
        token = graph->getArtifactVersionToken(artifact, NeverOverflowingCounter::ONE());
        REQUIRE(token);
        node = graph->addNode(NodeDesc("node"));
        REQUIRE(node);
    }

    REQUIRE(artifact);
    REQUIRE(token);
    REQUIRE(node);

    auto graph = Graph::create();
    REQUIRE(graph);
    CHECK(!graph->getArtifactVersionToken(artifact, NeverOverflowingCounter::ONE()));
    CHECK(!graph->getArtifactContent(artifact));
    CHECK(graph->waitForToken(token) == Graph::WaitResult::FAILED);
    CHECK(!graph->getNodeCompletionToken(node));
}
