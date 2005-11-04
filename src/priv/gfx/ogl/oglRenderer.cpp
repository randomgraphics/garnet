#include "pch.h"
#include "oglRenderer.h"

#if GN_MSVC
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib" )
#endif

// *****************************************************************************
// Global functions
// *****************************************************************************

#if GN_STATIC
GN::gfx::Renderer *
GN::gfx::createOGLRenderer( const UserOptions & uo )
#else
extern "C" GN_EXPORT GN::gfx::Renderer *
GNgfxCreateRenderer( const GN::gfx::UserOptions & uo )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::OGLRenderer> p( new GN::gfx::OGLRenderer );
    if( !p->init(uo) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// init/quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::init(const UserOptions & uo )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLRenderer, () );

    // init sub-components
    if( !dispInit()    ) { quit(); return selfOK(); }
    if( !capsInit()    ) { quit(); return selfOK(); }
    if( !shaderInit()  ) { quit(); return selfOK(); }
    if( !rsbInit()     ) { quit(); return selfOK(); }
    if( !textureInit() ) { quit(); return selfOK(); }
    if( !bufferInit()  ) { quit(); return selfOK(); }
    if( !paramInit()   ) { quit(); return selfOK(); }
    if( !drawInit()    ) { quit(); return selfOK(); }

    // create & reset device data
    if( !changeUserOptions( uo, true ) ) { quit(); return selfOK(); }

    // successful
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::quit()
{
    GN_GUARD;

    deviceDispose();
    deviceDestroy();

    drawQuit();
    paramQuit();
    bufferQuit();
    textureQuit();
    rsbQuit();
    shaderQuit();
    dispQuit();

    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::changeUserOptions(
    const UserOptions & uo, bool forceRecreation )
{
    GN_GUARD;

    // prepare for function re-entrance.
    if( mDeviceChanging )
    {
        GNOGL_WARN( "This call to changeUserOptions() is ignored to avoid function re-entance!" );
        return true;
    }
    ScopeBool __dummy__(mDeviceChanging);

    // store old display settings
    const UserOptions oldOptions = getUserOptions();
    const DispDesc oldDesc = getDispDesc();

    // setup new display settings
    if( !processUserOptions( uo ) ) return false;

    const DispDesc & newDesc = getDispDesc();

    if( forceRecreation ||
        oldDesc.windowHandle != newDesc.windowHandle )
    {
        // we have to perform a full device recreation
        deviceDispose();
        deviceDestroy();
        return deviceCreate() && deviceRestore();
    }
    else if( oldDesc != newDesc ||
        oldOptions.fullscreen != uo.fullscreen ||
        oldOptions.vsync != uo.vsync ||
        oldOptions.autoRestore != uo.autoRestore )
    {
        // a device reset should be enough
        deviceDispose();
        return deviceRestore();
    }
    else
    {
        // do nothing, if new setting is equal to current setting
        return true;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::deviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( !BasicRenderer::deviceCreate() ) return false;
    if( !dispDeviceCreate() ) return false;
    if( !capsDeviceCreate() ) return false;
    if( !shaderDeviceCreate() ) return false;
    if( !rsbDeviceCreate() ) return false;
    if( !textureDeviceCreate() ) return false;
    if( !bufferDeviceCreate() ) return false;
    if( !resourceDeviceCreate() ) return false;
    if( !paramDeviceCreate() ) return false;
    if( !drawDeviceCreate() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::deviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( !BasicRenderer::deviceRestore() ) return false;
    if( !dispDeviceRestore() ) return false;
    if( !capsDeviceRestore() ) return false;
    if( !shaderDeviceRestore() ) return false;
    if( !rsbDeviceRestore() ) return false;
    if( !textureDeviceRestore() ) return false;
    if( !bufferDeviceRestore() ) return false;
    if( !resourceDeviceRestore() ) return false;
    if( !paramDeviceRestore() ) return false;
    if( !drawDeviceRestore() ) return false;

    // trigger reset event
    if( !sigDeviceRestore() )
    {
        GNOGL_ERROR( "fail to process OGL device restore signal!" );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::deviceDispose()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // trigger dispose event
    sigDeviceDispose();

    drawDeviceDispose();
    paramDeviceDispose();
    resourceDeviceDispose();
    bufferDeviceDispose();
    textureDeviceDispose();
    rsbDeviceDispose();
    shaderDeviceDispose();
    capsDeviceDispose();
    dispDeviceDispose();
    BasicRenderer::deviceDispose();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::deviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    drawDeviceDestroy();
    paramDeviceDestroy();
    resourceDeviceDestroy();
    bufferDeviceDestroy();
    textureDeviceDestroy();
    rsbDeviceDestroy();
    shaderDeviceDestroy();
    capsDeviceDestroy();
    dispDeviceDestroy();
    BasicRenderer::deviceDestroy();

    GN_UNGUARD;
}

// *****************************************************************************
// DX resource management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::resourceDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::list<OGLResource*>::iterator i, e = mResourceList.end();
    for( i = mResourceList.begin(); i != e; ++i )
    {
        if( !(*i)->deviceCreate() ) return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::resourceDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::list<OGLResource*>::iterator i, e = mResourceList.end();
    for( i = mResourceList.begin(); i != e; ++i )
    {
        if( !(*i)->deviceRestore() ) return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::resourceDeviceDispose()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::for_each( mResourceList.rbegin(), mResourceList.rend(),
        std::mem_fun(&OGLResource::deviceDispose) );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::resourceDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::for_each( mResourceList.rbegin(), mResourceList.rend(),
        std::mem_fun(&OGLResource::deviceDestroy) );

    GN_UNGUARD;
}
