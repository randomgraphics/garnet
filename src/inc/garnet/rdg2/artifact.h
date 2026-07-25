#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg2/artifact.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

#include <cstddef>
#include <cstdint>
#include <chrono>
#include <limits>
#include <type_traits>
#include <utility>
#include <variant>

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
// Array container and proxy
// ============================================================

template<typename T>
using ArrayContainer = DynaArray<T, size_t>;

template<typename T>
using ArrayProxy = SafeArrayAccessor<T>;

// ============================================================
// Base class of everything with a ID and name.
// ============================================================

} // namespace GN::rdg2

/// Make NeverOverflowingCounter formattable by fmt (and thus by GN logging, e.g. the
/// destruction trace in GN::RefCountedRuntimeType).
template<>
struct fmt::formatter<GN::rdg2::NeverOverflowingCounter> {
    constexpr auto parse(fmt::format_parse_context & ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const GN::rdg2::NeverOverflowingCounter & c, FormatContext & ctx) const {
        if (c.value1) return fmt::format_to(ctx.out(), "{}.{}", c.value1, c.value0);
        return fmt::format_to(ctx.out(), "{}", c.value0);
    }
};

namespace GN::rdg2 {

/// The basic building block of the render graph module. Base class of everything that
/// needs reference counting and runtime type information. Ids are process-unique 128-bit
/// counters.
struct Entity : public GN::RefCountedRuntimeType<NeverOverflowingCounter> {
    GN_API GN_REGISTER_RUNTIME_TYPE();

protected:
    /// Expose the caller-supplied-id constructor to derived classes.
    using RefCountedRuntimeType<NeverOverflowingCounter>::RefCountedRuntimeType;

    /// Convenience constructor that automatically generates a unique id for this entity.
    /// Implemented inside GNcore (artifact.cpp) so the generated id is truly process-unique,
    /// even across DLL boundaries. The first id is 1; 0 (OOO) is never assigned and can serve
    /// as an "invalid/unassigned" sentinel.
    GN_API Entity(const RuntimeType::TypeInfo & type, const StrA & name);
};

/// Versioned payload slot independent of any graph implementation.
struct Artifact : public Entity {
    GN_API GN_REGISTER_RUNTIME_TYPE(Entity);

    typedef NeverOverflowingCounter Version;

    /// Result of waiting on an artifact version.
    enum class WaitStatus : uint8_t {
        READY, /// The requested version has been published.
        BUSY,  /// The requested version was not reached before the timeout.
    };

    template<typename C = Entity>
    struct Content {
        // Value of the content.
        AutoRef<C> value = {};

        // The actual version of the content.
        Version version = Version::OOO();

        bool empty() const { return value.empty() || version == Version::OOO(); }
        void clear() {
            value.clear();
            version = Version::OOO();
        }
        explicit operator bool() const { return !empty(); }
        bool     operator!() const { return empty(); }
        operator AutoRef<C>() const { return value; }
        C * operator->() const { return value.get(); }
    };

    /// Content captured atomically with the actual retained content version.
    struct Snapshot {
        WaitStatus status = WaitStatus::BUSY;

        /// Actual version of the returned content. This may be newer than the version passed to wait().
        Version version = Version::OOO();

        /// Content returned by the wait.
        AutoRef<Entity> content = {};
    };

    struct SubCollection {
        ArrayContainer<std::variant<AutoRef<Artifact>, SubCollection>> elements;

        bool empty() const { return elements.empty(); }

        // SubCollection & append(SubArray && a) {
        //     if (!a.empty()) elements.emplace_back(std::move(a));
        //     return *this;
        // }

        SubCollection & append(SubCollection && c) {
            if (!c.empty()) elements.append(std::move(c));
            return *this;
        }

        template<typename ARTIFACT_TYPE>
        SubCollection & append(const AutoRef<ARTIFACT_TYPE> & a) {
            static_assert(std::is_base_of_v<Artifact, ARTIFACT_TYPE>);
            if (!a) return *this;

            auto s = a->sub();
            if (s.empty()) {
                elements.append(AutoRef<Artifact>(a));
            } else {
                append(std::move(s));
            }
            return *this;
        }

        /// @brief Iterate through all sub artifacts, recursively.
        template<typename PROC>
        void forEach(PROC proc) const {
            for (const auto & e : elements) {
                auto a = std::get_if<AutoRef<Artifact>>(&e);
                if (a) {
                    if (*a) proc(*a);
                } else {
                    auto c = std::get_if<SubCollection>(&e);
                    GN_ASSERT(c);
                    c->forEach(proc);
                }
            }
        }
    };

    /// Fired whenever a new version is published.
    Signal<void(const Content<> &)> sigPublished;

    /// Get collection of sub artifacts.
    virtual SubCollection sub() const { return {}; };

    /// Get the latest published content. Could be empty if the artifact has not been published.
    virtual Content<> content() const = 0;

    template<typename T>
    Content<T> content() const {
        auto e = content();
        if (e.empty()) {
            static auto * logger = GN::getLogger("GN.rdg2");
            GN_ERROR(logger)("Artifact::content: artifact content is empty");
            return {};
        };
        GN_ASSERT(e.value);
        auto typed = RuntimeType::cast<T>(e.value.get());
        if (!typed) {
            static auto * logger = GN::getLogger("GN.rdg2");
            GN_ERROR(logger)("Artifact::content: stored='{}' requested='{}'", e.value->typeInfo().name, T::TYPE_INFO().name);
            return {};
        }
        return {GN::referenceTo(typed), e.version};
    }

    /// Get the latest published version. Zero means nothing has been published yet.
    virtual Version version() const = 0;

    /// Publish non-empty content and return the new version. Empty content is rejected and returns zero.
    virtual Version publish(AutoRef<Entity> content) = 0;

    /// Wait for \p version to be published and return the content snapshot that satisfied the wait.
    /// Passing OOO() waits for the next publish relative to the version observed inside this call.
    virtual Snapshot wait(Version version, std::chrono::milliseconds timeout = std::chrono::milliseconds::max()) const = 0;

    /// Create a standalone artifact with no content and version equal to 0.
    GN_API static AutoRef<Artifact> create(const StrA & name = StrA::EMPTYSTR());

protected:
    using Entity::Entity;
};
using ArtifactRef      = AutoRef<Artifact>;
using ConstArtifactRef = AutoRef<const Artifact>;

/// Strongly typed view over an artifact. Publishes are limited to T or derived content types.
template<typename T, typename A = Artifact>
struct TypedArtifact {
    static_assert(std::is_base_of_v<Entity, T>);

    using Content = T;
    using Ref     = AutoRef<A>;

    Ref artifact;

    struct Snapshot {
        Artifact::WaitStatus    status  = Artifact::WaitStatus::BUSY;
        NeverOverflowingCounter version = NeverOverflowingCounter::OOO();
        AutoRef<T>              content = {};
    };

    TypedArtifact() = default;

    explicit TypedArtifact(Ref artifact_): artifact(std::move(artifact_)) {}

    bool empty() const { return !artifact; }

    Artifact::Content<T> content() const {
        if (artifact)
            GN_LIKELY return artifact->template content<T>();
        else
            return {};
    }

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
