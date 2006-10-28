#include "pch.h"
#include "oglRenderer.h"
#include "oglShader.h"
#include "oglTexture.h"
#include "oglVtxFmt.h"
#include "oglVtxBuf.h"
#include "oglIdxBuf.h"

// *****************************************************************************
// local function
// *****************************************************************************

struct EqualFormat
{
    const GN::gfx::VtxFmtDesc & format;

    EqualFormat( const GN::gfx::VtxFmtDesc & f ) : format(f) {}

    bool operator()( void * p ) const
    {
        GN_ASSERT( p );
        return format == ((GN::gfx::OGLVtxFmt*)p)->getFormat();
    }
};

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::resourceDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "Not _ALL_ graphics resouces are released!" );
        return false;
    }

#ifdef HAS_CG_OGL
    // create Cg context
    mCgContext = cgCreateContext();
    if( !mCgContext )
    {
        GN_ERROR(sLogger)( "Fail to create Cg context!" );
        return false;
    }
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::resourceDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // release vertex formats
    for( VtxFmtHandle h = mVtxFmts.first(); h != 0; h = mVtxFmts.next( h ) )
    {
        GN_ASSERT( mVtxFmts[h] );
        delete mVtxFmts[h];
    }
    mVtxFmts.clear();

    // check for non-released resources
    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "All graphics resouces MUST be released, after recieving 'destroy' signal!" );
        GN_UNEXPECTED();
        for( std::list<OGLResource*>::iterator i = mResourceList.begin(); i != mResourceList.end(); ++i )
        {
            const OGLResource * r = *i;
            GN_ERROR(sLogger)( "0x%p", r );
        }
    }

#ifdef HAS_CG_OGL
    // destroy Cg context
    if( mCgContext )
    {
        cgDestroyContext( mCgContext );
        mCgContext = 0;
    }
#endif

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::Shader *
GN::gfx::OGLRenderer::createShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & )
{
    GN_GUARD;

    switch( type )
    {
        case SHADER_VS :
            switch( lang )
            {
                case LANG_OGL_ARB:
                {
                    AutoRef<OGLVtxShaderARB> p( new OGLVtxShaderARB(*this) );
                    if( !p->init( code ) ) return 0;
                    return p.detach();
                }

                case LANG_OGL_GLSL:
                {
                    AutoRef<OGLVtxShaderGLSL> p( new OGLVtxShaderGLSL(*this) );
                    if( !p->init( code ) ) return 0;
                    return p.detach();
                }

                default:
                    GN_ERROR(sLogger)( "unsupport shading language : %s", shadingLanguage2Str(lang) );
                    return 0;
            }

        case SHADER_PS :
            switch( lang )
            {
                case LANG_OGL_ARB:
                {
                    AutoRef<OGLPxlShaderARB> p( new OGLPxlShaderARB(*this) );
                    if( !p->init( code ) ) return 0;
                    return p.detach();
                }

                case LANG_OGL_GLSL:
                {
                    AutoRef<OGLPxlShaderGLSL> p( new OGLPxlShaderGLSL(*this) );
                    if( !p->init( code ) ) return 0;
                    return p.detach();
                }

                default:
                    GN_ERROR(sLogger)( "unsupport shading language : %s", shadingLanguage2Str(lang) );
                    return 0;
            }

        case SHADER_GS:
            GN_ERROR(sLogger)( "OpenGL does not support GeometryShader." );
            return 0;

        default:
            GN_UNEXPECTED(); // program should not reach here
            GN_ERROR(sLogger)( "invalid shader type: %d", type );
            return 0;
    }
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::OGLRenderer::createTexture( const TextureDesc & desc,
                                     const TextureLoader & loader )
{
    GN_GUARD;

    AutoRef<OGLTexture> p( new OGLTexture(*this) );
    p->setLoader( loader );
    if( !p->init( desc ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
uint32_t GN::gfx::OGLRenderer::createVtxFmt( const VtxFmtDesc & format )
{
    GN_GUARD;

    uint32_t h = mVtxFmts.findIf( EqualFormat(format) );

    if( 0 == h )
    {
        // create new vertex binding object
        AutoObjPtr<OGLVtxFmt> p( new OGLVtxFmt(*this) );
        if( !p->init( format ) ) return 0;
        h = mVtxFmts.add( p );
        p.detach();
    }

    // success
    return h;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf *
GN::gfx::OGLRenderer::createVtxBuf(
    size_t bytes, bool dynamic, bool sysCopy, const VtxBufLoader & loader )
{
    GN_GUARD;

    if( GLEW_ARB_vertex_buffer_object )
    {
        AutoRef<OGLVtxBufVBO> p( new OGLVtxBufVBO(*this) );
        p->setLoader( loader );
        if( !p->init( bytes, dynamic, sysCopy ) ) return 0;
        return p.detach();
    }
    else
    {
        AutoRef<OGLVtxBufNormal> p( new OGLVtxBufNormal );
        p->setLoader( loader );
        if( !p->init( bytes, dynamic ) ) return 0;
        return p.detach();
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf *
GN::gfx::OGLRenderer::createIdxBuf(
    size_t numIdx, bool dynamic, bool /*sysCopy*/, const IdxBufLoader & loader )
{
    GN_GUARD;

    AutoRef<OGLIdxBuf> p( new OGLIdxBuf );
    p->setLoader( loader );
    if( !p->init( numIdx, dynamic ) ) return 0;

    return p.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::removeGLSLShader( ShaderType st, Shader * sh )
{
    GN_GUARD;

    GN_ASSERT( 0 <= st && st < NUM_SHADER_TYPES );

    GLSLProgramMap::iterator i,t;
    for( i = mGLSLProgramMap.begin(); i != mGLSLProgramMap.end(); )
    {
        if( i->first.shaders[st] == sh )
        {
            t = i;
            ++i;

            // remove the program from program map
            GN_ASSERT( t->second );
            OGLProgramGLSL * prog = (OGLProgramGLSL*)t->second;
            delete prog;
            mGLSLProgramMap.erase( t );
        }
        else
        {
            ++i;
        }
    }

    GN_UNGUARD;
}
