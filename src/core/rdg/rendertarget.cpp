#include "pch.h"

namespace GN::rdg {

static GN::Logger * sLogger = getLogger("GN.rdg");

struct RenderTargetImpl : public RenderTarget {
    RenderTargetImpl(ArtifactDatabase & db, const StrA & name, const CreateParameters &)
        : RenderTarget(db, RenderTarget::TYPE_ID, RenderTarget::TYPE_NAME, name) {}

    SafeArrayAccessor<const Artifact * const> artifacts() const override {
        mReferencedArtifacts.reserve(8 + 1);
        mReferencedArtifacts.clear();
        for (size_t i = 0; i < colors.size(); ++i) {
            if (!colors[i].target.empty()) {
                auto a = colors[i].target.artifact();
                if (a) mReferencedArtifacts.append(a.get());
            }
        }
        if (!depthStencil.target.empty()) mReferencedArtifacts.append(depthStencil.target.get());
        return mReferencedArtifacts;
    }

private:
    mutable DynaArray<const Artifact *> mReferencedArtifacts;
};

AutoRef<RenderTarget> RenderTarget::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    return AutoRef<RenderTarget>(new RenderTargetImpl(db, name, params));
}

} // namespace GN::rdg