#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <garnet/GNengine2.h>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>

using namespace GN::e2;

TEST_CASE("e2 arcball reset derives stable fitted camera", "[e2][navigation]") {
    ArcballCameraController controller;
    controller.resetToFit({-2, -1, -3}, {4, 5, 7}, 45.0f);
    CHECK(controller.pivot == glm::vec3(1, 2, 2));
    CHECK(controller.distance > glm::length(glm::vec3(6, 6, 10)) * 0.5f);
    CHECK(controller.minimumDistance > 0.0f);
    CHECK(controller.maximumDistance > controller.distance);
}

TEST_CASE("e2 arcball rotation preserves pivot radius", "[e2][navigation]") {
    ArcballCameraController controller;
    controller.distance    = 8.0f;
    const glm::vec3 before = controller.eyePosition() - controller.pivot;
    controller.rotate({120, -45}, 720);
    const glm::vec3 after = controller.eyePosition() - controller.pivot;
    CHECK(glm::abs(glm::length(after) - glm::length(before)) < 0.0001f);
    CHECK(glm::length(after - before) > 0.1f);
}

TEST_CASE("e2 arcball pan follows rotated camera image plane", "[e2][navigation]") {
    ArcballCameraController controller;
    controller.distance    = 10.0f;
    controller.orientation = glm::angleAxis(glm::half_pi<float>(), glm::vec3(0, 1, 0));
    controller.pan({100, 50}, 1000, 45.0f);
    CHECK(controller.pivot.x == Catch::Approx(0.0f).margin(0.0001f));
    CHECK(controller.pivot.y > 0.0f);
    CHECK(controller.pivot.z > 0.0f);
}

TEST_CASE("e2 arcball zoom is exponential symmetric and clamped", "[e2][navigation]") {
    ArcballCameraController controller;
    controller.distance        = 10.0f;
    controller.minimumDistance = 2.0f;
    controller.maximumDistance = 20.0f;
    controller.zoom(3.0f);
    controller.zoom(-3.0f);
    CHECK(controller.distance == Catch::Approx(10.0f));
    controller.zoom(100.0f);
    CHECK(controller.distance == 2.0f);
    controller.zoom(-100.0f);
    CHECK(controller.distance == 20.0f);
}

TEST_CASE("e2 fly camera mouse look rotates around unchanged eye", "[e2][navigation]") {
    FlyCameraController controller;
    controller.position = {3, 4, 5};
    controller.rotate({180, 0}, 720);
    CHECK(controller.position == glm::vec3(3, 4, 5));
    const glm::vec3 forward = controller.orientation * glm::vec3(0, 0, -1);
    CHECK(forward.x > 0.5f);
    CHECK(forward.z < 0.0f);
}

TEST_CASE("e2 fly camera movement is local time-scaled and diagonal-normalized", "[e2][navigation]") {
    FlyCameraController controller;
    controller.movementSpeed = 4.0f;
    controller.move({1, 0, 1}, 0.5f);
    CHECK(glm::length(controller.position) == Catch::Approx(2.0f));
    CHECK(controller.position.x > 0.0f);
    CHECK(controller.position.z < 0.0f);

    controller.position    = glm::vec3(0);
    controller.orientation = glm::angleAxis(glm::half_pi<float>(), glm::vec3(0, 1, 0));
    controller.move({0, 0, 1}, 0.25f);
    CHECK(controller.position.x == Catch::Approx(-1.0f));
    CHECK(controller.position.z == Catch::Approx(0.0f).margin(0.0001f));
}
