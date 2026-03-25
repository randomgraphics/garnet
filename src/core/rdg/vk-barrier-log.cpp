#include "pch.h"
#include "vk-barrier-log.h"
#include <rapid-vulkan/rapid-vulkan.h>

namespace GN::rdg {

void logBarrierBatchVerboseSlow(GN::Logger * logger, const char * label, const rapid_vulkan::Barrier & barrier) {
    if (!logger || !logger->isOn(GN::Logger::VERBOSE)) GN_LIKELY return;
    
    if (barrier.memories.empty() && barrier.buffers.empty() && barrier.images.empty()) GN_LIKELY return;

    GN_VERBOSE(logger)
    ("barrier batch '{}': pipelineBarrier srcStage=0x{:x} dstStage=0x{:x} dependencyFlags=0x{:x} | memoryBarriers={} bufferBarriers={} imageBarriers={}",
     label, static_cast<uint32_t>(barrier.srcStage), static_cast<uint32_t>(barrier.dstStage), static_cast<uint32_t>(barrier.dependencies),
     barrier.memories.size(), barrier.buffers.size(), barrier.images.size());

    for (size_t i = 0; i < barrier.memories.size(); ++i) {
        const auto & m = barrier.memories[i];
        GN_VERBOSE(logger)("  [{}] memory: srcAccess=0x{:x} dstAccess=0x{:x}", i, static_cast<uint32_t>(m.srcAccessMask),
                           static_cast<uint32_t>(m.dstAccessMask));
    }
    for (size_t i = 0; i < barrier.buffers.size(); ++i) {
        const auto & bmb = barrier.buffers[i];
        GN_VERBOSE(logger)
        ("  [{}] buffer: handle=0x{:x} offset={} size={} srcAccess=0x{:x} dstAccess=0x{:x} srcQueueFamily={} dstQueueFamily={}", i,
         static_cast<uint64_t>(reinterpret_cast<uintptr_t>(static_cast<VkBuffer>(bmb.buffer))), static_cast<uint64_t>(bmb.offset),
         bmb.size == VK_WHOLE_SIZE ? uint64_t(-1) : static_cast<uint64_t>(bmb.size), static_cast<uint32_t>(bmb.srcAccessMask),
         static_cast<uint32_t>(bmb.dstAccessMask), bmb.srcQueueFamilyIndex, bmb.dstQueueFamilyIndex);
    }
    for (size_t i = 0; i < barrier.images.size(); ++i) {
        const auto & imb = barrier.images[i];
        const auto & r   = imb.subresourceRange;
        GN_VERBOSE(logger)
        ("  [{}] image: handle=0x{:x} oldLayout=0x{:x} newLayout=0x{:x} srcAccess=0x{:x} dstAccess=0x{:x} aspectMask=0x{:x} baseMip={} "
         "levelCount={} baseArrayLayer={} layerCount={} srcQueueFamily={} dstQueueFamily={}",
         i, static_cast<uint64_t>(reinterpret_cast<uintptr_t>(static_cast<VkImage>(imb.image))), static_cast<uint32_t>(imb.oldLayout),
         static_cast<uint32_t>(imb.newLayout),
         static_cast<uint32_t>(imb.srcAccessMask), static_cast<uint32_t>(imb.dstAccessMask), static_cast<uint32_t>(r.aspectMask), r.baseMipLevel,
         r.levelCount, r.baseArrayLayer, r.layerCount, imb.srcQueueFamilyIndex, imb.dstQueueFamilyIndex);
    }
}

} // namespace GN::rdg
