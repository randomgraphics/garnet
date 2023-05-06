#include <map>
#include <set>
#include <vector>
#include <string>

namespace GN::vulkan {

/// Extensible structure chain used by Vulkan API.
struct SimpleStructureChain {
    /// data buffer that stores the VK feature structure.
    std::vector<uint8_t> buffer;

    /// construct new feature
    template<typename T>
    SimpleStructureChain(const T & feature) {
        buffer.resize(sizeof(feature));
        memcpy(buffer.data(), &feature, sizeof(feature));
    }

    template<typename T>
    T * reset() {
        buffer.resize(sizeof(T));
        return (T *) buffer.data();
    }
};

class GN_API SimpleInstance {
public:
    GN_NO_COPY(SimpleInstance);
    GN_NO_MOVE(SimpleInstance);

    /// Define level of validation on Vulkan error.
    enum Validation {
        VALIDATION_DISABLED = 0,
        LOG_ON_VK_ERROR,
        LOG_ON_VK_ERROR_WITH_CALL_STACK,
        THROW_ON_VK_ERROR,
        BREAK_ON_VK_ERROR,
    };

    enum Verbosity {
        SILENCE = 0,
        BRIEF,
        VERBOSE,
    };

    struct ConstructParameters {
        /// The Vulkan API version. Default is 1.1
        uint32_t apiVersion = VK_MAKE_VERSION(1, 1, 0);

        /// Specify extra layers to initialize VK instance. The 2nd value indicates if the layer is required or not.
        /// We have to use vector instead of map here, because layer loading order matters.
        std::vector<std::pair<std::string, bool>> layers;

        /// Specify extra extension to initialize VK instance. Value indicate if the extension is required or not.
        std::map<std::string, bool> instanceExtensions;

        /// structure chain passed to VkInstanceCreateInfo::pNext
        std::vector<SimpleStructureChain> instanceCreateInfo;

        /// Set to true to enable validation layer.
        Validation validation = GN_BUILD_DEBUG_ENABLED ? LOG_ON_VK_ERROR_WITH_CALL_STACK : VALIDATION_DISABLED;

        /// Creation log output verbosity
        Verbosity printVkInfo = BRIEF;
    };

    SimpleInstance(ConstructParameters);

    ~SimpleInstance();

    const ConstructParameters & cp() const { return _cp; }

    VkInstance get() const { return _instance; }

    operator VkInstance() const { return _instance; }

private:
    ConstructParameters      _cp;
    VkInstance               _instance {};
    VkDebugReportCallbackEXT _debugReport = 0;
};

// ---------------------------------------------------------------------------------------------------------------------
//
class GN_API SimpleQueue {
public:
    GN_NO_COPY(SimpleQueue);
    GN_NO_MOVE(SimpleQueue);

    uint32_t family() const { return _family; }
    uint32_t index() const { return _index; }
    VkQueue  handle() const { return _handle; }

    operator VkQueue() const { return _handle; }

private:
    friend class SimpleDevice;
    SimpleQueue() {}
    ~SimpleQueue() {}
    uint32_t _family = VK_QUEUE_FAMILY_IGNORED;
    uint32_t _index  = 0;
    VkQueue  _handle = 0;
};

// ---------------------------------------------------------------------------------------------------------------------
//
class GN_API SimpleDevice {
public:
    GN_NO_COPY(SimpleDevice);
    GN_NO_MOVE(SimpleDevice);

    struct ConstructParameters {
        /// pointer to Vulkan instance.
        SimpleInstance * instance = nullptr;

        /// Leave it at zero to create an headless device w/o presentation support.
        VkSurfaceKHR surface = 0;

        /// Specify extra extension to initialize VK device. Value indicate if the extension is required or not.
        std::map<std::string, bool> deviceExtensions;

        /// Set to true to defer to VMA for device memory allocations.
        bool useVmaAllocator = false;

        /// set to false to make the creation log less verbose.
        SimpleInstance::Verbosity printVkInfo = SimpleInstance::BRIEF;

        /// Basic VK device feature list defined by Vulkan 1.0
        VkPhysicalDeviceFeatures features1 {};

        /// Extensible VK device feature list defined Vulkan 1.1
        std::vector<SimpleStructureChain> features2;

        // External feature chain
        void * features3 = nullptr;

        /// Add new feature to the feature2 list.
        template<typename T>
        T & addFeature(const T & feature) {
            features2.emplace_back(feature);
            return *(T *) features2.back().buffer.data();
        }

        /// A utility function to setup SimpleDevice::ConstructParameters to for ray tracing
        /// \param hw Whether to use hardware VK_KHR_ray_query extension. If set to false, then setup the construct
        ///           parameter for in-house compute shader based pipeline. In this case, return value is always false.
        /// \return   If the construction parameter is properly set for hardware ray query. If false is returned,
        ///           then construction parameter structure is set to do in-house shader based pipeline.
        bool setupForRayQuery(bool hw);
    };

    SimpleDevice(ConstructParameters);

    ~SimpleDevice();

    const ConstructParameters & cp() const { return _cp; }

    const VulkanGlobalInfo & vgi() const { return _vgi; }

    SimpleQueue & graphics() const { return *_graphics; }
    SimpleQueue & compute() const { return *_compute; }
    SimpleQueue & transfer() const { return *_transfer; }
    SimpleQueue & present() const { return *_present; }

    bool separatePresentQueue() const { return _present != _graphics; }

    // VkResult waitIdle() const { return _vgi.device ? threadSafeDeviceWaitIdle(_vgi.device) : VK_SUCCESS; }

    class Details;
    Details & details() const { return *_details; }

private:
    Details *                  _details = nullptr;
    ConstructParameters        _cp;
    VulkanGlobalInfo           _vgi {};
    std::vector<SimpleQueue *> _queues; // one for each queue family
    SimpleQueue *              _graphics = nullptr;
    SimpleQueue *              _compute  = nullptr;
    SimpleQueue *              _transfer = nullptr;
    SimpleQueue *              _present  = nullptr;
};

/// This is used to temporarily mute error log of Vulkan validation error, when you are expecting some VK errors and don't want to those pollute the log output.
GN_API void muteValidationErrorLog();

/// Call this to unmute the VK validation error log.
GN_API void unmuteValidationErrorLog();

/// A helper class to mute VK error log within the current execution scope.
template<bool ENABLED = true>
class MuteValidationErrorWithinCurrentScope {
public:
    MuteValidationErrorWithinCurrentScope() {
        if (ENABLED) muteValidationErrorLog();
    }
    ~MuteValidationErrorWithinCurrentScope() {
        if (ENABLED) unmuteValidationErrorLog();
    }
};

} // namespace GN::vulkan
