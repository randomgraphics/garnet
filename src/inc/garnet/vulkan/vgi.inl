namespace GN::vulkan {

// ---------------------------------------------------------------------------------------------------------------------
/// A little struct to hold all essential Vulkan global objects. This is to make it easier to reference key Vulkan
/// information across the library.
struct VulkanGlobalInfo {
    const VkAllocationCallbacks * allocator    = nullptr;
    VkInstance                    instance     = nullptr;
    VkPhysicalDevice              phydev       = nullptr;
    VkDevice                      device       = nullptr;
    VmaAllocator                  vmaAllocator = nullptr;

    template<typename HANDLE>
    void safeDestroy(HANDLE &, VmaAllocation & = DUMMY_ALLOCATION) const;

    bool operator==(const VulkanGlobalInfo & rhs) const {
        return allocator == rhs.allocator && instance == rhs.instance && phydev == rhs.phydev && device == rhs.device;
    }

    bool operator!=(const VulkanGlobalInfo & rhs) const { return !operator==(rhs); }

private:
    inline static VmaAllocation DUMMY_ALLOCATION = {0};
};

template<>
inline void VulkanGlobalInfo::safeDestroy<VkBuffer>(VkBuffer & h, VmaAllocation & a) const {
    if (!h) return;
    if (a) {
        GN_VERIFY(vmaAllocator);
        vmaDestroyBuffer(vmaAllocator, h, a);
    } else {
        vkDestroyBuffer(device, h, allocator);
    }
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkCommandPool>(VkCommandPool & h, VmaAllocation & a) const {
    if (!h) return;
    vkResetCommandPool(device, h, 0);
    vkDestroyCommandPool(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkDescriptorPool>(VkDescriptorPool & h, VmaAllocation & a) const {
    if (!h) return;
    vkResetDescriptorPool(device, h, 0);
    vkDestroyDescriptorPool(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkDescriptorSetLayout>(VkDescriptorSetLayout & h, VmaAllocation & a) const {
    if (!h) return;
    vkDestroyDescriptorSetLayout(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkDeviceMemory>(VkDeviceMemory & h, VmaAllocation & a) const {
    if (!h) return;
    vkFreeMemory(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkFence>(VkFence & h, VmaAllocation & a) const {
    if (!h) return;
    vkDestroyFence(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkFramebuffer>(VkFramebuffer & h, VmaAllocation & a) const {
    if (!h) return;
    vkDestroyFramebuffer(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkImage>(VkImage & h, VmaAllocation & a) const {
    if (!h) return;
    if (a) {
        GN_VERIFY(vmaAllocator);
        vmaDestroyImage(vmaAllocator, h, a);
    } else {
        vkDestroyImage(device, h, allocator);
    }
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkImageView>(VkImageView & h, VmaAllocation & a) const {
    if (!h) return;
    vkDestroyImageView(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkPipeline>(VkPipeline & h, VmaAllocation & a) const {
    if (!h) return;
    vkDestroyPipeline(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkPipelineLayout>(VkPipelineLayout & h, VmaAllocation & a) const {
    if (!h) return;
    vkDestroyPipelineLayout(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkRenderPass>(VkRenderPass & h, VmaAllocation & a) const {
    if (!h) return;
    vkDestroyRenderPass(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkSemaphore>(VkSemaphore & h, VmaAllocation & a) const {
    if (!h) return;
    vkDestroySemaphore(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkShaderModule>(VkShaderModule & h, VmaAllocation & a) const {
    if (!h) return;
    vkDestroyShaderModule(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkSurfaceKHR>(VkSurfaceKHR & h, VmaAllocation & a) const {
    if (!h) return;
    vkDestroySurfaceKHR(instance, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkSwapchainKHR>(VkSwapchainKHR & h, VmaAllocation & a) const {
    if (!h) return;
    vkDestroySwapchainKHR(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkSampler>(VkSampler & h, VmaAllocation & a) const {
    if (!h) return;
    vkDestroySampler(device, h, allocator);
    h = 0;
    a = 0;
}

template<>
inline void VulkanGlobalInfo::safeDestroy<VkQueryPool>(VkQueryPool & h, VmaAllocation & a) const {
    if (!h) return;
    vkDestroyQueryPool(device, h, allocator);
    h = 0;
    a = 0;
}

} // namespace GN::vulkan