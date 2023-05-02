#include <vector>
#include <sstream>
#include <algorithm>

namespace GN::vk {

// ---------------------------------------------------------------------------------------------------------------------
/// Utility function to enumerate vulkan item/feature/extension list.
template<typename T, typename Q>
inline std::vector<T> completeEnumerate(Q query) {
    // It's possible, though very rare, that the number of items
    // could change. For example, installing something
    // could include new layers that the loader would pick up
    // between the initial query for the count and the
    // request for VkLayerProperties. The loader indicates that
    // by returning a VK_INCOMPLETE status and will update the
    // the count parameter.
    // The count parameter will be updated with the number of
    // entries loaded into the data pointer - in case the number
    // of layers went down or is smaller than the size given.
    std::vector<T> result;
    // if constexpr (std::is_same_v<void, std::result_of_t<Q>>) {
    //     uint32_t count;
    //     query(&count, nullptr);
    //     result.resize(count);
    //     query(&count, result.data());
    // } else {
    VkResult res;
    do {
        uint32_t count;
        GN_VK_CHK(query(&count, nullptr), return {});
        if (0 == count) return {};
        result.resize(count);
        res = query(&count, result.data());
    } while (res == VK_INCOMPLETE);
    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
//
std::vector<VkPhysicalDevice> enumeratePhysicalDevices(VkInstance instance);

// ---------------------------------------------------------------------------------------------------------------------
// This function currently selects the devie with the longest extension list.
VkPhysicalDevice selectTheMostPowerfulPhysicalDevice(const VkPhysicalDevice * phydevs, size_t count);

// ---------------------------------------------------------------------------------------------------------------------
//
std::vector<VkExtensionProperties> enumerateDeviceExtenstions(VkPhysicalDevice dev);

// ---------------------------------------------------------------------------------------------------------------------
//
inline std::string printVulkanVersion(uint32_t v) {
    std::stringstream ss;
    ss << "v" << VK_VERSION_MAJOR(v) << "." << VK_VERSION_MINOR(v) << "." << VK_VERSION_PATCH(v);
    return ss.str();
};

} // namespace GN::vk