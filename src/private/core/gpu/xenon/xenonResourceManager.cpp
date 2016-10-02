#include "pch.h"
#include "xenonGpu.h"
#include "xenonShader.h"
#include "xenonTexture.h"
#include "xenonVertexDecl.h"
#include "xenonVtxBuf.h"
#include "xenonIdxBuf.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.xenon");

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonGpu::resourceInit()
{
    GN_GUARD;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpu::resourceQuit()
{
    GN_GUARD;

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
// from Gpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::Blob *
GN::gfx::XenonGpu::compileGpuProgram( const GpuProgramDesc & desc )
{
    GN_GUARD;

    AutoRef<SelfContainedGpuProgramDesc> s( new SelfContainedGpuProgramDesc );
    if( !s->init( desc ) ) return NULL;

    // success
    return s.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuProgram *
GN::gfx::XenonGpu::createGpuProgram( const void * compiledGpuProgramBinary, size_t length  )
{
    GN_GUARD;

    GN_ASSERT( isGpuThread() );

    /// get shader description of compiled binary
    AutoRef<SelfContainedGpuProgramDesc> s( new SelfContainedGpuProgramDesc );
    if( !s->init( compiledGpuProgramBinary, length ) ) return NULL;
    const GpuProgramDesc & desc = s->getDesc();

    switch( desc.lang )
    {
        case GpuProgramLanguage::MICROCODE:
        {
            AutoRef<XenonGpuProgramASM> prog( new XenonGpuProgramASM(*this) );
            if( !prog->init( desc ) ) return NULL;
            return prog.detach();
        }

        case GpuProgramLanguage::HLSL9:
        {
            AutoRef<XenonGpuProgramHLSL> prog( new XenonGpuProgramHLSL(*this) );
            if( !prog->init( desc ) ) return NULL;
            return prog.detach();
            break;
        }

        default:
            GN_ERROR(sLogger)( "invalid or unsupported GPU program language: %d", desc.lang );
            return NULL;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Uniform *
GN::gfx::XenonGpu::createUniform( size_t size )
{
    return new SysMemUniform( size );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::XenonGpu::createTexture( const TextureDesc & desc )
{
    GN_GUARD;

    GN_ASSERT( isGpuThread() );

    AutoRef<XenonTexture> p( new XenonTexture(*this) );
    if( !p->init( desc ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::XenonGpu::createVtxBuf( const VtxBufDesc & desc )
{
    GN_GUARD;

    GN_ASSERT( isGpuThread() );

    AutoRef<XenonVtxBuf> buf( new XenonVtxBuf(*this) );

    if( !buf->init( desc ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::XenonGpu::createIdxBuf( const IdxBufDesc & desc )
{
    GN_GUARD;

    GN_ASSERT( isGpuThread() );

    AutoRef<XenonIdxBuf> buf( new XenonIdxBuf(*this) );

    if( !buf->init( desc ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}
