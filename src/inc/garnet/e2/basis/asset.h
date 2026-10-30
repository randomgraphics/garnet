#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/basis/asset.h> directly. Include <garnet/GNengine2.h> instead."
#endif

namespace GN::e2::basis {

/// Retains registered GPU meshes/textures; create directly or use Visual::assets().
/// A GPU-backed service initializes MESH_BOX before returning from create().
struct Assets : Being {
    GN_E2_DEFINE_A_BEING(Being);

    /// Visual mesh asset backed by GPU raster geometry.
    struct MeshIdType;
    using MeshId = MeshIdType *;
    struct Mesh : RefCounter, RuntimeType {
        GN_API GN_REGISTER_RUNTIME_TYPE();

        const MeshId id;   ///< Asset-service-local handle; valid while the service retains this asset.
        const StrA   name; ///< Optional display name; not required to be unique.

    protected:
        Mesh(const RuntimeType::TypeInfo & type, MeshId id_, const StrA & name_): RuntimeType(type), id(id_), name(name_) {}

    public:
        virtual const gpu2::RasterGeometry & geometry() const = 0;
    };

    /// Visual texture asset backed by a GPU texture.
    struct TextureIdType;
    using TextureId = TextureIdType *;
    struct Texture : RefCounter, RuntimeType {
        GN_API GN_REGISTER_RUNTIME_TYPE();

        const TextureId id;   ///< Asset-service-local handle; valid while the service retains this asset.
        const StrA      name; ///< Optional display name; not required to be unique.

    protected:
        Texture(const RuntimeType::TypeInfo & type, TextureId id_, const StrA & name_): RuntimeType(type), id(id_), name(name_) {}

    public:
        virtual AutoRef<gpu2::Texture> texture() const = 0;
    };

    struct CreateParameters {
        Universe &                universe;
        AutoRef<gpu2::GpuContext> gpu;
    };

    /// Well-known mesh IDs. MESH_SPHERE is reserved and is not currently populated.
    static inline const MeshId MESH_BOX    = (MeshId) (uintptr_t) 1;
    static inline const MeshId MESH_SPHERE = (MeshId) (uintptr_t) 2;

    /// Register a custom mesh with the asset service.
    virtual Ref<Mesh> registerMesh(const StrA & name, const gpu2::RasterGeometry & mesh) = 0;

    /// Retrieve a registered mesh, or null if not found.
    virtual Ref<Mesh> findMesh(MeshId) const = 0;

    /// Register a GPU texture with the asset service.
    virtual Ref<Texture> registerTexture(const StrA & name, AutoRef<gpu2::Texture> texture) = 0;

    /// Retrieve a registered texture, or null if not found.
    virtual Ref<Texture> findTexture(TextureId) const = 0;

    GN_API static Ref<Assets> create(const CreateParameters &);
};

} // namespace GN::e2::basis
