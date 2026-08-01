#include "pch.h"
#include <garnet/GNrdg.h>
#include "submission.h"
#include <cstddef>
#include <mutex>

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// ============================================================================
// RenderGraph implementation
// ============================================================================

class RenderGraphImpl : public RenderGraph {
    int64_t            mNextSequence = 0;
    mutable std::mutex mMutex;

public:
    RenderGraphImpl() {}
    ~RenderGraphImpl() {}

    Workflow createWorkflow(StrA name) override {
        auto     payload = new WorkflowImplPayload(std::move(name));
        Workflow w;
        w.mPayload = payload;
        return w;
    }

    AutoRef<Submission> submit(const SubmitParameters & params) override {
        std::lock_guard<std::mutex>      lock(mMutex);
        DynaArray<WorkflowImplPayload *> pending;
        pending.reserve(params.workflows.size());
        for (size_t i = 0; i < params.workflows.size(); ++i) {
            auto impl = static_cast<WorkflowImplPayload *>(params.workflows.at(i).mPayload);
            if (impl) GN_LIKELY {
                    impl->sequence = mNextSequence++;
                    pending.append(impl);
                    params.workflows.at(i).mPayload = nullptr;
                }
        }
        GN_VERBOSE(sLogger, "Submitting {} workflows.", pending.size());
        return AutoRef<Submission>(new SubmissionImpl(std::move(pending), params));
    }
};

GN_API RenderGraph * RenderGraph::create(const CreateParameters & params) {
    (void) params; // unused for now
    return new RenderGraphImpl();
}

} // namespace GN::rdg
