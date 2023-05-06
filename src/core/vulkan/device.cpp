#include "pch.h"
#include "device.h"
#include "info.h"

#include <atomic>
#include <iomanip>
#include <map>
#include <sstream>

static GN::Logger * sLogger = GN::getLogger("GN.vk.device");

namespace GN::vulkan {

std::list<SimpleVulkanDevice *> SimpleVulkanDevice::Details::_table;
std::mutex                      SimpleVulkanDevice::Details::_mutex;

static std::atomic<int> debugMuteCounter {0};

// ---------------------------------------------------------------------------------------------------------------------
//
static VkBool32 VKAPI_PTR debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object,
                                        size_t,  // location,
                                        int32_t, // messageCode,
                                        const char * prefix, const char * message, void * userData) {
    if (debugMuteCounter > 0) return VK_FALSE;

    auto inst = (SimpleVulkanInstance *) userData;

    auto reportVkError = [=]() {
        if (objectType == VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT) {
            auto dev = SimpleVulkanDevice::Details::find(object);
            if (dev && dev->details().lost()) {
                // Ignore validation errors on lost device, to avoid spamming log with useless messages.
                return;
            }
        }

        auto              v = inst->cp().validation;
        std::stringstream ss;
        ss << str::format("[Vulkan] %s : %s", prefix, message);
        // if (v >= SimpleVulkanInstance::LOG_ON_VK_ERROR_WITH_CALL_STACK) { ss << std::endl << backtrace(false); }
        auto str = ss.str();
        GN_ERROR(sLogger)("%s", str.c_str());
        if (v == SimpleVulkanInstance::THROW_ON_VK_ERROR) {
            GN_THROW("%s", str.data());
        } else if (v == SimpleVulkanInstance::BREAK_ON_VK_ERROR) {
            breakIntoDebugger();
        }
    };

    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) reportVkError();

    // treat warning as error.
    if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) reportVkError();

    // if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
    //     GN_WARN(sLogger)("[Vulkan] %s : %s", prefix, message);
    // }

    // if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
    //     GN_INFO(sLogger)("[Vulkan] %s : %s", prefix, message);
    // }

    // if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
    //     PH_LOGV("[Vulkan] %s : %s", prefix, message);
    // }

    return VK_FALSE;
}

// ---------------------------------------------------------------------------------------------------------------------
//
static void printPhysicalDeviceInfo(const std::vector<VkPhysicalDevice> & available, VkPhysicalDevice selected, bool) {
#define PRINT_LIMIT(name) "        " #name " = " << p.limits.name << std::endl
    std::stringstream ss;
    ss << "===================================" << std::endl << "Available Vulkan physical devices :" << std::endl;
    for (const auto & d : available) {
        VkPhysicalDeviceDescriptorIndexingProperties dip = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES};
        VkPhysicalDeviceProperties2                  p2  = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR, &dip};
        vkGetPhysicalDeviceProperties2(d, &p2);
        const auto & p = p2.properties;
        ss << ((d == selected) ? "  * " : "    ") << p.deviceName << std::endl
           << "        API version = " << printVulkanVersion(p.apiVersion) << std::endl
           << "        Driver version = " << printVulkanVersion(p.driverVersion)
           << std::endl
           // clang-format off
           << PRINT_LIMIT(maxBoundDescriptorSets)
           << PRINT_LIMIT(maxPerStageDescriptorSamplers)
           << PRINT_LIMIT(maxPerStageDescriptorUniformBuffers)
           << PRINT_LIMIT(maxPerStageDescriptorStorageBuffers)
           << PRINT_LIMIT(maxPerStageDescriptorSampledImages)
           << PRINT_LIMIT(maxPerStageDescriptorStorageImages)
           << PRINT_LIMIT(maxPerStageDescriptorInputAttachments)
           << PRINT_LIMIT(maxPerStageResources)
           << PRINT_LIMIT(maxDescriptorSetSamplers)
           << PRINT_LIMIT(maxDescriptorSetUniformBuffers)
           << PRINT_LIMIT(maxDescriptorSetUniformBuffersDynamic)
           << PRINT_LIMIT(maxDescriptorSetStorageBuffers)
           << PRINT_LIMIT(maxDescriptorSetStorageBuffersDynamic)
           << PRINT_LIMIT(maxDescriptorSetSampledImages)
           << PRINT_LIMIT(maxDescriptorSetStorageImages)
           << PRINT_LIMIT(maxDescriptorSetInputAttachments);
        // clang-format on
    }
    ss << std::endl;
    GN_INFO(sLogger)(ss.str().c_str());
}

// ---------------------------------------------------------------------------------------------------------------------
//
static void printDeviceFeatures(VkPhysicalDevice phydev, const PhysicalDeviceFeatureList & enabled, bool verbose) {
    // retrieve physical device properties
    VkPhysicalDeviceProperties properties = {};
    vkGetPhysicalDeviceProperties(phydev, &properties);

    // retrieve supported feature list
    VkPhysicalDeviceFeatures supported = {};
    vkGetPhysicalDeviceFeatures(phydev, &supported);

    // print enabled feature list
    bool              none = true;
    std::stringstream ss;
    ss << "=====================================================================" << std::endl;
    ss << (verbose ? "Available" : "Enabled") << " features of the selected device : " << properties.deviceName << std::endl;
#define PRINT_FEATURE(x)                                          \
    if (supported.x) {                                            \
        if (enabled.root().features.x) {                          \
            ss << (verbose ? "  * " : "    ") << #x << std::endl; \
            none = false;                                         \
        } else if (verbose) {                                     \
            ss << "    " << #x << std::endl;                      \
            none = false;                                         \
        }                                                         \
    }
    PRINT_FEATURE(robustBufferAccess);
    PRINT_FEATURE(fullDrawIndexUint32);
    PRINT_FEATURE(imageCubeArray);
    PRINT_FEATURE(independentBlend);
    PRINT_FEATURE(geometryShader);
    PRINT_FEATURE(tessellationShader);
    PRINT_FEATURE(sampleRateShading);
    PRINT_FEATURE(dualSrcBlend);
    PRINT_FEATURE(logicOp);
    PRINT_FEATURE(multiDrawIndirect);
    PRINT_FEATURE(drawIndirectFirstInstance);
    PRINT_FEATURE(depthClamp);
    PRINT_FEATURE(depthBiasClamp);
    PRINT_FEATURE(fillModeNonSolid);
    PRINT_FEATURE(depthBounds);
    PRINT_FEATURE(wideLines);
    PRINT_FEATURE(largePoints);
    PRINT_FEATURE(alphaToOne);
    PRINT_FEATURE(multiViewport);
    PRINT_FEATURE(samplerAnisotropy);
    PRINT_FEATURE(textureCompressionETC2);
    PRINT_FEATURE(textureCompressionASTC_LDR);
    PRINT_FEATURE(textureCompressionBC);
    PRINT_FEATURE(occlusionQueryPrecise);
    PRINT_FEATURE(pipelineStatisticsQuery);
    PRINT_FEATURE(vertexPipelineStoresAndAtomics);
    PRINT_FEATURE(fragmentStoresAndAtomics);
    PRINT_FEATURE(shaderTessellationAndGeometryPointSize);
    PRINT_FEATURE(shaderImageGatherExtended);
    PRINT_FEATURE(shaderStorageImageExtendedFormats);
    PRINT_FEATURE(shaderStorageImageMultisample);
    PRINT_FEATURE(shaderStorageImageReadWithoutFormat);
    PRINT_FEATURE(shaderStorageImageWriteWithoutFormat);
    PRINT_FEATURE(shaderUniformBufferArrayDynamicIndexing);
    PRINT_FEATURE(shaderSampledImageArrayDynamicIndexing);
    PRINT_FEATURE(shaderStorageBufferArrayDynamicIndexing);
    PRINT_FEATURE(shaderStorageImageArrayDynamicIndexing);
    PRINT_FEATURE(shaderClipDistance);
    PRINT_FEATURE(shaderCullDistance);
    PRINT_FEATURE(shaderFloat64);
    PRINT_FEATURE(shaderInt64);
    PRINT_FEATURE(shaderInt16);
    PRINT_FEATURE(shaderResourceResidency);
    PRINT_FEATURE(shaderResourceMinLod);
    PRINT_FEATURE(sparseBinding);
    PRINT_FEATURE(sparseResidencyBuffer);
    PRINT_FEATURE(sparseResidencyImage2D);
    PRINT_FEATURE(sparseResidencyImage3D);
    PRINT_FEATURE(sparseResidency2Samples);
    PRINT_FEATURE(sparseResidency4Samples);
    PRINT_FEATURE(sparseResidency8Samples);
    PRINT_FEATURE(sparseResidency16Samples);
    PRINT_FEATURE(sparseResidencyAliased);
    PRINT_FEATURE(variableMultisampleRate);
    PRINT_FEATURE(inheritedQueries);
#undef PRINT_FEATURE
    if (none) ss << std::endl << "  [None]" << std::endl;
    ss << std::endl;
    GN_INFO(sLogger)(ss.str().c_str());
}

// ---------------------------------------------------------------------------------------------------------------------
//
static void printDeviceExtenstions(VkPhysicalDevice phydev, const std::vector<VkExtensionProperties> & available, const std::vector<const char *> & enabled,
                                   bool verbose) {
    // retrieve physical device properties
    VkPhysicalDeviceProperties properties = {};
    vkGetPhysicalDeviceProperties(phydev, &properties);

    std::stringstream ss;
    ss << "=====================================================================" << std::endl;
    ss << (verbose ? "Available" : "Enabled") << " extensions of the selected device: " << properties.deviceName << std::endl;
    bool none = true;
    for (size_t i = 0; i < available.size(); ++i) {
        const auto & e         = available[i];
        bool         supported = false;
        for (size_t j = 0; j < enabled.size(); ++j) {
            if (0 == strcmp(e.extensionName, enabled[j])) {
                supported = true;
                break;
            }
        }
        if (!supported && !verbose) continue;
        ss << ((supported && verbose) ? "*" : " ") << std::setw(3) << i << " : " << e.extensionName << " ( ver. " << e.specVersion << " )" << std::endl;
        none = false;
    }
    if (none) ss << std::endl << "  [None]" << std::endl;
    ss << std::endl;
    GN_INFO(sLogger)(ss.str().c_str());
}

// ---------------------------------------------------------------------------------------------------------------------
//
static void printAvailableQueues(VkPhysicalDevice phydev, const std::vector<VkQueueFamilyProperties> & queues, bool) {
    // retrieve physical device properties
    VkPhysicalDeviceProperties properties = {};
    vkGetPhysicalDeviceProperties(phydev, &properties);

    auto flags2str = [](VkQueueFlags flags) -> std::string {
        std::stringstream ss;
        bool              empty = true;
        ss << flags << " (";
        if (flags & VK_QUEUE_COMPUTE_BIT) {
            if (empty) {
                empty = false;
            } else {
                ss << "|";
            }
            ss << "Graphics";
        }
        if (flags & VK_QUEUE_GRAPHICS_BIT) {
            if (empty) {
                empty = false;
            } else {
                ss << "|";
            }
            ss << "Compute";
        }
        if (flags & VK_QUEUE_TRANSFER_BIT) {
            if (empty) {
                empty = false;
            } else {
                ss << "|";
            }
            ss << "Transfer";
        }
        if (flags & VK_QUEUE_SPARSE_BINDING_BIT) {
            if (empty) {
                empty = false;
            } else {
                ss << "|";
            }
            ss << "SparseBinding";
        }
        ss << ")";
        return ss.str();
    };

    std::stringstream ss;
    ss << "=====================================================================" << std::endl;
    ss << "Available queues on selected device : " << properties.deviceName << std::endl;
    for (size_t i = 0; i < queues.size(); ++i) {
        const auto & q = queues[i];
        auto         w = q.minImageTransferGranularity.width;
        auto         h = q.minImageTransferGranularity.height;
        auto         d = q.minImageTransferGranularity.depth;
        ss << " " << i << " : queueCount = " << q.queueCount << std::endl
           << "     minImageTransferGranularity = (" << w << "x" << h << "x" << d << ")" << std::endl
           << "     timestampValidBits = " << q.timestampValidBits << std::endl
           << "     queueFlags = " << flags2str(q.queueFlags) << std::endl;
    }
    ss << std::endl;

    GN_INFO(sLogger)(ss.str().c_str());
}

// ---------------------------------------------------------------------------------------------------------------------
//
static std::vector<const char *> validateExtensions(const std::vector<VkExtensionProperties> & available, const std::map<std::string, bool> & asked) {
    std::vector<const char *> supported;
    for (const auto & a : asked) {
        bool found = false;
        for (const auto & b : available) {
            if (a.first == b.extensionName) {
                supported.push_back(b.extensionName);
                found = true;
                break;
            }
        }
        if (found) continue;
        if (a.second) {
            GN_THROW("Extension %s is not supported by current device.", a.first.c_str());
        } else {
            GN_WARN(sLogger)("Optional feature %s is not supported by the current device.", a.first.c_str());
        }
    }
    return supported;
}

// ---------------------------------------------------------------------------------------------------------------------
//
static inline SimpleVulkanInstance::ConstructParameters & adjust(SimpleVulkanInstance::ConstructParameters & cp) {
    // GN_INFO(sLogger)("Detecting RenderDoc...");
    // if (isRenderDocPresent()) {
    //     GN_INFO(sLogger)("RenderDoc is detected. Turn off validation.");
    //     cp.validation = SimpleVulkanInstance::VALIDATION_DISABLED;
    // } else {
    //     GN_INFO(sLogger)("RenderDoc not found.");
    // }

    // done
    return cp;
}

// ---------------------------------------------------------------------------------------------------------------------
//
SimpleVulkanInstance::SimpleVulkanInstance(ConstructParameters cp): _cp(adjust(cp)) {
    // initialize volk loader
    volkInitialize();

    InstanceInfo instanceInfo;
    instanceInfo.init();

    std::vector<std::pair<const char *, bool>> layers;
    for (const auto & l : cp.layers) { layers.push_back({l.first.c_str(), l.second}); }
    if (cp.validation) {
        // add validation layer as an "optional" layer
        layers.push_back({"VK_LAYER_KHRONOS_validation", false});
    }

    // setup extension list
    std::map<const char *, bool> instanceExtensions {
#if PH_ANDROID
        // somehow w/o this extension, the rt core crashes on android.
        { VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, true }
#endif
    };
    if (cp.validation) {
        // Enable in-shader debug printf, if supported.
        instanceExtensions[VK_EXT_DEBUG_REPORT_EXTENSION_NAME] = false;
        instanceExtensions[VK_EXT_DEBUG_UTILS_EXTENSION_NAME]  = false;
    }
    for (const auto & e : cp.instanceExtensions) { instanceExtensions[e.first.c_str()] = e.second; }

    // make sure all required layers and extensions are actually supported
    auto supported = instanceInfo.validate(layers, instanceExtensions);

    std::stringstream instanceCreationPrompt;
    instanceCreationPrompt << "Try creating Vulkan instance with the following layers:";
    if (supported.layers.empty()) {
        instanceCreationPrompt << " <none>";
    } else {
        for (auto & l : supported.layers) { instanceCreationPrompt << " " << l; }
    }
    instanceCreationPrompt << std::endl << "Try creating Vulkan instance with the following extensions:";
    if (supported.instanceExtensions.empty()) {
        instanceCreationPrompt << " <none>";
    } else {
        for (auto & l : supported.instanceExtensions) { instanceCreationPrompt << " " << l; }
    }
    instanceCreationPrompt << std::endl;
    GN_INFO(sLogger)(instanceCreationPrompt.str().c_str());

    // create VK 1.1 instance
    // TODO: check against available version.
    auto appInfo       = VkApplicationInfo {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    appInfo.apiVersion = _cp.apiVersion;
    auto ici           = VkInstanceCreateInfo {
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        buildStructureChain(_cp.instanceCreateInfo.begin(), _cp.instanceCreateInfo.end()),
                  {}, // flags
        &appInfo,
        (uint32_t) supported.layers.size(),
        supported.layers.data(),
        (uint32_t) supported.instanceExtensions.size(),
        supported.instanceExtensions.data(),
    };
    GN_VK_REQUIRE(vkCreateInstance(&ici, nullptr, &_instance));

    // Print instance information
    if (cp.printVkInfo) {
        auto message = instanceInfo.print(ici, VERBOSE == cp.printVkInfo);
        GN_INFO(sLogger)(message.data());
    }

    // load all function poivovolnters.
    volkLoadInstance(_instance);

    // setup debug callback
    if (cp.validation) {
        auto debugci = VkDebugReportCallbackCreateInfoEXT {
            VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
            nullptr,
            VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT,
            debugCallback,
            this,
        };
        GN_VK_REQUIRE(vkCreateDebugReportCallbackEXT(_instance, &debugci, nullptr, &_debugReport));
    }

    GN_INFO(sLogger)("Vulkan instance initialized.");
}

// ---------------------------------------------------------------------------------------------------------------------
//
SimpleVulkanInstance::~SimpleVulkanInstance() {
    if (_debugReport) vkDestroyDebugReportCallbackEXT(_instance, _debugReport, nullptr), _debugReport = 0;
    if (_instance) vkDestroyInstance(_instance, nullptr), _instance = 0;
    GN_INFO(sLogger)("Vulkan instance destroyed.");
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool SimpleVulkanDevice::ConstructParameters::setupForRayQuery(bool hw) {
    // setup some common extension and feature that are needed regardless if we are in HW or SW mode.

    // Required to reference resource descriptor by index in shader.
    auto & dynamicIndexing                                      = addFeature(VkPhysicalDeviceDescriptorIndexingFeatures {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES,
    });
    dynamicIndexing.shaderSampledImageArrayNonUniformIndexing   = true;
    dynamicIndexing.shaderStorageBufferArrayNonUniformIndexing  = true;
    dynamicIndexing.descriptorBindingUpdateUnusedWhilePending   = true; // updating descriptors while the set is in use
    dynamicIndexing.descriptorBindingPartiallyBound             = true;
    dynamicIndexing.descriptorBindingVariableDescriptorCount    = true; // Descriptor sets with a variable-sized last binding
    dynamicIndexing.runtimeDescriptorArray                      = true; // Arrays of resources which are sized at run-time.
    deviceExtensions[VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME] = true;

    // Enable 16-bit support in shader
    auto & f16Feature                   = addFeature(VkPhysicalDevice16BitStorageFeatures {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES,
    });
    f16Feature.storageBuffer16BitAccess = true;
#if !PH_ANDROID
    f16Feature.uniformAndStorageBuffer16BitAccess = true; // needed for index-16-to-32.comp. note this feature is not required and not available on Android.
#endif
    features1.shaderInt16              = true;
    features1.fragmentStoresAndAtomics = true; // needed for writing restir buffers from the graphics pipeline

    // enable buffer device address extension
    addFeature(VkPhysicalDeviceBufferDeviceAddressFeatures {
                   VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES,
               })
        .bufferDeviceAddress                                      = true;
    deviceExtensions[VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME] = true;

    // // Detect renderdoc
    // if (hw && va::isRenderDocPresent()) {
    //     GN_INFO(sLogger)("RenderDoc is detected. Turn off HW ray query.");
    //     hw = false;
    // };

    // That's all we need for SW ray tracing. The following features and extensions are only needed
    // when we are using VK_KHR_ray_query extension.
    if (!hw) return false;

    addFeature(VkPhysicalDeviceAccelerationStructureFeaturesKHR {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR}).accelerationStructure =
        true;
    addFeature(VkPhysicalDeviceRayQueryFeaturesKHR {
                   VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR,
               })
        .rayQuery                                                    = true;
    deviceExtensions[VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME]   = true;
    deviceExtensions[VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME] = true;
    deviceExtensions[VK_KHR_RAY_QUERY_EXTENSION_NAME]                = true;
    deviceExtensions[VK_KHR_SPIRV_1_4_EXTENSION_NAME]                = true;
    deviceExtensions[VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME]    = true;

    // done
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------
//
SimpleVulkanDevice::SimpleVulkanDevice(ConstructParameters cp): _cp(cp) {
    _details = new Details(this);

    // check instance pointer
    GN_VERIFY(cp.instance);
    _vgi.instance = cp.instance->get();

    // enumerate physical devices
    auto phydevs = enumeratePhysicalDevices(_vgi.instance);

    // TODO: pick the one specified by user.
    _vgi.phydev  = selectTheMostPowerfulPhysicalDevice(phydevs.data(), phydevs.size());
    bool verbose = cp.printVkInfo == SimpleVulkanInstance::VERBOSE;
    if (cp.printVkInfo) printPhysicalDeviceInfo(phydevs, _vgi.phydev, verbose);

    // query queues
    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(_vgi.phydev, &count, nullptr);
    std::vector<VkQueueFamilyProperties> families;
    families.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(_vgi.phydev, &count, families.data());
    if (cp.printVkInfo) printAvailableQueues(_vgi.phydev, families, verbose);

    // setup device feature and extension
    PhysicalDeviceFeatureList   deviceFeatures(cp.features1, cp.features2, cp.features3);
    std::map<std::string, bool> askedDeviceExtensions = cp.deviceExtensions;

    // more extension
    askedDeviceExtensions[VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME] = true;

    // #if PH_ANDROID == 0
    //     if (isRenderDocPresent()) {                                                       // only add this when renderdoc is available
    //         askedDeviceExtensions[VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME] = true; // add this to allow debugging on compute shaders
    //     }
    // #endif

    // Determine if this is an headless rendering or not.
    bool presenting = false;
    if (_cp.surface) {
        for (auto & e : _cp.instance->cp().instanceExtensions) {
            if (e.first == VK_KHR_SURFACE_EXTENSION_NAME) {
                askedDeviceExtensions[VK_KHR_SWAPCHAIN_EXTENSION_NAME] = true;
                presenting                                             = true;
                break;
            }
        }
    }

    // make sure all extensions are actually supported by the hardware.
    auto availableDeviceExtensions = enumerateDeviceExtenstions(_vgi.phydev);
    auto enabledDeviceExtensions   = validateExtensions(availableDeviceExtensions, askedDeviceExtensions);

    // create device, one queue for each family
    float                                queuePriority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfo;
    for (uint32_t i = 0; i < families.size(); ++i) {
        queueCreateInfo.push_back(VkDeviceQueueCreateInfo {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0, i, 1, &queuePriority});
    }

    // create device
    VkDeviceCreateInfo deviceCreateInfo = {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        &deviceFeatures, // Vulkan >= 1.1 uses pNext to enable features, not pEnabledFeatures
        0,               // flags
        (uint32_t) queueCreateInfo.size(),
        queueCreateInfo.data(),
        0,
        nullptr, // layers
        (uint32_t) enabledDeviceExtensions.size(),
        enabledDeviceExtensions.data(),
    };
    GN_VK_REQUIRE(vkCreateDevice(_vgi.phydev, &deviceCreateInfo, _vgi.allocator, &_vgi.device));

    // TODO: intialize device specific dispatcher.`

    //     // initialize a memory allocator for Vulkan images
    //     if (_cp.useVmaAllocator) {
    //         VmaVulkanFunctions vf {};
    //         vf.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
    //         vf.vkGetDeviceProcAddr   = vkGetDeviceProcAddr;
    //         VmaAllocatorCreateInfo ai {};
    //         ai.vulkanApiVersion = _cp.instance->cp().apiVersion;
    //         ai.physicalDevice   = _vgi.phydev;
    //         ai.device           = _vgi.device;
    //         ai.instance         = _vgi.instance;
    //         ai.pVulkanFunctions = &vf;

    // #if 0 // uncomment this section to enable vma allocation recording
    //         VmaRecordSettings vmaRecordSettings;
    //         vmaRecordSettings.pFilePath = "vmaReplay.csv";
    //         vmaRecordSettings.flags = VMA_RECORD_FLUSH_AFTER_CALL_BIT;
    //         ai.pRecordSettings = &vmaRecordSettings;
    // #endif

    //         if (askedDeviceExtensions.find(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME) != askedDeviceExtensions.end()) {
    //             GN_INFO(sLogger)("Enable VMA allocator with buffer device address.");
    //             ai.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
    //         }
    //         GN_VK_REQUIRE(vmaCreateAllocator(&ai, &_vgi.vmaAllocator));
    //     }

    // print device information
    if (cp.printVkInfo) {
        printDeviceFeatures(_vgi.phydev, deviceFeatures, verbose);
        printDeviceExtenstions(_vgi.phydev, availableDeviceExtensions, enabledDeviceExtensions, verbose);
    }

    // classify queue families. create command pool for each of them.
    _queues.resize(families.size());
    for (uint32_t i = 0; i < families.size(); ++i) {
        const auto & f = families[i];

        // create an submission proxy for each queue.
        _queues[i] = std::make_unique<SimpleQueue>(SimpleQueue::ConstructParameters{_vgi, i});

        // classify all queues
        if (!_graphics && f.queueFlags & VK_QUEUE_GRAPHICS_BIT) _graphics = _queues[i].get();
        
        if (!_compute && !(f.queueFlags & VK_QUEUE_GRAPHICS_BIT) && (f.queueFlags & VK_QUEUE_COMPUTE_BIT)) _compute = _queues[i].get();
        
        if (!_transfer && !(f.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(f.queueFlags & VK_QUEUE_COMPUTE_BIT) && (f.queueFlags & VK_QUEUE_TRANSFER_BIT)) _transfer = _queues[i].get();
        
        if (!_present && presenting) {
            VkBool32 supportPresenting = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(_vgi.phydev, i, _cp.surface, &supportPresenting);
            if (supportPresenting) _present = _queues[i].get();
        }
    }

    GN_INFO(sLogger)("Vulkan device initialized.");
}

// ---------------------------------------------------------------------------------------------------------------------
//
SimpleVulkanDevice::~SimpleVulkanDevice() {
    delete _details;
    _queues.clear();
    // if (_vgi.vmaAllocator) vmaDestroyAllocator(_vgi.vmaAllocator), _vgi.vmaAllocator = nullptr;
    if (_vgi.device) {
        GN_INFO(sLogger)("[SimpleVulkanDevice] destroying device...");
        vkDestroyDevice(_vgi.device, _vgi.allocator);
        _vgi.device = nullptr;
        GN_INFO(sLogger)("[SimpleVulkanDevice] device destroyed");
    }
}

// ---------------------------------------------------------------------------------------------------------------------
//
GN_API void muteValidationErrorLog() { ++debugMuteCounter; }

// ---------------------------------------------------------------------------------------------------------------------
//
GN_API void unmuteValidationErrorLog() { --debugMuteCounter; }

} // namespace GN::vulkan
