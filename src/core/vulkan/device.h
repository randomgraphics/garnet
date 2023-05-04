#pragma once
#include <garnet/GNvulkan.h>

namespace GN::vulkan {

class SimpleVulkanDevice::Details {
public:
    Details(SimpleVulkanDevice * owner): _owner(owner) { add(owner); }

    ~Details() { del(_owner); }

    static SimpleVulkanDevice * find(uint64_t handle) {
        std::lock_guard<std::mutex> lock(_mutex);
        auto                        iter = std::find_if(_table.begin(), _table.end(), [&](auto p) { return (uint64_t) p->vgi().device == handle; });
        return iter != _table.end() ? *iter : nullptr;
    }

    bool lost() const { return _lost; }

    void setLost(const char *) {
        _lost   = true;
        // auto bt = backtrace();
        // if (!bt.empty()) { PH_LOGE("Device 0x%" PRIx64 ": %s failed due to VK_ERROR_DEVICE_LOST:\n%s", (uint64_t) _owner->vgi().device, action, bt.c_str()); }
    }

private:
    static std::list<SimpleVulkanDevice *> _table;
    static std::mutex                      _mutex;
    SimpleVulkanDevice *                   _owner;
    bool                                   _lost = false;

    static void add(SimpleVulkanDevice * p) {
        std::lock_guard<std::mutex> lock(_mutex);
        _table.push_back(p);
    }

    static void del(SimpleVulkanDevice * p) {
        std::lock_guard<std::mutex> lock(_mutex);
        _table.remove(p);
    }
};

} // namespace ph