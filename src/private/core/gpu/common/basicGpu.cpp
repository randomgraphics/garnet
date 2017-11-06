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

    // TODO: remove Cg support
#ifdef HAS_CG
    // Initialize Cg context
    if( !mCgContext.init() ) return failure();
#endif

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

#ifdef HAS_CG
    // clear Cg context
    mCgContext.quit();
#endif

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
    DispDesc desc = {};

    // determine display
    desc.displayHandle = ro.displayHandle;
    if( 0 == desc.displayHandle ) {
        desc.displayHandle = getDefaultDisplay();
        if( 0 == desc.displayHandle ) return false;
    }
    GN_ASSERT( desc.displayHandle );

    // determine monitor handle
    desc.monitorHandle = ro.monitorHandle;
    if (0 == desc.monitorHandle) {
        desc.monitorHandle = determineMonitor(ro, desc.displayHandle);
        if( 0 == desc.monitorHandle ) return false;
    }

    // initialize external window
    if (ro.useExternalWindow) {
        GN::win::WindowAttachingParameters wap = {};
        wap.display = desc.displayHandle;
        wap.window = ro.renderWindow;
        mWindow = GN::win::attachToExistingWindow(wap);
        if (0 == mWindow) return false;
    }

    // determine display mode and size
    DisplayMode dm;
    if( !getCurrentDisplayMode( dm, desc.displayHandle, desc.monitorHandle ) ) return false;
    if( ro.fullscreen ) {
        desc.width   = (0==ro.displayMode.width)   ? dm.width  : ro.displayMode.width;
        desc.height  = (0==ro.displayMode.height)  ? dm.height : ro.displayMode.height;
        desc.depth   = (0==ro.displayMode.depth)   ? dm.depth  : ro.displayMode.depth;
        desc.refrate = (0==ro.displayMode.refrate) ? 0         : ro.displayMode.refrate;
    } else {
        auto defaultSize = ro.useExternalWindow ? mWindow->getClientSize() : Vector2<size_t>(640, 480);
        desc.width = ro.windowedWidth   ? ro.windowedWidth  : (uint32)defaultSize.x;
        desc.height = ro.windowedHeight ? ro.windowedHeight : (uint32)defaultSize.y;
        desc.depth = dm.depth;
        desc.refrate = 0;
    }
    GN_ASSERT( desc.width && desc.height && desc.depth );

    // create internal render window
    if (!ro.useExternalWindow) {
        GN::win::WindowCreationParameters wcp = {};
        wcp.caption = "Garnet 3D"; // TODO: make it a parameter.
        wcp.display = desc.displayHandle;
        wcp.parent = ro.parentWindow;
        wcp.clientWidth = desc.width;
        wcp.clientHeight = desc.height;
        wcp.hasBorder = !ro.fullscreen;
        wcp.hasTitleBar = !ro.fullscreen;
        wcp.topMost = false;
        wcp.closebox = true;
        mWindow = GN::win::createWindow(wcp);
        if (!mWindow) return false;
    }

    desc.windowHandle  = mWindow->getWindowHandle();
    GN_ASSERT_EX(
        desc.windowHandle && desc.monitorHandle,
        str::format( "win(0x%X), monitor(0x%X)", desc.windowHandle, desc.monitorHandle ).rawptr() );

    // success
    mOptions = ro;
    mDispDesc = desc;
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
    if( mOptions.fullscreen ) return;

    // get client window size
    auto s = mWindow->getClientSize();
    auto m = mWindow->getMonitorHandle();

    // compare with old window properties
    if( s != mOldWindowSize || m != mOldMonitor) {
        mOldWindowSize = s;
        mOldMonitor = m;

        // trigger renderer signal when window size is changed or window is moved to another monitor
        getSignals().rendererWindowSizeMove( m, (uint32)s.x, (uint32)s.y );
    }

    GN_UNGUARD;
}