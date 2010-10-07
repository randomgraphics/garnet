#include "pch.h"
#include "d3d11Gpu.h"
#include "d3d11Resource.h"
#include "d3d11Shader.h"
#include "d3d11Texture.h"
#include "d3d11Buffer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D11");

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11Gpu::resourceInit()
{
    GN_GUARD;

    GN_ASSERT( mResourceList.empty() );

#ifdef HAS_CG_D3D
    GN_DX_CHECK_RETURN( cgD3D11SetDevice( getCgContext(), &getDeviceRefInlined() ), false );
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::resourceQuit()
{
    GN_GUARD;

    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "All graphics resouces MUST be released, before destroying GPU!" );

        std::list<D3D11Resource*>::iterator i = mResourceList.begin();
        while( i != mResourceList.end() )
        {
            D3D11Resource * r = *i;
            GN_ERROR(sLogger)( "0x%p", r );
            ++i;
        }
    }

#ifdef HAS_CG_D3D
    GN_DX_CHECK_RETURN_VOID( cgD3D11SetDevice( getCgContext(), NULL ) );
#endif

    GN_UNGUARD;
}

// *****************************************************************************
// from Gpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::Blob *
GN::gfx::D3D11Gpu::compileGpuProgram( const GpuProgramDesc & gpd )
{
    AutoRef<SelfContainedGpuProgramDesc> s( new SelfContainedGpuProgramDesc );
    if( !s->init( gpd ) ) return NULL;

    // success
    return s.detach();
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuProgram *
GN::gfx::D3D11Gpu::createGpuProgram( const void * data, size_t length )
{
    AutoRef<SelfContainedGpuProgramDesc> s( new SelfContainedGpuProgramDesc );
    if( !s->init( data, length ) ) return NULL;

    const GpuProgramDesc & desc = s->getDesc();

    if( GpuProgramLanguage::HLSL11 == desc.lang ||
        GpuProgramLanguage::HLSL10 == desc.lang ||
        GpuProgramLanguage::HLSL9 == desc.lang )
    {
        AutoRef<D3D11GpuProgramHLSL> prog( new D3D11GpuProgramHLSL(*this) );
        if( !prog->init( desc ) ) return NULL;
        return prog.detach();
    }
#ifdef HAS_CG_D3D
    else if( GpuProgramLanguage::CG == desc.lang )
    {
        AutoRef<D3D11GpuProgramCG> prog( new D3D11GpuProgramCG(*this) );
        if( !prog->init( desc ) ) return NULL;
        return prog.detach();
    }
#endif
    else
    {
        GN_ERROR(sLogger)( "Unsupported or invalid GPU program language: %d", desc.lang.toRawEnum() );
        return NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Uniform *
GN::gfx::D3D11Gpu::createUniform( size_t size )
{
    return new SysMemUniform( size );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::D3D11Gpu::createTexture( const TextureDesc & desc )
{
    AutoRef<D3D11Texture> p( new D3D11Texture(*this) );
    if( !p->init( desc ) ) return 0;
    return p.detach();
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::D3D11Gpu::createVtxBuf( const VtxBufDesc & desc )
{
    AutoRef<D3D11VtxBuf> buf( new D3D11VtxBuf(*this) );

    if( !buf->init( desc ) ) return 0;

    return buf.detach();
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::D3D11Gpu::createIdxBuf( const IdxBufDesc & desc )
{
    AutoRef<D3D11IdxBuf> buf( new D3D11IdxBuf(*this) );

    if( !buf->init( desc ) ) return 0;

    return buf.detach();
}
