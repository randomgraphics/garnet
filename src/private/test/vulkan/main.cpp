#include "pch.h"
#include <vector>
#include <unordered_map>

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.test.vulkan");

StrA VkResultToString(VkResult error)
{
    static std::unordered_map<VkResult, StrA> table {
        std::make_pair(VK_SUCCESS, "VK_SUCCESS"),
        std::make_pair(VK_NOT_READY, "VK_NOT_READY"),
        std::make_pair(VK_TIMEOUT, "VK_TIMEOUT"),
        std::make_pair(VK_EVENT_SET, "VK_EVENT_SET"),
        std::make_pair(VK_EVENT_RESET, "VK_EVENT_RESET"),
        std::make_pair(VK_INCOMPLETE, "VK_INCOMPLETE"),
        std::make_pair(VK_ERROR_OUT_OF_HOST_MEMORY, "VK_ERROR_OUT_OF_HOST_MEMORY"),
        std::make_pair(VK_ERROR_OUT_OF_DEVICE_MEMORY, "VK_ERROR_OUT_OF_DEVICE_MEMORY"),
        std::make_pair(VK_ERROR_INITIALIZATION_FAILED, "VK_ERROR_INITIALIZATION_FAILED"),
        std::make_pair(VK_ERROR_DEVICE_LOST, "VK_ERROR_DEVICE_LOST"),
        std::make_pair(VK_ERROR_MEMORY_MAP_FAILED, "VK_ERROR_MEMORY_MAP_FAILED"),
        std::make_pair(VK_ERROR_LAYER_NOT_PRESENT, "VK_ERROR_LAYER_NOT_PRESENT"),
        std::make_pair(VK_ERROR_EXTENSION_NOT_PRESENT, "VK_ERROR_EXTENSION_NOT_PRESENT"),
        std::make_pair(VK_ERROR_FEATURE_NOT_PRESENT, "VK_ERROR_FEATURE_NOT_PRESENT"),
        std::make_pair(VK_ERROR_INCOMPATIBLE_DRIVER, "VK_ERROR_INCOMPATIBLE_DRIVER"),
        std::make_pair(VK_ERROR_TOO_MANY_OBJECTS, "VK_ERROR_TOO_MANY_OBJECTS"),
        std::make_pair(VK_ERROR_FORMAT_NOT_SUPPORTED, "VK_ERROR_FORMAT_NOT_SUPPORTED"),
        std::make_pair(VK_ERROR_FRAGMENTED_POOL, "VK_ERROR_FRAGMENTED_POOL"),
        std::make_pair(VK_ERROR_SURFACE_LOST_KHR, "VK_ERROR_SURFACE_LOST_KHR"),
        std::make_pair(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR, "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR"),
        std::make_pair(VK_SUBOPTIMAL_KHR, "VK_SUBOPTIMAL_KHR"),
        std::make_pair(VK_ERROR_OUT_OF_DATE_KHR, "VK_ERROR_OUT_OF_DATE_KHR"),
        std::make_pair(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR, "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR"),
        std::make_pair(VK_ERROR_VALIDATION_FAILED_EXT, "VK_ERROR_VALIDATION_FAILED_EXT"),
        std::make_pair(VK_ERROR_INVALID_SHADER_NV, "VK_ERROR_INVALID_SHADER_NV"),
        std::make_pair(VK_RESULT_BEGIN_RANGE, "VK_RESULT_BEGIN_RANGE"),
        std::make_pair(VK_RESULT_END_RANGE, "VK_RESULT_END_RANGE"),
        std::make_pair(VK_RESULT_RANGE_SIZE, "VK_RESULT_RANGE_SIZE"),
    };

    auto iter = table.find(error);
    if (iter != table.end()) return iter->second;

    return str::format("Unrecognized VkResult 0x%08X", error);
}

#define CHECK_VK(x) if (1) { auto result__ = (x); if (VK_SUCCESS != (result__)) { GN_ERROR(sLogger)("%s failed: %s", #x, VkResultToString(result__).rawptr()); return false; } } else void(0)

#define CHECK_BOOL(x) if (!(x)) { GN_ERROR(sLogger)("%s returned false.", #x); return false; } else void(0)

struct App {
    bool           mSuccess = false;
    VkAllocationCallbacks * mAllocator = nullptr;
    win::Window *  mWin = nullptr;
    VkInstance     mInstance = nullptr;
    VkDebugReportCallbackEXT mDebugReport = nullptr;
    VkSurfaceKHR   mSurface = nullptr;
    VkDevice       mDevice = nullptr;
    VkQueue        mGraphicsQueue = nullptr, mPresentQueue = nullptr;
    VkSwapchainKHR mSwapchain = nullptr;
    std::vector<VkImageView> mBackBuffers;
    VkSemaphore    mBackBufferAvailableSemaphore = nullptr;
    VkSemaphore    mRenderFinishedSemaphore = nullptr;

    VkRenderPass     mRenderPass = nullptr;
    VkPipelineLayout mPipelineLayout = nullptr;
    VkPipeline       mPipeline = nullptr;
    std::vector<VkFramebuffer> mFramebuffers;

    VkCommandPool    mCommandPool = nullptr;
    std::vector<VkCommandBuffer> mCommandBuffers;

    App() {
        mSuccess = initVulkan();
    }

    ~App() {
        if (mCommandPool) vkDestroyCommandPool(mDevice, mCommandPool, mAllocator);
        for(auto i: mFramebuffers) vkDestroyFramebuffer(mDevice, i, mAllocator);
        if (mPipeline) vkDestroyPipeline(mDevice, mPipeline, mAllocator);
        if (mPipelineLayout) vkDestroyPipelineLayout(mDevice, mPipelineLayout, mAllocator);
        if (mRenderPass) vkDestroyRenderPass(mDevice, mRenderPass, mAllocator);
        if (mRenderFinishedSemaphore) vkDestroySemaphore(mDevice, mRenderFinishedSemaphore, mAllocator);
        if (mBackBufferAvailableSemaphore) vkDestroySemaphore(mDevice, mBackBufferAvailableSemaphore, mAllocator);
        for(auto i : mBackBuffers) vkDestroyImageView(mDevice, i, mAllocator);
        if (mSwapchain) vkDestroySwapchainKHR(mDevice, mSwapchain, mAllocator);
        if (mDevice) vkDestroyDevice(mDevice, mAllocator);
        if (mSurface) vkDestroySurfaceKHR(mInstance, mSurface, mAllocator);
        if (mDebugReport) vkDestroyDebugReportCallbackEXT(mInstance, mDebugReport, mAllocator);
        if (mInstance) vkDestroyInstance(mInstance, mAllocator);
        if (mWin) delete mWin;
    }

    bool initVulkan() {
        // query layers
        uint32 count;
        vkEnumerateInstanceLayerProperties(&count, nullptr);
        std::vector<VkLayerProperties> availableLayers(count);
        vkEnumerateInstanceLayerProperties(&count, availableLayers.data());
        StrA s = "available Vulkan layers:\n";
        for (const auto & l : availableLayers) {
            s += str::format("\t%s\n", l.layerName);
        }
        GN_INFO(sLogger)(s);

        // query extensions
        vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(count);
        vkEnumerateInstanceExtensionProperties(nullptr, &count, availableExtensions.data());
        s = "available Vulkan extensions:\n";
        for (const auto & e : availableExtensions) {
            s += str::format("\t%s\n", e.extensionName);
        }
        GN_INFO(sLogger)(s);

        // create VK instance
        std::vector<const char*> layers = {
            "VK_LAYER_LUNARG_standard_validation"
        };
        std::vector<const char *> extensions = {
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        };
        auto ci = VkInstanceCreateInfo {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            nullptr,
            0, // flags,
            nullptr,
            (uint32)layers.size(), layers.data(),
            (uint32)extensions.size(), extensions.data()
        };
        CHECK_VK(vkCreateInstance(&ci, nullptr, &mInstance));

        // setup debug callback
        VkDebugReportCallbackCreateInfoEXT debugCallbackCreateInfo = {};
        debugCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        debugCallbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        debugCallbackCreateInfo.pfnCallback = debugCallback;
        CHECK_VK(vkCreateDebugReportCallbackEXT(mInstance, &debugCallbackCreateInfo, mAllocator, &mDebugReport));

        // create a window
        const uint32 WIDTH = 1960;
        const uint32 HEIGHT = 1080;
        mWin = win::createWindow(win::WindowCreationParameters{
            "vulkan test",
            0, // parent
            WIDTH, HEIGHT,
            true, true, false, true
        });

        // create a surface
        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.hwnd = (HWND)mWin->getWindowHandle();
        surfaceCreateInfo.hinstance = (HINSTANCE)mWin->getModuleHandle();
        CHECK_VK(vkCreateWin32SurfaceKHR(mInstance, &surfaceCreateInfo, mAllocator, &mSurface));

        // enumverate physical device
        CHECK_VK(vkEnumeratePhysicalDevices(mInstance, &count, nullptr));
        std::vector<VkPhysicalDevice> devices(count);
        CHECK_VK(vkEnumeratePhysicalDevices(mInstance, &count, devices.data()));

        // Pick a physical device. For simplicity, pick the first one for now.
        auto selectedPhysicalDevice = devices[0];
        #if 0
        for(const auto & d : devices) {
            VkPhysicalDeviceFeatures f;
            CHECK_VK(vkGetPhysicalDeviceFeatures(d, &f));
            VkPhysicalDeviceProperties p;
            CHECK_VK(vkGetPhysicalDeviceProperties(d, &p));
            // TODO: check features and properties
        }
        #endif

        // query device extensions
        std::vector<VkExtensionProperties> availableDeviceExtensions;
        vkEnumerateDeviceExtensionProperties(selectedPhysicalDevice, nullptr, &count, nullptr);
        availableDeviceExtensions.resize(count);
        vkEnumerateDeviceExtensionProperties(selectedPhysicalDevice, nullptr, &count, availableDeviceExtensions.data());
        s = "available Vulkan device extensions:\n";
        for (const auto & e : availableDeviceExtensions) {
            s += str::format("\t%s\n", e.extensionName);
        }
        GN_INFO(sLogger)(s);

        // query queues
        int graphicsQueue = -1;
        int presentQueue = -1;
        vkGetPhysicalDeviceQueueFamilyProperties(selectedPhysicalDevice, &count, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(count);
        vkGetPhysicalDeviceQueueFamilyProperties(selectedPhysicalDevice, &count, queueFamilies.data());
        for(uint32 i = 0; i < queueFamilies.size(); ++i) {
            const auto & p = queueFamilies[i];
            if (0 == p.queueCount) continue;
            if (graphicsQueue < 0 && p.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                graphicsQueue = i;
            }
            if (presentQueue < 0) {
                VkBool32 presentSupport;
                vkGetPhysicalDeviceSurfaceSupportKHR(selectedPhysicalDevice, i, mSurface, &presentSupport);
                if (presentSupport) {
                    presentQueue = i;
                }
            }
        }
        if (graphicsQueue < 0 || presentQueue < 0) {
            GN_ERROR(sLogger)("the VK device needs to support both graphics and present queue.");
            return false;
        }

        // create logical device
        float queuePriority = 1.0f;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfo;
        queueCreateInfo.push_back(VkDeviceQueueCreateInfo{
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0,
            (uint32)graphicsQueue,
            1,
            &queuePriority
        });
        if (presentQueue != graphicsQueue) {
            queueCreateInfo.push_back(VkDeviceQueueCreateInfo{
                VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0,
                (uint32)presentQueue,
                1,
                &queuePriority
            });
        }
        VkPhysicalDeviceFeatures deviceFeatures = {};
        std::vector<const char *> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };
        VkDeviceCreateInfo deviceCreateInfo = {
            VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, nullptr, 0,
            (uint32)queueCreateInfo.size(), queueCreateInfo.data(),
            (uint32)layers.size(), layers.data(),
            (uint32)deviceExtensions.size(), deviceExtensions.data(),
            &deviceFeatures,
        };
        CHECK_VK(vkCreateDevice(selectedPhysicalDevice, &deviceCreateInfo, mAllocator, &mDevice));
        //CHECK_BOOL(vkelDeviceInit(selectedPhysicalDevice, mDevice));

        // acquire queue handles
        vkGetDeviceQueue(mDevice, graphicsQueue, 0, &mGraphicsQueue);
        vkGetDeviceQueue(mDevice, presentQueue, 0, &mPresentQueue);

        // create swap chain
        VkSurfaceCapabilitiesKHR surfaceCaps = {};
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(selectedPhysicalDevice, mSurface, &surfaceCaps);
        std::vector<uint32> queueIndices;
        if (graphicsQueue != presentQueue) {
            queueIndices.push_back(graphicsQueue);
            queueIndices.push_back(presentQueue);
        }
        VkSwapchainCreateInfoKHR swapChainCreateInfo = {
            VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, nullptr, 0,
            mSurface,
            2, // double buffer
            VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
            { WIDTH, HEIGHT },
            1, // image array layers (2 for stereoscopic rendering)
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            graphicsQueue == presentQueue ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
            (uint32)queueIndices.size(), queueIndices.data(),
            surfaceCaps.currentTransform,
            VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            VK_PRESENT_MODE_FIFO_KHR, // mailbox?
            true, // clipped.
            nullptr, // old swap chain
        };
        CHECK_VK(vkCreateSwapchainKHR(mDevice, &swapChainCreateInfo, mAllocator, &mSwapchain));

        // acquire back buffers
        std::vector<VkImage> swapchainImages;
        vkGetSwapchainImagesKHR(mDevice, mSwapchain, &count, nullptr);
        swapchainImages.resize(count);
        vkGetSwapchainImagesKHR(mDevice, mSwapchain, &count, swapchainImages.data());
        mBackBuffers.resize(swapchainImages.size());
        for (size_t i = 0; i < swapchainImages.size(); i++) {
            VkImageViewCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapchainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapChainCreateInfo.imageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            CHECK_VK(vkCreateImageView(mDevice, &createInfo, mAllocator, &mBackBuffers[i]));
        }

        // create back buffer semaphore
        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        CHECK_VK(vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr, &mBackBufferAvailableSemaphore));
        CHECK_VK(vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr, &mRenderFinishedSemaphore));

        // create pipeline object
        CHECK_BOOL(createGraphicsPipeline(WIDTH, HEIGHT, swapChainCreateInfo.imageFormat));

        // create frame buffers (render targets)
        mFramebuffers.resize(swapchainImages.size());
        for (size_t i = 0; i < swapchainImages.size(); i++) {
            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = mRenderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = &mBackBuffers[i];
            framebufferInfo.width = WIDTH;
            framebufferInfo.height = HEIGHT;
            framebufferInfo.layers = 1;
            CHECK_VK(vkCreateFramebuffer(mDevice, &framebufferInfo, mAllocator, &mFramebuffers[i]));
        }

        // create command buffer pool
        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = graphicsQueue;
        poolInfo.flags = 0; // Optional
        CHECK_VK(vkCreateCommandPool(mDevice, &poolInfo, mAllocator, &mCommandPool));

        // create primary command buffers
        mCommandBuffers.resize(swapchainImages.size());
        VkCommandBufferAllocateInfo cmdbufAllocInfo = {};
        cmdbufAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdbufAllocInfo.commandPool = mCommandPool;
        cmdbufAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        cmdbufAllocInfo.commandBufferCount = (uint32_t) mCommandBuffers.size();
        CHECK_VK(vkAllocateCommandBuffers(mDevice, &cmdbufAllocInfo, mCommandBuffers.data()));

        // Record commmands
        for (size_t i = 0; i < mCommandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
            beginInfo.pInheritanceInfo = nullptr; // Optional
            CHECK_VK(vkBeginCommandBuffer(mCommandBuffers[i], &beginInfo));

            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = mRenderPass;
            renderPassInfo.framebuffer = mFramebuffers[i];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = { WIDTH, HEIGHT };
            VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;
            vkCmdBeginRenderPass(mCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(mCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);
            vkCmdDraw(mCommandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(mCommandBuffers[i]);
            CHECK_VK(vkEndCommandBuffer(mCommandBuffers[i]));
        }

        // done
        GN_INFO(sLogger)("Vulkan initialized!");
        return true;
    }

    bool createGraphicsPipeline(uint32 width, uint32 height, VkFormat format) {
        const uint32 vsbin[] = {
             #include "shaders/vert.h"
        };
        VkShaderModuleCreateInfo shaderCreateInfo = {};
        shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize = sizeof(vsbin);
        shaderCreateInfo.pCode = vsbin;
        VkShaderModule vs;
        CHECK_VK(vkCreateShaderModule(mDevice, &shaderCreateInfo, nullptr, &vs));

        const uint32 psbin[] = {
            #include "shaders/frag.h"
        };
        VkShaderModule ps;
        shaderCreateInfo.codeSize = sizeof(psbin);
        shaderCreateInfo.pCode = psbin;
        CHECK_VK(vkCreateShaderModule(mDevice, &shaderCreateInfo, nullptr, &ps));

        VkPipelineShaderStageCreateInfo shaderStages[] = {
            {   VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                nullptr, // next
                0,  // flags
                VK_SHADER_STAGE_VERTEX_BIT,
                vs, "main",
                nullptr, //pSpecializationInfo;
            },
            {   VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                nullptr, // next
                0,  // flags
                VK_SHADER_STAGE_FRAGMENT_BIT,
                ps, "main",
                nullptr, //pSpecializationInfo;
            },
        };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport = { 0, 0, (float)width, (float)height, .0f, 1.0f };
        VkRect2D scissor = { { 0, 0, }, { width, height } };
        VkPipelineViewportStateCreateInfo viewportState = {
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, nullptr, 0,
            1, &viewport,
            1, &scissor,
        };

        VkPipelineRasterizationStateCreateInfo rasterizer = {
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, nullptr, 0,
            false, // depth clamp,
            false, // rasterizer discard
            VK_POLYGON_MODE_FILL,
            VK_CULL_MODE_BACK_BIT,
            VK_FRONT_FACE_CLOCKWISE,
            false, 0, 0, 0, // depth bias
            1.0f, // line width
        };

        VkPipelineMultisampleStateCreateInfo multisampling = {
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, nullptr, 0,
            VK_SAMPLE_COUNT_1_BIT,
            false, // sample shading
            1.0f, // min sample shading
            nullptr, // sample mask
            false, // alpha to coverage
            false, // alpha to one
        };

        // color blending
        VkPipelineColorBlendAttachmentState colorBlendAttachment = {
            false, // blending disabled
            VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, // color blend src/dst/op
            VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, // alpha blend src/dst/op
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        };
        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;

        VkDynamicState dynamicStates[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_LINE_WIDTH
        };
        VkPipelineDynamicStateCreateInfo dynamicState = {
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, nullptr, 0,
            _countof(dynamicStates),
            dynamicStates,
        };

        // create pipeline layout
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = 0; // Optional
        CHECK_VK(vkCreatePipelineLayout(mDevice, &pipelineLayoutInfo, nullptr, &mPipelineLayout));

        // create render pass
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstSubpass = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;
        CHECK_VK(vkCreateRenderPass(mDevice, &renderPassInfo, nullptr, &mRenderPass));

        // create pipeline
        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr; // Optional
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = nullptr; // Optional
        pipelineInfo.layout = mPipelineLayout;
        pipelineInfo.renderPass = mRenderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional
        CHECK_VK(vkCreateGraphicsPipelines(mDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mPipeline));

        // done
        vkDestroyShaderModule(mDevice, vs, nullptr);
        vkDestroyShaderModule(mDevice, ps, nullptr);
        return true;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugReportFlagsEXT,// flags,
        VkDebugReportObjectTypeEXT,// objType,
        uint64_t,// obj,
        size_t,// location,
        int32_t,// code,
        const char*,// layerPrefix,
        const char* msg,
        void* userData) {
        GN_UNUSED_PARAM(userData);
        GN_INFO(sLogger)("[vulkan] %s", msg);
        return VK_FALSE;
    }

    static VkResult vkCreateDebugReportCallbackEXT(VkInstance instance,
                                                 const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
                                                 const VkAllocationCallbacks* pAllocator,
                                                 VkDebugReportCallbackEXT* pCallback) {
        auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pCallback);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static void vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
        if (func != nullptr) {
            func(instance, callback, pAllocator);
        }
    }

    int run() {
        mWin->show();
        while(mWin->runUntilNoNewEvents()) {
            // acquire back buffer
            uint32_t imageIndex;
            vkAcquireNextImageKHR(mDevice, mSwapchain, (uint64_t)-1, mBackBufferAvailableSemaphore, VK_NULL_HANDLE, &imageIndex); // TODO: handle errors

            // submit the command buffer
            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            VkSemaphore waitSemaphores[] = { mBackBufferAvailableSemaphore };
            VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = waitSemaphores;
            submitInfo.pWaitDstStageMask = waitStages;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &mCommandBuffers[imageIndex];
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = &mRenderFinishedSemaphore;
            CHECK_VK(vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));

            // present
            VkPresentInfoKHR presentInfo = {};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &mRenderFinishedSemaphore;
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &mSwapchain;
            presentInfo.pImageIndices = &imageIndex;
            presentInfo.pResults = nullptr; // Optional
            vkQueuePresentKHR(mPresentQueue, &presentInfo); // TODO: handle errors
        }
        vkDeviceWaitIdle(mDevice);
        return 0;
    }
};

int main() {
    App app;
    return app.mSuccess ? app.run() : -1;
}
