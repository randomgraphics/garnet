#include "pch.h"
#include "d3d11Renderer.h"
#include "d3d11Resource.h"
#include "../common/basicShader.h"
//#include "d3d11Shader.h"
//#include "d3d11Texture.h"
//#include "d3d11Buffer.h"
//#include "d3d11VtxLayout.h"
//#include "d3d11Font.h"
//#include "d3d11Quad.h"
//#include "d3d11Line.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D11");

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11Renderer::resourceInit()
{
    GN_GUARD;

    GN_ASSERT( mResourceList.empty() );

#ifdef HAS_CG_D3D11
    GN_DX10_CHECK_RV( cgD3D11SetDevice( &getDeviceRefInlined() ), false );
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Renderer::resourceQuit()
{
    GN_GUARD;

    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "All graphics resouces MUST be released, before destroying renderer!" );

        std::list<D3D11Resource*>::iterator i = mResourceList.begin();
        while( i != mResourceList.end() )
        {
            D3D11Resource * r = *i;
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
GN::gfx::D3D11Renderer::compileGpuProgram( const GpuProgramDesc & gpd )
{
    GN_UNIMPL();
    return NULL;
    /*AutoRef<SelfContainedGpuProgramDesc> s( new SelfContainedGpuProgramDesc );
    if( !s->init( gpd ) ) return NULL;

    // success
    return s.detach();*/
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuProgram *
GN::gfx::D3D11Renderer::createGpuProgram( const void * data, size_t length )
{
    GN_UNIMPL();
    return NULL;

    /*AutoRef<SelfContainedGpuProgramDesc> s( new SelfContainedGpuProgramDesc );
    if( !s->init( data, length ) ) return NULL;

    const GpuProgramDesc & desc = s->desc();

    if( GpuProgramLanguage::HLSL10 == desc.lang ||
        GpuProgramLanguage::HLSL9 == desc.lang )
    {
        bool hlsl9 = GpuProgramLanguage::HLSL9 == desc.lang;
        AutoRef<D3D11GpuProgram> prog( new D3D11GpuProgram(*this) );
        if( !prog->init( desc, hlsl9 ) ) return NULL;
        return prog.detach();
    }
    else
    {
        GN_ERROR(sLogger)( "Unsupported or invalid GPU program language: %d", desc.lang.toRawEnum() );
        return NULL;
    }*/
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Uniform *
GN::gfx::D3D11Renderer::createUniform( size_t size )
{
    return new SysMemUniform( size );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::D3D11Renderer::createTexture( const TextureDesc & desc )
{
    GN_UNIMPL();
    return NULL;

    /*AutoRef<D3D11Texture> p( new D3D11Texture(*this) );
    if( !p->init( desc ) ) return 0;
    return p.detach();*/
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::D3D11Renderer::createVtxBuf( const VtxBufDesc & desc )
{
    GN_UNIMPL();
    return NULL;

    /*AutoRef<D3D11VtxBuf> buf( new D3D11VtxBuf(*this) );

    if( !buf->init( desc ) ) return 0;

    return buf.detach();*/
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::D3D11Renderer::createIdxBuf( const IdxBufDesc & desc )
{
    GN_UNIMPL();
    return NULL;

    /*AutoRef<D3D11IdxBuf> buf( new D3D11IdxBuf(*this) );

    if( !buf->init( desc ) ) return 0;

    return buf.detach();*/
}
