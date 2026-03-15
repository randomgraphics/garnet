#include "pch.h"

namespace GN::rdg {

static GN::Logger * sLogger = getLogger("GN.rdg");

struct RenderTargetImpl : public RenderTarget {
    RenderTargetImpl(const StrA & name, const CreateParameters &): RenderTarget(RenderTarget::TYPE_INFO(), name) {}

private:
    mutable DynaArray<const Artifact *> mReferencedArtifacts;
};

AutoRef<RenderTarget> RenderTarget::create(const StrA & name, const CreateParameters & params) {
    return AutoRef<RenderTarget>(new RenderTargetImpl(name, params));
}

} // namespace GN::rdg