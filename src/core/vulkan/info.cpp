#include "pch.h"
#include "info.h"

static GN::Logger * sLogger = GN::getLogger("GN.vk.info");

namespace GN::vulkan {

// ---------------------------------------------------------------------------------------------------------------------
//
std::vector<VkPhysicalDevice> enumeratePhysicalDevices(VkInstance instance) {
    return completeEnumerate<VkPhysicalDevice>([&](uint32_t * count, VkPhysicalDevice * data) { return vkEnumeratePhysicalDevices(instance, count, data); });
}

// ---------------------------------------------------------------------------------------------------------------------
// This function currently selects the devie with the longest extension list.
VkPhysicalDevice selectTheMostPowerfulPhysicalDevice(const VkPhysicalDevice * phydevs, size_t count) {
    size_t result = 0;
    size_t maxext = 0;
    for (size_t i = 0; i < count; ++i) {
        auto dev        = phydevs[i];
        auto extensions = completeEnumerate<VkExtensionProperties>(
            [&](uint32_t * count, VkExtensionProperties * data) { return vkEnumerateDeviceExtensionProperties(dev, nullptr, count, data); });
        if (extensions.size() > maxext) {
            maxext = extensions.size();
            result = i;
        }
    }
    return phydevs[result];
}

// ---------------------------------------------------------------------------------------------------------------------
//
std::vector<VkExtensionProperties> enumerateDeviceExtenstions(VkPhysicalDevice dev) {
    auto extensions = completeEnumerate<VkExtensionProperties>(
        [&](uint32_t * count, VkExtensionProperties * data) { return vkEnumerateDeviceExtensionProperties(dev, nullptr, count, data); });
    std::sort(extensions.begin(), extensions.end(), [](const auto & a, const auto & b) -> bool { return strcmp(a.extensionName, b.extensionName) < 0; });
    return extensions;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void InstanceInfo::init() {
    vkEnumerateInstanceVersion(&version);

    auto properties =
        completeEnumerate<VkLayerProperties>([&](uint32_t * count, VkLayerProperties * data) { return vkEnumerateInstanceLayerProperties(count, data); });

    layers.resize(properties.size());

    for (size_t i = 0; i < layers.size(); ++i) {
        layers[i].properties = properties[i];
        layers[i].extensions = completeEnumerate<VkExtensionProperties>(
            [&](uint32_t * count, VkExtensionProperties * data) { return vkEnumerateInstanceExtensionProperties(properties[i].layerName, count, data); });
    }

    std::sort(layers.begin(), layers.end(), [](const auto & a, const auto & b) { return strcmp(a.properties.layerName, b.properties.layerName) < 0; });

    extensions = completeEnumerate<VkExtensionProperties>(
        [&](uint32_t * count, VkExtensionProperties * data) { return vkEnumerateInstanceExtensionProperties(nullptr, count, data); });

    std::sort(extensions.begin(), extensions.end(), [](const auto & a, const auto & b) { return strcmp(a.extensionName, b.extensionName) < 0; });
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool InstanceInfo::checkLayer(const char * layer) const {
    if (!layer || !*layer) return true; // empty layer is always supported.
    auto iter = std::find_if(layers.begin(), layers.end(), [&](const LayerInfo & a) { return 0 == strcmp(a.properties.layerName, layer); });
    return iter != layers.end();
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool InstanceInfo::checkExtension(const char * extension) const {
    if (!extension || !*extension) return true; // return true for empty extension.
    auto iter = std::find_if(extensions.begin(), extensions.end(), [&](const VkExtensionProperties & e) { return 0 == strcmp(e.extensionName, extension); });
    return iter != extensions.end();
}

// ---------------------------------------------------------------------------------------------------------------------
//
auto InstanceInfo::validate(const std::vector<std::pair<const char *, bool>> & layers_, std::map<const char *, bool> extensions_) const
    -> ValidatedExtentions {
    ValidatedExtentions v;

    // hold list of supported extensions in a set to avoid duplicated extension names.
    std::set<const char *> available;

    auto processSupportedExtension = [&](const VkExtensionProperties & available) {
        // Add supported ones to v.instanceExtenions list. Then remove it from extensions_ list. So we won't check
        // the same extension twice.
        for (auto asked = extensions_.begin(); asked != extensions_.end();) {
            if (0 == strcmp(asked->first, available.extensionName)) {
                v.instanceExtensions.push_back(asked->first);
                // Remove "found" extensions from the list. So it won't be checked again.
                asked = extensions_.erase(asked);
            } else {
                ++asked;
            }
        }
    };

    // check against each layers
    for (auto l : layers_) {
        auto iter = std::find_if(layers.begin(), layers.end(), [&](const LayerInfo & a) { return 0 == strcmp(a.properties.layerName, l.first); });
        if (iter == layers.end()) {
            if (l.second) {
                GN_THROW("Required VK layer %s is not supported.", l.first);
            } else {
                GN_WARN(sLogger)("Optional VK layer %s is not supported.", l.first);
            }
            continue;
        }
        v.layers.push_back(l.first);

        // Check against extension list of the layer.
        for (const auto & e : iter->extensions) { processSupportedExtension(e); }
    }

    // Check against each extensions
    for (const auto & e : extensions) { processSupportedExtension(e); }

    // We have gone through all available layers and extensions. All supported extensions should have been removed
    // from extensions_list. Now it is time to see if there's any unsupported but required extensions.
    for (const auto & asked : extensions_) {
        if (asked.second) {
            GN_THROW("Required VK extension %s is not supported.", asked.first);
        } else {
            GN_WARN(sLogger)("Optional VK extension %s is not supported.", asked.first);
        }
    }

    // done
    return v;
}

// ---------------------------------------------------------------------------------------------------------------------
//
std::string InstanceInfo::print(const VkInstanceCreateInfo & ici, bool verbose) const {
    std::stringstream ss;

    auto isLayerEnabled = [&](const std::string & layer) -> bool {
        for (size_t i = 0; i < ici.enabledLayerCount; ++i) {
            const char * e = ici.ppEnabledLayerNames[i];
            if (layer == e) return true;
        }
        return false;
    };

    auto isExtensionEnabled = [&](const std::string & name) -> bool {
        for (size_t i = 0; i < ici.enabledExtensionCount; ++i) {
            const char * e = ici.ppEnabledExtensionNames[i];
            if (name == e) return true;
        }
        return false;
    };

    // retrieve supported API version.
    ss << "========================================" << std::endl
       << "Vulkan API version :" << std::endl
       << "  supported: " << printVulkanVersion(version) << std::endl
       << "    enabled: " << printVulkanVersion(ici.pApplicationInfo->apiVersion) << std::endl
       << "========================================" << std::endl
       << (verbose ? "Available" : "Enabled") << " Vulkan layers :" << std::endl;
    if (layers.empty()) {
        ss << "  <empty>" << std::endl;
    } else {
        for (const auto & l : layers) {
            bool enabled = isLayerEnabled(l.properties.layerName);
            if (!verbose && !enabled) continue;
            ss << ((enabled && verbose) ? "  * " : "    ") << l.properties.layerName << " ( "
               << "v" << VK_VERSION_MAJOR(l.properties.specVersion) << "." << VK_VERSION_MINOR(l.properties.specVersion) << "."
               << VK_VERSION_PATCH(l.properties.specVersion) << " ) : " << l.properties.description << std::endl;
            for (const auto & e : l.extensions) { ss << "                " << e.extensionName << " ( ver." << e.specVersion << " )" << std::endl; }
        }
    }
    ss << "========================================" << std::endl << (verbose ? "Available" : "Enabled") << " Instance extensions :" << std::endl;
    if (extensions.empty()) {
        ss << "  <empty>" << std::endl;
    } else {
        for (const auto & e : extensions) {
            bool enabled = isExtensionEnabled(e.extensionName);
            if (!verbose && !enabled) continue;
            ss << ((enabled && verbose) ? "  * " : "    ") << e.extensionName << " ( ver." << e.specVersion << " )" << std::endl;
        }
    }
    ss << std::endl;

    // done
    return ss.str();
}

} // namespace GN::vulkan