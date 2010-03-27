#include "pch.h"
#include "oglGpu.h"
#include "oglShader.h"
#include "oglVtxBuf.h"
#include "oglIdxBuf.h"
#include "oglTexture.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.OGL.ResourceMgr");

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpu::resourceInit()
{
    GN_GUARD;

    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "Not _ALL_ graphics resouces are released!" );
        return false;
    }

#ifdef HAS_CG_OGL
    if( !mCgContext.Init() ) return false;
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpu::resourceQuit()
{
    GN_GUARD;

    // check for non-released resources
    if( !mResourceList.empty() )
    {
        GN_UNEXPECTED_EX( "All graphics resouces have to be released, before renderer is destroied!" );
        for( std::list<OGLResource*>::iterator i = mResourceList.begin(); i != mResourceList.end(); ++i )
        {
            const OGLResource * r = *i;
            GN_ERROR(sLogger)( "0x%p", r );
        }
    }

#ifdef HAS_CG_OGL
    mCgContext.Quit();
#endif

    GN_UNGUARD;
}

// *****************************************************************************
// from Gpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
Blob *
GN::gfx::OGLGpu::CompileGpuProgram( const GpuProgramDesc & gpd )
{
    GN_GUARD;

    AutoRef<SelfContainedGpuProgramDesc> s( new SelfContainedGpuProgramDesc );
    if( !s->Init( gpd ) ) return NULL;

    // success
    return s.Detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuProgram *
GN::gfx::OGLGpu::CreateGpuProgram( const void * data, size_t length )
{
    GN_GUARD;

    AutoRef<SelfContainedGpuProgramDesc> s( new SelfContainedGpuProgramDesc );
    if( !s->Init( data, length ) ) return NULL;

    const GpuProgramDesc & desc = s->desc();

    switch( desc.lang )
    {
        case GpuProgramLanguage::GLSL:
        {
            AutoRef<OGLGpuProgramGLSL> prog( new OGLGpuProgramGLSL(*this) );
            if( !prog->Init( desc ) ) return NULL;
            return prog.Detach();
        }

        default:
            GN_ERROR(sLogger)( "invalid or unsupported GPU program language: %s", desc.lang.ToString() );
            return NULL;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Uniform *
GN::gfx::OGLGpu::CreateUniform( size_t size )
{
    return new SysMemUniform( size );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::OGLGpu::CreateTexture( const TextureDesc & desc )
{
    GN_GUARD;

    AutoRef<OGLTexture> p( new OGLTexture(*this) );
    if( !p->Init( desc ) ) return 0;
    return p.Detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::OGLGpu::CreateVtxBuf( const VtxBufDesc & desc )
{
    GN_GUARD;

    if( GLEW_ARB_vertex_buffer_object )
    {
        AutoRef<OGLVtxBufVBO> p( new OGLVtxBufVBO(*this) );
        if( !p->Init( desc ) ) return 0;
        return p.Detach();
    }
    else
    {
        AutoRef<OGLVtxBufNormal> p( new OGLVtxBufNormal(*this) );
        if( !p->Init( desc ) ) return 0;
        return p.Detach();
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::OGLGpu::CreateIdxBuf( const IdxBufDesc & desc )
{
    GN_GUARD;

    AutoRef<OGLIdxBuf> p( new OGLIdxBuf );
    if( !p->Init( desc ) ) return 0;

    return p.Detach();

    GN_UNGUARD;
}
