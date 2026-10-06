#pragma once

#include <garnet/GNengine2.h>

#include <string>

namespace GN::taixu::prototype {

/// Creates the initial Form-based world without the optional sample edit.
bool createWorld(e2::Universe &, e2::World &, e2::PhysicalScale);

/// Adds six named Forms at a finite world-space origin; rejects invalid input before changing the world.
bool addWoodHouse(e2::Universe &, e2::World &, e2::PhysicalScale, const glm::dvec3 & origin);

/// Writes the app's versioned semantic snapshot, rejecting unsupported parent links or mesh assets.
bool saveWorldSnapshot(const e2::PrimeView &, e2::PhysicalScale, const char * filename);

/// Parses and validates the complete snapshot before constructing a new Form-based World.
e2::Ref<e2::World> loadWorldSnapshot(e2::Universe &, e2::PhysicalScale, const char * filename, std::string & error);

} // namespace GN::taixu::prototype
