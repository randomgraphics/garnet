#pragma once

#include "texture.h"
#include "vk-gpu-context.h"

namespace GN::rdg {

struct TextureState {
    struct ImageState {
        vk::ImageLayout        layout = vk::ImageLayout::eUndefined;
        vk::AccessFlags        access = vk::AccessFlagBits::eNone;
        vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eBottomOfPipe;

        bool operator==(const ImageState & other) const { return layout == other.layout && access == other.access && stages == other.stages; }
        bool operator!=(const ImageState & other) const { return !operator==(other); }

        inline static constexpr const ImageState UNDEFINED() {
            return {vk::ImageLayout::eUndefined, vk::AccessFlagBits::eNone, vk::PipelineStageFlagBits::eBottomOfPipe};
        };
    };

    struct ImageStateTransitionFlags {
        /// @brief Set to true to discard the content of the image during the transition.
        /// This effectively will set the source layout of the iamge to UNDEFINED.
        bool discardContent : 1 = false;

        inline static constexpr const ImageStateTransitionFlags DEFAULT() { return {false}; };
    };

    struct ImageStateTransition {
        ImageState prev;
        ImageState curr;

        ImageStateTransition & transitTo(const ImageState & newState, ImageStateTransitionFlags flags) {
            if (curr != newState) {
                if (flags.discardContent) {
                    prev = ImageState::UNDEFINED();
                } else {
                    prev = curr;
                }
                curr = newState;
            }
            return *this;
        }
    };

    TextureState(): TextureState(0, 0) {}
    TextureState(size_t mipLevels, size_t arrayLayers): mNumMips(mipLevels), mNumArrayLayers(arrayLayers), mSubresourceStates(mipLevels * arrayLayers) {}

    auto get(uint32_t mip, uint32_t arrayLayer) const -> const ImageStateTransition *;

    // returns true, if the state is changed. False, if the state is redundant or invalid.
    bool set(const GpuResourceView::SubresourceRange & range, const ImageState & newState,
             ImageStateTransitionFlags flags = ImageStateTransitionFlags::DEFAULT());

    /// Copies the current state (curr) of each subresource from \p src into this.
    /// Used when flushing command-buffer-level state back to the texture resource after submit.
    void assignFrom(const TextureState & src);

private:
    size_t subResourceIndex(uint32_t mip, uint32_t arrayLayer) const { return mip * mNumArrayLayers + arrayLayer; }

    uint32_t                        mNumMips;
    uint32_t                        mNumArrayLayers;
    DynaArray<ImageStateTransition> mSubresourceStates;
};

/// Vulkan texture: holds descriptor and GPU context; actual VkImage creation can be deferred or done in init().
class TextureVulkan : public TextureCommon {
public:
    using ImageState                = TextureState::ImageState;
    using ImageStateTransition      = TextureState::ImageStateTransition;
    using ImageStateTransitionFlags = TextureState::ImageStateTransitionFlags;

    TextureVulkan(const StrA & name);

    /// Initialize from create parameters. Returns false on failure.
    bool init(const Texture::CreateParameters & params);

    /// Initialize from load parameters (create + load from file). Returns false on failure.
    bool initFromLoad(const Texture::LoadParameters & params);

    /// Upload 1x1 RGBA pixel to mip 0, layer 0. Texture must be 1x1 and already created. Returns false on failure.
    bool upload1x1Mip(const uint8_t rgba[4]);

    auto gpu() const -> GpuContext & override { return *mGpuContext; }
    auto descriptor() const -> const Texture::Descriptor & override { return mDescriptor; }
    auto image() const -> const rapid_vulkan::Image * { return mImage.get(); }
    auto dimensions(uint32_t mip) const -> vk::Extent3D;

    auto readback() const -> gfx::img::Image override;

    TextureState & state() { return mState; }

    /// Returns state transition for one subresource; null if out of range.
    const ImageStateTransition * getImageState(uint32_t mip, uint32_t arrayLayer) const { return mState.get(mip, arrayLayer); }
    /// Returns true if any subresource state changed.
    bool trackImageState(uint32_t mip, uint32_t levels, uint32_t arrayLayer, uint32_t layers, const ImageState & newState,
                         ImageStateTransitionFlags flags = ImageStateTransitionFlags::DEFAULT());

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
        GN_ASSERT(mip < mDescriptor.levels);
        GN_ASSERT(arrayLayer < mDescriptor.faces);
        return mip * mDescriptor.faces + arrayLayer; // layer major order
    }

    AutoRef<GpuContext>                    mGpuContext;
    Texture::Descriptor                    mDescriptor;
    rapid_vulkan::Ref<rapid_vulkan::Image> mImage;
    TextureState                           mState;
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
AutoRef<Texture> createVulkanTexture(const StrA & name, const Texture::CreateParameters & params);

/// Load a texture from file and create a Vulkan-backed Texture. Called from Texture::load() when context is Vulkan.
/// Name is derived from filename. Returns null on failure or duplicate type+name.
AutoRef<Texture> loadVulkanTexture(const Texture::LoadParameters & params);

/// Create a 1x1 RGBA8 texture with the given pixel and upload it. Used for PBR default textures when a material slot is missing.
AutoRef<Texture> createDefault1x1Texture(AutoRef<GpuContext> context, const StrA & name, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

} // namespace GN::rdg
