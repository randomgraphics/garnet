#include "pch.h"

namespace GN::rdg {

static GN::Logger * sLogger = getLogger("GN.rdg");

struct RenderTargetImpl : public RenderTarget {
    RenderTargetImpl(ArtifactDatabase & db, const StrA & name, const CreateParameters &): RenderTarget(db, RenderTarget::TYPE_INFO(), name) {}

private:
    mutable DynaArray<const Artifact *> mReferencedArtifacts;
};

AutoRef<RenderTarget> RenderTarget::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    return AutoRef<RenderTarget>(new RenderTargetImpl(db, name, params));
}

} // namespace GN::rdg