#pragma once

#include <garnet/GNrdg.h>
#include "submission.h"

namespace GN::rdg {

// BackbufferCommon is the "common impl" base for the Backbuffer artifact: it extends the
// public artifact (Backbuffer) with an API-neutral frame interface (prepare/present) that
// backend-specific implementations (BackbufferVulkan, etc.) inherit and implement.

/// Common base for API-specific Backbuffer implementations (Vulkan, D3D12, Metal).
/// Adds frame boundaries: prepare() = beginning of frame, present() = end of frame.
class BackbufferCommon : public Backbuffer {
public:
    /// Beginning of a frame. Acquire backbuffer image; call before rendering.
    virtual Action::ExecutionResult prepare(SubmissionImpl & submission) = 0;
    /// End of a frame. Present to display; call after rendering.
    virtual Action::ExecutionResult present(SubmissionImpl & submission) = 0;

protected:
    BackbufferCommon(ArtifactDatabase & db, const StrA & name): Backbuffer(db, TYPE_ID, TYPE_NAME, name) {}
};

class PresentBackbufferImpl : public PresentBackbuffer {
public:
    PresentBackbufferImpl(ArtifactDatabase & db, const StrA & name): PresentBackbuffer(db, TYPE_ID, TYPE_NAME, name) {}

    std::pair<ExecutionResult, ExecutionContext *> prepare(TaskInfo &, Arguments &) override { return std::make_pair(PASSED, nullptr); }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments, ExecutionContext *) override;
}; // namespace GN::rd;

} // namespace GN::rdg
