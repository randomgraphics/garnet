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
    graph2->completeNode(node);
}

TEST_CASE("rdg2::OpenGraph: manual node ignores completeNode while action is running", "[rdg2][open-graph]") {
    auto graph = Graph::create();
    REQUIRE(graph);

    bool    manualRan     = false;
    int     downstreamRan = 0;
    NodePtr manual;

    NodeDesc desc("manual node");
    desc.manualComplete = true;
    desc.action         = Action::createFromLambda("try to complete while running", [&] {
        manualRan = true;
        graph->completeNode(manual);
    });
    manual              = graph->addNode(desc);
    REQUIRE(manual);

    auto token = graph->getNodeCompletionToken(manual);
    REQUIRE(token);

    NodeDesc downstreamDesc("downstream");
    downstreamDesc.dependsOn(token);
    downstreamDesc.action = Action::createFromLambda("downstream", [&] { ++downstreamRan; });
    REQUIRE(graph->addNode(downstreamDesc));

    CHECK(graph->waitForIdle(std::chrono::milliseconds::zero()) == Graph::WaitResult::BUSY);
    CHECK(manualRan);
    CHECK(downstreamRan == 0);

    graph->completeNode(manual);
    CHECK(graph->waitForIdle(std::chrono::milliseconds::zero()) == Graph::WaitResult::IDLE);
    CHECK(downstreamRan == 1);
}

TEST_CASE("rdg2::OpenGraph: manual parent waits for completeNode after children finish", "[rdg2][open-graph]") {
    auto graph = Graph::create();
    REQUIRE(graph);

    int parentRan     = 0;
    int childRan      = 0;
    int downstreamRan = 0;

    NodeDesc parentDesc("manual parent");
    parentDesc.manualComplete = true;
    parentDesc.action         = Action::createFromLambda("parent", [&] { ++parentRan; });

    auto parent = graph->addNode(parentDesc);
    REQUIRE(parent);

    NodeDesc childDesc("child");
    childDesc.parent = parent;
    childDesc.action = Action::createFromLambda("child", [&] { ++childRan; });

    auto child = graph->addNode(childDesc);
    REQUIRE(child);

    auto token = graph->getNodeCompletionToken(parent);
    REQUIRE(token);

    NodeDesc downstreamDesc("downstream");
    downstreamDesc.dependsOn(token);
    downstreamDesc.action = Action::createFromLambda("downstream", [&] { ++downstreamRan; });
    REQUIRE(graph->addNode(downstreamDesc));

    CHECK(graph->waitForIdle(std::chrono::milliseconds::zero()) == Graph::WaitResult::BUSY);
    CHECK(parentRan == 1);
    CHECK(childRan == 1);
    CHECK(downstreamRan == 0);

    graph->completeNode(parent);
    CHECK(graph->waitForIdle(std::chrono::milliseconds::zero()) == Graph::WaitResult::IDLE);
    CHECK(downstreamRan == 1);
}

TEST_CASE("rdg2::OpenGraph: completeNode before children complete has no side effects", "[rdg2][open-graph]") {
    auto graph = Graph::create();
    REQUIRE(graph);

    int parentRan     = 0;
    int childRan      = 0;
    int downstreamRan = 0;

    auto blockerArtifact = graph->createArtifact("child blocker");
    REQUIRE(blockerArtifact);
    auto childBlocker = graph->getArtifactVersionToken(blockerArtifact, NeverOverflowingCounter::ONE());
    REQUIRE(childBlocker);

    NodeDesc parentDesc("manual parent");
    parentDesc.manualComplete = true;
    parentDesc.action         = Action::createFromLambda("parent", [&] { ++parentRan; });

    auto parent = graph->addNode(parentDesc);
    REQUIRE(parent);

    NodeDesc childDesc("blocked child");
    childDesc.parent = parent;
    childDesc.dependsOn(childBlocker);
    childDesc.action = Action::createFromLambda("child", [&] { ++childRan; });
    REQUIRE(graph->addNode(childDesc));

    auto parentToken = graph->getNodeCompletionToken(parent);
    REQUIRE(parentToken);

    NodeDesc downstreamDesc("downstream");
    downstreamDesc.dependsOn(parentToken);
    downstreamDesc.action = Action::createFromLambda("downstream", [&] { ++downstreamRan; });
    REQUIRE(graph->addNode(downstreamDesc));

    CHECK(graph->waitForIdle(std::chrono::milliseconds::zero()) == Graph::WaitResult::BUSY);
    CHECK(parentRan == 1);
    CHECK(childRan == 0);
    CHECK(downstreamRan == 0);

    graph->completeNode(parent);
    graph->publishArtifact(blockerArtifact, AutoRef<Entity>(new TestContent(1)));

    CHECK(graph->waitForIdle(std::chrono::milliseconds::zero()) == Graph::WaitResult::BUSY);
    CHECK(childRan == 1);
    CHECK(downstreamRan == 0);

    graph->completeNode(parent);
    CHECK(graph->waitForIdle(std::chrono::milliseconds::zero()) == Graph::WaitResult::IDLE);
    CHECK(downstreamRan == 1);
}

TEST_CASE("rdg2::OpenGraph: completeNode on completed node is ignored", "[rdg2][open-graph]") {
    auto graph = Graph::create();
    REQUIRE(graph);

    int downstreamRan = 0;

    NodeDesc manualDesc("manual node");
    manualDesc.manualComplete = true;
    auto manual               = graph->addNode(manualDesc);
    REQUIRE(manual);

    auto token = graph->getNodeCompletionToken(manual);
    REQUIRE(token);

    NodeDesc downstreamDesc("downstream");
    downstreamDesc.dependsOn(token);
    downstreamDesc.action = Action::createFromLambda("downstream", [&] { ++downstreamRan; });
    REQUIRE(graph->addNode(downstreamDesc));

    CHECK(graph->waitForIdle(std::chrono::milliseconds::zero()) == Graph::WaitResult::BUSY);

    graph->completeNode(manual);
    CHECK(graph->waitForIdle(std::chrono::milliseconds::zero()) == Graph::WaitResult::IDLE);
    CHECK(downstreamRan == 1);

    graph->completeNode(manual);
    CHECK(graph->waitForIdle(std::chrono::milliseconds::zero()) == Graph::WaitResult::IDLE);
    CHECK(downstreamRan == 1);
}

TEST_CASE("rdg2::OpenGraph: completeNode on auto-complete node is ignored", "[rdg2][open-graph]") {
    auto graph = Graph::create();
    REQUIRE(graph);

    int     autoRan       = 0;
    int     downstreamRan = 0;
    NodePtr automatic;

    NodeDesc autoDesc("auto node");
    autoDesc.action = Action::createFromLambda("auto", [&] {
        ++autoRan;
        graph->completeNode(automatic);
    });

    automatic = graph->addNode(autoDesc);
    REQUIRE(automatic);

    auto token = graph->getNodeCompletionToken(automatic);
    REQUIRE(token);

    NodeDesc downstreamDesc("downstream");
    downstreamDesc.dependsOn(token);
    downstreamDesc.action = Action::createFromLambda("downstream", [&] { ++downstreamRan; });
    REQUIRE(graph->addNode(downstreamDesc));

    graph->completeNode(automatic);
    CHECK(autoRan == 0);
    CHECK(downstreamRan == 0);

    CHECK(graph->waitForIdle(std::chrono::milliseconds::zero()) == Graph::WaitResult::IDLE);
    CHECK(autoRan == 1);
    CHECK(downstreamRan == 1);

    graph->completeNode(automatic);
    CHECK(graph->waitForIdle(std::chrono::milliseconds::zero()) == Graph::WaitResult::IDLE);
    CHECK(autoRan == 1);
    CHECK(downstreamRan == 1);
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
