#include "pch.h"
#include "xenonShader.h"
#include "xenonGpu.h"
#include "garnet/GNd3d.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.xenon");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonGpuProgramASM::Init( const GpuProgramDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( XenonGpuProgramASM, () );

    GN_ASSERT( GpuProgramLanguage::MICROCODE == desc.lang );

    IDirect3DDevice9 & dev = getGpu().getDeviceInlined();

    if( desc.vs.source )
    {
        mVs = d3d9::ShaderCompiler<IDirect3DVertexShader9>::assembleAndCreate( dev, desc.vs.source );
        if( NULL == mVs ) return Failure();
    }

    if( desc.ps.source )
    {
        mPs = d3d9::ShaderCompiler<IDirect3DPixelShader9>::assembleAndCreate( dev, desc.ps.source );
        if( NULL == mPs ) return Failure();
    }

    GN_TODO( "initialize parameter descriptor" );
    memset( &mParamDesc, 0, sizeof(mParamDesc) );

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpuProgramASM::Quit()
{
    GN_GUARD;

    SafeRelease( mVs );
    SafeRelease( mPs );

    // standard Quit procedure
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
    IDirect3DDevice9 & dev = getGpu().getDeviceInlined();
    dev.SetVertexShader( mVs );
    dev.SetPixelShader( mPs );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpuProgramASM::applyUniforms(
    const Uniform * const * /*uniforms*/,
    size_t                  /*count*/,
    bool                    /*skipDirtyCheck*/ ) const
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpuProgramASM::applyTextures(
    const TextureBinding * /*bindings*/,
    size_t                 /*count*/,
    bool                   /*skipDirtyCheck*/ ) const
{
    GN_UNIMPL();
}
