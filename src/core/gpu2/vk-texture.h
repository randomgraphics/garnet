#pragma once

#include <garnet/GNgpu2.h>
#include "vk-gpu-context.h"
#include "vk-gpu-image-state.h"
#include "vk-format-utils.h"

namespace GN::gpu2 {

/// Shared Vulkan texture logic for owned images and swapchain-backed backbuffers.
class TextureVulkanBase : public Texture {
public:
    GN_REGISTER_RUNTIME_TYPE(Texture);

    const Descriptor & descriptor() const override { return mDescriptor; }

    gfx::img::Image readback() const override;
    bool            setContent(const gfx::img::Image & image) override;

    vk::Image nativeImage() const { return mImage; }

    /// Returns a VkImageView matching the requested subresource range and format.
    /// If \p v.format is UNKNOWN the image's native format is used. Views are cached
    /// internally by rapid-vulkan so repeated calls with the same parameters are cheap.
    vk::ImageView nativeView(const GpuResourceView::ImageView & v) const {
        if (!mRvImage) GN_UNLIKELY return vk::ImageView {};
        rv::Image::GetViewParameters vp;
        vp.type   = getTypeFromSubresourceRange(v.range);
        vp.format = pixelFormatToVkFormat(v.format != gfx::img::PixelFormat::UNKNOWN() ? v.format : mDescriptor.format);
        vp.range  = toVkSubresourceRange(vp.format, v.range);
        return mRvImage->getView(vp);
    }

    mutable TextureGpuImageState gpuStates {};

protected:
    /// \p leafType must be the concrete class \c TYPE_INFO() (so \c Entity stores the leaf type for \c RuntimeType::cast).
    explicit TextureVulkanBase(const GN::RuntimeType::TypeInfo & leafType, const StrA & entityName);
    ~TextureVulkanBase() override;

    Texture::Descriptor        mDescriptor {};
    AutoRef<GpuContextVulkan2> mGpu;

    void setVulkanHandles(rv::Image * rvImage) {
        mRvImage = rvImage;
        mImage   = rvImage ? rvImage->handle() : vk::Image {};
    }

private:
    rv::Image * mRvImage {};
    vk::Image   mImage {};

    /// Infers the VkImageViewType from the subresource range's array layer count.
    /// Returns (vk::ImageViewType)(-1) when the count is unspecified so rapid-vulkan
    /// picks the image's natural view type.
    static inline vk::ImageViewType getTypeFromSubresourceRange(const GpuResourceView::SubresourceRange & r) {
        const uint32_t remaining = (uint32_t) -1;
        if (r.e.numArrayLayers == 1) return vk::ImageViewType::e2D;
        if (r.e.numArrayLayers == remaining) return (vk::ImageViewType) (-1); // use image default
        return vk::ImageViewType::e2DArray;
    }

    /// Converts a GpuResourceView subresource range to a VkImageSubresourceRange.
    /// The aspect is derived from the image's native VkFormat so depth/stencil images
    /// automatically get the correct aspect flags.
    static inline vk::ImageSubresourceRange toVkSubresourceRange(vk::Format format, const GpuResourceView::SubresourceRange & r) {
        vk::ImageAspectFlags aspect = rv::Image::determineImageAspect(format);
        return vk::ImageSubresourceRange(aspect, r.i.mip, r.e.numMipLevels, r.i.face, r.e.numArrayLayers);
    }
};

AutoRef<Texture> createTextureVulkan2(const StrA & name, const Texture::CreateParameters & params);
AutoRef<Texture> loadTextureVulkan2(const Texture::LoadParameters & params);

} // namespace GN::gpu2
