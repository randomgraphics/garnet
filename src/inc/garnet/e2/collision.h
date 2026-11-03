#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Include <garnet/GNengine2.h> instead."
#endif

namespace GN::e2 {

/// Observational notification emitted by a collision Law when a body pair begins touching.
/// It carries stable Form identities and an approximate absolute-world contact point.
struct CollisionEvent : Event {
    GN_API GN_REGISTER_RUNTIME_TYPE(Event);
    CollisionEvent(FormId first = 0, FormId second = 0,
                   const WorldVector3 & position = {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO()})
        : Event(TYPE_INFO()), a(first), b(second), point(position) {}
    Ref<Event> clone() const override {
        if (typeInfo().id != TYPE_INFO().id) return {};
        return Ref<Event>(new CollisionEvent(*this));
    }
    FormId       a = 0, b = 0;
    WorldVector3 point {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO()};
};

} // namespace GN::e2
