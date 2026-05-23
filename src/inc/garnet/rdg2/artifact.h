#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg2/artifact.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

#include <cstddef>
#include <cstdint>
#include <chrono>
#include <limits>
#include <type_traits>
#include <utility>

namespace GN::rdg2 {

/// A counter that never overflows.
///
/// This is internally a 128-bit counter. Even if you increment it a billion times per second,
/// it'll take over 5 sextillion (10^21) years to overflow, which is 700 billion times longer than
/// the age of our universe.
///
/// It is overkill for almost all practical use cases. Just create this class for fun and learning purposes.
struct NeverOverflowingCounter {
    /// Low 64 bits of the 128-bit counter.
    uint64_t value0 = 0;
    /// High 64 bits of the 128-bit counter.
    uint64_t value1 = 0;

    /// Zero: default for fresh counters (e.g. artifact version before the first publish).
    /// The same value is also used as a sentinel in APIs such as `Graph::getArtifactVersionToken`
    /// to mean "wait for the next published version" rather than a fixed version number.
    static inline constexpr NeverOverflowingCounter OOO() { return NeverOverflowingCounter {0, 0}; }

    /// Smallest positive value: use when you need an explicit first version (1) after zero,
    /// or a minimal non-zero bound in version logic.
    static inline constexpr NeverOverflowingCounter ONE() { return NeverOverflowingCounter {1, 0}; }

    /// Saturated maximum: use as an upper sentinel so this compares greater than any realistic
    /// version, e.g. open-ended ranges or "no ceiling" in ordering.
    static inline constexpr NeverOverflowingCounter INF() {
        return NeverOverflowingCounter {std::numeric_limits<uint64_t>::max(), std::numeric_limits<uint64_t>::max()};
    }

    /// hash function for this counter, so it can be used as a key in hash-based containers.
    static inline size_t hash(const NeverOverflowingCounter & c) {
        auto h = std::hash<uint64_t>()(c.value0);
        combineHash(h, c.value1);
        return h;
    }

    NeverOverflowingCounter & increment() {
        if (++value0 == 0) { ++value1; }
        return *this;
    }

    NeverOverflowingCounter & operator++() { return increment(); }

    NeverOverflowingCounter operator++(int) {
        auto old = *this;
        increment();
        return old;
    }

    NeverOverflowingCounter & decrement() {
        if (--value0 == std::numeric_limits<uint64_t>::max()) { --value1; }
        return *this;
    }

    NeverOverflowingCounter & reset() {
        value0 = 0;
        value1 = 0;
        return *this;
    }

    bool operator==(const NeverOverflowingCounter & other) const { return value0 == other.value0 && value1 == other.value1; }

    bool operator!=(const NeverOverflowingCounter & other) const { return !(*this == other); }

    bool operator<(const NeverOverflowingCounter & other) const {
        if (value1 != other.value1) return value1 < other.value1;
        return value0 < other.value0;
    }

    bool operator>(const NeverOverflowingCounter & other) const { return other < *this; }

    bool operator<=(const NeverOverflowingCounter & other) const { return !(*this > other); }

    bool operator>=(const NeverOverflowingCounter & other) const { return !(*this < other); }
};

// ============================================================
// Base class of everything with a ID and name.
// ============================================================

/// The basic building block of the render graph module. Base class of everything that
/// needs reference counting and runtime type information.
struct Entity : public RefCounter, public RuntimeType {
    GN_API GN_REGISTER_RUNTIME_TYPE();

    /// ID of the entity. Guaranteed to be unique within the process.
    const NeverOverflowingCounter id;

    /// Name of the entity. Optional. No uniqueness requirement.
    const StrA name;

    virtual ~Entity() {
#if GN_BUILD_DEBUG_ENABLED
        static auto * logger = GN::getLogger("GN.rdg2");
        GN_VVTRACE(logger)("Destroying RDG2 entity: name='{}', type = {}, id={}.{}", name, typeInfo().name, id.value0, id.value1);
#endif
    }

protected:
    /// Constructor
    GN_API Entity(const RuntimeType::TypeInfo & type, const StrA & name);
};

/// Versioned payload slot independent of any graph implementation.
struct Artifact : public Entity {
    GN_API GN_REGISTER_RUNTIME_TYPE(Entity);

    /// Result of waiting on an artifact version.
    enum class WaitStatus : uint8_t {
        READY, /// The requested version has been published.
        BUSY,  /// The requested version was not reached before the timeout.
    };

    /// Content captured atomically with the actual retained content version.
    struct Snapshot {
        WaitStatus status = WaitStatus::BUSY;
        /// Actual version of the returned content. This may be newer than the version passed to wait().
        NeverOverflowingCounter version = NeverOverflowingCounter::OOO();
        AutoRef<Entity>         content = {};
    };

    /// Fired whenever a new version is published.
    Signal<void(const Snapshot &)> sigPublished;

    /// Create a standalone artifact with no content and version equal to 0.
    GN_API static AutoRef<Artifact> create(const StrA & name = StrA::EMPTYSTR());

    /// Get the latest published content. Could be null if the artifact has not been published.
    virtual AutoRef<Entity> content() const = 0;

    template<typename T>
    AutoRef<T> content() const {
        auto e = content();
        if (!e) {
            static auto * logger = GN::getLogger("GN.rdg2");
            GN_ERROR(logger)("Artifact::content: artifact content is empty");
            return {};
        };
        auto typed = RuntimeType::cast<T>(e.get());
        if (!typed) {
            static auto * logger = GN::getLogger("GN.rdg2");
            GN_ERROR(logger)("Artifact::content: stored='{}' requested='{}'", e->typeInfo().name, T::TYPE_INFO().name);
            return {};
        }
        return GN::referenceTo(typed);
    }

    /// Get the latest published version. Zero means nothing has been published yet.
    virtual NeverOverflowingCounter version() const = 0;

    /// Publish non-empty content and return the new version. Empty content is rejected and returns zero.
    virtual NeverOverflowingCounter publish(AutoRef<Entity> content) = 0;

    /// Wait for \p version to be published and return the content snapshot that satisfied the wait.
    ///
    /// Passing OOO() waits for the next publish relative to the version observed inside this call.
    virtual Snapshot wait(NeverOverflowingCounter version, std::chrono::milliseconds timeout = std::chrono::milliseconds::max()) const = 0;

protected:
    using Entity::Entity;
};
using ArtifactPtr = AutoRef<Artifact>;

/// Strongly typed view over an artifact. Publishes are limited to T or derived content types.
template<typename T>
struct TypedArtifact {
    static_assert(std::is_base_of_v<Entity, T>);

    using Content = T;

    ArtifactPtr artifact;

    struct Snapshot {
        Artifact::WaitStatus    status  = Artifact::WaitStatus::BUSY;
        NeverOverflowingCounter version = NeverOverflowingCounter::OOO();
        AutoRef<T>              content = {};
    };

    TypedArtifact() = default;

    explicit TypedArtifact(ArtifactPtr artifact_): artifact(std::move(artifact_)) {}

    static TypedArtifact create(const StrA & name = StrA::EMPTYSTR()) { return TypedArtifact(Artifact::create(name)); }

    bool empty() const { return !artifact; }

    AutoRef<T> content() const { return artifact ? artifact->content<T>() : AutoRef<T>(); }

    NeverOverflowingCounter version() const { return artifact ? artifact->version() : NeverOverflowingCounter::OOO(); }

    template<typename PUBLISHED>
    NeverOverflowingCounter publish(AutoRef<PUBLISHED> content) const {
        static_assert(std::is_base_of_v<T, PUBLISHED>);
        if (!artifact) GN_UNLIKELY return NeverOverflowingCounter::OOO();
        return artifact->publish(content);
    }

    Snapshot wait(NeverOverflowingCounter version, std::chrono::milliseconds timeout = std::chrono::milliseconds::max()) const {
        if (!artifact) GN_UNLIKELY return {};
        auto result = artifact->wait(version, timeout);
        return {result.status, result.version, RuntimeType::cast<T, Entity>(result.content)};
    }
};

} // namespace GN::rdg2
