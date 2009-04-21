#include "pch.h"
#include "d3d10Renderer.h"
#include "d3d10Shader.h"
#include "d3d10Texture.h"
#include "d3d10Buffer.h"
//#include "d3d10VtxLayout.h"
//#include "d3d10Font.h"
//#include "d3d10Quad.h"
//#include "d3d10Line.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D10");

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Renderer::resourceInit()
{
    GN_GUARD;

    GN_ASSERT( mResourceList.empty() );

#ifdef HAS_CG_D3D10
    GN_DX10_CHECK_RV( cgD3D10SetDevice( &getDeviceRefInlined() ), false );
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Renderer::resourceQuit()
{
    GN_GUARD;

    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "All graphics resouces MUST be released, before destroying renderer!" );

        std::list<D3D10Resource*>::iterator i = mResourceList.begin();
        while( i != mResourceList.end() )
        {
            D3D10Resource * r = *i;
            GN_ERROR(sLogger)( "0x%p", r );
            ++i;
        }
    }

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::CompiledGpuProgram *
GN::gfx::D3D10Renderer::compileGpuProgram( const GpuProgramDesc & gpd )
{
    GN_GUARD;

    AutoRef<SelfContainedGpuProgramDesc> s( new SelfContainedGpuProgramDesc );
    if( !s->init( gpd ) ) return NULL;

    // success
    return s.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuProgram *
GN::gfx::D3D10Renderer::createGpuProgram( const void * data, size_t length )
{
    GN_GUARD;

    AutoRef<SelfContainedGpuProgramDesc> s( new SelfContainedGpuProgramDesc );
    if( !s->init( data, length ) ) return NULL;

    const GpuProgramDesc & desc = s->desc();

    if( GpuProgramLanguage::HLSL == desc.lang )
    {
        AutoRef<D3D10GpuProgram> prog( new D3D10GpuProgram(*this) );
        if( !prog->init( desc ) ) return NULL;
        return prog.detach();
    }
    else
    {
        GN_ERROR(sLogger)( "Unsupported or invalid GPU program language: %d", desc.lang );
        return NULL;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Uniform *
GN::gfx::D3D10Renderer::createUniform( size_t size )
{
    return new SysMemUniform( size );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::D3D10Renderer::createTexture( const TextureDesc & desc )
{
    GN_GUARD;

    AutoRef<D3D10Texture> p( new D3D10Texture(*this) );
    if( !p->init( desc ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::D3D10Renderer::createVtxBuf( const VtxBufDesc & desc )
{
    GN_GUARD;

    AutoRef<D3D10VtxBuf> buf( new D3D10VtxBuf(*this) );

    if( !buf->init( desc ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::D3D10Renderer::createIdxBuf( const IdxBufDesc & desc )
{
    GN_GUARD;

    AutoRef<D3D10IdxBuf> buf( new D3D10IdxBuf(*this) );

    if( !buf->init( desc ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}
