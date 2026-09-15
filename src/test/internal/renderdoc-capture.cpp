#include "renderdoc-capture.h"

#include <garnet/GNbase.h>
#include <renderdoc_app.h>

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif defined(__linux__)
    #include <dlfcn.h>
#endif

namespace GN::test {

namespace {

Logger * const sLogger = getLogger("GN.test.renderdoc");

} // namespace

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

void initializeRenderDoc() {
    if (sRenderDocApi) {
        GN_INFO(sLogger, "RenderDoc API is present");
        return;
    }
    void * module = getRenderDocModule();
    if (!module) {
        GN_INFO(sLogger, "RenderDoc API is not present");
        return;
    }
    auto getApi = reinterpret_cast<pRENDERDOC_GetAPI>(getRenderDocSymbol(module));
    if (!getApi) {
        GN_INFO(sLogger, "RenderDoc API is not present");
        return;
    }
    void * api = nullptr;
    if (getApi(eRENDERDOC_API_Version_1_5_0, &api)) {
        sRenderDocApi = static_cast<RENDERDOC_API_1_5_0 *>(api);
        GN_INFO(sLogger, "RenderDoc API is present");
    } else {
        GN_INFO(sLogger, "RenderDoc API is not present");
    }
}

void startRenderDocCapture() {
    if (sRenderDocApi && sRenderDocApi->StartFrameCapture) {
        GN_INFO(sLogger, "Starting RenderDoc capture");
        sRenderDocApi->StartFrameCapture(nullptr, nullptr);
    } else {
        GN_INFO(sLogger, "Skipping RenderDoc capture start: API is not present");
    }
}

void endRenderDocCapture() {
    if (sRenderDocApi && sRenderDocApi->EndFrameCapture) {
        uint32_t captured = sRenderDocApi->EndFrameCapture(nullptr, nullptr);
        GN_INFO(sLogger, "Stopped RenderDoc capture ({}).", captured ? "capture saved" : "no capture saved");
    } else {
        GN_INFO(sLogger, "Skipping RenderDoc capture stop: API is not present");
    }
}

} // namespace GN::test
