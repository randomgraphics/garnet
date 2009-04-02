#include "pch.h"
#include "xenonRenderer.h"
#include "xenonShader.h"
#include "xenonTexture.h"
#include "xenonVertexDecl.h"
#include "xenonVtxBuf.h"
#include "xenonIdxBuf.h"
#include "xenonLine.h"

// *****************************************************************************
// local functions
// *****************************************************************************

//
// Functor to compare sampler descriptor
//
struct EqualSampler
{
    const GN::gfx::SamplerDesc & desc;
    EqualSampler( const GN::gfx::SamplerDesc & d ) : desc(d) {}
    bool operator()( const GN::gfx::XenonSamplerObject & so ) const { return desc == so.getDesc(); }
};

//
// Functor to compare vertex format
//
struct EqualFormat
{
    const GN::gfx::VtxFmtDesc & format;
    EqualFormat( const GN::gfx::VtxFmtDesc & f ) : format(f) {}
    bool operator()( const GN::gfx::XenonVtxDeclDesc & vbd ) const { return format == vbd.format; }
};

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonRenderer::resourceInit()
{
    GN_GUARD;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonRenderer::resourceQuit()
{
    GN_GUARD;

    safeDelete( mLine );

    // release vertex formats
    mVertexFormats.clear();

    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "All graphics resouces MUST be released, after receiving 'destroy' signal!" );

        std::list<XenonResource*>::iterator i = mResourceList.begin();
        while( i != mResourceList.end() )
        {
            XenonResource * r = *i;
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
GN::gfx::XenonRenderer::compileGpuProgram( const GpuProgramDesc & desc )
{
    GN_ASSERT( getCurrentThreadId() == mThreadId );
    GN_UNUSED_PARAM(desc);
    GN_UNIMPL();
    return NULL;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuProgram *
GN::gfx::XenonRenderer::createGpuProgram( const void * compiledGpuProgramBinary, size_t length  )
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    GN_UNUSED_PARAM( compiledGpuProgramBinary );
    GN_UNUSED_PARAM( length );
    GN_UNIMPL();
    return NULL;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Uniform *
GN::gfx::XenonRenderer::createUniform( size_t size )
{
    GN_UNUSED_PARAM( size );
    GN_UNIMPL();
    return NULL;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::XenonRenderer::createTexture( const TextureDesc & desc )
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    AutoRef<XenonTexture> p( new XenonTexture(*this) );
    if( !p->init( desc ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::SamplerHandle GN::gfx::XenonRenderer::createSampler( const SamplerDesc & desc )
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    SamplerHandle  h = mSamplers.findIf( EqualSampler(desc) );

    if( 0 == h )
    {
        // create new vertex decl
        XenonSamplerObject s;
        if( !s.init(desc) ) return false;
        h = mSamplers.add( s );
    }

    // success
    return h;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxFmtHandle GN::gfx::XenonRenderer::createVtxFmt( const VtxFmtDesc & format )
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    VtxFmtHandle  h = mVertexFormats.findIf( EqualFormat(format) );

    if( 0 == h )
    {
        // create new vertex decl
        XenonVtxDeclDesc vbd;
        vbd.format = format;
        vbd.decl.attach( createXenonVertexDecl( mDevice, format ) );
        if( !vbd.decl ) return 0;

        h = mVertexFormats.add( vbd );
    }

    // success
    return h;

    GN_UNGUARD;
}


//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::XenonRenderer::createVtxBuf( const VtxBufDesc & desc )
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    AutoRef<XenonVtxBuf> buf( new XenonVtxBuf(*this) );

    if( !buf->init( desc ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::XenonRenderer::createIdxBuf( const IdxBufDesc & desc )
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    AutoRef<XenonIdxBuf> buf( new XenonIdxBuf(*this) );

    if( !buf->init( desc ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

