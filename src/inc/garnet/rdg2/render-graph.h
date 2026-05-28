#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg2/render-graph.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

#include <cstdint>
#include <chrono>

#define GN_RDG2_DEFINE_PUBLIC_ENTITY(base) \
public:                                    \
    GN_API GN_REGISTER_RUNTIME_TYPE(base); \
                                           \
protected:                                 \
    using base::base;                      \
                                           \
public:

namespace GN::rdg2 {

struct LogicalGpuResource;
struct LogicalGpuResourceScope;
struct Quest;
struct Plan;
struct Execution;

using LogicalGpuResourceRef      = AutoRef<LogicalGpuResource>;
using LogicalGpuResourcePtr      = LogicalGpuResourceRef;
using LogicalGpuResourceScopeRef = AutoRef<LogicalGpuResourceScope>;
using LogicalGpuResourceScopePtr = LogicalGpuResourceScopeRef;
using QuestRef                   = AutoRef<Quest>;
using QuestPtr                   = QuestRef;
using PlanRef                    = AutoRef<Plan>;
using PlanPtr                    = PlanRef;
using ExecutionRef               = AutoRef<Execution>;
using ExecutionPtr               = ExecutionRef;

/// Logical GPU resource identity used by closed render-graph declarations.
///
/// Dependency semantics are based on logical resource identity, not on matching
/// descriptions. Two quests that use the same LogicalGpuResource refer to the
/// same semantic storage. Two different transient logical resources may still be
/// mapped to the same physical gpu2 allocation by the planner if their lifetimes
/// do not overlap and their requirements are compatible.
struct LogicalGpuResource : public Entity {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Entity);

    // enum class Kind : uint8_t {
    //     TEXTURE,
    //     BUFFER,
    // };

    // enum class Storage : uint8_t {
    //     /// A concrete existing gpu2 resource/view provided by the snapshot or quest.
    //     IMPORTED,

    //     /// A graph-provided resource allocated or aliased by the backend.
    //     TRANSIENT,
    // };

    // struct Desc {
    //     /// Resource kind.
    //     Kind kind = Kind::TEXTURE;

    //     /// Whether this resource is imported or graph-provided.
    //     Storage storage = Storage::TRANSIENT;

    //     /// Texture properties when kind is TEXTURE.
    //     gpu2::Texture::Descriptor textureDesc = {};

    //     /// Buffer size in bytes when kind is BUFFER.
    //     uint64_t bufferSize = 0;

    //     /// Concrete imported view when storage is IMPORTED.
    //     /// Its resource must match kind.
    //     /// Its range may be narrowed further by individual uses.
    //     gpu2::GpuResourceView importedView = {};
    // };

    // /// Stable name for diagnostics and shared lookup inside a resource scope.
    // virtual const StrA & name() const = 0;

    // /// Resource requirements or imported concrete view.
    // virtual const Desc & desc() const = 0;

    /// The physical GPU resource that this logic resource mapped to. Could be null.
    gpu2::GpuResourceView physical = {};
};

/// Shared namespace of logical GPU resources for one snapshot/plan build.
///
/// Quests that need to communicate through the same resource should use the same
/// LogicalGpuResourceRef from this scope. Matching descriptors do not imply
/// matching identity.
struct LogicalGpuResourceScope : public Entity {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Entity);

    /// Create an empty logical GPU resource scope.
    GN_API static LogicalGpuResourceScopeRef create();

    // inline static const StrA MAIN_COLOR_TARGET = "MAIN_FRAME_TARGET"_s;
    // inline static const StrA MAIN_DEPTH_TARGET = "MAIN_DEPTH_TARGET"_s;
    // inline static const StrA SHADOW_MAP_0      = "SHADOW_MAP_0"_s;  // the first shadow map texture
    // inline static const StrA FRAME_UBO         = "FRAME_UBO"_s;     // uniform buffer that contains shared frame data.
    // inline static const StrA LIGHT_UBO         = "LIGHT_UBO"_s;     // shared uniform buffer containing light information.

    /// Declare or retrieve a logical resource with the given name and descriptor.
    /// Implementations should reject incompatible redeclarations of an existing name.
    virtual LogicalGpuResourceRef declareResource(const StrA & name) = 0;

    /// Find a previously declared resource. Returns empty if no such name exists.
    virtual LogicalGpuResourceRef findResource(const StrA & name) const = 0;
};

/// How a quest uses a logical GPU resource.
enum class GpuResourceAccess : uint8_t {
    /// Reads a resource produced outside the plan or by an earlier quest.
    READ_ONLY,

    /// Writes a complete replacement and does not depend on prior contents.
    DISCARD_WRITE,

    /// Reads previous contents and writes updated contents.
    READ_WRITE,
};

/// Declared GPU resource use by a quest.
///
/// LogicalGpuResource is the unit of semantic identity. The graph resolves each
/// use to a concrete gpu2::GpuResourceView during planning, then applies overlap
/// and hazard rules to the resolved views.
///
/// Cameras, draw lists, material choices, LOD decisions, effect parameters, and
/// other CPU values are quest-owned inputs and are irrelevant to dependency
/// compilation unless they select which logical GPU resources a quest
/// declares.
struct GpuResourceUse {
    /// Human-readable name for diagnostics and quest-local lookup.
    StrA name = StrA::EMPTYSTR();

    /// Logical resource being used.
    LogicalGpuResourceRef resource = {};

    /// Access mode used for dependency and barrier planning.
    GpuResourceAccess access = GpuResourceAccess::READ_ONLY;

    /// Use this resource as a root when determining the executable dependency subgraph.
    bool root = false;
};

/// Pure declaration returned by Quest::declare().
struct QuestDeclaration {
    /// Human-readable name for diagnostics and stable tie-breaking.
    StrA name = StrA::EMPTYSTR();

    /// Logical GPU texture/buffer resources read or written by this quest.
    ///
    /// CPU-side quest inputs are intentionally absent. They are stored by the
    /// concrete quest object and are not part of closed-graph dependency
    /// compilation.
    ArrayContainer<GpuResourceUse> gpuResourceUses = {};

    /// Explicit quest dependencies in addition to GPU resource producer/consumer dependencies.
    ArrayContainer<QuestRef> explicitDependencies = {};
};

/// Result returned by a quest after CPU-side execution.
struct QuestResult {
    enum class Status : uint8_t {
        FAILED,
        SUCCEEDED,
    };

    Status status = Status::FAILED;

    /// Optional diagnostic message. Empty means no additional detail.
    StrA message = StrA::EMPTYSTR();

    static QuestResult succeeded() {
        QuestResult result;
        result.status = Status::SUCCEEDED;
        return result;
    }

    static QuestResult failed(const StrA & message_ = StrA::EMPTYSTR()) {
        QuestResult result;
        result.status  = Status::FAILED;
        result.message = std::move(message_);
        return result;
    }
};

/// Execution-owned interface exposed to a running quest.
struct QuestContext {
    virtual ~QuestContext() = default;

    /// Get the resolved GPU resource view for a declared use by name.
    ///
    /// Imported logical resources resolve to their concrete imported resource.
    /// Transient logical resources resolve to backend-provided concrete resources
    /// selected during planning.
    virtual const gpu2::GpuResourceView & view(const StrA & name) const = 0;

    /// Emit a sealed GPU payload for later deterministic gathering.
    virtual void emit(AutoRef<gpu2::GpuPayload> payload) = 0;

    /// GPU context used for creating GPU objects and sealed payloads.
    virtual AutoRef<gpu2::GpuContext> gpu() const = 0;

protected:
    QuestContext() = default;
};

/// Declared executable unit in the render graph.
struct Quest : public Entity {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Entity);

    /// Describe GPU resource uses, explicit dependencies.
    virtual QuestDeclaration declare() const = 0;

    /// Execute CPU-side work through an execution-owned context.
    virtual QuestResult execute(QuestContext & context) = 0;
};

/// Compiled dependency graph of quests and GPU resource use.
struct Plan : public Entity {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Entity);

    /// Number of quests retained by this plan after validation and culling.
    virtual size_t questCount() const = 0;

    /// Quest in deterministic compiled order.
    virtual QuestRef quest(size_t index) const = 0;
};

/// Active running instance of a compiled plan.
struct Execution : public Entity {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Entity);

    enum class Status : uint8_t {
        READY,
        RUNNING,
        SUCCEEDED,
        FAILED,
        CANCELED,
    };

    /// Current lifecycle status.
    virtual Status status() const = 0;

    /// Request cancellation. Implementations may complete already-running quests first.
    virtual void cancel() = 0;

    /// Wait for execution to leave READY/RUNNING or until the timeout expires.
    virtual Status wait(std::chrono::milliseconds timeout = std::chrono::milliseconds::max()) const = 0;
};

// =====================================================================================================================
// Experimental artifact and quests
// =====================================================================================================================

// // struct TextureContent : Entity {
// //     GN_REGISTER_RUNTIME_TYPE(Entity);

// //     AutoRef<gpu2::Texture> texture;

// //     explicit TextureContent(const StrA & name, AutoRef<gpu2::Texture> texture_ = {}): Entity(TYPE_INFO(), name), texture(std::move(texture_)) {}
// // };

// // /// This is the most basic, atomic, artifact that represents a GPU resource that the render graph can operates.
// // /// This is also the item that is used by compiler to determine dependencies.
// // /// All other artifact will eventually be decomposed into this basic form.
// // using TypedArtifact<gpu2::GpuResourceView> GpuResourceArtifact;
// // using TypedArtifact<gpu2::GpuShader>       GpuShaderArtifact;

// struct GpuResourceEntity : Entity {
//     GN::gpu2::GpuResourceView value;
// };

// struct SscParameters : Entity {
//     SharedShaderConstants::Set0Parameters value;
// };

// struct SscGpuResources : Entity {
//     GN::gpu2::GpuResourceSet value;
// };

// struct RasterTargetEntity : Entity {
//     GN::gpu2::RasterTarget value;
// };

// struct PbrModelList : Entity {
//     ArrayContainer<AutoRef<PbrShading::Asset>> value;
// };

// typedef TypedArtifact<SscParameters>      SscParametersArtifact;
// typedef TypedArtifact<SscGpuResources>    SccGpuResourcesArtifact;
// typedef TypedArtifact<PbrModelList>       PbrModelListArtifact;
// typedef TypedArtifact<RasterTargetEntity> RasterTargetArtifact;

// template<size_t N>
// struct QuestParameterName {
//     char data[N];
//     constexpr QuestParameterName(const char (&str)[N]) { std::copy_n(str, N, data); }
//     constexpr size_t size() const { return N - 1; }
// };

// template<QuestParameterName NAME, GpuResourceAccess ACCESS>
// struct QuestParameter {

//     inline static constexpr GpuResourceAccess Access = ACCESS;

//     bool empty() const { return artifact.empty(); }

//     void declare(QuestDeclaration & d) const {
//         if (!artifact.artifact) return;
//         auto        s = artifact.artifact->sub();
//         ArtifactUse use;
//         use.access   = ACCESS;
//         use.selector = SELECTOR;
//         if (s.empty()) {
//             // if this is a leaf artifact, append itself to the decl array.
//             use.artifacts.append(artifact.artifact);
//         } else {
//             s.forEach([&](const ArtifactRef & a) { use.artifacts.append(a); });
//         }
//         if (!use.artifacts.empty()) { d.artifactUses.append(std::move(use)); }
//     }

//     AutoRef<RELIC> relic() const {
//         if constexpr (RelicSelector::LAST_KNOWN_GOOD == Selector) {
//             return artifact.content().value;
//         } else {
//             GN_UNIMPL();
//             return {};
//         }
//     }
// };

struct FrameBeginQuest : public Quest {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Quest);

    struct CreateParameters {
        AutoRef<LogicalGpuResourceScope> scope;
        AutoRef<gpu2::Swapchain>         swapchain;
    };

    static GN_API AutoRef<FrameBeginQuest> create(const CreateParameters &);
};

struct FrameEndQuest : public Quest {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Quest);

    struct CreateParameters {
        AutoRef<LogicalGpuResourceScope> scope;
        AutoRef<gpu2::Swapchain>         swapchain;
    };

    static GN_API AutoRef<FrameBeginQuest> create(const CreateParameters &);
};

struct SscQuest : public Quest {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Quest);

    struct CreateParameters {
        AutoRef<LogicalGpuResourceScope> scope;
        AutoRef<SharedShaderConstants>   ssc;
    };

    static GN_API AutoRef<SscQuest> create(const CreateParameters &);
};

struct PbrQuest : public Quest {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Quest);

    struct CreateParameters {
        AutoRef<LogicalGpuResourceScope>  scope;
        AutoRef<gpu2::GpuContext>         gpu;
        ArrayContainer<PbrShading::Asset> assets;
    };

    static GN_API AutoRef<PbrQuest> create(const CreateParameters &);
};

/// quest to generate shadow map for certain light.
struct ShadowMapQuest : public Quest {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Quest);

    struct CreateParameters {
        AutoRef<LogicalGpuResourceScope>       scope;
        AutoRef<gpu2::GpuContext>              gpu;
        ArrayProxy<const gpu2::RasterGeometry> casters;
    };

    GN_API AutoRef<ShadowMapQuest> create(const CreateParameters &);
};

// };

} // namespace GN::rdg2
