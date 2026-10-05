// os.cpp — the official Platform implementation. Wraps the GN::win window/event
// system to provide engine2 with a render surface and an event pump.

#include <garnet/GNengine2.h>
#include <garnet/GNwin.h>

#include <memory>

using namespace GN;
using namespace GN::e2;
using namespace GN::e2::basis;

namespace {

GN::Logger * sLogger = GN::getLogger("GN.e2.os");

struct PlatformImpl : Platform {
    GN_REGISTER_RUNTIME_TYPE(Platform);

    explicit PlatformImpl(Universe & u): Platform(TYPE_INFO(), u.generateUniqueIdentifier(), "platform") {}

    bool init(const CreateParameters & cp) {
        win::WindowCreateParameters wcp;
        wcp.caption      = (const char *) cp.caption; // StrA -> const char* (always non-null)
        wcp.clientWidth  = cp.width;
        wcp.clientHeight = cp.height;
        mWindow.reset(win::createWindow(wcp));
        if (!mWindow) {
            GN_ERROR(sLogger, "Failed to create application window.");
            return false;
        }
        mWindow->show();
        return true;
    }

    intptr_t createRenderSurface(intptr_t graphicsInstanceHandle) const override {
        return mWindow ? mWindow->createVulkanSurfaceHandle(graphicsInstanceHandle) : 0;
    }

    void destroyRenderSurface(intptr_t graphicsInstanceHandle, intptr_t surfaceHandle) const override {
        if (mWindow) mWindow->destroyVulkanSurfaceHandle(graphicsInstanceHandle, surfaceHandle);
    }

    Vector2<uint32_t> clientSize() const override { return mWindow ? mWindow->getClientSize() : Vector2<uint32_t>(0, 0); }

    win::Window * window() const override { return mWindow.get(); }

    bool processEvents() override { return mWindow ? mWindow->runUntilNoNewEvents() : false; }

private:
    std::unique_ptr<win::Window> mWindow;
};

} // namespace

namespace GN::e2::basis {

Ref<Platform> Platform::create(const CreateParameters & cp) {
    auto d = referenceTo(new PlatformImpl(cp.universe));
    if (!d->init(cp)) return {};
    return d;
}

} // namespace GN::e2::basis
