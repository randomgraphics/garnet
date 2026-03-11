#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
// These includes prove Tier 2 is needed:
// - "rdg/vk-format-utils.h" is a private header, not on the public include path.
// - pixelFormatToVkFormat / vkFormatToPixelFormat are not exported from GNcore.dll/.so.
// Both the compile and link would fail from GNtest-unit-tests.
#include "rdg/vk-format-utils.h"
#include <garnet/GNrdg.h>

using namespace GN::rdg;
using PF = GN::gfx::img::PixelFormat;

TEST_CASE("vkFormatToPixelFormat: known formats", "[rdg][format]") {
    CHECK(vkFormatToPixelFormat(vk::Format::eR8G8B8A8Unorm) == PF::RGBA_8_8_8_8_UNORM());
    CHECK(vkFormatToPixelFormat(vk::Format::eR32Sfloat) == PF::R_32_FLOAT());
    CHECK(vkFormatToPixelFormat(vk::Format::eD24UnormS8Uint) == PF::RG_24_UNORM_8_UINT());
    CHECK(vkFormatToPixelFormat(vk::Format::eUndefined) == PF::UNKNOWN());
    CHECK(vkFormatToPixelFormat(static_cast<vk::Format>(9999)) == PF::UNKNOWN());
}

TEST_CASE("pixelFormatToVkFormat: known formats", "[rdg][format]") {
    CHECK(pixelFormatToVkFormat(PF::RGBA_8_8_8_8_UNORM()) == vk::Format::eR8G8B8A8Unorm);
    CHECK(pixelFormatToVkFormat(PF::R_32_FLOAT()) == vk::Format::eR32Sfloat);
    CHECK(pixelFormatToVkFormat(PF::UNKNOWN()) == vk::Format::eUndefined);
}

TEST_CASE("round-trip: vkFormat -> pixelFormat -> vkFormat", "[rdg][format]") {
    // GENERATE runs the body once per value — Catch2 parameterized test.
    auto fmt = GENERATE(vk::Format::eR8G8B8A8Unorm, vk::Format::eR8G8B8A8Snorm, vk::Format::eR32Sfloat, vk::Format::eR16Sfloat, vk::Format::eR8Unorm,
                        vk::Format::eR8Uint, vk::Format::eD24UnormS8Uint, vk::Format::eBc1RgbUnormBlock, vk::Format::eBc7UnormBlock);
    PF   pf  = vkFormatToPixelFormat(fmt);
    REQUIRE(pf != PF::UNKNOWN());
    CHECK(pixelFormatToVkFormat(pf) == fmt);
}
