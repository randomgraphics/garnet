#ifndef __GN_ENGINE2_GNENGINE2_H__
#define __GN_ENGINE2_GNENGINE2_H__
// *****************************************************************************
/// \file
/// \brief   main interface of engine v2
/// \author  CHEN@@CHENLI-HOMEPC (2015.4.8)
// *****************************************************************************

#include "GNgpu2.h"

#include <glm/gtc/quaternion.hpp>        // glm::quat, glm::mat4_cast, glm::angleAxis
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#define GN_E2_DEFINE_A_THING(baseType) protected: using baseType::baseType; public: GN_API GN_REGISTER_RUNTIME_TYPE(baseType);

namespace GN::e2 {

template<typename T>
using ArrayBody = DynaArray<T>;

template<typename T>
using ArrayView = SafeArrayAccessor<T>;

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

/// Position is defined as 64-bit signed integer to support large open world.
/// 1 unit = 1 um (micrometer) for better precision.
struct Position {
    UnitOfLength x, y, z;
};

// struct Transformation {
//     glm::vec3 translation;
//     glm::quat rotation;
//     glm::vec3 scaling;
// };

typedef int64_t UniqueIdentifier;

struct Universe;

/// @brief The very basic/root type of everything in this world.
struct Thing : RefCounter, RuntimeType {
    GN_API GN_REGISTER_RUNTIME_TYPE();

    const UniqueIdentifier id;
    const StrA             name;

protected:
    Thing(const RuntimeType::TypeInfo & type, UniqueIdentifier id_, const StrA & name_) : RuntimeType(type), id(id_), name(name_) {}
};

// class SpacialShard : public Thing {
//     GN_E2_DEFINE_A_THING(Thing);

//     // spacial properties
//     WeakRef<Entity> _spacialParent;
//     Position        _worldPosition; // only when _spacialParent is empty.
//     Spheref         _boundingSphere;
//     Transformation  _transformation; // local to parent transformation
// };

// class VisualShard : Thing {
// public:
//     virtual void RenderPass0();
//     virtual void RenderPass1();
//     virtual void RenderPass2();
// };

// // Camera object
// class Camera : public VisualEntity {};

// // The geometry object that never changes
// class ImmutableGeometry : public VisualEntity {};

// class SceneGraph {
// public:
//     void AddEntity(SpacialEntity *);
//     void RemoveEntity(uint64_t id);
// };

// class AsyncNofitication : RefCounter {
// public:
//     enum Status {
//         PENDING = -2,
//         ABORT   = -1,
//         FAILED  = 0,
//         DONE    = 1,
//     };

//     void   Notify(OperationResult);
//     Status Pool();
//     void   WaitForFinish();
// };

/// @brief Prepresents an visual observer of the world.
struct Camera : Thing {
    GN_E2_DEFINE_A_THING(Thing);
};

/// A visual snapshot of something. consumed by graphics domain for graphics rendering.
struct VisualMoment : Thing {
    GN_E2_DEFINE_A_THING(Thing);
};

/// An audio snapshot of something, consumed by audio domain to play sound & music.
struct AudioMoment : Thing {
    GN_E2_DEFINE_A_THING(Thing);
};

/// The main class that represents a presence in the world that could affect the state of the world and could
/// interact with other forms.
struct Form : Thing {
    GN_E2_DEFINE_A_THING(Thing);

    /// update this form's internal state. called by the world, usually with an fixed interval.
    virtual bool update() = 0;

    /// update the visual part of the form.
    virtual AutoRef<VisualMoment> captureVisualMoment() = 0;

    /// update the audio part of the form.
    virtual AutoRef<AudioMoment> captureAudioMoment() = 0;
};

/// This represents a continuously evolving game world with diffent form of things living in it.
struct World : Thing {
    GN_E2_DEFINE_A_THING(Thing);

    struct CreateParameters {
        Universe & universe;
    };

    virtual void populate(ArrayView<AutoRef<Form>>) = 0;

    virtual void run() = 0;

    virtual AutoRef<VisualMoment> captureVisualMoment() = 0;

    GN_API AutoRef<World> create(const CreateParameters &);
};

// This is the single largest container of everything that is always initialized first
// and is always available for the entire life time of the engine.
struct Universe {
    GN_NO_COPY(Universe);
    GN_NO_MOVE(Universe);

    UniqueIdentifier generateUniqueIdentifier() {
        auto old = mNextID.load(std::memory_order_relaxed);
        for (;;) {
            auto next = old + 1;
            if (next == 0) next = 1;
            if (mNextID.compare_exchange_weak(old, next, std::memory_order_relaxed)) {
                return next;
            }
        }
    };
protected:
    Universe() = default;
    virtual ~Universe() = default;

private:
    std::atomic<UniqueIdentifier> mNextID = {};
};

} // namespace GN::e2

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE2_GNENGINE2_H__
