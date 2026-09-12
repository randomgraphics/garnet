#pragma once

#include <garnet/GNfx2.h>

namespace GN::fx2 {

/// Source container recognized by the importer.
enum class ModelSourceFormat : uint8_t {
    UNKNOWN,
    FBX,
    GLTF,
    GLB,
    STL,
    ASE,
};

// Importer dispatch helper, also used by module-local format classification tests.
ModelSourceFormat classifyModelSourcePath(const StrA & path);

} // namespace GN::fx2
