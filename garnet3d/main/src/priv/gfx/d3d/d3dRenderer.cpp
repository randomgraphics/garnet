#include "pch.h"
#include "d3dRenderer.h"
#include "d3dResource.h"

#if GN_MSVC
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dxerr9.lib" )
#if GN_DEBUG
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif
#endif

// *****************************************************************************
// Global functions
// *****************************************************************************

#if GN_STATIC
GN::gfx::Renderer *
GN::gfx::createD3DRenderer( const RendererOptions & ro )
#else
extern "C" GN_EXPORT GN::gfx::Renderer *
GNgfxCreateRenderer( const GN::gfx::RendererOptions & ro )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::D3DRenderer> p( new GN::gfx::D3DRenderer );
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
bool GN::gfx::D3DRenderer::init(const RendererOptions & ro )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3DRenderer, () );

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
    if( !changeOptions( ro, true ) ) { quit(); return selfOK(); }

    // successful
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::quit()
{
    GN_GUARD;

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
bool GN::gfx::D3DRenderer::changeOptions( RendererOptions ro, bool forceRecreation )
{
    GN_GUARD;

    // prepare for function re-entrance.
    if( mDeviceChanging )
    {
        GNGFX_WARN( "This call to changeOptions() is ignored to avoid function re-entance!" );
        return true;
    }
    ScopeBool __dummy__(mDeviceChanging);

#if GN_XENON
    if( !ro.fullscreen )
    {
        GNGFX_WARN( "Windowed mode is not supported on Xenon platform. Force fullscreen mode." );
        ro.fullscreen = true;
        ro.displayMode.set(0,0,0,0);
    }
    if( ro.useExternalWindow )
    {
        GNGFX_WARN( "External render windowe is not supported on Xenon platform. Force internal render window." );
        ro.useExternalWindow = false;
    }
#endif

    // store old settings
    const RendererOptions oldOptions = getOptions();
    const DispDesc oldDesc = getDispDesc();

    // setup new settings
    if( !processUserOptions( ro ) ) return false;

    const DispDesc & newDesc = getDispDesc();

    if( forceRecreation ||
        oldDesc.windowHandle != newDesc.windowHandle ||
        oldDesc.monitorHandle != newDesc.monitorHandle ||
        oldOptions.reference != ro.reference ||
        oldOptions.software != ro.software )
    {
        // we have to recreate the whole device.
        deviceDestroy();
        return deviceCreate();
    }
    else if(
        oldDesc != newDesc ||
        oldOptions.fullscreen != ro.fullscreen ||
        oldOptions.vsync != ro.vsync )
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
bool GN::gfx::D3DRenderer::deviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( mDeviceChanging );

    #define COMPONENT_RECREATE(X) if( !X##DeviceCreate() || !X##DeviceRestore() ) return false;

    COMPONENT_RECREATE( disp );
    COMPONENT_RECREATE( caps );
    COMPONENT_RECREATE( shader );
    COMPONENT_RECREATE( rsb );
    COMPONENT_RECREATE( texture );
    COMPONENT_RECREATE( buffer );
    COMPONENT_RECREATE( resource );
    COMPONENT_RECREATE( param );
    COMPONENT_RECREATE( draw );

    #undef COMPONENT_RECREATE

    // trigger restore events
    if( !sigDeviceRestore() )
    {
        GNGFX_ERROR( "fail to process D3D device restore signal!" );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::deviceRestore()
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
    if( !paramDeviceRestore() ) return false;
    if( !drawDeviceRestore() ) return false;

    // trigger reset event
    if( !sigDeviceRestore() )
    {
        GNGFX_ERROR( "fail to process D3D device restore signal!" );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::deviceDispose()
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

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::deviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // trigger dispose event
    sigDeviceDispose();

    #define COMPONENT_DESTROY(X) X##DeviceDispose(); X##DeviceDestroy();

    COMPONENT_DESTROY( draw );
    COMPONENT_DESTROY( param );
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
bool GN::gfx::D3DRenderer::resourceDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::list<D3DResource*>::iterator i = mResourceList.begin();
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
bool GN::gfx::D3DRenderer::resourceDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::list<D3DResource*>::iterator i = mResourceList.begin();
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
void GN::gfx::D3DRenderer::resourceDeviceDispose()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::list<D3DResource*>::reverse_iterator i = mResourceList.rbegin();
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
void GN::gfx::D3DRenderer::resourceDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::list<D3DResource*>::reverse_iterator i = mResourceList.rbegin();
    while( i != mResourceList.rend() )
    {
        (*i)->deviceDestroy();
        ++i;
    }

    GN_UNGUARD;
}
