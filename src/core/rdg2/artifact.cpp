#include "pch.h"

#include <condition_variable>
#include <mutex>

namespace GN::rdg2 {

namespace {

GN::Logger * sLogger = GN::getLogger("GN.rdg2.artifact");

/// Generates a process-wide unique entity id.
///
/// Thread-safe and monotonic, using NeverOverflowingCounter to avoid wraparound.
NeverOverflowingCounter nextEntityNeverOverflowingId() {
    static std::mutex              m;
    static NeverOverflowingCounter counter = NeverOverflowingCounter::OOO();
    std::lock_guard<std::mutex>    lock(m);
    auto                           out = counter;
    counter.increment();
    return out;
}

class ArtifactImpl final : public Artifact {
public:
    GN_REGISTER_RUNTIME_TYPE(Artifact);

    explicit ArtifactImpl(const StrA & name): Artifact(TYPE_INFO(), name) {}

public:
    Content<> content() const override {
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

        Content<>               publishedContent;
        NeverOverflowingCounter publishedVersion;
        {
            std::lock_guard lock(mMutex);
            ++mVersion;
            mLatestContent   = std::move(content);
            publishedContent = contentLocked_();
            publishedVersion = mVersion;
        }

        // Signal handlers may bridge into graph runtimes that take their own locks.
        mCv.notify_all();
        sigPublished(publishedContent);
        return publishedVersion;
    }

    Snapshot wait(NeverOverflowingCounter version, std::chrono::milliseconds timeout) const override {
        using steady_clock = std::chrono::steady_clock;

        std::unique_lock lock(mMutex);
        const auto       target    = targetVersionLocked_(version);
        const bool       finiteCap = (timeout < std::chrono::milliseconds::max() && timeout.count() > 0);
        const auto       deadline  = finiteCap ? steady_clock::now() + timeout : steady_clock::time_point::max();

        for (;;) {
            if (mVersion >= target) { return snapshotLocked_(WaitStatus::READY); }
            if (timeout == std::chrono::milliseconds::zero()) { return snapshotLocked_(WaitStatus::BUSY); }
            if (finiteCap && steady_clock::now() >= deadline) { return snapshotLocked_(WaitStatus::BUSY); }
            if (finiteCap) {
                mCv.wait_until(lock, deadline);
            } else {
                mCv.wait(lock);
            }
        }
    }

private:
    mutable std::mutex              mMutex;
    mutable std::condition_variable mCv;
    NeverOverflowingCounter         mVersion = NeverOverflowingCounter::OOO();
    AutoRef<Entity>                 mLatestContent;

    NeverOverflowingCounter targetVersionLocked_(NeverOverflowingCounter requested) const {
        if (requested != NeverOverflowingCounter::OOO()) { return requested; }
        auto target = mVersion;
        ++target;
        return target;
    }

    Content<> contentLocked_() const { return {mLatestContent, mVersion}; }

    Snapshot snapshotLocked_(WaitStatus status) const { return {status, mVersion, mLatestContent}; }
};

} // namespace

/// Constructs an RDG entity with a unique id and human-readable name.
GN_API Entity::Entity(const GN::RuntimeType::TypeInfo & type, const StrA & name)
    : RefCounter(), RuntimeType(type), id(nextEntityNeverOverflowingId()), name(name) {}

GN_API ArtifactPtr Artifact::create(const StrA & name) { return ArtifactPtr(new ArtifactImpl(name)); }

} // namespace GN::rdg2
