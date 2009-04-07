#include "pch.h"
#include "xenonShader.h"
#include "xenonRenderer.h"
#include "garnet/GNd3d9.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.xenon");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonGpuProgramHLSL::init( const GpuProgramDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( XenonGpuProgramHLSL, () );

    GN_ASSERT( GPL_ASM == desc.lang );

    IDirect3DDevice9 & dev = getRenderer().getDeviceInlined();

    if( desc.vs.source )
    {
        mVs = d3d9::ShaderCompiler<IDirect3DVertexShader9>::compile( dev, desc.vs.source, 0, 0, desc.vs.entry );
        if( NULL == mVs ) return failure();
    }

    if( desc.ps.source )
    {
        mPs = d3d9::ShaderCompiler<IDirect3DPixelShader9>::compile( dev, desc.ps.source, 0, 0, desc.ps.entry );
        if( NULL == mPs ) return failure();
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpuProgramHLSL::quit()
{
    GN_GUARD;

    safeRelease( mVs );
    safeRelease( mPs );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}
