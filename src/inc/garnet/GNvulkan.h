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
// Include volk.inl to get all vulkan function pointers.
#if GN_MSWIN && !defined(VK_USE_PLATFORM_WIN32_KHR)
    #define VK_USE_PLATFORM_WIN32_KHR
#elif GN_ANDROID && !defined(VK_USE_PLATFORM_ANDROID_KHR)
    #define VK_USE_PLATFORM_ANDROID_KHR
#endif
#include "vulkan/volk.inl"

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

#include "vulkan/barrier.inl"
#include "vulkan/handle.inl"
// #include "vulkan/buffer.inl"
// #include "vulkan/image.inl"
// #include "vulkan/pipeline.inl"
#include "vulkan/info.inl"
#include "vulkan/device.inl"

