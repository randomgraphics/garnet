// os.cpp — the official OperatingDomain implementation. Wraps the GN::win window/event
// system to provide engine2 with a render surface and an event pump.

#include <garnet/GNengine2.h>
#include <garnet/GNwin.h>

#include <memory>
#include <string>

using namespace GN;
using namespace GN::e2;

namespace {

GN::Logger * sLogger = GN::getLogger("GN.e2.os");

struct OperatingDomainImpl : OperatingDomain {
    GN_REGISTER_RUNTIME_TYPE(OperatingDomain);

    explicit OperatingDomainImpl(Universe & u): OperatingDomain(TYPE_INFO(), u.generateUniqueIdentifier(), "os-domain") {}

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

    bool processEvents() override { return mWindow ? mWindow->runUntilNoNewEvents() : false; }

private:
    std::unique_ptr<win::Window> mWindow;
};

} // namespace

namespace GN::e2 {

Ref<OperatingDomain> OperatingDomain::create(const CreateParameters & cp) {
    auto d = referenceTo(new OperatingDomainImpl(cp.universe));
    if (!d->init(cp)) return {};
    return d;
}

} // namespace GN::e2
