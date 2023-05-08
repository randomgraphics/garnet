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
    #define VK_DEFINE_NON_DISPATCHABLE_HANDLE(object)                       \
        typedef struct object##_T {                                         \
            uint64_t u64;                                                   \
            constexpr object##_T(): u64(0) {}                               \
            constexpr object##_T(uint64_t u): u64(u) {}                     \
            constexpr object##_T(int u): u64((uint64_t) u) {}               \
            constexpr object##_T(nullptr_t): u64(0) {}                      \
            constexpr object##_T(const object##_T &)             = default; \
            constexpr object##_T & operator=(const object##_T &) = default; \
            constexpr object##_T(object##_T &&)                  = default; \
            constexpr object##_T & operator=(object##_T &&)      = default; \
            constexpr operator uint64_t() const { return u64; }             \
        } object;
#endif

// ---------------------------------------------------------------------------------------------------------------------
// Include volk.inl to get all vulkan function pointers.
#if GN_MSWIN && !defined(VK_USE_PLATFORM_WIN32_KHR)
    #define VK_USE_PLATFORM_WIN32_KHR
#elif GN_ANDROID && !defined(VK_USE_PLATFORM_ANDROID_KHR)
    #define VK_USE_PLATFORM_ANDROID_KHR
#endif
#include "vulkan/3rd-party/volk.h"

// Check if volk.h is too old.
#if VOLK_HEADER_VERSION < VK_HEADER_VERSION
    #pragma message("[WARNING] VOLK_HEADER_VERSION(" GN_STR(VOLK_HEADER_VERSION) ") is older VK_HEADER_VERSION(" GN_STR( \
            VK_HEADER_VERSION) ")! You might see link error of missing Vulkan symbos. Consider downgrade your Vulkan SDK to match the version of volk.h.")
#endif

// ---------------------------------------------------------------------------------------------------------------------
// Include VMA allocator header
#if PH_MSWIN
    #pragma warning(push, 0)
#elif PH_UNIX_LIKE
    #pragma GCC diagnostic push
    #if PH_ANDROID
        #pragma GCC diagnostic ignored "-Wnullability-completeness"
    #endif
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #pragma GCC diagnostic ignored "-Wunused-variable"
    #pragma GCC diagnostic ignored "-Wtype-limits"
    #pragma GCC diagnostic ignored "-Wformat"
    #pragma GCC diagnostic ignored "-Wundef"
#endif

// Enable these defines to help debug VK memory corruption.
// #ifndef VMA_DEBUG_DETECT_CORRUPTION
//     #define VMA_DEBUG_DETECT_CORRUPTION 1
//     #define VMA_DEBUG_MARGIN            32
// #endif

#ifndef VMA_DEBUG_ERROR_LOG
    #define VMA_DEBUG_ERROR_LOG PH_LOGE
#endif

#include <vma/vk_mem_alloc.h>
#if PH_MSWIN
    #pragma warning(pop)
#elif PH_UNIX_LIKE
    #pragma GCC diagnostic pop
#endif

#include <cassert>

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

} // namespace GN::vulkan

#include "vulkan/vgi.inl"
#include "vulkan/barrier.inl"
#include "vulkan/handle.inl"
// #include "vulkan/buffer.inl"
// #include "vulkan/image.inl"
// #include "vulkan/pipeline.inl"
#include "vulkan/info.inl"
#include "vulkan/device.inl"
