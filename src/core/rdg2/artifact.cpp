#include "pch.h"

#include <mutex>

namespace GN::rdg2 {

namespace {

GN::Logger * sLogger = GN::getLogger("GN.rdg2.artifact");

class ArtifactImpl final : public Artifact {
public:
    GN_REGISTER_RUNTIME_TYPE(Artifact);

    explicit ArtifactImpl(const StrA & name): Artifact(TYPE_INFO(), name) {}

public:
    Relic<> content() const override {
        std::lock_guard lock(mMutex);
        return {mLatestContent, mVersion};
    }

    NeverOverflowingCounter version() const override {
        std::lock_guard lock(mMutex);
        return mVersion;
    }

    NeverOverflowingCounter publish(AutoRef<Entity> content) override {
        if (!content) GN_UNLIKELY {
                GN_ERROR(sLogger)("Artifact '{}': publishing empty content is prohibited.", name);
                return NeverOverflowingCounter::OOO();
            }

        Relic<>                 publishedContent;
        NeverOverflowingCounter publishedVersion;
        {
            std::lock_guard lock(mMutex);
            ++mVersion;
            mLatestContent   = std::move(content);
            publishedContent = contentLocked_();
            publishedVersion = mVersion;
        }

        // Signal handlers may bridge into graph runtimes that take their own locks.
        sigPublished(publishedContent);
        return publishedVersion;
    }

private:
    mutable std::mutex      mMutex;
    NeverOverflowingCounter mVersion = NeverOverflowingCounter::OOO();
    AutoRef<Entity>         mLatestContent;

    Relic<> contentLocked_() const { return {mLatestContent, mVersion}; }
};

} // namespace

/// Process-wide unique entity id generator. The first id is 1; 0 (OOO) is never assigned.
static NeverOverflowingCounter nextEntityId() {
    static std::mutex              m;
    static NeverOverflowingCounter counter = NeverOverflowingCounter::OOO();
    std::lock_guard<std::mutex>    lock(m);
    return counter.increment();
}

/// Constructs an rdg2 entity with an automatically generated id. Defined here (and exported via
/// GN_API on the declaration) so the id counter lives in exactly one place in the process,
/// keeping generated ids unique even across DLL boundaries.
GN_API Entity::Entity(const RuntimeType::TypeInfo & type, const StrA & name_): RefCountedRuntimeType(type, nextEntityId(), name_) {}

GN_API ArtifactPtr Artifact::create(const StrA & name) { return ArtifactPtr(new ArtifactImpl(name)); }

} // namespace GN::rdg2
