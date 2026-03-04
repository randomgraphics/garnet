#include "pch.h"
#include <garnet/GNrdg.h>
#include "submission.h"
#include <unordered_map>
#include <mutex>
#include <cstddef>

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// ============================================================================
// ArtifactDatabase implementation
// ============================================================================

// Key for lookup by type and name (artifact admits itself via admit(Artifact*))
struct TypeNameKey {
    const uint64_t type;
    const StrA &   name;
    bool           operator==(const TypeNameKey & o) const { return type == o.type && name == o.name; }
    bool           operator!=(const TypeNameKey & o) const { return !(*this == o); }
};

struct TypeNameKeyHash {
    size_t operator()(const TypeNameKey & k) const {
        size_t h = std::hash<uint64_t>()(k.type);
        combineHash(h, k.name);
        return h;
    }
};

class ArtifactDatabaseImpl : public ArtifactDatabase {
    // Map from (type, name) to artifact
    std::unordered_map<TypeNameKey, AutoRef<Artifact>, TypeNameKeyHash> mArtifactsById;

    // Map from sequence number to artifact (for fast lookup by sequence)
    std::unordered_map<uint64_t, AutoRef<Artifact>> mArtifactsBySeq;

    // Sequence number counter
    uint64_t mNextSequence = 1;

    // Mutex for thread safety
    mutable std::mutex mMutex;

public:
    ArtifactDatabaseImpl()  = default;
    ~ArtifactDatabaseImpl() = default;

    uint64_t admit(Artifact * artifact) override {
        if (!artifact) return 0;
        std::lock_guard<std::mutex> lock(mMutex);

        TypeNameKey key {artifact->typeId, artifact->name};
        if (mArtifactsById.find(key) != mArtifactsById.end()) {
            GN_ERROR(sLogger)("Failed to admit artifact: type and name already exist: type={}, name={}", artifact->typeId, artifact->name);
            return 0;
        }

        uint64_t          seq = mNextSequence++;
        AutoRef<Artifact> ref(artifact);
        mArtifactsById[key]  = ref;
        mArtifactsBySeq[seq] = ref;
        return seq;
    }

    auto fetch(uint64_t type, const StrA & name) -> AutoRef<Artifact> override {
        std::lock_guard<std::mutex> lock(mMutex);

        TypeNameKey key {type, name};
        auto        it = mArtifactsById.find(key);
        if (it != mArtifactsById.end()) { return it->second; }
        return AutoRef<Artifact>();
    }

    auto fetch(uint64_t sequence) -> AutoRef<Artifact> override {
        std::lock_guard<std::mutex> lock(mMutex);

        auto it = mArtifactsBySeq.find(sequence);
        if (it != mArtifactsBySeq.end()) { return it->second; }
        return AutoRef<Artifact>();
    }

    bool erase(uint64_t sequence) override {
        std::lock_guard<std::mutex> lock(mMutex);

        auto it = mArtifactsBySeq.find(sequence);
        if (it == mArtifactsBySeq.end()) { return false; }

        Artifact *  a = it->second;
        TypeNameKey key {a->typeId, a->name};
        mArtifactsBySeq.erase(it);
        mArtifactsById.erase(key);
        return true;
    }
};

GN_API ArtifactDatabase * ArtifactDatabase::create(const CreateParameters & params) {
    (void) params;
    return new ArtifactDatabaseImpl();
}

// ============================================================================
// RenderGraph implementation
// ============================================================================

class RenderGraphImpl : public RenderGraph {
    int64_t            mNextSequence = 0;
    mutable std::mutex mMutex;

public:
    RenderGraphImpl() {}
    ~RenderGraphImpl() {}

    Workflow * createWorkflow(StrA name) override { return new WorkflowImpl(name); }

    AutoRef<Submission> submit(const SubmitParameters & params) override {
        std::lock_guard<std::mutex> lock(mMutex);
        DynaArray<WorkflowImpl *>   pending;
        pending.reserve(params.workflows.size());
        for (auto * w : params.workflows) {
            auto * p = WorkflowImpl::promote(w);
            if (p) {
                p->sequence = mNextSequence++;
                pending.append(p);
            }
        }
        GN_VERBOSE(sLogger)("Submitting {} workflows.", pending.size());
        auto submission = AutoRef<Submission>(new SubmissionImpl(std::move(pending), params));
        GN_ASSERT(pending.empty());
        return submission;
    }
};

GN_API RenderGraph * RenderGraph::create(const CreateParameters & params) {
    (void) params; // unused for now
    return new RenderGraphImpl();
}

} // namespace GN::rdg
