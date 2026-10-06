#pragma once

#include <garnet/GNengine2.h>

namespace GN::taixu::prototype {

/// Creates the initial Form-based world without the optional sample edit.
bool createWorld(e2::Universe &, e2::World &, e2::PhysicalScale);

/// Adds six named Forms at a finite world-space origin; rejects invalid input before changing the world.
bool addWoodHouse(e2::Universe &, e2::World &, e2::PhysicalScale, const glm::dvec3 & origin);

} // namespace GN::taixu::prototype
