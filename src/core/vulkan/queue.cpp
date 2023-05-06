#include "pch.h"

namespace GN::vulkan {

SimpleQueue::SimpleQueue(const ConstructParameters & cp) : _vgi(cp.vgi) {
    auto ci = VkDeviceQueueCreateInfo { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
    ci.queueFamilyIndex = cp.family;
    ci.queueCount = 1;
    ci.pQueuePriorities = &cp.priority;
    GN_VK_REQUIRE(vkCreateQueue(_vgi.device, &ci, _vgi.allocator, &_handle));
}

SimpleQueue::~SimpleQueue() {
    if (_handle) {
        vkDestroyQueue(_vgi.device, _handle, _vgi.allocator);
        _handle = 0;
    }
}

}