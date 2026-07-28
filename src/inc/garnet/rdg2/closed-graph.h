#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg2/closed-graph.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

#include <cstdint>
#include <chrono>
#include <functional>

namespace GN::rdg2 {

struct Quest;
struct Plan;
struct Execution;

using QuestRef     = AutoRef<Quest>;
using QuestPtr     = QuestRef;
using PlanRef      = AutoRef<Plan>;
using PlanPtr      = PlanRef;
using ExecutionRef = AutoRef<Execution>;
using ExecutionPtr = ExecutionRef;

/// How a quest uses an artifact.
enum class ArtifactAccess : uint8_t {
    /// Reads a relic published outside the plan or by an earlier quest.
    READ_ONLY,

    /// Publishes a complete replacement and does not depend on prior contents.
    DISCARD_WRITE,

    /// Reads the previous relic and publishes an updated relic.
    READ_WRITE,
};

/// Symbolic relic version request resolved by the execution layer.
///
/// IMPORTANT — in-plan producers override the selector. When an earlier quest
/// in the same plan writes the artifact, a read binds to that writer's
/// publication (a compile-time dependency edge) and the selector is ignored.
/// The selector is consulted only for reads with no in-plan producer:
/// imported artifacts and background-published relics. A quest that needs the
/// pre-execution relic of an artifact that is also written in the same plan
/// must use two artifacts (e.g. historyIn/historyOut); selectors cannot
/// express that.
///
/// Outside execution, code must not use concrete version numbers for ordinary
/// reads; a queried concrete version can become stale immediately. Selectors
/// are resolved to concrete relic versions at the synchronization point chosen
/// by the execution layer.
enum class RelicSelector : uint8_t {
    /// The latest relic at plan compile time.
    LATEST_AT_COMPILE,

    /// The latest relic when the execution starts.
    LATEST_AT_EXECUTION_START,

    /// Wait for the next relic published after execution starts.
    NEXT_PUBLISHED,
};

/// Declared artifact use by a quest.
///
/// The artifact is the unit of semantic identity. Dependency semantics are
/// based on artifact identity, not on matching descriptions: quests that need
/// to communicate through the same data must use the same Artifact. The graph
/// resolves each read to a concrete relic according to the declared selector,
/// resolves each write to a publication of a new relic, then applies overlap
/// and hazard rules to the GPU resources carried by the resolved relics.
/// Distinct artifacts may still be mapped to one physical gpu2 allocation by
/// the planner when their lifetimes do not overlap and their requirements are
/// compatible.
///
/// Cameras, draw lists, material choices, LOD decisions, effect parameters, and
/// other CPU values are quest-owned inputs and are irrelevant to dependency
/// compilation unless they select which artifacts a quest declares.
struct ArtifactUse {
    /// Human-readable name for diagnostics and quest-local lookup.
    StrA name = StrA::EMPTYSTR();

    /// Artifact being used.
    ArtifactRef artifact = {};

    /// Access mode used for dependency and barrier planning.
    ArtifactAccess access = ArtifactAccess::READ_ONLY;

    /// Relic version policy for READ_ONLY and READ_WRITE accesses. Consulted
    /// only when no earlier quest in the plan writes this artifact; an in-plan
    /// producer always overrides the selector (see RelicSelector).
    RelicSelector selector = RelicSelector::LATEST_AT_COMPILE;

    /// Use this artifact as a root when determining the executable dependency subgraph.
    bool root = false;
};

/// Pure declaration returned by Quest::declare().
struct QuestDeclaration {
    /// Human-readable name for diagnostics and stable tie-breaking.
    StrA name = StrA::EMPTYSTR();

    /// Artifacts read or written by this quest.
    ///
    /// CPU-side quest inputs are intentionally absent. They are stored by the
    /// concrete quest object and are not part of closed-graph dependency
    /// compilation.
    ArrayContainer<ArtifactUse> artifactUses = {};

    /// Explicit quest dependencies in addition to artifact producer/consumer dependencies.
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

    /// Read the relic resolved for a declared artifact use.
    ///
    /// The concrete relic version is selected by the plan/execution according
    /// to the declared selector; quests never resolve versions themselves.
    virtual Artifact::Relic<> read(const ArtifactRef & artifact) const = 0;

    /// Typed convenience over read(). Returns an empty relic when the resolved
    /// content is missing or not compatible with T.
    template<typename T>
    Artifact::Relic<T> read(const ArtifactRef & artifact) const {
        auto e = read(artifact);
        if (!e.value) return {};
        auto typed = RuntimeType::cast<T>(e.value.get());
        if (!typed) return {};
        return {GN::referenceTo(typed), e.version};
    }

    /// Publish new content for a declared DISCARD_WRITE or READ_WRITE artifact.
    virtual void publish(const ArtifactRef & artifact, AutoRef<Entity> content) = 0;

    /// Emit a sealed GPU payload for later deterministic gathering.
    virtual void emit(AutoRef<gpu2::GpuPayload> payload) = 0;

    /// Register an external GPU payload that the gathered submission must wait
    /// on, such as the swapchain acquire payload.
    virtual void await(AutoRef<gpu2::GpuPayload> dependency) = 0;

    /// Request presentation of \p swapchain after this execution's gathered
    /// payloads are submitted. The execution performs the present after the
    /// final submission; quests never present directly.
    virtual void present(AutoRef<gpu2::Swapchain> swapchain) = 0;

    /// GPU context used for creating GPU objects and sealed payloads.
    virtual AutoRef<gpu2::GpuContext> gpu() const = 0;

protected:
    QuestContext() = default;
};

/// Declared executable unit in the render graph.
///
/// Most pass-style quests are fully described by their artifact uses plus an
/// execute callable and should be created through create(). Subclass Quest
/// only when the quest has a real lifecycle: stateful incremental producers,
/// heavyweight effect adapters, or API-specific implementations.
struct Quest : public Entity {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Entity);

    /// Configuration for a generic pass-style quest.
    struct CreateParameters {
        /// Human-readable name for diagnostics and stable tie-breaking.
        StrA name = StrA::EMPTYSTR();

        /// Artifacts read or written by the quest.
        ArrayContainer<ArtifactUse> artifactUses = {};

        /// Explicit quest dependencies in addition to artifact producer/consumer dependencies.
        ArrayContainer<QuestRef> explicitDependencies = {};

        /// CPU-side execution body. Must honor the declared artifact uses.
        std::function<QuestResult(QuestContext &)> execute = {};
    };

    /// Create a generic quest whose declaration and execution body are fully
    /// supplied by the caller. Returns an empty ref when no execute callable
    /// is provided.
    GN_API static QuestRef create(const CreateParameters & parameters);

    /// Describe artifact uses, explicit dependencies.
    virtual QuestDeclaration declare() const = 0;

    /// Execute CPU-side work through an execution-owned context.
    virtual QuestResult execute(QuestContext & context) = 0;
};

/// Compiled dependency graph of quests and artifact use.
struct Plan : public Entity {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Entity);

    struct CompileParameters {
        /// Quests to compile. Order is semantically meaningful: it defines
        /// "prior" for read-binds-to-latest-prior-write, orders multiple
        /// writes to the same artifact, and a reader must be listed after its
        /// in-plan producer. The first implementation also executes in exactly
        /// this order; a future compiler may reorder independent quests but
        /// keeps declaration order as the deterministic tie-breaker.
        ArrayContainer<QuestRef> quests;
    };

    /// Compile quest declarations into an executable plan. Returns an empty
    /// ref when validation fails: a read whose artifact has neither an
    /// earlier in-plan writer nor an already-published (imported) relic, an
    /// unknown explicit dependency, or a null quest.
    GN_API static PlanRef compile(const CompileParameters &);

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

    struct RunParameters {
        /// Plan to execute.
        PlanRef plan;

        /// GPU context used to submit gathered payloads. May stay empty for
        /// CPU-only plans that emit no payloads.
        AutoRef<gpu2::GpuContext> gpu;

        /// Human-readable name for diagnostics and submission labeling.
        StrA name = "rdg2-execution";
    };

    /// Execute a compiled plan synchronously on the calling thread: run quests
    /// in compiled order, commit each quest's staged publications on success,
    /// gather emitted payloads, submit them in one batch, then perform any
    /// present requested by a quest. CPU failure skips GPU submission.
    ///
    /// The returned execution is already in a terminal state. GPU-side
    /// completion tracking (onComplete-driven status) is not implemented yet;
    /// callers that need the GPU work finished use gpu2 synchronization.
    GN_API static ExecutionRef run(const RunParameters &);

    /// Current lifecycle status.
    virtual Status status() const = 0;

    /// Request cancellation. Implementations may complete already-running quests first.
    virtual void cancel() = 0;

    /// Wait for execution to leave READY/RUNNING or until the timeout expires.
    virtual Status wait(std::chrono::milliseconds timeout = std::chrono::milliseconds::max()) const = 0;
};

// =====================================================================================================================
// Frame lifecycle helpers
// =====================================================================================================================

/// Relic content published by the frame-begin quest: one acquired swapchain
/// frame (render target view plus the acquire payload).
struct SwapchainFrameEntity : public Entity {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Entity);

    /// The acquired frame. Consumers render into frame.view; the execution
    /// waits on frame.ready before the gathered submission executes.
    gpu2::Swapchain::Frame frame;

    explicit SwapchainFrameEntity(gpu2::Swapchain::Frame frame_): Entity(TYPE_INFO(), "swapchain-frame"), frame(std::move(frame_)) {}
};

/// Parameters for createFrameBeginQuest().
struct FrameBeginParameters {
    /// Swapchain to acquire from.
    AutoRef<gpu2::Swapchain> swapchain;

    /// Artifact that receives the acquired frame as a SwapchainFrameEntity relic.
    ArtifactRef backbuffer;
};

/// Create the quest that acquires the next swapchain frame, publishes it as a
/// SwapchainFrameEntity relic of the backbuffer artifact, and registers the
/// acquire payload as an external dependency of the gathered submission.
GN_API QuestRef createFrameBeginQuest(const FrameBeginParameters &);

/// Parameters for createFrameEndQuest().
struct FrameEndParameters {
    /// Swapchain to present.
    AutoRef<gpu2::Swapchain> swapchain;

    /// Final frame artifact; declared as a read and as the frame's output root
    /// so upstream rendering is retained and ordered before the present.
    ArtifactRef backbuffer;
};

/// Create the quest that declares the backbuffer as the frame's output root
/// and requests presentation. The execution performs the actual present after
/// the gathered submission; quests never present directly.
GN_API QuestRef createFrameEndQuest(const FrameEndParameters &);

// =====================================================================================================================
// Experimental artifact and quests
// =====================================================================================================================

// Well-known artifact name sketch retained from the removed ArtifactScope draft:
//
// inline static const StrA MAIN_COLOR_TARGET = "MAIN_FRAME_TARGET"_s;
// inline static const StrA MAIN_DEPTH_TARGET = "MAIN_DEPTH_TARGET"_s;
// inline static const StrA SHADOW_MAP_0      = "SHADOW_MAP_0"_s;  // the first shadow map texture
// inline static const StrA FRAME_UBO         = "FRAME_UBO"_s;     // uniform buffer that contains shared frame data.
// inline static const StrA LIGHT_UBO         = "LIGHT_UBO"_s;     // shared uniform buffer containing light information.

// Resource descriptor sketch retained from the earlier logical-GPU-resource draft. If artifacts
// need planner-visible GPU requirements (kind, storage class, texture/buffer descriptors, imported
// concrete views), something like this may return as an artifact-level descriptor:
//
// enum class Kind : uint8_t {
//     TEXTURE,
//     BUFFER,
// };
//
// enum class Storage : uint8_t {
//     /// A concrete existing gpu2 resource/view provided by the snapshot or quest.
//     IMPORTED,
//
//     /// A graph-provided resource allocated or aliased by the backend.
//     TRANSIENT,
// };
//
// struct Desc {
//     /// Resource kind.
//     Kind kind = Kind::TEXTURE;
//
//     /// Whether this resource is imported or graph-provided.
//     Storage storage = Storage::TRANSIENT;
//
//     /// Texture properties when kind is TEXTURE.
//     gpu2::Texture::Descriptor textureDesc = {};
//
//     /// Buffer size in bytes when kind is BUFFER.
//     uint64_t bufferSize = 0;
//
//     /// Concrete imported view when storage is IMPORTED.
//     /// Its resource must match kind.
//     /// Its range may be narrowed further by individual uses.
//     gpu2::GpuResourceView importedView = {};
// };

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

// NOTE: this sketch predates the removal of Artifact::sub(), grouped ArtifactUse artifact lists,
// and the LAST_KNOWN_GOOD selector; refresh it against the current API before reviving.
// template<QuestParameterName NAME, ArtifactAccess ACCESS>
// struct QuestParameter {

//     inline static constexpr ArtifactAccess Access = ACCESS;

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

/// Parked sketch of a bespoke effect-adapter quest — the subclass authoring
/// tier. Pass-style adapters (frame begin/swapchain acquire, shared shader
/// constants, shadow map, postprocess, ...) are authored through
/// Quest::create() instead; swapchain present belongs to the future
/// execution/submission policy, not to a quest.
struct PbrQuest : public Quest {
    GN_RDG2_DEFINE_PUBLIC_ENTITY(Quest);

    struct CreateParameters {
        AutoRef<gpu2::GpuContext>         gpu;
        ArrayContainer<PbrShading::Asset> assets;
    };

    static GN_API AutoRef<PbrQuest> create(const CreateParameters &);
};

} // namespace GN::rdg2
