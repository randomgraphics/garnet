#pragma once

#include <garnet/GNrdg.h>

namespace GN::rdg {

// TextureCommon is the "common impl" base for the Texture artifact: backend-specific
// implementations (TextureVulkan, etc.) inherit from it and implement descriptor() and gpu().

/// Common base for API-specific Texture implementations (Vulkan, D3D12, Metal).
class TextureCommon : public Texture {
public:
protected:
    TextureCommon(ArtifactDatabase & db, const StrA & name): Texture(db, Texture::TYPE_ID, Texture::TYPE_NAME, name) {}
};

} // namespace GN::rdg
