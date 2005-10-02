#include "pch.h"
#include "BasicRenderer.h"

// ****************************************************************************
//                         BasicRenderer init / quit functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::BasicRenderer::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( BasicRenderer, () );

    // initialize sub-modules one-by-one
    if( !dispInit()  ) { quit(); return selfOK(); }
    //if( !rsbInit()   ) { quit(); return selfOK(); }
    //if( !drawInit()  ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::quit()
{
    GN_GUARD;

    // shutdown sub-components in reverse sequence
    //drawQuit();
    //rsbQuit();
    dispQuit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// ****************************************************************************
// device management
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::BasicRenderer::deviceCreate( const DeviceSettings & ds )
{
    _GN_RENDER_DEVICE_TRACE();

    setDeviceSettings( ds );

    if( !dispDeviceCreate() ) return false;

    return true;
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::BasicRenderer::deviceRestore( const DeviceSettings & ds )
{
    _GN_RENDER_DEVICE_TRACE();

    setDeviceSettings( ds );

    if( !dispDeviceRestore() ) return false;

    return true;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::deviceDispose()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

    dispDeviceDispose();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::deviceDestroy()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

    dispDeviceDestroy();

    GN_UNGUARD;
}
