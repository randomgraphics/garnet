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
createOGLRenderer( const GN::gfx::RendererOptions & ro )
#else
extern "C" GN_EXPORT GN::gfx::Renderer *
GNgfxCreateRenderer( const GN::gfx::RendererOptions & ro )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::OGLRenderer> p( new GN::gfx::OGLRenderer );
    if( !p->init(ro) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// init/quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::init(const RendererOptions & ro )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLRenderer, () );

    // init sub-components
    if( !dispInit()         ) { quit(); return selfOK(); }
    if( !capsInit()         ) { quit(); return selfOK(); }
    if( !shaderInit()       ) { quit(); return selfOK(); }
    if( !rsbInit()          ) { quit(); return selfOK(); }
    if( !textureInit()      ) { quit(); return selfOK(); }
    if( !bufferInit()       ) { quit(); return selfOK(); }
    if( !ffpInit()        ) { quit(); return selfOK(); }
    if( !renderTargetInit() ) { quit(); return selfOK(); }
    if( !drawInit()         ) { quit(); return selfOK(); }

    // create & reset device data
    if( !changeOptions( ro, true ) ) { quit(); return selfOK(); }

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

    deviceDestroy();

    drawQuit();
    renderTargetQuit();
    ffpQuit();
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
bool GN::gfx::OGLRenderer::changeOptions( RendererOptions ro, bool forceRecreation )
{
    GN_GUARD;

    // prepare for function re-entrance.
    if( mDeviceChanging )
    {
        GN_WARN( "This call to changeOptions() is ignored to avoid function re-entance!" );
        return true;
    }
    ScopeBool __dummy__(mDeviceChanging);

    // store old display settings
    const RendererOptions oldOptions = getOptions();
    const DispDesc oldDesc = getDispDesc();

    // setup new display settings
    if( !processUserOptions( ro ) ) return false;

    const DispDesc & newDesc = getDispDesc();

    if( forceRecreation ||
        oldDesc.windowHandle != newDesc.windowHandle )
    {
        // we have to perform a full device recreation
        deviceDestroy();
        return deviceCreate();
    }
    else if( oldDesc != newDesc ||
        oldOptions.fullscreen != ro.fullscreen ||
        oldOptions.vsync != ro.vsync ||
        oldOptions.autoRestore != ro.autoRestore )
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

    #define COMPONENT_RECREATE(X) if( !X##DeviceCreate() || !X##DeviceRestore() ) return false;

    COMPONENT_RECREATE( disp );
    COMPONENT_RECREATE( caps );
    COMPONENT_RECREATE( shader );
    COMPONENT_RECREATE( rsb );
    COMPONENT_RECREATE( texture );
    COMPONENT_RECREATE( buffer );
    COMPONENT_RECREATE( resource );
    COMPONENT_RECREATE( ffp );
    COMPONENT_RECREATE( renderTarget );
    COMPONENT_RECREATE( draw );

    #undef COMPONENT_RECREATE

    // trigger renderer signals
    if( !sSigDeviceCreate() || !sSigDeviceRestore() )
    {
        GN_ERROR( "fail to process OGL device restore signal!" );
        return false;
    }

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

    if( !dispDeviceRestore() ) return false;
    if( !capsDeviceRestore() ) return false;
    if( !shaderDeviceRestore() ) return false;
    if( !rsbDeviceRestore() ) return false;
    if( !textureDeviceRestore() ) return false;
    if( !bufferDeviceRestore() ) return false;
    if( !resourceDeviceRestore() ) return false;
    if( !ffpDeviceRestore() ) return false;
    if( !renderTargetDeviceRestore() ) return false;
    if( !drawDeviceRestore() ) return false;

    // trigger reset event
    if( !sSigDeviceRestore() )
    {
        GN_ERROR( "fail to process OGL device restore signal!" );
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

    // trigger signals
    sSigDeviceDispose();

    drawDeviceDispose();
    renderTargetDeviceDispose();
    ffpDeviceDispose();
    resourceDeviceDispose();
    bufferDeviceDispose();
    textureDeviceDispose();
    rsbDeviceDispose();
    shaderDeviceDispose();
    capsDeviceDispose();
    dispDeviceDispose();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::deviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // trigger signals
    sSigDeviceDispose();
    sSigDeviceDestroy();

    #define COMPONENT_DESTROY(X) X##DeviceDispose(); X##DeviceDestroy();

    COMPONENT_DESTROY( draw );
    COMPONENT_DESTROY( renderTarget );
    COMPONENT_DESTROY( ffp );
    COMPONENT_DESTROY( resource );
    COMPONENT_DESTROY( buffer );
    COMPONENT_DESTROY( texture );
    COMPONENT_DESTROY( rsb );
    COMPONENT_DESTROY( shader );
    COMPONENT_DESTROY( caps );
    COMPONENT_DESTROY( disp );

    #undef COMPONENT_DESTROY

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

    std::list<OGLResource*>::iterator i = mResourceList.begin();
    while( i != mResourceList.end() )
    {
        if( !(*i)->deviceCreate() ) return false;
        ++i;
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

    std::list<OGLResource*>::iterator i = mResourceList.begin();
    while( i != mResourceList.end() )
    {
        if( !(*i)->deviceRestore() ) return false;
        ++i;
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

    std::list<OGLResource*>::reverse_iterator i = mResourceList.rbegin();
    while( i != mResourceList.rend() )
    {
        (*i)->deviceDispose();
        ++i;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::resourceDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::list<OGLResource*>::reverse_iterator i = mResourceList.rbegin();
    while( i != mResourceList.rend() )
    {
        (*i)->deviceDestroy();
        ++i;
    }

    GN_UNGUARD;
}
