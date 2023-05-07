#include "pch.h"
#include <vector>
#include <unordered_map>

using namespace GN;
using namespace GN::vulkan;

static GN::Logger * sLogger = GN::getLogger("GN.test.vulkan");

#define CHECK_VK(x) GN_VK_CHK(x, return false;)

#define CHECK_BOOL(x)                                \
    if (!(x)) {                                      \
        GN_ERROR(sLogger)("%s returned false.", #x); \
        return false;                                \
    } else                                           \
        void(0)

struct App {
    bool                     mSuccess  = false;
    win::Window *            mWin      = nullptr;
    SimpleInstance *         mInstance = nullptr;
    VkSurfaceKHR             mSurface  = 0;
    SimpleDevice *           mDevice   = nullptr;
    VulkanGlobalInfo         mVgi {};
    VkSwapchainKHR           mSwapchain = 0;
    std::vector<VkImageView> mBackBuffers;
    VkSemaphore              mBackBufferAvailableSemaphore = 0;
    VkSemaphore              mRenderFinishedSemaphore      = 0;

    VkRenderPass               mRenderPass     = 0;
    VkPipelineLayout           mPipelineLayout = 0;
    VkPipeline                 mPipeline       = 0;
    std::vector<VkFramebuffer> mFramebuffers;

    VkCommandPool                mCommandPool = 0;
    std::vector<VkCommandBuffer> mCommandBuffers;

    App() { mSuccess = initVulkan(); }

    ~App() {
        cleanupSwapchain();
        if (mCommandPool) vkDestroyCommandPool(mVgi.device, mCommandPool, mVgi.allocator);
        safeDelete(mDevice);
        if (mSurface) vkDestroySurfaceKHR(mVgi.instance, mSurface, mVgi.allocator);
        safeDelete(mInstance);
        if (mWin) delete mWin;
    }

    bool initVulkan() {
        // create instance
        mInstance = new SimpleInstance(SimpleInstance::ConstructParameters {
            .instanceExtensions = {{VK_KHR_SURFACE_EXTENSION_NAME, true}, {VK_KHR_WIN32_SURFACE_EXTENSION_NAME, true}},
            .validation         = SimpleInstance::BREAK_ON_VK_ERROR,
            .printVkInfo        = SimpleInstance::VERBOSE,
        });

        // create a window
        const uint32 WIDTH  = 1960;
        const uint32 HEIGHT = 1080;
        mWin                = win::createWindow({
            "vulkan test", WIDTH, HEIGHT,
            0,     // diplay
            0,     // monitor
            0,     // parent
            true,  // border
            true,  // title
            false, // top most
            true   // close box
        });
        mWin->show();

        // create a surface
        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
        surfaceCreateInfo.sType                       = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.hwnd                        = (HWND) mWin->getWindowHandle();
        surfaceCreateInfo.hinstance                   = (HINSTANCE) mWin->getModuleHandle();
        CHECK_VK(vkCreateWin32SurfaceKHR(*mInstance, &surfaceCreateInfo, nullptr, &mSurface));

        // create device
        mDevice = new SimpleDevice({
            .instance = mInstance,
            .surface  = mSurface,
        });
        mVgi    = mDevice->vgi();

        // create command buffer pool
        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex        = mDevice->graphics()->family();
        poolInfo.flags                   = 0; // Optional
        CHECK_VK(vkCreateCommandPool(mVgi.device, &poolInfo, mVgi.allocator, &mCommandPool));

        CHECK_BOOL(createSwapchain());

        // done
        GN_INFO(sLogger)("Vulkan initialized!");
        return true;
    }

    bool createSwapchain() {
        cleanupSwapchain();

        // query window size
        auto windowSize = mWin->getClientSize();

        // create swap chain
        VkSurfaceCapabilitiesKHR surfaceCaps = {};
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mVgi.phydev, mSurface, &surfaceCaps);
        std::vector<uint32> queueIndices;
        if (mDevice->separatePresentQueue()) {
            queueIndices.push_back(mDevice->graphics()->family());
            queueIndices.push_back(mDevice->present()->family());
        }
        VkSwapchainCreateInfoKHR swapChainCreateInfo = {
            VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            nullptr,
            0,
            mSurface,
            2, // double buffer
            VK_FORMAT_B8G8R8A8_UNORM,
            VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
            {windowSize.x, windowSize.y},
            1, // image array layers (2 for stereoscopic rendering)
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            mDevice->separatePresentQueue() ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
            (uint32) queueIndices.size(),
            queueIndices.data(),
            surfaceCaps.currentTransform,
            VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            VK_PRESENT_MODE_FIFO_KHR, // mailbox?
            true,                     // clipped.
            0,                        // old swap chain
        };
        CHECK_VK(vkCreateSwapchainKHR(mVgi.device, &swapChainCreateInfo, mVgi.allocator, &mSwapchain));

        // acquire back buffers
        uint32               count;
        std::vector<VkImage> swapchainImages;
        vkGetSwapchainImagesKHR(mVgi.device, mSwapchain, &count, nullptr);
        swapchainImages.resize(count);
        vkGetSwapchainImagesKHR(mVgi.device, mSwapchain, &count, swapchainImages.data());
        mBackBuffers.resize(swapchainImages.size());
        for (size_t i = 0; i < swapchainImages.size(); i++) {
            VkImageViewCreateInfo createInfo           = {};
            createInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image                           = swapchainImages[i];
            createInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format                          = swapChainCreateInfo.imageFormat;
            createInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel   = 0;
            createInfo.subresourceRange.levelCount     = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount     = 1;
            CHECK_VK(vkCreateImageView(mVgi.device, &createInfo, mVgi.allocator, &mBackBuffers[i]));
        }

        // create back buffer semaphore
        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType                 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        CHECK_VK(vkCreateSemaphore(mVgi.device, &semaphoreInfo, nullptr, &mBackBufferAvailableSemaphore));
        CHECK_VK(vkCreateSemaphore(mVgi.device, &semaphoreInfo, nullptr, &mRenderFinishedSemaphore));

        // create pipeline object
        CHECK_BOOL(createGraphicsPipeline(windowSize.x, windowSize.y, swapChainCreateInfo.imageFormat));

        // create frame buffers (render targets)
        mFramebuffers.resize(swapchainImages.size());
        for (size_t i = 0; i < swapchainImages.size(); i++) {
            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass              = mRenderPass;
            framebufferInfo.attachmentCount         = 1;
            framebufferInfo.pAttachments            = &mBackBuffers[i];
            framebufferInfo.width                   = windowSize.x;
            framebufferInfo.height                  = windowSize.y;
            framebufferInfo.layers                  = 1;
            CHECK_VK(vkCreateFramebuffer(mVgi.device, &framebufferInfo, mVgi.allocator, &mFramebuffers[i]));
        }

        // create primary command buffers
        mCommandBuffers.resize(swapchainImages.size());
        VkCommandBufferAllocateInfo cmdbufAllocInfo = {};
        cmdbufAllocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdbufAllocInfo.commandPool                 = mCommandPool;
        cmdbufAllocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        cmdbufAllocInfo.commandBufferCount          = (uint32_t) mCommandBuffers.size();
        CHECK_VK(vkAllocateCommandBuffers(mVgi.device, &cmdbufAllocInfo, mCommandBuffers.data()));

        // Record commmands
        for (size_t i = 0; i < mCommandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
            beginInfo.pInheritanceInfo         = nullptr; // Optional
            CHECK_VK(vkBeginCommandBuffer(mCommandBuffers[i], &beginInfo));

            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass            = mRenderPass;
            renderPassInfo.framebuffer           = mFramebuffers[i];
            renderPassInfo.renderArea.offset     = {0, 0};
            renderPassInfo.renderArea.extent     = {windowSize.x, windowSize.y};
            VkClearValue clearColor              = {0.0f, 0.0f, 0.0f, 1.0f};
            renderPassInfo.clearValueCount       = 1;
            renderPassInfo.pClearValues          = &clearColor;
            vkCmdBeginRenderPass(mCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(mCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);
            vkCmdDraw(mCommandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(mCommandBuffers[i]);
            CHECK_VK(vkEndCommandBuffer(mCommandBuffers[i]));
        }

        // done
        return true;
    }

    void cleanupSwapchain() {
        // cleanup all command buffers.
        if (!mCommandBuffers.empty()) vkFreeCommandBuffers(mVgi.device, mCommandPool, (uint32_t) mCommandBuffers.size(), mCommandBuffers.data());

        for (auto i : mFramebuffers) {
            vkDestroyFramebuffer(mVgi.device, i, mVgi.allocator);
            mFramebuffers.clear();
        }
        if (mPipeline) vkDestroyPipeline(mVgi.device, mPipeline, mVgi.allocator), mPipeline = 0;
        if (mPipelineLayout) vkDestroyPipelineLayout(mVgi.device, mPipelineLayout, mVgi.allocator), mPipelineLayout = 0;
        if (mRenderPass) vkDestroyRenderPass(mVgi.device, mRenderPass, mVgi.allocator), mRenderPass = 0;
        if (mRenderFinishedSemaphore) vkDestroySemaphore(mVgi.device, mRenderFinishedSemaphore, mVgi.allocator), mRenderFinishedSemaphore = 0;
        if (mBackBufferAvailableSemaphore) vkDestroySemaphore(mVgi.device, mBackBufferAvailableSemaphore, mVgi.allocator), mBackBufferAvailableSemaphore = 0;
        for (auto i : mBackBuffers) { vkDestroyImageView(mVgi.device, i, mVgi.allocator), mBackBuffers.clear(); }
        if (mSwapchain) vkDestroySwapchainKHR(mVgi.device, mSwapchain, mVgi.allocator), mSwapchain = 0;
    }

    bool createGraphicsPipeline(uint32 width, uint32 height, VkFormat format) {
        const uint32 vsbin[] = {
#include "shaders/vert.h"
        };
        VkShaderModuleCreateInfo shaderCreateInfo = {};
        shaderCreateInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize                 = sizeof(vsbin);
        shaderCreateInfo.pCode                    = vsbin;
        VkShaderModule vs;
        CHECK_VK(vkCreateShaderModule(mVgi.device, &shaderCreateInfo, nullptr, &vs));

        const uint32 psbin[] = {
#include "shaders/frag.h"
        };
        VkShaderModule ps;
        shaderCreateInfo.codeSize = sizeof(psbin);
        shaderCreateInfo.pCode    = psbin;
        CHECK_VK(vkCreateShaderModule(mVgi.device, &shaderCreateInfo, nullptr, &ps));

        VkPipelineShaderStageCreateInfo shaderStages[] = {
            {
                VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                nullptr, // next
                0,       // flags
                VK_SHADER_STAGE_VERTEX_BIT, vs, "main",
                nullptr, // pSpecializationInfo;
            },
            {
                VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                nullptr, // next
                0,       // flags
                VK_SHADER_STAGE_FRAGMENT_BIT, ps, "main",
                nullptr, // pSpecializationInfo;
            },
        };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology                               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable                 = VK_FALSE;

        VkViewport                        viewport      = {0, 0, (float) width, (float) height, .0f, 1.0f};
        VkRect2D                          scissor       = {{
                                0,
                                0,
                            },
                            {width, height}};
        VkPipelineViewportStateCreateInfo viewportState = {
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, nullptr, 0, 1, &viewport, 1, &scissor,
        };

        VkPipelineRasterizationStateCreateInfo rasterizer = {
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            nullptr,
            0,
            false, // depth clamp,
            false, // rasterizer discard
            VK_POLYGON_MODE_FILL,
            VK_CULL_MODE_BACK_BIT,
            VK_FRONT_FACE_CLOCKWISE,
            false,
            0,
            0,
            0,    // depth bias
            1.0f, // line width
        };

        VkPipelineMultisampleStateCreateInfo multisampling = {
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            nullptr,
            0,
            VK_SAMPLE_COUNT_1_BIT,
            false,   // sample shading
            1.0f,    // min sample shading
            nullptr, // sample mask
            false,   // alpha to coverage
            false,   // alpha to one
        };

        // color blending
        VkPipelineColorBlendAttachmentState colorBlendAttachment = {
            false, // blending disabled
            VK_BLEND_FACTOR_ONE,
            VK_BLEND_FACTOR_ZERO,
            VK_BLEND_OP_ADD, // color blend src/dst/op
            VK_BLEND_FACTOR_ONE,
            VK_BLEND_FACTOR_ZERO,
            VK_BLEND_OP_ADD, // alpha blend src/dst/op
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        };
        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType                               = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable                       = VK_FALSE;
        colorBlending.logicOp                             = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount                     = 1;
        colorBlending.pAttachments                        = &colorBlendAttachment;

        VkDynamicState                   dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};
        VkPipelineDynamicStateCreateInfo dynamicState    = {
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, nullptr, 0, (uint32_t) countof(dynamicStates), dynamicStates,
        };

        // create pipeline layout
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount             = 0;       // Optional
        pipelineLayoutInfo.pSetLayouts                = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount     = 0;       // Optional
        pipelineLayoutInfo.pPushConstantRanges        = 0;       // Optional
        CHECK_VK(vkCreatePipelineLayout(mVgi.device, &pipelineLayoutInfo, nullptr, &mPipelineLayout));

        // create render pass
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format                  = format;
        colorAttachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp                  = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout             = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment            = 0;
        colorAttachmentRef.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments    = &colorAttachmentRef;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
        dependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask       = 0;
        dependency.dstSubpass          = 0;
        dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount        = 1;
        renderPassInfo.pAttachments           = &colorAttachment;
        renderPassInfo.subpassCount           = 1;
        renderPassInfo.pSubpasses             = &subpass;
        renderPassInfo.dependencyCount        = 1;
        renderPassInfo.pDependencies          = &dependency;
        CHECK_VK(vkCreateRenderPass(mVgi.device, &renderPassInfo, nullptr, &mRenderPass));

        // create pipeline
        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount                   = 2;
        pipelineInfo.pStages                      = shaderStages;
        pipelineInfo.pVertexInputState            = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState          = &inputAssembly;
        pipelineInfo.pViewportState               = &viewportState;
        pipelineInfo.pRasterizationState          = &rasterizer;
        pipelineInfo.pMultisampleState            = &multisampling;
        pipelineInfo.pDepthStencilState           = nullptr; // Optional
        pipelineInfo.pColorBlendState             = &colorBlending;
        pipelineInfo.pDynamicState                = nullptr; // Optional
        pipelineInfo.layout                       = mPipelineLayout;
        pipelineInfo.renderPass                   = mRenderPass;
        pipelineInfo.subpass                      = 0;
        pipelineInfo.basePipelineHandle           = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex            = -1;             // Optional
        CHECK_VK(vkCreateGraphicsPipelines(mVgi.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mPipeline));

        // done
        vkDestroyShaderModule(mVgi.device, vs, nullptr);
        vkDestroyShaderModule(mVgi.device, ps, nullptr);
        return true;
    }

    bool HandleWindowResize(VkResult vr) {
        if (VK_ERROR_OUT_OF_DATE_KHR == vr || VK_SUBOPTIMAL_KHR == vr) {
            GN_INFO(sLogger)("Window size changed. Try recreate swap chain.");
            CHECK_VK(vkDeviceWaitIdle(mVgi.device));
            CHECK_BOOL(createSwapchain());
            return true;
        }
        CHECK_VK(vr);
        return true;
    }

    int run() {
        mWin->show();
        while (mWin->runUntilNoNewEvents()) {
            // acquire back buffer
            uint32_t imageIndex;
            auto     vr = VK_SUCCESS;
            do {
                if (!HandleWindowResize(vr)) return -1;
                vr = vkAcquireNextImageKHR(mVgi.device, mSwapchain, (uint64_t) -1, mBackBufferAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
            } while (VK_ERROR_OUT_OF_DATE_KHR == vr);
            CHECK_VK(vr);

            // submit the command buffer
            VkSubmitInfo submitInfo               = {};
            submitInfo.sType                      = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            VkSemaphore          waitSemaphores[] = {mBackBufferAvailableSemaphore};
            VkPipelineStageFlags waitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submitInfo.waitSemaphoreCount         = 1;
            submitInfo.pWaitSemaphores            = waitSemaphores;
            submitInfo.pWaitDstStageMask          = waitStages;
            submitInfo.commandBufferCount         = 1;
            submitInfo.pCommandBuffers            = &mCommandBuffers[imageIndex];
            submitInfo.signalSemaphoreCount       = 1;
            submitInfo.pSignalSemaphores          = &mRenderFinishedSemaphore;
            CHECK_VK(vkQueueSubmit(*mDevice->graphics(), 1, &submitInfo, VK_NULL_HANDLE));

            // present
            VkPresentInfoKHR presentInfo   = {};
            presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores    = &mRenderFinishedSemaphore;
            presentInfo.swapchainCount     = 1;
            presentInfo.pSwapchains        = &mSwapchain;
            presentInfo.pImageIndices      = &imageIndex;
            presentInfo.pResults           = nullptr; // Optional
            if (!HandleWindowResize(vkQueuePresentKHR(*mDevice->present(), &presentInfo))) return -1;
        }
        vkDeviceWaitIdle(mVgi.device);
        return 0;
    }
};

int main() {
    GN_GUARD_ALWAYS;
    App app;
    return app.mSuccess ? app.run() : -1;
    GN_UNGUARD_ALWAYS_DO(return -1;);
}
