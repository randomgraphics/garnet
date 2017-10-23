#include "pch.h"
#include <vector>

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.test.vulkan");

#define CHECK_VK(x) if (VK_SUCCESS == (x)) return;

struct App {
    bool          success;
    win::Window * win;
    VkInstance    vkInst;
    VkDebugReportCallbackEXT vkDebugReport;
    VkDevice      vkDev;

    App()
        : success(false)
        , win(nullptr)
        , vkInst(nullptr)
        , vkDebugReport(nullptr)
        , vkDev(nullptr)
    {
        win = win::createWindow(win::WindowCreationParams{
            "vulkan test",
            0, // parent
            1960, 1080,
            true, true, false, true
        });

        // Initialize VKEL
        CHECK_VK(vkelInit());

        // query layers
        uint32 layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        StrA s = "available Vulkan layers:\n";
        for (const auto & l : availableLayers) {
            s += str::format("\t%s\n", l.layerName);
        }
        GN_INFO(sLogger)(s);

        // query extensions
        uint32 extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
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
        };
        auto ci = VkInstanceCreateInfo {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            nullptr,
            0, // flags,
            nullptr,
            (uint32)layers.size(), layers.data(),
            (uint32)extensions.size(), extensions.data()
        };
        CHECK_VK(vkCreateInstance(&ci, nullptr, &vkInst));
        vkelInit();

        // setup debug callback
        VkDebugReportCallbackCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        createInfo.pfnCallback = debugCallback;
        CHECK_VK(vkCreateDebugReportCallbackEXT(vkInst, &createInfo, nullptr, &vkDebugReport));

        // done
        success = true;
    }

    ~App() {
        if (vkDebugReport) vkDestroyDebugReportCallbackEXT(vkInst, vkDebugReport, nullptr);
        if (vkInst) vkDestroyInstance(vkInst, nullptr);
        vkelUninit();
        if (win) delete win;
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

    int run() {
        return 0;
    }
};

int main() {
    App app;
    return app.success ? app.run() : -1;
}
