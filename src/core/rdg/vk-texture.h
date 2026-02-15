#pragma once

#include "texture.h"
#include "vk-gpu-context.h"

namespace GN::rdg {

/// Vulkan texture: holds descriptor and GPU context; actual VkImage creation can be deferred or done in init().
class TextureVulkan : public TextureCommon {
    AutoRef<GpuContext>    mGpuContext;
    Texture::Descriptor   mDescriptor;

public:
    TextureVulkan(ArtifactDatabase & db, const StrA & name);

    /// Initialize from create parameters. Returns false on failure.
    bool init(const Texture::CreateParameters & params);

    /// Initialize from load parameters (create + load from file). Returns false on failure.
    bool initFromLoad(const Texture::LoadParameters & params);

    GpuContext & gpu() const override { return *mGpuContext; }
    const Texture::Descriptor & descriptor() const override { return mDescriptor; }
};

/// Create a Vulkan-backed Texture. Called from Texture::create() when context is Vulkan.
/// Returns null on failure or duplicate type+name.
AutoRef<Texture> createVulkanTexture(ArtifactDatabase & db, const StrA & name, const Texture::CreateParameters & params);

/// Load a texture from file and create a Vulkan-backed Texture. Called from Texture::load() when context is Vulkan.
/// Name is derived from filename. Returns null on failure or duplicate type+name.
AutoRef<Texture> loadVulkanTexture(ArtifactDatabase & db, const Texture::LoadParameters & params);

// =============================================================================
// TextureReadback - Vulkan implementation
// =============================================================================

/// Vulkan texture readback action: copies texture/backbuffer content to host image.
class TextureReadbackVulkan : public TextureReadback {
public:
    TextureReadbackVulkan(ArtifactDatabase & db, const StrA & name);

    std::pair<ExecutionResult, ExecutionContext *> prepare(Submission & submission, Arguments & arguments) override;
    ExecutionResult execute(Submission & submission, Arguments & arguments, ExecutionContext * context) override;
};

/// Create a Vulkan-backed TextureReadback. Called from TextureReadback::create() when context is Vulkan.
AutoRef<TextureReadback> createVulkanTextureReadback(ArtifactDatabase & db, const StrA & name, const TextureReadback::CreateParameters & params);

} // namespace GN::rdg
