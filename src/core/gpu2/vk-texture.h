#pragma once

#include <garnet/GNgpu2.h>
#include "vk-gpu-context.h"
#include "vk-gpu-image-state.h"

namespace GN::gpu2 {

/// Shared Vulkan texture logic for owned images and swapchain-backed backbuffers.
class TextureVulkanBase : public Texture {
public:
    GN_REGISTER_RUNTIME_TYPE(Texture);

    const Descriptor & descriptor() const override { return mDescriptor; }

    gfx::img::Image readback() const override;
    bool            setContent(const gfx::img::Image & image) override;

    vk::Image     nativeImage() const { return mImage; }
    vk::ImageView nativeView() const { return mView; }

    mutable TextureGpuImageState gpuStates {};

protected:
    /// \p leafType must be the concrete class \c TYPE_INFO() (so \c Entity stores the leaf type for \c RuntimeType::cast).
    explicit TextureVulkanBase(const GN::RuntimeType::TypeInfo & leafType, const StrA & entityName);
    ~TextureVulkanBase() override;

    Texture::Descriptor        mDescriptor {};
    AutoRef<GpuContextVulkan2> mGpu;

    void setVulkanHandles(rv::Image * rvImage, vk::ImageView iv) {
        mRvImage = rvImage;
        mImage   = rvImage ? rvImage->handle() : vk::Image {};
        mView    = iv;
    }

private:
    rv::Image *   mRvImage {};
    vk::Image     mImage {};
    vk::ImageView mView {};
};

AutoRef<Texture> createTextureVulkan2(const StrA & name, const Texture::CreateParameters & params);
AutoRef<Texture> loadTextureVulkan2(const Texture::LoadParameters & params);

} // namespace GN::gpu2
