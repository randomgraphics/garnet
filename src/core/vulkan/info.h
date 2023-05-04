#pragma once
#include <garnet/GNvulkan.h>
#include <map>
#include <list>

namespace GN::vulkan {

// ---------------------------------------------------------------------------------------------------------------------
//
struct InstanceInfo {
    struct LayerInfo {
        VkLayerProperties                  properties;
        std::vector<VkExtensionProperties> extensions;
    };

    uint32_t                           version;
    std::vector<LayerInfo>             layers;
    std::vector<VkExtensionProperties> extensions;

    /// initialize the structure. Populate all data members.
    void init();

    /// Check to see if certain instance layer is supported.
    bool checkLayer(const char * layer) const;

    /// Check to see if certain instance extension is supported.
    bool checkExtension(const char * extension) const;

    /// result structure of check() method
    struct ValidatedExtentions {
        std::vector<const char *> layers;             ///< list of layers to initialize VK instance.
        std::vector<const char *> instanceExtensions; ///< list of extensions to initialize VK instance.
    };

    /// Check to ensure all requested layers and extensions are supported. Throw exception, if any of required
    /// layers/extensions are not supported.
    ValidatedExtentions validate(const std::vector<std::pair<const char *, bool>> & layers_, std::map<const char *, bool> extensions_) const;

    std::string print(const VkInstanceCreateInfo & ici, bool verbose = false) const;
};

// ---------------------------------------------------------------------------------------------------------------------
//
struct PhysicalDeviceInfo {
    VkPhysicalDeviceProperties         properties;
    std::vector<VkExtensionProperties> extensions;

    void query(VkPhysicalDevice dev) {
        vkGetPhysicalDeviceProperties(dev, &properties);
        extensions = completeEnumerate<VkExtensionProperties>(
            [&](uint32_t * count, VkExtensionProperties * data) -> VkResult { return vkEnumerateDeviceExtensionProperties(dev, nullptr, count, data); });
    }
};

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename StructureChainIterator>
inline void * buildStructureChain(StructureChainIterator begin, StructureChainIterator end, void * extra = nullptr) {
    struct Segment {
        uint32_t type;
        void *   next = nullptr;
    };
    void * next = nullptr;

    // Link extra nodes
    if (extra) next = extra;

    for (auto iter = begin; iter != end; ++iter) {
        auto & c = *iter;
        auto   s = (Segment *) c.buffer.data();
        s->next  = next;
        next     = s;
    }
    return next;
}

// ---------------------------------------------------------------------------------------------------------------------
// An helper structure to for easier use of VkPhysicalDeviceFeatures2 structure
struct PhysicalDeviceFeatureList {
    /// constructor
    PhysicalDeviceFeatureList(const VkPhysicalDeviceFeatures & features1, const std::vector<SimpleStructureChain> & features2, void * features3 = nullptr) {
        // store basic feature list
        _deviceFeatures.features = features1;

        // build extensible feature chain.
        _list.assign(features2.begin(), features2.end());
        _deviceFeatures.pNext = buildStructureChain(_list.begin(), _list.end(), features3);
    }

    // Returns the first (root) level struct in the feature list chain.
    const VkPhysicalDeviceFeatures2 & root() const { return _deviceFeatures; }

    // Returns the first (root) level struct in the feature list chain.
    VkPhysicalDeviceFeatures2 & root() { return _deviceFeatures; }

    /// Add new feature to the feature2 list.
    template<typename T>
    T & addFeature(const T & feature) {
        SimpleStructureChain sc(feature);
        _list.emplace_back(sc);
        auto t                = (T *) _list.back().buffer.data();
        t->pNext              = _deviceFeatures.pNext;
        _deviceFeatures.pNext = t;
        return *t;
    }

private:
    VkPhysicalDeviceFeatures2 _deviceFeatures {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR};
    std::list<SimpleStructureChain> _list;
};

} // namespace GN::vulkan