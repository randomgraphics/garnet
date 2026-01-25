#include <garnet/GNrender-graph.h>
#include <unordered_map>
#include <mutex>

static GN::Logger * sLogger = GN::getLogger("GN.rg");

namespace GN::rg {

// ============================================================================
// Action::Parameter implementation
// ============================================================================

// Constructor for parameter with type, name, and options
// Options string specifies parameter behavior:
// 'O' or 'o' = optional parameter (not required)
// 'R' or 'r' = reading access (input parameter)
// 'W' or 'w' = writing access (output parameter)
GN_API Action::Parameter::Parameter(const Guid & type, const StrA & name, const char * options)
    : type(type)
    , name(name)
{
    if (options) {
        for (const char * opt = options; *opt; opt++) {
            switch (*opt) {
                case 'O':
                case 'o':
                    optional = true;
                    break;
                case 'R':
                case 'r':
                    reading = true;
                    break;
                case 'W':
                case 'w':
                    writing = true;
                    break;
                default:
                    break;
            }
        }
    }
}

// ============================================================================
// ArtifactDatabase implementation
// ============================================================================

// Hash function for Artifact::Identification
struct IdentificationHash {
    size_t operator()(const Artifact::Identification & id) const {
        // Combine hash of Guid and name
        Guid::Hash guidHash;
        std::hash<StrA> nameHash;
        return guidHash(id.type) ^ (nameHash(id.name) << 1);
    }
};

// Equality function for Artifact::Identification
struct IdentificationEqual {
    bool operator()(const Artifact::Identification & a, const Artifact::Identification & b) const {
        return &a.type == &b.type && a.name == b.name;
    }
};

class ArtifactDatabaseImpl : public ArtifactDatabase {
    // Map from artifact type GUID to creator function
    std::unordered_map<Guid, std::function<AutoRef<Artifact>(const StrA &, uint64_t)>, Guid::Hash> mCreators;
    
    // Map from Identification to artifact (for search by Identification)
    std::unordered_map<Artifact::Identification, AutoRef<Artifact>, IdentificationHash, IdentificationEqual> mArtifactsById;
    
    // Map from sequence number to artifact (for fast lookup by sequence)
    std::unordered_map<uint64_t, AutoRef<Artifact>> mArtifactsBySeq;
    
    // Sequence number counter
    uint64_t mNextSequence = 1;
    
    // Mutex for thread safety
    mutable std::mutex mMutex;

public:
    ArtifactDatabaseImpl() = default;
    ~ArtifactDatabaseImpl() = default;

    void registerArtifactType(const Guid & type, std::function<AutoRef<Artifact>(const StrA & name, uint64_t sequence)> && creator) override {
        std::lock_guard<std::mutex> lock(mMutex);
        
        // Check if type is already registered
        if (mCreators.find(type) != mCreators.end()) {
            // Type already registered, redundant registration not allowed
            GN_ERROR(sLogger)("Artifact type already registered: {}", type.toStr());
            return;
        }
        
        mCreators[type] = std::move(creator);
    }

    AutoRef<Artifact> create(const Artifact::Identification & id, bool returnExisting) override {
        std::lock_guard<std::mutex> lock(mMutex);
        
        // Check if artifact already exists
        auto it = mArtifactsById.find(id);
        if (it != mArtifactsById.end()) {
            if (returnExisting) {
                return it->second;
            } else {
                // ID already exists and returnExisting is false, return null
                GN_ERROR(sLogger)("Failed to create artifact: artifact already exists: type={}, name={}", id.type.toStr(), id.name);
                return AutoRef<Artifact>();
            }
        }

        // Find creator for this type
        auto creatorIt = mCreators.find(id.type);
        if (creatorIt == mCreators.end()) {
            // No creator registered for this type
            return AutoRef<Artifact>();
        }

        // Create new artifact with next sequence number
        uint64_t seq = mNextSequence++;
        AutoRef<Artifact> artifact = creatorIt->second(id.name, seq);
        
        if (!artifact) {
            // Creator returned null
            return AutoRef<Artifact>();
        }

        // Store in both maps
        mArtifactsById[id] = artifact;
        mArtifactsBySeq[seq] = artifact;

        return artifact;
    }

    AutoRef<Artifact> search(const Artifact::Identification & id) override {
        std::lock_guard<std::mutex> lock(mMutex);
        
        auto it = mArtifactsById.find(id);
        if (it != mArtifactsById.end()) {
            return it->second;
        }
        return AutoRef<Artifact>();
    }

    AutoRef<Artifact> search(uint64_t sequence) override {
        std::lock_guard<std::mutex> lock(mMutex);
        
        auto it = mArtifactsBySeq.find(sequence);
        if (it != mArtifactsBySeq.end()) {
            return it->second;
        }
        return AutoRef<Artifact>();
    }

    bool erase(const Artifact::Identification & id) override {
        std::lock_guard<std::mutex> lock(mMutex);
        
        auto it = mArtifactsById.find(id);
        if (it == mArtifactsById.end()) {
            // Artifact not found
            return false;
        }
        
        // Get the sequence number before erasing
        uint64_t seq = it->second->sequence;
        
        // Erase from both maps
        mArtifactsById.erase(it);
        mArtifactsBySeq.erase(seq);
        
        return true;
    }

    bool erase(uint64_t sequence) override {
        std::lock_guard<std::mutex> lock(mMutex);
        
        auto it = mArtifactsBySeq.find(sequence);
        if (it == mArtifactsBySeq.end()) {
            // Artifact not found
            return false;
        }
        
        // Get the identification before erasing
        const Artifact::Identification & id = it->second->id;
        
        // Erase from both maps
        mArtifactsBySeq.erase(it);
        mArtifactsById.erase(id);
        
        return true;
    }
};

GN_API ArtifactDatabase * createArtifactDatabase(const CreateArtifactDatabaseParams & params) {
    (void)params; // unused for now
    return new ArtifactDatabaseImpl();
}

} // namespace GN::rg