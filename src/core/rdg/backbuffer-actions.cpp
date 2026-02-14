#include "backbuffer.h"
#include <garnet/GNrdg.h>

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
    PrepareBackbufferImpl(ArtifactDatabase & db, const StrA & name): PrepareBackbuffer(db, TYPE, name) {}

    std::pair<ExecutionResult, ExecutionContext *> prepare(Submission &, Arguments &) override {
        return std::make_pair(PASSED, nullptr);
    }

    ExecutionResult execute(Submission &, Arguments & arguments, ExecutionContext *) override {
        auto * a = arguments.castTo<PrepareBackbuffer::A>();
        if (!a) {
            GN_ERROR(sLogger)("PrepareBackbuffer::execute: invalid arguments");
            return FAILED;
        }
        auto * backbufferRef = a->backbuffer.get();
        if (!backbufferRef || !backbufferRef->get()) {
            GN_ERROR(sLogger)("PrepareBackbuffer::execute: backbuffer not set");
            return FAILED;
        }
        auto * common = backbufferRef->get()->castTo<BackbufferCommon>();
        if (!common) {
            GN_ERROR(sLogger)("PrepareBackbuffer::execute: backbuffer is not BackbufferCommon");
            return FAILED;
        }
        common->prepare();
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

class PresentBackbufferImpl : public PresentBackbuffer {
public:
    PresentBackbufferImpl(ArtifactDatabase & db, const StrA & name): PresentBackbuffer(db, TYPE, name) {}

    std::pair<ExecutionResult, ExecutionContext *> prepare(Submission &, Arguments &) override {
        return std::make_pair(PASSED, nullptr);
    }

    ExecutionResult execute(Submission &, Arguments & arguments, ExecutionContext *) override {
        auto * a = arguments.castTo<PresentBackbuffer::A>();
        if (!a) GN_UNLIKELY {
                GN_ERROR(sLogger)("PresentBackbuffer::execute: invalid arguments");
                return FAILED;
            }
        auto * backbufferRef = a->backbuffer.get();
        if (!backbufferRef || !backbufferRef->get()) GN_UNLIKELY {
                GN_ERROR(sLogger)("PresentBackbuffer::execute: backbuffer not set");
                return FAILED;
            }
        auto * common = backbufferRef->get()->castTo<BackbufferCommon>();
        if (!common) GN_UNLIKELY {
                GN_ERROR(sLogger)("PresentBackbuffer::execute: backbuffer is not BackbufferCommon");
                return FAILED;
            }
        common->present();
        return PASSED;
    }
};

GN_API AutoRef<PresentBackbuffer> PresentBackbuffer::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    (void) params;
    auto * p = new PresentBackbufferImpl(db, name);
    if (p->sequence == 0) {
        GN_ERROR(sLogger)("PresentBackbuffer::create: duplicate type+name, name='{}'", name);
        delete p;
        return {};
    }
    return AutoRef<PresentBackbuffer>(p);
}

} // namespace GN::rdg
