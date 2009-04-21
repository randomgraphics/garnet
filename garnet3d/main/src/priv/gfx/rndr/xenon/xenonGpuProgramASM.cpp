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
bool GN::gfx::XenonGpuProgramASM::init( const GpuProgramDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( XenonGpuProgramASM, () );

    GN_ASSERT( GpuProgramLanguage::MICROCODE == desc.lang );

    IDirect3DDevice9 & dev = getRenderer().getDeviceInlined();

    if( desc.vs.source )
    {
        mVs = d3d9::ShaderCompiler<IDirect3DVertexShader9>::assembleAndCreate( dev, desc.vs.source );
        if( NULL == mVs ) return failure();
    }

    if( desc.ps.source )
    {
        mPs = d3d9::ShaderCompiler<IDirect3DPixelShader9>::assembleAndCreate( dev, desc.ps.source );
        if( NULL == mPs ) return failure();
    }

    GN_TODO( "initialize parameter descriptor" );
    memset( &mParamDesc, 0, sizeof(mParamDesc) );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpuProgramASM::quit()
{
    GN_GUARD;

    safeRelease( mVs );
    safeRelease( mPs );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from XenonBasicGpuProgram
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpuProgramASM::apply() const
{
    IDirect3DDevice9 & dev = getRenderer().getDeviceInlined();
    dev.SetVertexShader( mVs );
    dev.SetPixelShader( mPs );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpuProgramASM::applyUniforms(
    const SysMemUniform * const * /*uniforms*/,
    size_t                        /*count*/ ) const
{
    GN_UNIMPL();
}
