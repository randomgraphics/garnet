#include "pch.h"
#include "d3d10Shader.h"
#include "d3d10Renderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D10");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10GpuProgram::init( const GpuProgramDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D10GpuProgram, () );

    // clear parameter descriptor
    memset( &mParamDesc, 0, sizeof(mParamDesc) );

    // covert shader compile options
    D3D10ShaderCompileOptions options;
    options.compileFlags = 0;
    if( !desc.optimize ) options.compileFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
    if( !desc.debug ) options.compileFlags |= D3D10_SHADER_DEBUG;

    // compile all shaders
    if( !mVs.init( desc.vs, options ) ) return failure();
    if( !mGs.init( desc.gs, options ) ) return failure();
    if( !mPs.init( desc.ps, options ) ) return failure();

    GN_TODO( "initialize parameter descriptor" );
    memset( &mParamDesc, 0, sizeof(mParamDesc) );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10GpuProgram::quit()
{
    GN_GUARD;

    mVs.quit();
    mGs.quit();
    mPs.quit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10GpuProgram::apply() const
{
    GN_UNIMPL();
}
