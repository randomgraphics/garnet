#pragma once
#include <garnet/GNfx2.h>

namespace GN::fx2 {

/// Private module helper to retrieve the descriptor heap material index from a SkyMaterial.
/// Returns uint32_t(-1) if material is null or not a valid bindless SkyMaterial.
uint32_t getSkyMaterialIndex(const SkyMaterial * material);

/// Return whether a bindless sky material uses the supplied descriptor heap.
bool skyMaterialUsesHeap(const SkyMaterial * material, ::GN::gpu2::DescriptorHeap & heap);

} // namespace GN::fx2
