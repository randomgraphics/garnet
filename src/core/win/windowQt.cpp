#include "pch.h"
#ifdef HAS_QT
    #include "windowQt.h"
    #include <QtCore/QSize>

static GN::Logger * sLogger = GN::getLogger("GN.win.Qt");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::win::WindowQt::init(const WindowCreateParameters & wcp) {
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    auto app = QApplication::instance();
    if (!app) {
        int          argc   = 1;
        const char * argv[] = {"garnet"};
        mApp                = new QApplication(argc, (char **) argv);
    }

    if (!createWindow(wcp)) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::win::WindowQt::init(const WindowAttachingParameters & wap) {
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    auto app = QApplication::instance();
    if (!app) {
        int          argc   = 1;
        const char * argv[] = {"garnet"};
        mApp                = new QApplication(argc, (char **) argv);
    }

    mWindow = QWindow::fromWinId((WId) wap.window);
    if (!mWindow) {
        GN_ERROR(sLogger)("Failed to attach to external window handle 0x%x", wap.window);
        return failure();
    }

    // TODO: query screen from window
    mScreen = QGuiApplication::primaryScreen();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowQt::quit() {
    GN_GUARD;

    safeDelete(mWindow);
    // safeDelete(mScreen);
    safeDelete(mApp);

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
intptr_t GN::win::WindowQt::getMonitorHandle() const {
    GN_GUARD;
    GN_ASSERT(mScreen);
    return (intptr_t) mScreen->handle();
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::Vector2<uint32_t> GN::win::WindowQt::getClientSize() const {
    GN_GUARD;
    Vector2<uint32_t> sz;
    sz.x = (uint32_t) mWindow->width();
    sz.y = (uint32_t) mWindow->height();
    return sz;
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowQt::show() {
    GN_GUARD;
    mWindow->show();
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowQt::hide() {
    GN_GUARD;
    mWindow->hide();
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowQt::moveTo(int x, int y) {
    GN_GUARD;
    mWindow->setPosition(x, y);
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowQt::setClientSize(size_t w, size_t h) {
    GN_GUARD;
    QSize sz((int) w, (int) h);
    mWindow->resize(sz);
    GN_UNGUARD;
}

//
//
// -------------------------------------------------------------------------
bool GN::win::WindowQt::runUntilNoNewEvents(bool) {
    GN_GUARD_SLOW;
    auto app = (QApplication *) QApplication::instance();
    app->processEvents();
    return !mClosing; // returns false, if user is trying to close the window
    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// Private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::win::WindowQt::createWindow(const WindowCreateParameters & wcp) {
    GN_GUARD;

    // check parent
    if (wcp.parent) {
        GN_ERROR(sLogger)("Not implented");
        return false;
    }

    // TODO: multi screen support
    mScreen = QGuiApplication::primaryScreen();

    // determine client size
    size_t width  = wcp.clientWidth;
    size_t height = wcp.clientHeight;
    if (0 == width) width = mScreen->availableGeometry().x();
    if (0 == height) height = mScreen->availableGeometry().y();

    // TODO: determine if window has border and title bar

    // create the window
    mWindow = new MainWindow(mScreen, &mClosing);
    mWindow->resize((int) width, (int) height);

    // TODO: connect to window close signal

    // success
    return true;

    GN_UNGUARD;
}

#endif