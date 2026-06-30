#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/thing.h> directly. Include <garnet/GNengine2.h> instead."
#endif

#include <chrono>
#include <type_traits>
#include <concepts>

#include <glm/gtc/quaternion.hpp> // glm::quat, glm::mat4_cast, glm::angleAxis
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#define GN_E2_DEFINE_A_THING(baseType) \
protected:                             \
    using baseType::baseType;          \
                                       \
public:                                \
    GN_API GN_REGISTER_RUNTIME_TYPE(baseType);

namespace GN::e2 {

template<typename T>
using ArrayBody = DynaArray<T>;

template<typename T>
using ArrayView = SafeArrayAccessor<T>;

/// Unit of time used in by E2
using UnitOfTime = std::chrono::nanoseconds;

typedef int64_t UniqueIdentifier;

struct Universe;

/// The very basic/root type of everything in this world.
struct Thing : RefCounter, RuntimeType {
    GN_API GN_REGISTER_RUNTIME_TYPE();

    const UniqueIdentifier id;
    const StrA             name;

protected:
    Thing(const RuntimeType::TypeInfo & type, UniqueIdentifier id_, const StrA & name_): RuntimeType(type), id(id_), name(name_) {}
};

namespace detail {

template<typename T>
concept CompleteType = requires { sizeof(T); };

// Ref<T> appears in declarations that mention the class currently being defined
// and forward-declared E2 types, so derivation can only be checked for complete types.
template<typename T>
concept ThingRefTarget = (!CompleteType<T>) || std::derived_from<T, Thing>;

} // namespace detail

/// Smart reference to a thing.
template<detail::ThingRefTarget T>
using Ref = AutoRef<T>;

/// Abstraction of host operating system, providing cross platform abstraction to Window, keyboard, mouse support.
struct OperatingDomain : Thing {
    GN_E2_DEFINE_A_THING(Thing);

    struct CreateParameters {
        Universe & universe;
        StrA       caption = "Garnet engine2";
        uint32_t   width   = 1280; ///< initial client width in pixels. 0 = pick a platform default.
        uint32_t   height  = 720;  ///< initial client height in pixels. 0 = pick a platform default.
    };

    /// Create the operating-system domain (main window + event pump). Returns null on failure.
    GN_API static Ref<OperatingDomain> create(const CreateParameters &);

    /// Native render-surface handle for the given graphics-API instance handle (e.g. a
    /// VkSurfaceKHR built from a VkInstance, both passed/returned as intptr_t). Returns 0 if
    /// unsupported. The domain owns the surface; the caller must not destroy it.
    virtual intptr_t renderSurfaceHandle(intptr_t graphicsInstanceHandle) const = 0;

    /// Current client-area size of the main window, in pixels.
    virtual Vector2<uint32_t> clientSize() const = 0;

    /// Pump the OS event queue once. Returns false when the user has requested the
    /// application to quit (e.g. the main window was closed).
    virtual bool processEvents() = 0;
};

} // namespace GN::e2
