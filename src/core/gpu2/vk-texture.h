#pragma once

#include <garnet/GNgpu2.h>
#include "vk-gpu-context.h"
#include "vk-gpu-image-state.h"

namespace GN::gpu2::rv {
class Image;
}

namespace GN::gpu2 {

/// Shared Vulkan texture logic for owned images and swapchain-backed backbuffers.
class TextureVulkanBase : public Texture {
public:
    GN_REGISTER_RUNTIME_TYPE(Texture);

    const Descriptor & descriptor() const override { return mDescriptor; }

    gfx::img::Image readback() const override;
    bool            setContent(const gfx::img::Image & image) override;

    /// Fills native color-attachment handles using \c vulkanColorAttachmentHandles() and \p viewSpec format hints.
    bool resolveVulkanColorAttachment(const GpuResourceView & viewSpec, vk::Image * outImage, vk::ImageView * outView, vk::Extent2D * outExt,
                                      vk::Format * outVkFormat) const;

    /// Returns the underlying VkImage handle, or a null handle if unavailable.
    vk::Image vulkanNativeImage() const {
        vk::Image     img {};
        vk::ImageView iv {};
        vulkanColorAttachmentHandles(&img, &iv);
        return img;
    }

    mutable TextureGpuImageState gpuStates {};

protected:
    /// \p leafType must be the concrete class \c TYPE_INFO() (so \c Entity stores the leaf type for \c RuntimeType::cast).
    explicit TextureVulkanBase(const GN::RuntimeType::TypeInfo & leafType, const StrA & entityName);
    ~TextureVulkanBase() override;

    Texture::Descriptor        mDescriptor {};
    AutoRef<GpuContextVulkan2> mGpu;

private:
    virtual const rv::Image * vulkanSourceImageForReadback() const = 0;
    virtual rv::Image *       vulkanImageForSetContent() { return nullptr; }
    virtual bool              vulkanColorAttachmentHandles(vk::Image * outImage, vk::ImageView * outView) const = 0;
};

AutoRef<Texture> createTextureVulkan2(const StrA & name, const Texture::CreateParameters & params);
AutoRef<Texture> loadTextureVulkan2(const Texture::LoadParameters & params);

} // namespace GN::gpu2
