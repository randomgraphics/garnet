#include "pch.h"

#if GN_POSIX

//!
//! Implementation class of window wrapper on XWin platform
//!
class GN::Window::Impl
{
    uint32_t mWidth, mHeight;

public:

    Impl( GN::Window & ) {}

    ~Impl() {}

    bool create( const CreateParam & cp ) { mWidth = cp.clientWidth; mHeight = cp.clientHeight; return true; }
    void * getWindow() { return (void*)1; }
    void * getMonitor() { return (void*)1; }
    bool getClientSize( uint32_t & width, uint32_t & height ) const { width = mWidth; height = mHeight; return true; }
};

// *****************************************************************************
// public function of GN::Window
// *****************************************************************************

GN::Window::Window() { mImpl = new Impl(*this); }
GN::Window::~Window() { delete mImpl; }
bool GN::Window::create( const CreateParam & cp ) { return mImpl->create(cp); }
void * GN::Window::getWindow() { return mImpl->getWindow(); }
void * GN::Window::getMonitor() { return mImpl->getMonitor(); }
bool GN::Window::getClientSize( uint32_t & width, uint32_t & height ) const { return mImpl->getClientSize(width,height); }

#endif
