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

TEST_CASE("rdg2::Artifact: publish and wait are graph independent", "[rdg2][artifact]") {
    auto artifact = Artifact::create("content");
    REQUIRE(artifact);
    CHECK(artifact->version() == NeverOverflowingCounter::OOO());
    CHECK(artifact->publish({}) == NeverOverflowingCounter::OOO());
    CHECK(artifact->version() == NeverOverflowingCounter::OOO());

    auto beforePublish = artifact->wait(NeverOverflowingCounter::ONE(), std::chrono::milliseconds::zero());
    CHECK(beforePublish.status == Artifact::WaitStatus::BUSY);

    TypedArtifact<TestContent> typed(artifact);
    int                        publishSignals = 0;
    NeverOverflowingCounter    signaledVersion;
    auto                       onPublish = [&](const Artifact::Content<> & content) {
        ++publishSignals;
        signaledVersion = content.version;
    };
    auto signalTether = artifact->sigPublished.connect(onPublish);

    CHECK(typed.publish(AutoRef<TestContent>(new TestContent(7))) == NeverOverflowingCounter::ONE());
    CHECK(publishSignals == 1);
    CHECK(signaledVersion == NeverOverflowingCounter::ONE());

    auto afterPublish = typed.wait(NeverOverflowingCounter::ONE(), std::chrono::milliseconds::zero());
    CHECK(afterPublish.status == Artifact::WaitStatus::READY);
    CHECK(afterPublish.version == NeverOverflowingCounter::ONE());
    REQUIRE(afterPublish.content);
    CHECK(afterPublish.content->value == 7);

    auto secondVersion = NeverOverflowingCounter::ONE();
    ++secondVersion;
    CHECK(typed.publish(AutoRef<TestContent>(new TestContent(8))) == secondVersion);
    CHECK(publishSignals == 2);
    CHECK(signaledVersion == secondVersion);

    auto oldVersionWait = typed.wait(NeverOverflowingCounter::ONE(), std::chrono::milliseconds::zero());
    CHECK(oldVersionWait.status == Artifact::WaitStatus::READY);
    CHECK(oldVersionWait.version == secondVersion);
    REQUIRE(oldVersionWait.content);
    CHECK(oldVersionWait.content->value == 8);
}

TEST_CASE("rdg2::OpenGraph: artifact keeps only latest published content", "[rdg2][open-graph]") {
    TestContent::alive = 0;

    auto graph = Graph::create();
    REQUIRE(graph);

    auto artifact = graph->createArtifact("content");
    REQUIRE(artifact);

    graph->publishArtifact(artifact, AutoRef<Entity>(new TestContent(1)));
    CHECK(TestContent::alive.load() == 1);
    auto first = artifact->content<TestContent>();
    REQUIRE(first);
    CHECK(first->value == 1);

    graph->publishArtifact(artifact, AutoRef<Entity>(new TestContent(2)));
    CHECK(TestContent::alive.load() == 2); // artifact holds content#2, first holds the content#1
    auto second = artifact->content<TestContent>();
    REQUIRE(second);
    CHECK(second->value == 2);

    first.clear();
    CHECK(TestContent::alive.load() == 1); // content#1 is released, content#2 is alive.
    second.clear();
    CHECK(TestContent::alive.load() == 1); // content#2 is still alive (referenced by artifact)

    graph.clear();
    CHECK(artifact->content());
    CHECK(TestContent::alive.load() == 1); // content#2 is still hold by artifact

    artifact.clear();
    CHECK(TestContent::alive.load() == 0); // all released.
}

TEST_CASE("rdg2::OpenGraph: artifacts are graph independent while nodes and tokens are graph-owned", "[rdg2][open-graph]") {
    auto graph1 = Graph::create();
    auto graph2 = Graph::create();
    REQUIRE(graph1);
    REQUIRE(graph2);

    auto artifact = graph1->createArtifact("artifact from graph1");
    REQUIRE(artifact);
    auto artifactToken = graph2->getArtifactVersionToken(artifact, NeverOverflowingCounter::ONE());
    REQUIRE(artifactToken);
    graph1->publishArtifact(artifact, AutoRef<Entity>(new TestContent(1)));
    CHECK(graph2->waitForToken(artifactToken) == Graph::WaitResult::IDLE);

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

    NodeDesc desc("manual node", [&] {
        manualRan = true;
        graph->completeNode(manual);
    });
    desc.manualComplete = true;
    manual              = graph->addNode(desc);
    REQUIRE(manual);

    auto token = graph->getNodeCompletionToken(manual);
    REQUIRE(token);

    NodeDesc downstreamDesc("downstream", [&] { ++downstreamRan; });
    downstreamDesc.dependsOn(token);
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

    NodeDesc parentDesc("manual parent", [&] { ++parentRan; });
    parentDesc.manualComplete = true;

    auto parent = graph->addNode(parentDesc);
    REQUIRE(parent);

    NodeDesc childDesc("child", [&] { ++childRan; });
    childDesc.parent = parent;

    auto child = graph->addNode(childDesc);
    REQUIRE(child);

    auto token = graph->getNodeCompletionToken(parent);
    REQUIRE(token);

    NodeDesc downstreamDesc("downstream", [&] { ++downstreamRan; });
    downstreamDesc.dependsOn(token);
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

    NodeDesc parentDesc("manual parent", [&] { ++parentRan; });
    parentDesc.manualComplete = true;

    auto parent = graph->addNode(parentDesc);
    REQUIRE(parent);

    NodeDesc childDesc("blocked child", [&] { ++childRan; });
    childDesc.parent = parent;
    childDesc.dependsOn(childBlocker);
    REQUIRE(graph->addNode(childDesc));

    auto parentToken = graph->getNodeCompletionToken(parent);
    REQUIRE(parentToken);

    NodeDesc downstreamDesc("downstream", [&] { ++downstreamRan; });
    downstreamDesc.dependsOn(parentToken);
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

    NodeDesc downstreamDesc("downstream", [&] { ++downstreamRan; });
    downstreamDesc.dependsOn(token);
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

    NodeDesc autoDesc("auto node", [&] {
        ++autoRan;
        graph->completeNode(automatic);
    });

    automatic = graph->addNode(autoDesc);
    REQUIRE(automatic);

    auto token = graph->getNodeCompletionToken(automatic);
    REQUIRE(token);

    NodeDesc downstreamDesc("downstream", [&] { ++downstreamRan; });
    downstreamDesc.dependsOn(token);
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
    CHECK(!artifact->content());
    auto artifactToken = graph->getArtifactVersionToken(artifact, NeverOverflowingCounter::ONE());
    REQUIRE(artifactToken);
    graph->publishArtifact(artifact, AutoRef<Entity>(new TestContent(1)));
    CHECK(graph->waitForToken(artifactToken) == Graph::WaitResult::IDLE);
    CHECK(graph->waitForToken(token) == Graph::WaitResult::FAILED);
    CHECK(!graph->getNodeCompletionToken(node));
}
