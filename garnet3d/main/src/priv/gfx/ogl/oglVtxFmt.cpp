#include "pch.h"
#include "oglVtxFmt.h"
#include "oglRenderer.h"

// *****************************************************************************
// Local classes and functions
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
static GLuint sClrFmt2OGLFormat( GN::gfx::ClrFmt fmt )
{
    switch ( fmt )
    {
        case GN::gfx::FMT_FLOAT1   :
        case GN::gfx::FMT_FLOAT2   :
        case GN::gfx::FMT_FLOAT3   :
        case GN::gfx::FMT_FLOAT4   : return GL_FLOAT;
        case GN::gfx::FMT_D3DCOLOR : return GL_UNSIGNED_BYTE;
        default: return 0;
    }
}

//
//
// ----------------------------------------------------------------------------
static GLboolean sClrFmt2OGLNormalization( GN::gfx::ClrFmt fmt )
{
    GN_ASSERT( 0 <= fmt && fmt <= GN::gfx::NUM_CLRFMTS );

    const GN::gfx::ClrFmtDesc & desc = GN::gfx::getClrFmtDesc( fmt );

    for( uint8_t i = 0; i < desc.numChannels; ++i )
    {
        if( GN::gfx::TYPE_UNORM != desc.channels[i].type ||
            GN::gfx::TYPE_SNORM != desc.channels[i].type )
        {
            return false;
        }
    }
    return true;
}


struct OGLVtxSem
{
    GLenum    semantic;
    GLuint    attribute;
};

struct OGLClrFmt
{
    GLuint    format;
    GLuint    components;
    GLboolean normalization;
};

static OGLVtxSem sVtxSem2OGL[] =
{
    #define GNGFX_DEFINE_VTXSEM( tag, d3decl, d3dindex, glname, glindex, cgname ) \
        { static_cast<GLenum>(glname), static_cast<GLuint>(glindex) },
    #include "garnet/gfx/vertexSemanticMeta.h"
    #undef GNGFX_DEFINE_VTXSEM
};

static OGLClrFmt sClrFmt2OGL[] =
{
    #define GN_COLOR_FORMAT( format, bits, channels ) \
        { sClrFmt2OGLFormat( GN::gfx::FMT_##format ), \
          GN::gfx::getClrFmtDesc( GN::gfx::FMT_##format ).numChannels, \
          sClrFmt2OGLNormalization( GN::gfx::FMT_##format ) },
    #include "garnet/gfx/colorFormatMeta.h"
    #undef GN_COLOR_FORMAT
};

//!
//! Vertex semantic status
//!
static class VtxSemStatus
{
    struct Item
    {
        //bool supported;
        bool enabled;
    } mItems[GN::gfx::NUM_VTXSEMS];

public:

    VtxSemStatus()
    {
        memset( mItems, 0, sizeof(mItems) );
    }

    void apply( const GN::gfx::VtxFmtDesc & format,
                GN::gfx::VtxSem sem,
                bool useConventionalVertexArray )
    {
        GN_GUARD_SLOW;

        GN_ASSERT( 0 <= sem && sem < GN::gfx::NUM_VTXSEMS );

        Item & item = mItems[sem];

        if( format.attribs[sem].used  )
        {
            if( !item.enabled )
            {
                if( useConventionalVertexArray )
                {
                    GN_OGL_CHECK( glEnableClientState( sVtxSem2OGL[sem].semantic ) );
                }
                else if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
                {
                    GN_OGL_CHECK( glEnableVertexAttribArrayARB( sVtxSem2OGL[sem].attribute ) );
                }
                else
                {
                    GN_ERROR( "unsupport semantic %d!", sem );
                    return;
                }
                item.enabled = true;
            }
        }
        else if( item.enabled )
        {
            if( useConventionalVertexArray )
            {
                GN_OGL_CHECK( glDisableClientState( sVtxSem2OGL[sem].semantic ) );
            }
            else if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
            {
                GN_OGL_CHECK( glDisableVertexAttribArrayARB( sVtxSem2OGL[sem].attribute ) );
            }
            else
            {
                GN_ERROR( "unsupport semantic %d!", sem );
                return;
            }
            item.enabled = false;
        }

        GN_UNGUARD_SLOW;
    }
} sVtxSemStatus;

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::init( const VtxFmtDesc & format )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLVtxFmt, () );

    mFormat = format;

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// device management
// *****************************************************************************

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::bind() const
{
    GN_GUARD_SLOW;

    OGLRenderer & r = getRenderer();

    // VERTSEM_COORD
    if( mFormat.attribs[VTXSEM_COORD].used )
        GN_OGL_CHECK( glEnableClientState( GL_VERTEX_ARRAY ) );
    else
        GN_OGL_CHECK( glDisableClientState( GL_VERTEX_ARRAY ) );

    // VTXSEM_WEIGHT
    sVtxSemStatus.apply( mFormat, VTXSEM_WEIGHT, false );

    // VTXSEM_NORMAL
    if( mFormat.attribs[VTXSEM_NORMAL].used )
        GN_OGL_CHECK( glEnableClientState( GL_NORMAL_ARRAY ) );
    else
        GN_OGL_CHECK( glDisableClientState( GL_NORMAL_ARRAY ) );

    // VTXSEM_COLOR0
    if( mFormat.attribs[VTXSEM_COLOR0].used )
        GN_OGL_CHECK( glEnableClientState( GL_COLOR_ARRAY ) );
    else
        GN_OGL_CHECK( glDisableClientState( GL_COLOR_ARRAY ) );

    // VTXSEM_COLOR1
    sVtxSemStatus.apply( mFormat, VTXSEM_COLOR1, !!GLEW_EXT_secondary_color );

    // VTXSEM_FOGCOORD
    sVtxSemStatus.apply( mFormat, VTXSEM_FOGCOORD, !!GLEW_EXT_fog_coord );

    // VTXSEM_TANGENT
    sVtxSemStatus.apply( mFormat, VTXSEM_TANGENT, false );

    // VTXSEM_BINORMAL
    sVtxSemStatus.apply( mFormat, VTXSEM_BINORMAL, false );

    // VTXSEM_TEXn
    for( uint32_t i = 0; i < r.getCaps( CAPS_MAX_TEXTURE_STAGES ); ++i )
    {
        r.chooseClientTextureStage( i );
        if( mFormat.attribs[VTXSEM_TEX0+i].used )
            GN_OGL_CHECK( glEnableClientState( GL_TEXTURE_COORD_ARRAY ) );
        else
            GN_OGL_CHECK( glDisableClientState( GL_TEXTURE_COORD_ARRAY ) );
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::bindBuffer(
    size_t index, const uint8_t * buf, size_t startVtx, size_t stride ) const
{
    GN_GUARD_SLOW;

    GN_ASSERT( index < mFormat.numStreams );

    const VtxFmtDesc::StreamDesc & sd = mFormat.streams[index];

    buf += stride * startVtx;

    VtxSem sem;

    uint32_t texStage;

    for( uint8_t i = 0; i < sd.numAttribs; ++i )
    {
        sem = sd.attribs[i];

        const VtxFmtDesc::AttribDesc & ad = mFormat.attribs[sem];

        switch( sem )
        {
            case VTXSEM_COORD:
                GN_OGL_CHECK( glVertexPointer(
                    sClrFmt2OGL[ad.format].components,
                    sClrFmt2OGL[ad.format].format,
                    (GLsizei)stride,
                    buf + ad.offset ) );
                break;

            case VTXSEM_NORMAL:
                GN_OGL_CHECK( glNormalPointer(
                    sClrFmt2OGL[ad.format].format,
                    (GLsizei)stride,
                    buf + ad.offset ) );
                break;

            case VTXSEM_COLOR0:
                GN_OGL_CHECK( glColorPointer(
                    sClrFmt2OGL[ad.format].components,
                    sClrFmt2OGL[ad.format].format,
                    (GLsizei)stride,
                    buf+ad.offset ) );
                break;

            case VTXSEM_COLOR1:
                if( GLEW_EXT_secondary_color )
                {
                    GN_OGL_CHECK( glSecondaryColorPointerEXT(
                        sClrFmt2OGL[ad.format].components,
                        sClrFmt2OGL[ad.format].format,
                        (GLsizei)stride,
                        const_cast<uint8_t*>(buf+ad.offset) ) );
                }
                else if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
                {
                    GN_OGL_CHECK( glVertexAttribPointerARB(
                        sVtxSem2OGL[sem].semantic,
                        sClrFmt2OGL[ad.format].components,
                        sClrFmt2OGL[ad.format].format,
                        sClrFmt2OGL[ad.format].normalization,
                        (GLsizei)stride,
                        buf+ad.offset ) );
                }
                break;

            case VTXSEM_FOGCOORD:
                if( GLEW_EXT_fog_coord )
                {
                    GN_OGL_CHECK( glFogCoordPointerEXT(
                        sClrFmt2OGL[ad.format].format,
                        (GLsizei)stride,
                        const_cast<uint8_t*>(buf+ad.offset) ) );
                }
                else if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
                {
                    GN_OGL_CHECK( glVertexAttribPointerARB(
                        sVtxSem2OGL[sem].attribute,
                        sClrFmt2OGL[ad.format].components,
                        sClrFmt2OGL[ad.format].format,
                        sClrFmt2OGL[ad.format].normalization,
                        (GLsizei)stride,
                        buf+ad.offset ) );
                }
                break;

            case VTXSEM_WEIGHT:
            case VTXSEM_TANGENT:
            case VTXSEM_BINORMAL:
                if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
                {
                    GN_OGL_CHECK( glVertexAttribPointerARB(
                        sVtxSem2OGL[sem].attribute,
                        sClrFmt2OGL[ad.format].components,
                        sClrFmt2OGL[ad.format].format,
                        sClrFmt2OGL[ad.format].normalization,
                        (GLsizei)stride,
                        buf + ad.offset ) );
                }
                else
                {
                    GN_WARN( "unsupported vertex semantic!" );
                }
                break;

            case VTXSEM_TEX0:
            case VTXSEM_TEX1:
            case VTXSEM_TEX2:
            case VTXSEM_TEX3:
            case VTXSEM_TEX4:
            case VTXSEM_TEX5:
            case VTXSEM_TEX6:
            case VTXSEM_TEX7:
                texStage = sem - VTXSEM_TEX0;
                if( texStage < getRenderer().getCaps(CAPS_MAX_TEXTURE_STAGES) )
                {
                    getRenderer().chooseClientTextureStage( texStage );
                    GN_OGL_CHECK( glTexCoordPointer(
                        sClrFmt2OGL[ad.format].components,
                        sClrFmt2OGL[ad.format].format,
                        (GLsizei)stride,
                        buf + ad.offset ) );
                }
                else
                {
                    GN_WARN( "unsupported vertex semantic!" );
                }
                break;

            default:
                GN_ASSERT_EX( 0, "unexpected semantic value!" );
                break;
        }
    }

    GN_UNGUARD_SLOW;
}
