#pragma once

#include "texture.h"
#include "vk-gpu-context.h"

namespace GN::rdg {

/// Vulkan texture: holds descriptor and GPU context; actual VkImage creation can be deferred or done in init().
class TextureVulkan : public TextureCommon {
    AutoRef<GpuContext>                    mGpuContext;
    Texture::Descriptor                    mDescriptor;
    rapid_vulkan::Ref<rapid_vulkan::Image> mImage;

public:
    TextureVulkan(ArtifactDatabase & db, const StrA & name);

    /// Initialize from create parameters. Returns false on failure.
    bool init(const Texture::CreateParameters & params);

    /// Initialize from load parameters (create + load from file). Returns false on failure.
    bool initFromLoad(const Texture::LoadParameters & params);

    vk::Image handle() const { return mImage.valid() ? mImage->handle() : vk::Image {}; }

    GpuContext &                gpu() const override { return *mGpuContext; }
    const Texture::Descriptor & descriptor() const override { return mDescriptor; }

    gfx::img::Image readback() const override;
};

/// Create a Vulkan-backed Texture. Called from Texture::create() when context is Vulkan.
/// Returns null on failure or duplicate type+name.
AutoRef<Texture> createVulkanTexture(ArtifactDatabase & db, const StrA & name, const Texture::CreateParameters & params);

/// Load a texture from file and create a Vulkan-backed Texture. Called from Texture::load() when context is Vulkan.
/// Name is derived from filename. Returns null on failure or duplicate type+name.
AutoRef<Texture> loadVulkanTexture(ArtifactDatabase & db, const Texture::LoadParameters & params);

} // namespace GN::rdg
