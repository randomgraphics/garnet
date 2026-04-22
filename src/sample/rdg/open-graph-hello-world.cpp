#include <garnet/GNrdg2.h>

#include <cstdio>

using namespace GN;

namespace GN::rdg2 {

struct HelloArgs final : public Arguments {
    GN_RDG_REGISTER_RUNTIME_TYPE(Arguments);

    explicit HelloArgs(const StrA & who_): Arguments(TYPE_INFO(), "HelloArgs"), who(who_) {}

    StrA who;
};

struct HelloAction final : public Action {
    GN_RDG_REGISTER_RUNTIME_TYPE(Action);

    HelloAction(): Action(TYPE_INFO(), "HelloAction") {}

    void execute(Arguments & arguments) override {
        const HelloArgs * a = GN::rdg::RuntimeType::cast<HelloArgs>(&arguments);
        std::printf("Hello, %s!\n", a ? a->who.data() : "<unknown>");
    }
};

struct StartMark final : public Entity {
    GN_RDG_REGISTER_RUNTIME_TYPE(Entity);
    StartMark(): Entity(TYPE_INFO(), "StartMark") {}
};

} // namespace GN::rdg2

int main() {
    using namespace GN::rdg2;

    AutoRef<Graph> g = Graph::create();

    // Unblock the graph by publishing a new artifact version after the node is scheduled.
    ArtifactPtr art = g->createArtifact("start");
    TokenPtr    t   = g->getArtifactVersionToken(art);
    NodeDesc    desc;
    desc.name      = "hello-node";
    desc.action    = AutoRef<HelloAction>(new HelloAction());
    desc.arguments = AutoRef<HelloArgs>(new HelloArgs("world"));
    (void) desc.dependencies.append(t);
    (void) g->addNode(desc);
    g->publishArtifact(art, AutoRef<StartMark>(new StartMark()));

    if (g->waitForIdle() != Graph::WaitResult::IDLE) { return 1; }
    return 0;
}
