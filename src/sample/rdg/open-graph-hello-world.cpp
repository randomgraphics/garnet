#include <garnet/GNrdg2.h>

#include <cstdio>

using namespace GN;

namespace GN::rdg2 {

struct HelloArgs final : public Arguments {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Arguments);

    explicit HelloArgs(const StrA & who_): Arguments(TYPE_INFO(), "HelloArgs"), who(who_) {}

    StrA who;
};

struct HelloAction final : public Action {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Action);

    HelloAction(): Action(TYPE_INFO(), "HelloAction") {}

    void execute(const Arguments & arguments) override {
        const HelloArgs * a = GN::rdg::RuntimeType::cast<HelloArgs>(&arguments);
        std::printf("Hello, %s!\n", a ? a->who.data() : "<unknown>");
    }
};

} // namespace GN::rdg2

int main() {
    using namespace GN::rdg2;

    AutoRef<Graph> g = Graph::create();

    // A simple dependency: node waits on an external token.
    TokenPtr start = g->createToken("start");

    NodeDesc desc;
    desc.name      = "hello-node";
    desc.action    = AutoRef<HelloAction>(new HelloAction());
    desc.arguments = AutoRef<HelloArgs>(new HelloArgs("world"));
    (void) desc.dependencies.append(Dependency::onToken(start));

    (void) g->addNode(desc.name, desc);

    // Unblock the graph.
    (void) g->satisfyToken(start);

    ReadyNode ready;
    if (!g->tryPopReadyNode(ready)) { return 1; }

    if (ready.action) { ready.action->execute(*ready.arguments); }

    NodeExecutionResult result;
    result.terminalState = NodeState::Completed;
    (void) g->completeNode(ready.node, result);

    return 0;
}
