#include "pch.h"
#include "windowMsw.h"
#include "windowX11.h"
#include "windowQt.h"

namespace GN {
namespace win {
static GN::Logger * sLogger = GN::getLogger("GN.win");

// *************************************************************************
// Fake Window
// *************************************************************************

///
/// Fake window class
///
class FakeWindow : public Window {
    // ********************************
    // from Window
    // ********************************
public:
    //@{

    intptr_t          getDisplayHandle() const { return (intptr_t) 1; }
    intptr_t          getMonitorHandle() const { return (intptr_t) 1; }
    intptr_t          getWindowHandle() const { return (intptr_t) 1; }
    intptr_t          getModuleHandle() const { return (intptr_t) 1; }
    Vector2<uint32_t> getClientSize() const { return Vector2<uint32_t>(640, 480); }
    void              show() {}
    void              hide() {}
    void              minimize() {}
    void              moveTo(int, int) {}
    void              setClientSize(size_t, size_t) {}
    bool              runUntilNoNewEvents(bool) { return false; }

    //@}

    // ********************************
    // private variables
    // ********************************
private:
    // ********************************
    // private functions
    // ********************************
private:
};

// *************************************************************************
// Public functions
// *************************************************************************

GN_API const WindowCreationParameters WCP_APPLICATION_WINDOW = {
    StrA("Garnet Application"),
    0,
    0, // default size
    0,
    0,
    0,     // no parent
    true,  // border
    true,  // titlebar
    false, // topmost
    true,  // has close box
};

GN_API const WindowCreationParameters WCP_WINDOWED_RENDER_WINDOW = {
    StrA("Garnet Application"),
    640,
    480, // size
    0,
    0,
    0,     // no parent
    true,  // border
    true,  // titlebar
    false, // topmost
    false, // no close box
};

GN_API const WindowCreationParameters WCP_FULLSCREEN_RENDER_WINDOW = {
    StrA(""), 640, 480, // size
    0,        0,   0,   // no parent
    false,              // border
    false,              // titlebar
    true,               // topmost
    false,              // no close box
};

//
//
// -------------------------------------------------------------------------
GN_API Window * createWindow(const WindowCreationParameters & wcp) {
    GN_GUARD;

#ifdef HAS_QT

    AutoObjPtr<WindowQt> p(new WindowQt);
    if (!p->init(wcp)) return 0;
    return p.detach();

#elif GN_XBOX2

    GN_UNUSED_PARAM(wcp);
    return new FakeWindow;

#elif GN_WINPC

    AutoObjPtr<WindowMsw> p(new WindowMsw);
    if (!p->init(wcp)) return 0;
    return p.detach();

#elif HAS_X11

    AutoObjPtr<WindowX11> p(new WindowX11);
    if (!p->init(wcp)) return 0;
    return p.detach();

#else

    GN_UNUSED_PARAM(wcp);
    GN_WARN(sLogger)("No window class implementation on current platform.");
    return new FakeWindow;

#endif

    GN_UNGUARD;
}

//
//
// -------------------------------------------------------------------------
GN_API Window * attachToExistingWindow(const WindowAttachingParameters & wap) {
    GN_GUARD;

#ifdef HAS_QT

    AutoObjPtr<WindowQt> p(new WindowQt);
    if (!p->init(wap)) return 0;
    return p.detach();

#elif GN_XBOX2

    GN_UNUSED_PARAM(wap);
    return new FakeWindow;

#elif GN_WINPC

    AutoObjPtr<WindowMsw> p(new WindowMsw);
    if (!p->init(wap)) return 0;
    return p.detach();

#else

    GN_UNUSED_PARAM(wap);
    GN_WARN(sLogger)("No window class implementation on current platform.");
    return new FakeWindow;

#endif

    GN_UNGUARD;
}
} // namespace win
} // namespace GN
