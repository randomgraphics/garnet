#pragma once
#include <garnet/GNfx2.h>

namespace GN::fx2::bindless {

/// Private module helper to retrieve the descriptor heap material index from a SkyMaterial.
/// Returns uint32_t(-1) if material is null or not a valid bindless SkyMaterial.
uint32_t getSkyMaterialIndex(const SkyMaterial * material);

} // namespace GN::fx2::bindless
