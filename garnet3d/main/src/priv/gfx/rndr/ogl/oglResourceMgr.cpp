#include "pch.h"
#include "oglRenderer.h"
#include "oglShader.h"
#include "oglVtxBuf.h"
#include "oglIdxBuf.h"
#include "oglTexture.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL.ResourceMgr");

// *****************************************************************************
// Local GPU program compile utilities
// *****************************************************************************

/// OGL compiled program binary
class OGLCompiledGpuProgram : public GN::gfx::CompiledGpuProgram
{
    DynaArray<UInt8> mBinary;

    static inline bool
    sCheckShaderCode(
        const char *       type, ///< shader type
        const ShaderCode & sc,
        const char *       begin,
        const char *       end )
    {
        if( sc.code != NULL && sc.lang >= NUM_GPU_PROGRAM_LANGUAGES )
        {
            GN_ERROR(sLogger)( "invalid %s shader language: %d", type, sc.lang );
            return false;
        }

        if( 0 != sc.code && ( sc.code < begin || sc.code >= end ) )
        {
            GN_ERROR(sLogger)( "invalid %s shader code pointer." );
            return false;
        }

        if( 0 != sc.entry && ( sc.entry < begin || sc.entry >= end ) )
        {
            GN_ERROR(sLogger)( "invalid %s shader entry pointer." );
            return false;
        }

        return true;
    }

public:

    /// initialize from shader description
    bool init( const GpuProgramDesc & desc )
    {
        // calculate buffer size
        size_t headerLen  = sizeof(desc);
        size_t vsCodeLen  = desc.vs.code ? ( strlen(desc.vs.code) + 1 ) : 0;
        size_t vsEntryLen = desc.vs.entry ? ( strlen(desc.vs.entry) + 1 ) : 0;
        size_t gsCodeLen  = desc.gs.code ? ( strlen(desc.gs.code) + 1 ) : 0;
        size_t gsEntryLen = desc.gs.entry ? ( strlen(desc.gs.entry) + 1 ) : 0;
        size_t psCodeLen  = desc.ps.code ? ( strlen(desc.ps.code) + 1 ) : 0;
        size_t psEntryLen = desc.ps.entry ? ( strlen(desc.ps.entry) + 1 ) : 0;
        size_t length     = headerLen +
                            vsCodeLen + vsEntryLen +
                            gsCodeLen + gsEntryLen +
                            psCodeLen + psEntryLen;

        // allocate buffer
        mBinary.resize( length );
        GpuProgramDesc & copy = *(GpuProgramDesc*)mBinary.cptr();
        UInt8 * start = mBinary.cptr();
        UInt8 * ptr = start;

        // copy header
        memcpy( ptr, &desc, sizeof(desc) );
        ptr += sizeof(desc);

#define COPY_CODE( X ) \
    memcpy( ptr, desc.X.code, X##CodeLen ); \
    copy.X.code = ( X##CodeLen > 0 ) ? (const char*)( ptr - start ) : 0; \
    ptr += X##CodeLen;

#define COPY_ENTRY( X ) \
    memcpy( ptr, desc.X.entry, X##EntryLen ); \
    copy.X.entry = ( X##EntryLen > 0 ) ? (const char*)( ptr - start ) : 0; \
    ptr += X##EntryLen;

        // copy codes and entries
        COPY_CODE( vs ); COPY_ENTRY( vs );
        COPY_CODE( gs ); COPY_ENTRY( gs );
        COPY_CODE( ps ); COPY_ENTRY( ps );

        // done
        GN_ASSERT( ((size_t)( ptr - start )) == length );
        return true;
    }

    /// initialize from raw data buffer
    bool init( const void * data, size_t length )
    {
        // copy input buffer
        mBinary.resize( length );
        memcpy( mBinary.cptr(), data, length );

        const char     * start = (const char *)mBinary.cptr();
        const char     * end   = start + length;
        GpuProgramDesc & desc  = *(GpuProgramDesc*)start;

        // patch all offsets to pointers
        if( 0 != desc.vs.code ) desc.vs.code = start + (size_t)desc.vs.code;
        if( 0 != desc.vs.entry ) desc.vs.entry = start + (size_t)desc.vs.entry;

        if( 0 != desc.gs.code ) desc.gs.code = start + (size_t)desc.gs.code;
        if( 0 != desc.gs.entry ) desc.gs.entry = start + (size_t)desc.gs.entry;

        if( 0 != desc.ps.code ) desc.ps.code = start + (size_t)desc.ps.code;
        if( 0 != desc.ps.entry ) desc.ps.entry = start + (size_t)desc.ps.entry;

        // check data integrity
        if( !sCheckShaderCode( "vertex", desc.vs, start, end ) ||
            !sCheckShaderCode( "geometry", desc.gs, start, end ) ||
            !sCheckShaderCode( "pixel", desc.ps, start, end ) )
        {
            GN_ERROR(sLogger)( "Invalid shader binary." );
            return false;
        }

        return true;
    }

    virtual const void * data() const { return mBinary.cptr(); }
    virtual size_t       size() const { return mBinary.size(); }
};

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::resourceInit()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "Not _ALL_ graphics resouces are released!" );
        return false;
    }

#ifdef HAS_CG_OGL
    if( !mCgContext.init() ) return false;
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::resourceQuit()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // check for non-released resources
    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "All graphics resouces have to be released, before renderer is destroied!" );
        GN_UNEXPECTED();
        for( std::list<OGLResource*>::iterator i = mResourceList.begin(); i != mResourceList.end(); ++i )
        {
            const OGLResource * r = *i;
            GN_ERROR(sLogger)( "0x%p", r );
        }
    }

#ifdef HAS_CG_OGL
    mCgContext.quit();
#endif

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::CompiledGpuProgram *
GN::gfx::OGLRenderer::compileGpuProgram( const GpuProgramDesc & gpd )
{
    GN_GUARD;

    AutoRef<OGLCompiledGpuProgram> cgp( new OGLCompiledGpuProgram );
    if( !cgp->init( gpd ) ) return NULL;

    // success
    return cgp.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuProgram *
GN::gfx::OGLRenderer::createGpuProgram( const void * data, size_t length )
{
    GN_GUARD;

    AutoRef<OGLCompiledGpuProgram> cgp( new OGLCompiledGpuProgram );
    if( !cgp->init( data, length ) ) return NULL;

    const GpuProgramDesc * desc = (const GpuProgramDesc *)cgp->data();
    AutoRef<OGLGpuProgramGLSL> prog( new OGLGpuProgramGLSL(*this) );
    if( !prog->init( *desc ) ) return NULL;
    return prog.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::OGLRenderer::createTexture( const TextureDesc & desc )
{
    GN_GUARD;

    AutoRef<OGLTexture> p( new OGLTexture(*this) );
    if( !p->init( desc ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::OGLRenderer::createVtxBuf( const VtxBufDesc & desc )
{
    GN_GUARD;

    if( GLEW_ARB_vertex_buffer_object )
    {
        AutoRef<OGLVtxBufVBO> p( new OGLVtxBufVBO(*this) );
        if( !p->init( desc ) ) return 0;
        return p.detach();
    }
    else
    {
        AutoRef<OGLVtxBufNormal> p( new OGLVtxBufNormal(*this) );
        if( !p->init( desc ) ) return 0;
        return p.detach();
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::OGLRenderer::createIdxBuf( const IdxBufDesc & desc )
{
    GN_GUARD;

    AutoRef<OGLIdxBuf> p( new OGLIdxBuf );
    if( !p->init( desc ) ) return 0;

    return p.detach();

    GN_UNGUARD;
}
