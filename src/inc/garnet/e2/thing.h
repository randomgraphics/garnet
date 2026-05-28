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

/// Strongly-typed unit of length used in this world. Defined here for future
/// use; current aliases (Location, Distance) use plain float meters for simplicity.
struct UnitOfLength {
    int64_t _value = 0;

    constexpr UnitOfLength() = default;

    explicit constexpr UnitOfLength(int64_t value): _value(value) {}

    constexpr UnitOfLength operator+(UnitOfLength o) const { return UnitOfLength(_value + o._value); }
    constexpr UnitOfLength operator-(UnitOfLength o) const { return UnitOfLength(_value - o._value); }
    constexpr UnitOfLength operator*(UnitOfLength o) const { return UnitOfLength(_value * o._value); }
    constexpr UnitOfLength operator/(UnitOfLength o) const { return UnitOfLength(_value / o._value); }
};

/// Spacial location defined by 3D coordinate within the world.
using WorldPosition = Vector3<UnitOfLength>;

// Spacial orientation defined by a rotation primitive.
using Rotation = glm::quat;

// struct Transformation {
//     glm::vec3 translation;
//     glm::quat rotation;
//     glm::vec3 scaling;
// };

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

/// Smart reference to a thing.
template<typename T>
using Ref = AutoRef<T>;

/// Abstraction of host operating system, providing cross platform abstraction to Window, keyboard, mouse support.
struct OperatingDomain : Thing {
    GN_E2_DEFINE_A_THING(Thing);

    GN_API Ref<OperatingDomain> create();
};

} // namespace GN::e2
