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

extern "C" GN_EXPORT GN::gfx::Renderer *
GNgfxCreateRenderer( const GN::gfx::DeviceSettings & ds )
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::D3DRenderer> p( new GN::gfx::D3DRenderer );
    if( !p->init(ds) ) return 0;
    return p.detatch();

    GN_UNGUARD;
}

// *****************************************************************************
// init/quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::init(const DeviceSettings & ds )
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
    if( !changeDevice( ds, true ) ) { quit(); return selfOK(); }

    // successful
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::quit()
{
    GN_GUARD_ALWAYS;

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

    GN_UNGUARD_ALWAYS_NO_THROW;
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::changeDevice(
    const DeviceSettings & ds, bool forceRecreation )
{
    GN_GUARD;

    struct SetToFalse
    {
        bool & mVar;
        SetToFalse( bool & var ) : mVar(var) {}
        ~SetToFalse() { mVar = false; }
    };

    // prepare for function re-entrance.
    if( mDeviceChanging )
    {
        GND3D_WARN( "This call to changeDevice() is ignored to avoid function re-entance!" );
        return true;
    }
    mDeviceChanging = true;
    SetToFalse __dummy(mDeviceChanging);

    // store old display descriptor
    const DispDesc oldDesc = getDispDesc();

    // setup new display descriptor
    if( !setupDispDesc( ds ) ) return false;

    const DispDesc & newDesc = getDispDesc();

    if( forceRecreation ||
        oldDesc.windowHandle != newDesc.windowHandle ||
        oldDesc.monitorHandle != newDesc.monitorHandle ||
        oldDesc.reference != newDesc.reference ||
        oldDesc.software != newDesc.software )
    {
        // we have to perform a full device recreation
        deviceDispose();
        deviceDestroy();
        return deviceCreate() && deviceRestore();
    }
    else if( oldDesc != newDesc )
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

    _GN_RENDER_DEVICE_TRACE();

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
bool GN::gfx::D3DRenderer::deviceRestore()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

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
        GND3D_ERROR( "fail to process D3D device restore signal!" );
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

    _GN_RENDER_DEVICE_TRACE();

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
void GN::gfx::D3DRenderer::deviceDestroy()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

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
bool GN::gfx::D3DRenderer::resourceDeviceCreate()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

    std::list<D3DResource*>::iterator i, e = mResourceList.end();
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
bool GN::gfx::D3DRenderer::resourceDeviceRestore()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

    std::list<D3DResource*>::iterator i, e = mResourceList.end();
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
void GN::gfx::D3DRenderer::resourceDeviceDispose()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

    std::for_each( mResourceList.rbegin(), mResourceList.rend(),
        std::mem_fun(&D3DResource::deviceDispose) );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::resourceDeviceDestroy()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

    std::for_each( mResourceList.rbegin(), mResourceList.rend(),
        std::mem_fun(&D3DResource::deviceDestroy) );

    GN_UNGUARD;
}
