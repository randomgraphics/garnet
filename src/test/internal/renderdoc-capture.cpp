#include "renderdoc-capture.h"

#if defined(GN_BUILD_HAS_VULKAN)
    #include <renderdoc_app.h>

    #if defined(_WIN32)
        #define WIN32_LEAN_AND_MEAN
        #include <windows.h>
    #elif defined(__linux__)
        #include <dlfcn.h>
    #endif
#endif

namespace GN::test {

#if defined(GN_BUILD_HAS_VULKAN)
namespace {

RENDERDOC_API_1_5_0 * sRenderDocApi = nullptr;

void * getRenderDocModule() {
    #if defined(_WIN32)
    return GetModuleHandleA("renderdoc.dll");
    #elif defined(__linux__)
    // RenderDoc injects this module into the test process. RTLD_NOLOAD keeps
    // ordinary test runs from loading RenderDoc as an accidental dependency.
    return dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD);
    #else
    return nullptr;
    #endif
}

void * getRenderDocSymbol(void * module) {
    #if defined(_WIN32)
    return reinterpret_cast<void *>(GetProcAddress(static_cast<HMODULE>(module), "RENDERDOC_GetAPI"));
    #elif defined(__linux__)
    return dlsym(module, "RENDERDOC_GetAPI");
    #else
    return nullptr;
    #endif
}

} // namespace
#endif

void initializeRenderDoc() {
#if defined(GN_BUILD_HAS_VULKAN)
    if (sRenderDocApi) return;
    void * module = getRenderDocModule();
    if (!module) return;
    auto getApi = reinterpret_cast<pRENDERDOC_GetAPI>(getRenderDocSymbol(module));
    if (!getApi) return;
    void * api = nullptr;
    if (getApi(eRENDERDOC_API_Version_1_5_0, &api)) sRenderDocApi = static_cast<RENDERDOC_API_1_5_0 *>(api);
#endif
}

void startRenderDocCapture() {
#if defined(GN_BUILD_HAS_VULKAN)
    if (sRenderDocApi && sRenderDocApi->StartFrameCapture) sRenderDocApi->StartFrameCapture(nullptr, nullptr);
#endif
}

void endRenderDocCapture() {
#if defined(GN_BUILD_HAS_VULKAN)
    if (sRenderDocApi && sRenderDocApi->EndFrameCapture) sRenderDocApi->EndFrameCapture(nullptr, nullptr);
#endif
}

} // namespace GN::test
