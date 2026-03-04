#include "pch.h"
#include "backbuffer.h"
#include "gpu-context.h"
#include "vk-backbuffer.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// =============================================================================
// PrepareBackbuffer - API-neutral impl (uses BackbufferCommon::prepare())
// =============================================================================

/// \note
///  - Can't use this as a mark of beginning of a frame, since it could be called very late in the frame, like
///    in a deferred rendering pipeline.
class PrepareBackbufferImpl : public PrepareBackbuffer {
public:
    PrepareBackbufferImpl(ArtifactDatabase & db, const StrA & name): PrepareBackbuffer(db, TYPE_ID, TYPE_NAME, name) {}

    std::pair<ExecutionResult, ExecutionContext *> prepare(TaskInfo &, Arguments &) override { return std::make_pair(PASSED, nullptr); }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments, ExecutionContext *) override {
        auto & submissionImpl = static_cast<SubmissionImpl &>(taskInfo.submission);
        auto   a              = arguments.castTo<PrepareBackbuffer::A>();
        if (!a) {
            GN_ERROR(sLogger)("PrepareBackbuffer::execute: invalid arguments");
            return FAILED;
        }
        if (!a->backbuffer.value) {
            GN_ERROR(sLogger)("PrepareBackbuffer::execute: backbuffer not set");
            return FAILED;
        }
        auto common = a->backbuffer.value->castTo<BackbufferCommon>();
        if (!common) {
            GN_ERROR(sLogger)("PrepareBackbuffer::execute: backbuffer is not BackbufferCommon");
            return FAILED;
        }
        common->prepare(submissionImpl);
        return PASSED;
    }
};

GN_API AutoRef<PrepareBackbuffer> PrepareBackbuffer::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    (void) params;
    auto * p = new PrepareBackbufferImpl(db, name);
    if (p->sequence == 0) {
        GN_ERROR(sLogger)("PrepareBackbuffer::create: duplicate type+name, name='{}'", name);
        delete p;
        return {};
    }
    return AutoRef<PrepareBackbuffer>(p);
}

// =============================================================================
// PresentBackbuffer - API-neutral impl (uses BackbufferCommon::present())
// =============================================================================

Action::ExecutionResult PresentBackbufferImpl::execute(TaskInfo & taskInfo, Arguments & arguments, ExecutionContext *) {
    auto & submissionImpl = static_cast<SubmissionImpl &>(taskInfo.submission);
    auto * a              = arguments.castTo<PresentBackbuffer::A>();
    if (!a) GN_UNLIKELY {
            GN_ERROR(sLogger)("PresentBackbuffer::execute: invalid arguments");
            return FAILED;
        }
    auto & backbuffer = a->backbuffer.value;
    if (!backbuffer) GN_UNLIKELY {
            GN_ERROR(sLogger)("PresentBackbuffer::execute: backbuffer not set");
            return FAILED;
        }
    auto common = backbuffer->castTo<BackbufferCommon>();
    if (!common) GN_UNLIKELY {
            GN_ERROR(sLogger)("PresentBackbuffer::execute: backbuffer is not BackbufferCommon");
            return FAILED;
        }
    common->present(submissionImpl);
    return PASSED;
}

GN_API AutoRef<PresentBackbuffer> PresentBackbuffer::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("PresentBackbuffer::create: context is null, name='{}'", name);
            return {};
        }
    auto common = static_cast<GpuContextCommon *>(params.gpu.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        return createVulkanPresentBackbuffer(db, name, params);
    default: {
        auto * p = new PresentBackbufferImpl(db, name);
        if (p->sequence == 0) {
            GN_ERROR(sLogger)("PresentBackbuffer::create: duplicate type+name, name='{}'", name);
            delete p;
            return {};
        }
        return AutoRef<PresentBackbuffer>(p);
    }
    }
}

} // namespace GN::rdg
