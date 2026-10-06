#include <catch2/catch_test_macros.hpp>
#include <garnet/GNengine2.h>

using namespace GN;
using namespace GN::e2;

TEST_CASE("E2 visual renders lit white geometry from a data tableau", "[e2][visual][gpu]") {
    Universe universe;
    auto     visual = basis::Visual::create({.universe = universe, .platform = {}});
    if (!visual) SKIP("No Vulkan GPU available");
    basis::Visual::Tableau tableau;
    tableau.scale            = PhysicalScale::MICROMETER();
    tableau.camera.position  = positionFromMeters({0, 0, 5}, tableau.scale);
    tableau.camera.nearPlane = positionFromMeters(glm::dvec3(0.1), tableau.scale).x;
    tableau.camera.farPlane  = positionFromMeters(glm::dvec3(100), tableau.scale).x;
    tableau.clearColor       = {{0, 0, 0, 1}};
    basis::Visual::Object object;
    object.meshId                = basis::Assets::MESH_BOX;
    object.transform.orientation = glm::quat(glm::vec3(0.3f, 0.5f, 0.f));
    tableau.objects.append(object);
    visual->renderFrame(tableau);
    auto image = visual->readbackFrame();
    REQUIRE_FALSE(image.empty());
    // Inspect encoded bytes: rapid-image does not convert SRGB planes.
    auto plane         = image.plane();
    plane.format       = gfx::img::PixelFormat::RGBA8();
    const auto pixels  = plane.toRGBA8(image.data());
    size_t     visible = 0;
    bool       neutral = true;
    unsigned   darkest = 255, brightest = 0;
    for (const auto & pixel : pixels) {
        if (pixel.r > 30) {
            ++visible;
            darkest   = std::min(darkest, unsigned(pixel.r));
            brightest = std::max(brightest, unsigned(pixel.r));
        }
        neutral = neutral && pixel.r == pixel.g && pixel.g == pixel.b;
    }
    CHECK(visible > 1000);
    CHECK(neutral);
    CHECK(brightest > darkest + 20);
    if (const auto path = getEnv("GN_E2_TEST_SNAPSHOT"); !path.empty()) {
        gfx::img::Image output(gfx::img::ImageDesc {}.set2D(gfx::img::PixelFormat::RGBA8(), image.width(), image.height()), image.data(), image.size());
        output.save(path.data());
    }
    // Reusing the renderer with an empty snapshot must not retain the previous geometry.
    tableau.objects.clear();
    visual->renderFrame(tableau);
    image = visual->readbackFrame();
    REQUIRE_FALSE(image.empty());
    const auto cleared = plane.toRGBA8(image.data());
    bool       black   = true;
    for (const auto & pixel : cleared) black = black && pixel.r == 0;
    CHECK(black);
}
