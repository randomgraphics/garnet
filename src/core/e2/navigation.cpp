#include <garnet/GNengine2.h>

#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>

namespace GN::e2 {

void ArcballCameraController::resetToFit(const glm::vec3 & minimum, const glm::vec3 & maximum, float verticalFovDegrees) {
    pivot               = (minimum + maximum) * 0.5f;
    orientation         = glm::quat(1, 0, 0, 0);
    const float radius  = glm::max(glm::length(maximum - minimum) * 0.5f, 0.001f);
    const float halfFov = glm::radians(glm::clamp(verticalFovDegrees, 1.0f, 179.0f)) * 0.5f;
    distance            = radius / glm::tan(halfFov);
    minimumDistance     = radius * 0.01f;
    maximumDistance     = radius * 1000.0f;
}

void ArcballCameraController::rotate(const glm::vec2 & pixelDelta, float viewportHeight) {
    if (viewportHeight <= 0.0f) return;
    const float radiansPerPixel = glm::pi<float>() / viewportHeight;
    const auto  yaw             = glm::angleAxis(-pixelDelta.x * radiansPerPixel, glm::vec3(0, 1, 0));
    const auto  pitch           = glm::angleAxis(-pixelDelta.y * radiansPerPixel, glm::vec3(1, 0, 0));
    orientation                 = glm::normalize(yaw * orientation * pitch);
}

void ArcballCameraController::pan(const glm::vec2 & pixelDelta, float viewportHeight, float verticalFovDegrees) {
    if (viewportHeight <= 0.0f) return;
    const float halfFov       = glm::radians(glm::clamp(verticalFovDegrees, 1.0f, 179.0f)) * 0.5f;
    const float unitsPerPixel = 2.0f * distance * glm::tan(halfFov) / viewportHeight;
    pivot += orientation * glm::vec3(-pixelDelta.x * unitsPerPixel, pixelDelta.y * unitsPerPixel, 0.0f);
}

void ArcballCameraController::zoom(float wheelDelta, float sensitivity) {
    distance = glm::clamp(distance * glm::exp(-wheelDelta * sensitivity), minimumDistance, maximumDistance);
}

glm::vec3 ArcballCameraController::eyePosition() const { return pivot + orientation * glm::vec3(0, 0, distance); }

void FlyCameraController::rotate(const glm::vec2 & pixelDelta, float viewportHeight) {
    if (viewportHeight <= 0.0f) return;
    const float radiansPerPixel = glm::pi<float>() / viewportHeight;
    const auto  yaw             = glm::angleAxis(-pixelDelta.x * radiansPerPixel, glm::vec3(0, 1, 0));
    const auto  pitch           = glm::angleAxis(-pixelDelta.y * radiansPerPixel, glm::vec3(1, 0, 0));
    orientation                 = glm::normalize(yaw * orientation * pitch);
}

void FlyCameraController::move(const glm::vec3 & localDirection, float elapsedSeconds) {
    const float length = glm::length(localDirection);
    if (length <= 0.0f || elapsedSeconds <= 0.0f) return;
    const glm::vec3 cameraMotion(localDirection.x, localDirection.y, -localDirection.z);
    position += orientation * (cameraMotion / length) * movementSpeed * elapsedSeconds;
}

} // namespace GN::e2
