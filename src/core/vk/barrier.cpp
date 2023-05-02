#include "pch.h"

namespace GN::vk {

SimpleBarriers & SimpleBarriers::addMemory(VkAccessFlags srcAccess, VkAccessFlags dstAccess) {
    memory.emplace_back(VkMemoryBarrier {
        VK_STRUCTURE_TYPE_MEMORY_BARRIER,
        nullptr,
        srcAccess,
        dstAccess,
    });
    return *this;
}

SimpleBarriers & SimpleBarriers::addBuffer(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkAccessFlags srcAccess, VkAccessFlags dstAccess) {
    if (!buffer) return *this;
    buffers.emplace_back(VkBufferMemoryBarrier {VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER, nullptr, srcAccess, dstAccess, VK_QUEUE_FAMILY_IGNORED,
                                                VK_QUEUE_FAMILY_IGNORED, buffer, offset, size});
    return *this;
}

SimpleBarriers & SimpleBarriers::addImage(VkImage image, VkAccessFlags srcAccess, VkAccessFlags dstAccess, VkImageLayout oldLayout, VkImageLayout newLayout,
                                          VkImageSubresourceRange subresourceRange) {
    if (!image) return *this;
    images.emplace_back(VkImageMemoryBarrier {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, nullptr, srcAccess, dstAccess, oldLayout, newLayout,
                                              VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, image, subresourceRange});
    return *this;
}

void SimpleBarriers::write(VkCommandBuffer cb) const {
    if (memory.empty() && buffers.empty() && images.empty()) return;
    vkCmdPipelineBarrier(cb, srcStage, dstStage, {}, (uint32_t) memory.size(), memory.data(), (uint32_t) buffers.size(), buffers.data(),
                         (uint32_t) images.size(), images.data());
}

} // namespace GN::vk