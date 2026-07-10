#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/simple.h> directly. Include <garnet/GNengine2.h> instead."
#endif

/// The Simple namespace provides a minimal concrete world used to exercise the engine2
/// world/visual-moment workflow: a world that evolves on its own cadence and can be
/// snapshotted into a VisualMoment, plus a couple of trivial form types (a box mesh and a
/// point light). It is intentionally tiny; the rendering is performed by the official
/// VisualDomain implementation, not by anything Simple-specific.
namespace GN::e2::Simple {

/// Create an empty simple world. Call World::run() to start its background simulation.
/// \p metersPerUnit defines the physical size of one WorldLength unit in this world.
GN_API Ref<World> createWorld(Universe & universe, double metersPerUnit = 1.0);

/// Create a solid box form, centered at \p position, sized \p dimensions. It slowly spins
/// while the world runs so that motion is visible across captured visual moments.
GN_API Ref<Form> createBox(Universe & universe, const WorldVector3 & position, const WorldVector3 & dimensions);

/// Create a point-light form at \p position emitting light of the given color/intensity.
GN_API Ref<Form> createPointLight(Universe & universe, const WorldVector3 & position, const IntensityRGB & color);

} // namespace GN::e2::Simple