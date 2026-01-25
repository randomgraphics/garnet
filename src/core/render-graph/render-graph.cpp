#include "../../inc/garnet/GNrender-graph.h"

namespace GN::rg {

class RenderGraphImpl : public RenderGraph {
public:
    RenderGraphImpl() {}
    ~RenderGraphImpl() {}

    Task * schedule() override {
        return nullptr;
    }

    Action::ExecutionResult execute() override {
        return Action::ExecutionResult::PASSED;
    }
};

GN_API RenderGraph * createRenderGraph() {
    return new RenderGraphImpl();
}

} // namespace GN::rg