#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/navigation.h> directly. Include <garnet/GNengine2.h> instead."
#endif

#include <glm/gtc/quaternion.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace GN::e2 {

/// Scale-independent orbit/arcball camera state expressed in scene-local units.
/// Orientation maps camera-local axes to world axes; identity looks down world -Z.
struct ArcballCameraController {
    glm::vec3 pivot      = glm::vec3(0);
    glm::quat orientation = glm::quat(1, 0, 0, 0);
    float     distance    = 1.0f;
    float     minimumDistance = 0.001f;
    float     maximumDistance = 1000.0f;

    /// Reset to the center of bounds and derive distance/clamps from its diagonal radius.
    GN_API void resetToFit(const glm::vec3 & minimum, const glm::vec3 & maximum, float verticalFovDegrees = 45.0f);

    /// Orbit around pivot from a pointer delta in pixels.
    GN_API void rotate(const glm::vec2 & pixelDelta, float viewportHeight);

    /// Pan eye and pivot in the camera image plane from a pointer delta in pixels.
    GN_API void pan(const glm::vec2 & pixelDelta, float viewportHeight, float verticalFovDegrees);

    /// Apply signed wheel motion using exponential distance scaling and configured clamps.
    GN_API void zoom(float wheelDelta, float sensitivity = 0.12f);

    /// Return eye position implied by pivot, orientation, and distance.
    GN_API glm::vec3 eyePosition() const;
};

} // namespace GN::e2
