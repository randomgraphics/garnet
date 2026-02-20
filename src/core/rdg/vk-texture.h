#pragma once

#include "texture.h"
#include "vk-gpu-context.h"

namespace GN::rdg {

/// Vulkan texture: holds descriptor and GPU context; actual VkImage creation can be deferred or done in init().
class TextureVulkan : public TextureCommon {
public:
    struct ImageState {
        vk::ImageLayout        layout = vk::ImageLayout::eUndefined;
        vk::AccessFlags        access = vk::AccessFlagBits::eNone;
        vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eBottomOfPipe;

        bool operator==(const ImageState & other) const { return layout == other.layout && access == other.access && stages == other.stages; }
        bool operator!=(const ImageState & other) const { return !operator==(other); }
    };

    struct ImageStateTransition {
        ImageState prev;
        ImageState curr;

        ImageStateTransition & transitTo(const ImageState & newState) {
            if (curr != newState) {
                prev = curr;
                curr = newState;
            }
            return *this;
        }
    };

    TextureVulkan(ArtifactDatabase & db, const StrA & name);

    /// Initialize from create parameters. Returns false on failure.
    bool init(const Texture::CreateParameters & params);

    /// Initialize from load parameters (create + load from file). Returns false on failure.
    bool initFromLoad(const Texture::LoadParameters & params);

    auto gpu() const -> GpuContext & override { return *mGpuContext; }
    auto descriptor() const -> const Texture::Descriptor & override { return mDescriptor; }
    auto image() const -> const rapid_vulkan::Image * { return mImage.get(); }

    auto getImageState(uint32_t mip, uint32_t arrayLayer) const -> const ImageStateTransition *;
    bool trackImageState(uint32_t mip, uint32_t levels, uint32_t arrayLayer, uint32_t layers, const ImageState & newState);

    auto readback() const -> gfx::img::Image override;

private:
    // struct ImageKey {
    //     vk::Image image;
    //     uint32_t  mip;
    //     uint32_t  arrayLayer;

    //     bool operator==(const ImageKey & other) const { return image == other.image && mip == other.mip && arrayLayer == other.arrayLayer; }
    // };

    // struct ImageKeyHash {
    //     size_t operator()(const ImageKey & k) const {
    //         return std::hash<uintptr_t>()(reinterpret_cast<uintptr_t>(static_cast<VkImage>(k.image))) ^ std::hash<uint32_t>()(k.mip) ^
    //                std::hash<uint32_t>()(k.arrayLayer);
    //     }
    // };

    size_t subResourceIndex(uint32_t mip, uint32_t arrayLayer) const {
        GN_ASSERT(0 <= mip && mip < mDescriptor.levels);
        GN_ASSERT(0 <= arrayLayer && arrayLayer < mDescriptor.faces);
        return mip * mDescriptor.faces + arrayLayer; // layer major order
    }

    AutoRef<GpuContext>                    mGpuContext;
    Texture::Descriptor                    mDescriptor;
    rapid_vulkan::Ref<rapid_vulkan::Image> mImage;
    std::vector<ImageStateTransition>      mSubresourceStates;
};

// /// Helper class to iterate over subresources of a texture, initialized from a Texture::Descriptor,
// /// with an optional subresource index and range.
// ///
// /// The class will first validate the subresource index and range, issue warning if the index and range is invalid.
// /// then it'll clamp the index and range into valid values, then remember it.
// ///
// /// The class provides a forEach function that accepts a functor and will run the functor on each and every of
// /// the subsource in the valid range. If the range is empty, the functor will not be called.
// /// It can iterate through the subresources in two different order: mip major and layer major.
// class SubresourceIterator {
// public:
//     enum class Order { MipMajor, LayerMajor };

//     SubresourceIterator(const Texture::Descriptor & desc, const Texture::SubresourceIndex & index = {}, const Texture::SubresourceRange & range = {});

//     template<typename F>
//     void forEach(F && fn, Order order = Order::MipMajor) const {
//         if (mNumMips == 0 || mNumLayers == 0) return;
//         if (order == Order::MipMajor) {
//             for (uint32_t mip = mBaseMip; mip < mBaseMip + mNumMips; ++mip)
//                 for (uint32_t face = mBaseFace; face < mBaseFace + mNumLayers; ++face)
//                     if (!fn(mip, face)) break;
//         } else {
//             for (uint32_t face = mBaseFace; face < mBaseFace + mNumLayers; ++face)
//                 for (uint32_t mip = mBaseMip; mip < mBaseMip + mNumMips; ++mip)
//                     if (!fn(mip, face)) break;
//         }
//     }

//     const Texture::Descriptor & descriptor() const { return mDesc; }

//     size_t numSubresources() const { return mNumMips * mNumLayers; }

// private:
//     Texture::Descriptor mDesc;
// };

/// Create a Vulkan-backed Texture. Called from Texture::create() when context is Vulkan.
/// Returns null on failure or duplicate type+name.
AutoRef<Texture> createVulkanTexture(ArtifactDatabase & db, const StrA & name, const Texture::CreateParameters & params);

/// Load a texture from file and create a Vulkan-backed Texture. Called from Texture::load() when context is Vulkan.
/// Name is derived from filename. Returns null on failure or duplicate type+name.
AutoRef<Texture> loadVulkanTexture(ArtifactDatabase & db, const Texture::LoadParameters & params);

} // namespace GN::rdg
