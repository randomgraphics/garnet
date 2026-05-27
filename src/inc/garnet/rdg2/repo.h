#if !defined(__GN_INSIDE_RDG2_H__)
    #error "Do not include <garnet/rdg2/repo.h> directly. Include <garnet/GNrdg2.h> instead."
#endif

namespace GN::rdg2 {

/// 64-bit logical handle for a resource stored in a specific typed repository.
///
/// The repository id lets implementations reject handles created by another repo,
/// while the generation rejects stale handles after a slot is removed and reused.
/// The default value is invalid. This handle is strictly in-memory, so the bit-field
/// layout is intentionally not an ABI or serialization contract.
struct RepoHandle64 {
    union {
        uint64_t value = 0;
        struct {
            uint64_t index      : 28;
            uint64_t generation : 28;
            uint64_t repo       : 8; // unique ID of owing repo, assuming no more than 256
                                     // repo of same type in one process.
        } bits;
    };

    static constexpr RepoHandle64 INVALID() { return {}; }

    static constexpr RepoHandle64 make(uint8_t repo, uint32_t index, uint32_t generation) {
        RepoHandle64 handle;
        handle.bits.repo       = repo;
        handle.bits.index      = index;
        handle.bits.generation = generation;
        return handle;
    }

    constexpr uint8_t  repo() const { return uint8_t(bits.repo); }
    constexpr uint32_t index() const { return uint32_t(bits.index); }
    constexpr uint32_t generation() const { return uint32_t(bits.generation); }
    constexpr bool     empty() const { return repo() == 0 || index() == 0; }
    constexpr bool     operator==(const RepoHandle64 & other) const { return value == other.value; }
    constexpr bool     operator!=(const RepoHandle64 & other) const { return value != other.value; }
    constexpr bool     operator<(const RepoHandle64 & other) const { return value < other.value; }
};

/// Uniquely determine the content of the resource. Can be extended by subclass.
struct RepoKey {
    /// is the canonical source identity, such as a resolved media path.
    StrA uri = {};

    /// represents load options that affect the content of the loaded object.
    uint64_t variantHash = 0;

    /// Make it usable in associative containers.
    bool operator<(const RepoKey & other) const {
        if (uri != other.uri) return uri < other.uri;
        return variantHash < other.variantHash;
    }
};

/// Parameters for loading or looking up a resource.
template<typename CONTENT, typename KEY = RepoKey>
struct RepoLoadDesc {
    typedef CONTENT Content;
    typedef KEY     Key;

    /// This is the argument that will be parsed to loader internally to load the resource.
    Key key = {};

    /// If loading failed, this is the one we'll fallback too.
    AutoRef<CONTENT> fallback = {};

    /// Set to true to deferred the actual loading action into a graph node.
    /// If false, then the repo's load method will try loading the resource
    /// directly inside the method body.
    bool deferred = true;

    /// Set to true to automatically reload and republish the resource.
    /// \todo reloading is not implemented yet.
    bool hotReload = true;
};

/// Shared interface for one typed RDG2 resource repository.
///
/// Implementations should provide O(1) average lookup by name and O(1) handle
/// lookup, loads by canonical key, publish fallback content promptly, and
/// publish new artifact versions when resources are hot-reloaded.
template<typename HANDLE, typename LOAD_DESC>
struct Repo : Entity {
    GN_REGISTER_RUNTIME_TYPE(Entity);

    typedef HANDLE                  Handle;
    typedef LOAD_DESC               LoadDesc;
    typedef typename LOAD_DESC::Key Key;

    struct Ref {
        Handle      handle;
        ArtifactPtr artifact;
    };

    /// Find a logical resource by key. Returns an empty ref when absent.
    virtual Ref find(const Key & key) const = 0;

    /// Load or return the existing logical resource for the descriptor.
    virtual Ref load(const LoadDesc & desc) = 0;

    /// Request an explicit reload. Implementations may schedule the work asynchronously.
    /// \todo not implemented yet.
    virtual bool reload(Handle handle) = 0;

    /// Poll source changes and schedule reloads for stale hot-reload resources.
    /// \todo not implemented yet.
    virtual void pollHotReload() = 0;

    /// Remove a logical resource from the repository.
    virtual bool remove(Handle handle) = 0;

    /// Number of live logical resources.
    virtual size_t size() const = 0;

protected:
    using Entity::Entity;
};

/// Base class for GPU resource content attached to a managed repository resource.
template<typename T>
struct RepoContent : Entity {
    GN_REGISTER_RUNTIME_TYPE(Entity);

    typedef T Value;

    T                                         value;
    ArrayContainer<AutoRef<gpu2::GpuPayload>> gpuWorks;

    RepoContent(const RuntimeType::TypeInfo & type, const StrA & name, T value_ = {}): Entity(type, name), value(std::move(value_)) {}
};

struct TextureResource {
    // Declare unique type to prevent misuse of handles of other resource.
    struct Handle : RepoHandle64 {};

    struct Key : RepoKey {
        // Can add more fields, if needed.

        // must provide less operator
        using RepoKey::operator<;
    };

    struct Content : RepoContent<AutoRef<gpu2::Texture>> {
        Content(const StrA & name, AutoRef<gpu2::Texture> value_ = {}): RepoContent(TYPE_INFO(), name, std::move(value_)) {}
    };

    struct LoadDesc : RepoLoadDesc<Content, Key> {
        // Can add more fields, if needed.
    };

    using RepoBase = Repo<Handle, LoadDesc>;

    using Ref = Repo<Handle, LoadDesc>::Ref;

    struct Repo : RepoBase {
        GN_REGISTER_RUNTIME_TYPE(RepoBase);

        AutoRef<Content> getContent(const Ref & ref) const {
            if (!ref.artifact) GN_UNLIKELY return {};
            return ref.artifact->content<Content>();
        }

        /// Parameters for creating a texture repository.
        struct CreateParameters {
            AutoRef<gpu2::GpuContext> gpu;
            GraphPtr                  graph;
        };

        /// Create a texture repository.
        GN_API static AutoRef<Repo> create(const StrA & name, const CreateParameters &);

    protected:
        using RepoBase::RepoBase;
    };
};

struct GpuGeometryResource {
    // Declare unique type to prevent misuse of handles of other resource.
    struct Handle : RepoHandle64 {};

    struct Content : RepoContent<gpu2::RasterGeometry> {
        Content(const StrA & name, gpu2::RasterGeometry value_ = {}): RepoContent(TYPE_INFO(), name, std::move(value_)) {}
    };

    struct LoadDesc : RepoLoadDesc<Content> {
        // Can add more fields, if needed.
    };

    using RepoBase = Repo<Handle, LoadDesc>;

    using Ref = RepoBase::Ref;

    /// Repository for GPU renderable geometry.
    struct Repo : RepoBase {
        GN_REGISTER_RUNTIME_TYPE(RepoBase);

        AutoRef<Content> getContent(const Ref & ref) const {
            if (!ref.artifact) GN_UNLIKELY return {};
            return ref.artifact->content<Content>();
        }

        /// Parameters for creating a geometry repository.
        struct CreateParameters {
            AutoRef<gpu2::GpuContext> gpu;
            GraphPtr                  graph;
        };

        /// Create a geometry repository.
        GN_API static AutoRef<Repo> create(const CreateParameters &);

    protected:
        Repo(const RuntimeType::TypeInfo & type, const StrA & name, Graph &): RepoBase(type, name) {}
    };
};

struct ShaderResource {
    // Declare unique type to prevent misuse of handles of other resource.
    struct Handle : RepoHandle64 {};

    struct Content : RepoContent<AutoRef<gpu2::GpuShader>> {
        Content(const StrA & name, AutoRef<gpu2::GpuShader> value_ = {}): RepoContent(TYPE_INFO(), name, std::move(value_)) {}
    };

    struct LoadDesc : RepoLoadDesc<Content> {
        // Can add more fields, if needed.
    };

    using RepoBase = Repo<Handle, LoadDesc>;

    using Ref = RepoBase::Ref;

    /// Repository for GPU shaders.
    struct Repo : RepoBase {
        GN_REGISTER_RUNTIME_TYPE(RepoBase);

        AutoRef<Content> getContent(const Ref & ref) const {
            if (!ref.artifact) GN_UNLIKELY return {};
            return ref.artifact->content<Content>();
        }

        /// Parameters for creating a shader repository.
        struct CreateParameters {
            AutoRef<gpu2::GpuContext> gpu;
            GraphPtr                  graph;
        };

        /// Create a shader repository.
        GN_API static AutoRef<Repo> create(const CreateParameters &);

    protected:
        Repo(const RuntimeType::TypeInfo & type, const StrA & name, Graph &): RepoBase(type, name) {}
    };
};

} // namespace GN::rdg2
