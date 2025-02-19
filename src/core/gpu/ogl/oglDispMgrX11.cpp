#include "pch.h"
#include "oglGpu.h"
#include "oglDebug.h"

#if GN_POSIX

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL");

// ****************************************************************************
// local functions
// ****************************************************************************

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpu::dispInit() {
    GN_GUARD;

    GN_ASSERT(!mRenderContext);

    auto & rw   = getRenderWindow();
    auto   disp = (Display *) rw.getDisplayHandle();
    auto   win  = (::Window) rw.getWindowHandle();
    GN_ASSERT(disp && win);

    // get window attributes
    XWindowAttributes wa;
    GN_X_CHECK_RETURN(XGetWindowAttributes(disp, win, &wa), false);

    // fill VisualInfo structure of the window
    XVisualInfo vi;
    vi.visual        = wa.visual;
    vi.visualid      = wa.visual->visualid;
    vi.screen        = rw.getMonitorHandle();
    vi.depth         = wa.depth;
    vi.c_class       = wa.visual->c_class;
    vi.red_mask      = wa.visual->red_mask;
    vi.green_mask    = wa.visual->green_mask;
    vi.blue_mask     = wa.visual->blue_mask;
    vi.colormap_size = wa.visual->map_entries;
    vi.bits_per_rgb  = wa.visual->bits_per_rgb;
    GN_ASSERT(vi.screen >= 0);

    // create a GLX context
    mRenderContext = glXCreateContext(disp, &vi,
                                      0,   // no sharing
                                      true // enable direct rendering, which yields best feature set and performance.
    );
    if (0 == mRenderContext) {
        GN_ERROR(sLogger)("Fail to create GLX context.");
        return false;
    }

    // make the context as current render context.
    if (!glXMakeCurrent(disp, win, mRenderContext)) {
        GN_ERROR(sLogger)("glXMakeCurrent() failed.");
        return false;
    }

    // init GLEW
    glewInit();

    // Setup the debug output
    if (getOptions().debug) OGLDebugOutputARB::enable();

    // setup swap control
    if (GLX_SGI_swap_control) {
        if (!glXSwapIntervalSGI(getOptions().vsync)) { GN_WARN(sLogger)("Fail to adjust SGI swap control"); }
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpu::dispQuit() {
    GN_GUARD;

    if (mRenderContext) {
        auto disp = (Display *) getRenderWindow().getDisplayHandle();
        GN_ASSERT(disp);
        glXMakeCurrent(disp, 0, 0);
        glXDestroyContext(disp, mRenderContext);
        mRenderContext = 0;
    }

    GN_UNGUARD;
}

#endif // GN_POSIX
