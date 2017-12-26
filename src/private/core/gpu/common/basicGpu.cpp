#include "pch.h"
#include "basicGpu.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.common");

//
// RIP for GPU modules
// -----------------------------------------------------------------------------
void GN::gfx::rip( const char * msg, ... )
{
    GN_UNUSED_PARAM(msg);
}

// *****************************************************************************
//                         BasicGpu init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicGpu::init( const GpuOptions & o )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( BasicGpu, () );

    // check GPU options
    if( o.api < 0 || o.api >= GpuAPI::NUM_APIs )
    {
        if( GpuAPI::AUTO == o.api )
        {
            GN_ERROR(sLogger)( "GpuAPI::AUTO must be changed to actual API value before initializing GPU." );
        }
        else
        {
            GN_ERROR(sLogger)( "Invalid API: %d", o.api.toRawEnum() );
        }

        return failure();
    }

    // sanity check: warning when render context size is larger than 2K bytes
    if( sizeof(GpuContext) > 2048 )
    {
        GN_WARN(sLogger)( "GN::gfx::GpuContext is huge! (%u bytes)", sizeof(GpuContext) );
    }

    // initialize sub-components one by one
    if( !dispInit(o) ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::quit()
{
    GN_GUARD;

    dispQuit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::bindContext( const GpuContext & newContext )
{
    // skip dirty check, if last context binding failed.
    bool skipDirtyCheck = !mContextOk;

    mContextOk = bindContextImpl( newContext, skipDirtyCheck );

    if( mContextOk )
    {
        mContext = newContext;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::rebindContext()
{
    mContextOk = bindContextImpl( mContext, true );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::getBackBufferContent( BackBufferContent & c )
{
    c.data.clear();
    c.format = ColorFormat::UNKNOWN;
    c.width = 0;
    c.height = 0;
    c.pitch = 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::setUserData( const Guid & id, const void * data, uint32 length )
{
    UserData * currentUserData = mUserData.find( id );

    if( NULL == data && 0 == length )
    {
        // delete existing data
        if( currentUserData )
        {
            mUserData.remove( id );
        }
        else
        {
            GN_ERROR(sLogger)( "Invalid user data GUID." );
        }
    }
    else if( currentUserData )
    {
        // overwrite existing data

        if( NULL != data && length > 0 )
        {
            currentUserData->resize( length );
            memcpy( currentUserData->rawptr(), data, length );
        }
        else
        {
            currentUserData->clear();
        }
    }
    else
    {
        // add new data
        DynaArray<uint8> & newUserData = mUserData[id];

        if( NULL != data && length > 0 )
        {
            newUserData.resize( length );
            memcpy( newUserData.rawptr(), data, length );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
const void * GN::gfx::BasicGpu::getUserData( const Guid & id, uint32 * length ) const
{
    const UserData * currentUserData = mUserData.find( id );

    if( NULL != currentUserData )
    {
        if( length ) *length = (uint32)currentUserData->size();

        return currentUserData->rawptr();
    }
    else
    {
        GN_ERROR(sLogger)( "Invalid user data GUID." );

        if( length ) *length = 0;

        return NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicGpu::hasUserData( const Guid & id ) const
{
    return NULL != mUserData.find( id );
}

// *****************************************************************************
// private function
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::BasicGpu::dispInit( const GpuOptions & ro )
{
    // create render window
    if (ro.useExternalWindow) {
        GN::win::WindowAttachingParameters wap = {};
        wap.display = ro.displayHandle;
        wap.window = ro.renderWindow;
        mWindow = GN::win::attachToExistingWindow(wap);
        if (0 == mWindow) return false;
    } else {
        auto w = ro.displayMode.width;
        auto h = ro.displayMode.height;
        if (ro.displayMode.mode == DisplayMode::WINDOWED)
        {
            if (0 == w) w = 1280;
            if (0 == h) h = 720;
        }
        GN::win::WindowCreationParameters wcp = {};
        wcp.caption = "Garnet 3D"; // TODO: make it a parameter.
        wcp.display = ro.displayHandle;
        wcp.monitor = ro.monitorHandle;
        wcp.parent = ro.parentWindow;
        wcp.clientWidth = w;
        wcp.clientHeight = h;
        wcp.hasBorder = ro.displayMode.mode == DisplayMode::WINDOWED;
        wcp.hasTitleBar = ro.displayMode.mode == DisplayMode::WINDOWED;
        wcp.topMost = false;
        wcp.closebox = true;
        mWindow = GN::win::createWindow(wcp);
    }
    if (!mWindow) return false;

    // Update display descriptor
    memset(&mDispDesc, 0, sizeof(mDispDesc));
    mDispDesc.displayHandle = mWindow->getDisplayHandle();
    mDispDesc.monitorHandle = mWindow->getMonitorHandle();
    mDispDesc.windowHandle  = mWindow->getWindowHandle();
    mDispDesc.width = mWindow->getClientSize().width;
    mDispDesc.height = mWindow->getClientSize().height;
    mDispDesc.depth = 0; // TODO: get display depth
    mDispDesc.refrate = 0; // TODO: get actual refresh rate.

    // success
    mOptions = ro;
    mWindow->show();
    return true;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::BasicGpu::dispQuit()
{
    safeDelete(mWindow);
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpu::handleRenderWindowSizeMove()
{
    GN_GUARD;

    // do nothing if in full screen mode
    if( mOptions.displayMode.mode == DisplayMode::FULL_SCREEN ) return;

    // get client window size
    auto s = mWindow->getClientSize();
    auto m = mWindow->getMonitorHandle();

    // compare with old window properties
    if( s != mOldWindowSize || m != mOldMonitor) {
        mOldWindowSize = s;
        mOldMonitor = m;

        // trigger renderer signal when window size is changed or window is moved to another monitor
        getSignals().rendererWindowSizeMove( m, s.x, s.y );
    }

    GN_UNGUARD;
}