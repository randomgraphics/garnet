// This is the main public interface of physray-va module
#pragma once

#include <garnet/GNbase.h>

// ---------------------------------------------------------------------------------------------------------------------
// this header must be included before the standard vulkan header.
#ifdef VULKAN_H_
    #error "Can't include <vulkan.h> before <ph/va.h>"
#endif

// ---------------------------------------------------------------------------------------------------------------------
/// On 32-bit platform, vulkan.h defines all handle types as uint64_t, which prevents compile time type traits on
/// handles from working properly. This macro refines all vulkan handles to unique structure that encapsulate
/// a 64-bit integer value.
#if !defined(__LP64__) && !defined(_WIN64) && (!defined(__x86_64__) || defined(__ILP32__)) && !defined(_M_X64) && !defined(__ia64) && !defined(_M_IA64) && \
    !defined(__aarch64__) && !defined(__powerpc64__)
    #define PH_SDK_CUSTOM_VK_HANDLE 1
    #define VK_DEFINE_NON_DISPATCHABLE_HANDLE(object)                        \
        typedef struct object##_T {                                          \
            uint64_t u64;                                                    \
            constexpr object##_T(): u64(0) {}                                \
            constexpr object##_T(uint64_t u): u64(u) {}                      \
            constexpr object##_T(int u): u64((uint64_t) u) {}                \
            constexpr object##_T(nullptr_t): u64(0) {}                       \
            constexpr object##_T(const object##_T &) = default;              \
            constexpr object##_T & operator=(const object##_T &) = default;  \
            constexpr object##_T(object##_T &&)                  = default;  \
            constexpr object##_T & operator=(object##_T &&) = default;       \
            constexpr              operator uint64_t() const { return u64; } \
        } object;
#endif

// ---------------------------------------------------------------------------------------------------------------------
// Include volk.h
#if GN_MSWIN && !defined(VK_USE_PLATFORM_WIN32_KHR)
    #define VK_USE_PLATFORM_WIN32_KHR
#elif GN_ANDROID && !defined(VK_USE_PLATFORM_ANDROID_KHR)
    #define VK_USE_PLATFORM_ANDROID_KHR
#endif
#include "vulkan/volk.hpp"

#include <cassert>
#include <functional>
#include <type_traits>

// ---------------------------------------------------------------------------------------------------------------------
// Error check helpers
#define GN_VK_CHK(func, actionOnFailure)                                                                    \
    if (true) {                                                                                             \
        auto result__ = (func);                                                                             \
        /* there are a few positive success code other than VK_SUCCESS */                                   \
        if (result__ < 0) {                                                                                 \
            GN_ERROR(getLogger("GN.vk"))("%s failed: %s", #func, ::GN::vulkan::VkResultToString(result__)); \
            actionOnFailure;                                                                                \
        }                                                                                                   \
    } else                                                                                                  \
        void(0)

/// Check on vulkan functions call, throw exception when function failed (returned negative VkResult)
#define GN_VK_REQUIRE(x) GN_VK_CHK(x, GN_THROW(#x))

// ---------------------------------------------------------------------------------------------------------------------
/// main name space for Vulkan utilities
namespace GN::vulkan {

// ---------------------------------------------------------------------------------------------------------------------
/// convert VKResult to string
GN_API const char * VkResultToString(VkResult);

// ---------------------------------------------------------------------------------------------------------------------
/// A helper function to insert resource/memory barriers to command buffer
struct SimpleBarriers {
    VkPipelineStageFlags               srcStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    VkPipelineStageFlags               dstStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    std::vector<VkMemoryBarrier>       memory;
    std::vector<VkBufferMemoryBarrier> buffers;
    std::vector<VkImageMemoryBarrier>  images;

    void clear() {
        memory.clear();
        buffers.clear();
        images.clear();
    }

    SimpleBarriers & reserveBuffers(size_t count) {
        buffers.reserve(count);
        return *this;
    }

    SimpleBarriers & addMemory(VkAccessFlags srcAccess, VkAccessFlags dstAccess);

    SimpleBarriers & addBuffer(VkBuffer buffer, VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE,
                               VkAccessFlags srcAccess = VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT,
                               VkAccessFlags dstAccess = VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT);

    SimpleBarriers & addImage(VkImage image, VkAccessFlags srcAccess, VkAccessFlags dstAccess, VkImageLayout oldLayout, VkImageLayout newLayout,
                              VkImageSubresourceRange subresourceRange);

    SimpleBarriers & addImage(VkImage image, VkAccessFlags srcAccess, VkAccessFlags dstAccess, VkImageLayout oldLayout, VkImageLayout newLayout,
                              VkImageAspectFlags aspect) {
        VkImageSubresourceRange range = {aspect, 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS};
        return addImage(image, srcAccess, dstAccess, oldLayout, newLayout, range);
    }

    SimpleBarriers & setStages(VkPipelineStageFlags src, VkPipelineStageFlags dst) {
        srcStage = src;
        dstStage = dst;
        return *this;
    }

    void write(VkCommandBuffer cb) const;
};

// ---------------------------------------------------------------------------------------------------------------------
/// You should ONLY USE THIS FOR DEBUGGING - this is not something that should ever ship in real code,
/// this will flush and invalidate all caches and stall everything. It is a tool not to be used lightly!
/// That said, it can be really handy if you think you have a race condition in your app and you just want
/// to serialize everything so you can debug it.
///
/// Note that this does not take care of image layouts - if you're debugging you can set the layout of all
/// your images to GENERAL to overcome this, but again - do not do this in release code!
///
/// (code and comments are referenced from: https://github.com/KhronosGroup/Vulkan-Docs/wiki/Synchronization-Examples)
void fullPipelineBarrier(VkCommandBuffer cb);

// ---------------------------------------------------------------------------------------------------------------------
/// @brief Get object type of a VK handle.
/// The base function is intentionally left undefined. See va.inl for all specialized versions.
/// \todo simplify this with "if constexpr" expression in c++17.
template<typename HANDLE>
inline VkObjectType getHandleObjectType() {
    if constexpr (std::is_same_v<HANDLE, VkBuffer>) {
        return VK_OBJECT_TYPE_BUFFER;
    } else if constexpr (std::is_same_v<HANDLE, VkCommandPool>) {
        return VK_OBJECT_TYPE_COMMAND_POOL;
    } else if constexpr (std::is_same_v<HANDLE, VkCommandBuffer>) {
        return VK_OBJECT_TYPE_COMMAND_BUFFER;
    } else if constexpr (std::is_same_v<HANDLE, VkDescriptorPool>) {
        return VK_OBJECT_TYPE_DESCRIPTOR_POOL;
    } else if constexpr (std::is_same_v<HANDLE, VkDescriptorSetLayout>) {
        return VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT;
    } else if constexpr (std::is_same_v<HANDLE, VkDescriptorSet>) {
        return VK_OBJECT_TYPE_DESCRIPTOR_SET;
    } else if constexpr (std::is_same_v<HANDLE, VkDeviceMemory>) {
        return VK_OBJECT_TYPE_DEVICE_MEMORY;
    } else if constexpr (std::is_same_v<HANDLE, VkFence>) {
        return VK_OBJECT_TYPE_FENCE;
    } else if constexpr (std::is_same_v<HANDLE, VkFramebuffer>) {
        return VK_OBJECT_TYPE_FRAMEBUFFER;
    } else if constexpr (std::is_same_v<HANDLE, VkImage>) {
        return VK_OBJECT_TYPE_IMAGE;
    } else if constexpr (std::is_same_v<HANDLE, VkImageView>) {
        return VK_OBJECT_TYPE_IMAGE_VIEW;
    } else if constexpr (std::is_same_v<HANDLE, VkPipeline>) {
        return VK_OBJECT_TYPE_PIPELINE;
    } else if constexpr (std::is_same_v<HANDLE, VkPipelineLayout>) {
        return VK_OBJECT_TYPE_PIPELINE_LAYOUT;
    } else if constexpr (std::is_same_v<HANDLE, VkRenderPass>) {
        return VK_OBJECT_TYPE_RENDER_PASS;
    } else if constexpr (std::is_same_v<HANDLE, VkSemaphore>) {
        return VK_OBJECT_TYPE_SEMAPHORE;
    } else if constexpr (std::is_same_v<HANDLE, VkShaderModule>) {
        return VK_OBJECT_TYPE_SHADER_MODULE;
    } else if constexpr (std::is_same_v<HANDLE, VkSurfaceKHR>) {
        return VK_OBJECT_TYPE_SURFACE_KHR;
    } else if constexpr (std::is_same_v<HANDLE, VkSwapchainKHR>) {
        return VK_OBJECT_TYPE_SWAPCHAIN_KHR;
    } else if constexpr (std::is_same_v<HANDLE, VkSampler>) {
        return VK_OBJECT_TYPE_SAMPLER;
    } else if constexpr (std::is_same_v<HANDLE, VkQueryPool>) {
        return VK_OBJECT_TYPE_QUERY_POOL;
    } else if constexpr (std::is_same_v<HANDLE, VkAccelerationStructureKHR>) {
        return VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR;
    } else {
        static_assert(std::false_type<HANDLE>::value, "Unsupported handle type");
        return VK_OBJECT_TYPE_UNKNOWN;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
/// A little struct to hold all essential Vulkan global objects. This is to make it easier to reference key Vulkan
/// information across the library.
struct VulkanGlobalInfo {
    const VkAllocationCallbacks * allocator = nullptr;
    VkInstance                    instance  = nullptr;
    VkPhysicalDevice              phydev    = nullptr;
    VkDevice                      device    = nullptr;
    // VmaAllocator                  vmaAllocator = nullptr;

    bool operator==(const VulkanGlobalInfo & rhs) const {
        return allocator == rhs.allocator && instance == rhs.instance && phydev == rhs.phydev && device == rhs.device;
    }

    bool operator!=(const VulkanGlobalInfo & rhs) const { return !operator==(rhs); }
};

} // namespace GN::vulkan

#include "vulkan/handle.inl"
// #include "vulkan/buffer.inl"
// #include "vulkan/image.inl"
// #include "vulkan/pipeline.inl"
#include "vulkan/info.inl"
#include "vulkan/device.inl"

