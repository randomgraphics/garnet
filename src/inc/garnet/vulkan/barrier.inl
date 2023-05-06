namespace GN::vulkan {

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

} // namespace GN::vulkan