#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/thing.h> directly. Include <garnet/GNengine2.h> instead."
#endif

namespace GN::e2::Simple {

GN_API Ref<World> createWorld(Universe & universe);

GN_API Ref<Form> createBox(Universe & universe, const Vector3<UnitOfLength> & dimensions);

GN_API Ref<Form> createPointLight(Universe & universe, const IntensityRGB & color);

} // namespace GN::e2::Simple