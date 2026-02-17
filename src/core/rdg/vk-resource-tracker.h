#pragma once

#include "submission.h"
#include "vk-gpu-context.h"
#include <garnet/base/array.h>
#include <rapid-vulkan/rapid-vulkan.h>
#include <unordered_map>

namespace GN::rdg {

/// Tracks usage and layout of Vulkan resources across actions. Two passes: prepare() gathers
/// resource uses per action; execute() records barriers to transition resources to the right layout.
class ResourceTrackerVulkan : public SubmissionImpl::Context {
public:
    inline static constexpr Guid TYPE = {0x8d9e0f1a, 0x2b3c, 0x4d5e, {0x6f, 0x7a, 0x8b, 0x9c, 0x0d, 0x1e, 0x2f, 0x3a}};

    struct ConstructParameters {
        AutoRef<GpuContextVulkan> gpu;
    };

    /// Intentional usage of a GPU resource for a single action. Used to derive Vulkan layout, access, and stage.
    enum class ResourceUsage : uint8_t {
        /// Transfer destination (e.g. texture loading, copy into image/buffer).
        Loading,
        /// Transfer source (e.g. readback to CPU, copy from image/buffer).
        Readback,
        /// Color render target (draw).
        ColorAttachment,
        /// Depth/stencil render target (draw).
        DepthStencilAttachment,
        /// Read-only in shader (sampled image, uniform buffer).
        ShaderRead,
        /// Read-write in compute (storage image, storage buffer).
        ComputeStorage,
        /// Transfer source (explicit).
        TransferSrc,
        /// Transfer destination (explicit).
        TransferDst,
        /// Present (swapchain image ready for present).
        Present,
    };

    struct TextureResourceUse {
        AutoRef<Texture>          texture;
        Texture::SubresourceIndex subresourceIndex;
        Texture::SubresourceRange subresourceRange;
        ResourceUsage             usage = ResourceUsage::ShaderRead;
    };

    /// Buffer resource use: Vulkan buffer plus intended usage and range.
    struct BufferResourceUse {
        AutoRef<Buffer> buffer;
        uint64_t        offset = 0;
        uint64_t        size   = VK_WHOLE_SIZE;
        ResourceUsage   usage  = ResourceUsage::ShaderRead;
    };

    struct ActionParameters {
        DynaArray<TextureResourceUse> textures;
        DynaArray<BufferResourceUse>  buffers;
        const RenderTarget *          renderTarget = nullptr;
    };

    ResourceTrackerVulkan(const ConstructParameters & params);
    ~ResourceTrackerVulkan() override;

    /// Execute pass: record barriers on \p commandBuffer so resources used by the action \p actionId
    /// are in the correct layout/access. Call this from the action's execute() before using the resources.
    /// \param params        The parameters of the action to track.
    /// \param commandBuffer Command buffer to record barriers into.
    /// \return true if barriers were recorded (or none needed), false on invalid \p actionId.
    bool execute(const ActionParameters & params, vk::CommandBuffer commandBuffer);

private:
    struct ImageKey {
        vk::Image image;
        uint32_t  mip;
        uint32_t  arrayLayer;

        bool operator==(const ImageKey & other) const { return image == other.image && mip == other.mip && arrayLayer == other.arrayLayer; }
    };

    struct ImageKeyHash {
        size_t operator()(const ImageKey & k) const {
            return std::hash<uintptr_t>()(reinterpret_cast<uintptr_t>(static_cast<VkImage>(k.image))) ^ std::hash<uint32_t>()(k.mip) ^
                   std::hash<uint32_t>()(k.arrayLayer);
        }
    };

    struct ImageState {
        vk::ImageLayout        layout = vk::ImageLayout::eUndefined;
        vk::AccessFlags        access {};
        vk::PipelineStageFlags stage = vk::PipelineStageFlagBits::eTopOfPipe;
    };

    struct BufferKeyHash {
        size_t operator()(vk::Buffer b) const { return std::hash<uintptr_t>()(reinterpret_cast<uintptr_t>(static_cast<VkBuffer>(b))); }
    };

    struct BufferState {
        vk::AccessFlags        access {};
        vk::PipelineStageFlags stage = vk::PipelineStageFlagBits::eTopOfPipe;
    };

    AutoRef<GpuContextVulkan>                                  mGpu;
    std::unordered_map<ImageKey, ImageState, ImageKeyHash>     mImageState;
    std::unordered_map<vk::Buffer, BufferState, BufferKeyHash> mBufferState;
};

} // namespace GN::rdg
