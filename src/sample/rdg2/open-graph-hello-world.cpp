#include <garnet/GNrdg2.h>

#include <cstdio>

using namespace GN;

namespace GN::rdg2 {

struct StartMark final : public Entity {
    GN_REGISTER_RUNTIME_TYPE(Entity);
    StartMark(): Entity(TYPE_INFO(), "StartMark") {}
};

} // namespace GN::rdg2

int main() {
    using namespace GN::rdg2;

    GraphPtr g = Graph::create();

    // Unblock the graph by publishing a new artifact version after the node is scheduled.
    ArtifactPtr art = g->createArtifact("start");
    TokenPtr    t   = g->getTokenToEnsureArtifactIsPublishedAtLeastOnce(art);
    NodeDesc    desc("hello-node", [] { std::printf("Hello, world!\n"); });
    (void) desc.dependencies.append(t);
    (void) g->addNode(desc);
    g->publishArtifact(art, AutoRef<StartMark>(new StartMark()));

    if (g->waitForIdle() != Graph::WaitResult::IDLE) { return 1; }
    return 0;
}
