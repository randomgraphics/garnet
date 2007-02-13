#include "pch.h"
#include "oglVtxFmt.h"
#include "oglRenderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// Local classes and functions
// *****************************************************************************

static const GLuint sWGHT_ATTRIB = 1;
static const GLuint sCLR1_ATTRIB = 4;
static const GLuint sFOG_ATTRIB  = 5;
static const GLuint sTANG_ATTRIB = 6;
static const GLuint sBNML_ATTRIB = 7;

//
// Compose VTXSEM_TEXn tag based on texture stage.
// -----------------------------------------------------------------------------
static GN_INLINE GN::gfx::VtxSem sGetVtxSemFromTexStage( size_t stage )
{
    GN::gfx::VtxSem sem = GN::gfx::VTXSEM_TEX0;
    if( stage < 10 )
    {
        sem.u8[3] = (unsigned char)( sem.u8[3] + stage );
    }
    else if( stage < 16 )
    {
        sem.u8[3] = (unsigned char)( 'A' + stage );
    }
    else
    {
        GN_ERROR(sLogger)( "texture stage is too large!" );
        sem = GN::gfx::VTXSEM_NULL;
    }
    return sem;
}

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

    if( !setupStreamBindings() || !setupStateBindings() ) return failure();

    // success
    return success();

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
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::bind() const
{
    GN_GUARD_SLOW;

    for( size_t i = 0; i < mStateBindings.size(); ++i )
    {
        const StateBinding & sb = mStateBindings[i];
        GN_ASSERT( sb.func );
        sb.func( sb.info );
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::bindBuffer(
    size_t index, const UInt8 * buf, size_t stride ) const
{
    GN_GUARD_SLOW;

    GN_ASSERT( index < mStreamBindings.size() );

    const StreamBinding & sb = mStreamBindings[index];

    for( size_t i = 0; i < sb.size(); ++i )
    {
        sb[i].bind( buf, stride );
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::setupStreamBindings()
{
    GN_GUARD;

    mStreamBindings.resize( mFormat.calcNumStreams() );
    for( size_t i = 0; i < mFormat.count; ++i )
    {
        const VtxFmtDesc::AttribDesc & a = mFormat.attribs[i];
        GN_ASSERT( a.stream < mStreamBindings.size() );

        // setup stream binding
        StreamBinding & sb = mStreamBindings[a.stream];
        sb.resize( sb.size() + 1 );
        if( !setupAttribBinding( sb.back(), a ) ) return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::setupAttribBinding(
    AttribBinding & ab, const VtxFmtDesc::AttribDesc & desc )
{
    ab.func = NULL;
    ab.info.self = NULL;
    ab.info.offset = desc.offset;
    ab.info.texStage = 0;
    ab.info.attribute = (GLuint)-1;

    switch( desc.semantic.u32 )
    {
        case GN_MAKE_FOURCC('P','O','S','0') :
            ab.func = &sSetVertexPointer;
            break;

        case GN_MAKE_FOURCC('W','G','H','T') :
            if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
            {
                ab.func = &sSetVertexAttributePointer;
                ab.info.attribute = sWGHT_ATTRIB;
            }
            else
            {
                ab.func = &sDummyStreamBinding;
            }
            break;

        case GN_MAKE_FOURCC('N','M','L','0') :
            ab.func = &sSetNormalPointer;
            break;

        case GN_MAKE_FOURCC('C','L','R','0') :
            ab.func = &sSetColorPointer;
            break;

        case GN_MAKE_FOURCC('C','L','R','1') :
            if( GLEW_EXT_secondary_color )
            {
                ab.func = &sSetSecondaryColorPointer;
            }
            else if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
            {
                ab.func = &sSetVertexAttributePointer;
                ab.info.attribute = sCLR1_ATTRIB;
            }
            else
            {
                ab.func = &sDummyStreamBinding;
            }
            break;

        case GN_MAKE_FOURCC('F','O','G','0') :
            if( GLEW_EXT_fog_coord )
            {
                ab.func = &sSetFogPointer;
            }
            else if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
            {
                ab.func = &sSetVertexAttributePointer;
                ab.info.attribute = sFOG_ATTRIB;
            }
            else
            {
                ab.func = &sDummyStreamBinding;
            }
            break;

        case GN_MAKE_FOURCC('T','A','N','G') :
            if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
            {
                ab.func = &sSetVertexAttributePointer;
                ab.info.attribute = sTANG_ATTRIB;
            }
            else
            {
                ab.func = &sDummyStreamBinding;
            }
            break;

        case GN_MAKE_FOURCC('B','N','M','L') :
            if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
            {
                ab.func = &sSetVertexAttributePointer;
                ab.info.attribute = sBNML_ATTRIB;
            }
            else
            {
                ab.func = &sDummyStreamBinding;
            }
            break;

        case GN_MAKE_FOURCC('T','E','X','0') :
        case GN_MAKE_FOURCC('T','E','X','1') :
        case GN_MAKE_FOURCC('T','E','X','2') :
        case GN_MAKE_FOURCC('T','E','X','3') :
        case GN_MAKE_FOURCC('T','E','X','4') :
        case GN_MAKE_FOURCC('T','E','X','5') :
        case GN_MAKE_FOURCC('T','E','X','6') :
        case GN_MAKE_FOURCC('T','E','X','7') :
        case GN_MAKE_FOURCC('T','E','X','8') :
        case GN_MAKE_FOURCC('T','E','X','9') :
            ab.func = &sSetTexCoordPointer;
            ab.info.self = this;
            ab.info.texStage = desc.semantic.u8[3] - VTXSEM_TEX0.u8[3];
            break;

        case GN_MAKE_FOURCC('T','E','X','A') :
        case GN_MAKE_FOURCC('T','E','X','B') :
        case GN_MAKE_FOURCC('T','E','X','C') :
        case GN_MAKE_FOURCC('T','E','X','D') :
        case GN_MAKE_FOURCC('T','E','X','E') :
        case GN_MAKE_FOURCC('T','E','X','F') :
            ab.func = &sSetTexCoordPointer;
            ab.info.self = this;
            ab.info.texStage = desc.semantic.u8[3] - VTXSEM_TEXA.u8[3] + 10;
            break;

        default :
            GN_ERROR(sLogger)( "unsupport vertex semantics: %s", desc.semantic.toStr() );
            return false;
    };

    switch ( desc.format )
    {
        case GN::gfx::FMT_FLOAT1 :
            ab.info.format = GL_FLOAT;
            ab.info.components = 1;
            ab.info.normalization = false;
            break;

        case GN::gfx::FMT_FLOAT2 :
            ab.info.format = GL_FLOAT;
            ab.info.components = 2;
            ab.info.normalization = false;
            break;

        case GN::gfx::FMT_FLOAT3 :
            ab.info.format = GL_FLOAT;
            ab.info.components = 3;
            ab.info.normalization = false;
            break;

        case GN::gfx::FMT_FLOAT4 :
            ab.info.format = GL_FLOAT;
            ab.info.components = 4;
            ab.info.normalization = false;
            break;

        case GN::gfx::FMT_D3DCOLOR :;
            ab.info.format = GL_UNSIGNED_BYTE;
            ab.info.components = 4;
            ab.info.normalization = true;
            break;

        default:
            GN_ERROR(sLogger)( "unsupport color format: %d", desc.format );
            return false;
    }

    // success
    GN_ASSERT( ab.func );
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::sSetVertexPointer(
    const AttribBindingInfo & info, const UInt8 * buf, size_t stride )
{
    GN_OGL_CHECK( glVertexPointer(
                    info.components,
                    info.format,
                    (GLsizei)stride,
                    buf + info.offset ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::sSetNormalPointer(
    const AttribBindingInfo & info, const UInt8 * buf, size_t stride )
{
    GN_OGL_CHECK( glNormalPointer(
                    info.format,
                    (GLsizei)stride,
                    buf + info.offset ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::sSetColorPointer(
    const AttribBindingInfo & info, const UInt8 * buf, size_t stride )
{
    GN_OGL_CHECK( glColorPointer(
                    info.components,
                    info.format,
                    (GLsizei)stride,
                    buf + info.offset ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::sSetSecondaryColorPointer(
    const AttribBindingInfo & info, const UInt8 * buf, size_t stride )
{
    GN_ASSERT( GLEW_EXT_secondary_color );
    GN_OGL_CHECK( glSecondaryColorPointerEXT(
                    info.components,
                    info.format,
                    (GLsizei)stride,
                    (GLvoid*)(buf + info.offset) ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::sSetFogPointer(
    const AttribBindingInfo & info, const UInt8 * buf, size_t stride )
{
    GN_ASSERT( GLEW_EXT_fog_coord );
    GN_OGL_CHECK( glFogCoordPointerEXT(
                    info.format,
                    (GLsizei)stride,
                    (GLvoid*)(buf + info.offset) ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::sSetTexCoordPointer(
    const AttribBindingInfo & info, const UInt8 * buf, size_t stride )
{
    GN_ASSERT( info.self );
    OGLRenderer & r = info.self->getRenderer();
    r.chooseClientTextureStage( info.texStage );
    GN_OGL_CHECK( glTexCoordPointer(
        info.components,
        info.format,
        (GLsizei)stride,
        buf + info.offset ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::sSetVertexAttributePointer(
    const AttribBindingInfo & info, const UInt8 * buf, size_t stride )
{
    GN_ASSERT( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader );
    GN_ASSERT( 0 <= info.attribute && info.attribute < 16 );
    GN_OGL_CHECK( glVertexAttribPointerARB(
                    info.attribute,
                    info.components,
                    info.format,
                    info.normalization,
                    (GLsizei)stride,
                    buf + info.offset ) );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::setupStateBindings()
{
    GN_GUARD;

    size_t numStages = getRenderer().getCaps( CAPS_MAX_TEXTURE_STAGES );

    mStateBindings.resize( 8 + numStages );

#if GN_DEBUG_BUILD
    // fill invalid data
    for( size_t i = 0; i < mStateBindings.size(); ++i )
    {
        mStateBindings[i].func = NULL;
        mStateBindings[i].info.self = NULL;
        mStateBindings[i].info.texStage  = (size_t)-1;
        mStateBindings[i].info.semantic  = (GLenum)-1;
        mStateBindings[i].info.attribute = (GLuint)-1;
    }
#endif

    // VTXSEM_POS0
    mStateBindings[0].func = mFormat.findAttrib( VTXSEM_POS0 ) ? &sEnableClientState : &sDisableClientState;
    mStateBindings[0].info.semantic = GL_VERTEX_ARRAY;

    // VTXSEM_WGHT
    if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
    {
        mStateBindings[1].func = mFormat.findAttrib( VTXSEM_WGHT ) ? &sEnableVAA : &sDisableVAA;
        mStateBindings[1].info.attribute = sWGHT_ATTRIB;
    }
    else
    {
        mStateBindings[1].func = &sDummyStateBinding;
    }

    // VTXSEM_NML0
    mStateBindings[2].func = mFormat.findAttrib( VTXSEM_NML0 ) ? &sEnableClientState : &sDisableClientState;
    mStateBindings[2].info.semantic = GL_NORMAL_ARRAY;

    // VTXSEM_CLR0
    mStateBindings[3].func = mFormat.findAttrib( VTXSEM_CLR0 ) ? &sEnableClientState : &sDisableClientState;
    mStateBindings[3].info.semantic = GL_COLOR_ARRAY;

    // VTXSEM_CLR1
    if( GLEW_EXT_secondary_color )
    {
        mStateBindings[4].func = mFormat.findAttrib( VTXSEM_CLR1 ) ? &sEnableClientState : &sDisableClientState;
        mStateBindings[4].info.semantic = GL_SECONDARY_COLOR_ARRAY_EXT;
    }
    else if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
    {
        mStateBindings[4].func = mFormat.findAttrib( VTXSEM_WGHT ) ? &sEnableVAA : &sDisableVAA;;
        mStateBindings[4].info.attribute = sCLR1_ATTRIB;
    }
    else
    {
        mStateBindings[4].func = &sDummyStateBinding;
    }

    // VTXSEM_FOG 
    if( GLEW_EXT_fog_coord )
    {
        mStateBindings[5].func = mFormat.findAttrib( VTXSEM_FOG ) ? &sEnableClientState : &sDisableClientState;
        mStateBindings[5].info.semantic = GL_FOG_COORDINATE_ARRAY_EXT;
    }
    else if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
    {
        mStateBindings[5].func = mFormat.findAttrib( VTXSEM_FOG ) ? &sEnableVAA : &sDisableVAA;;
        mStateBindings[5].info.attribute = sFOG_ATTRIB;
    }
    else
    {
        mStateBindings[5].func = &sDummyStateBinding;
    }

    // VTXSEM_TANG
    if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
    {
        mStateBindings[6].func = mFormat.findAttrib( VTXSEM_TANG ) ? &sEnableVAA : &sDisableVAA;;
        mStateBindings[6].info.attribute = sTANG_ATTRIB;
    }
    else
    {
        mStateBindings[6].func = &sDummyStateBinding;
    }

    // VTXSEM_BNML
    if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
    {
        mStateBindings[7].func = mFormat.findAttrib( VTXSEM_BNML ) ? &sEnableVAA : &sDisableVAA;;
        mStateBindings[7].info.attribute = sBNML_ATTRIB;
    }
    else
    {
        mStateBindings[7].func = &sDummyStateBinding;
    }

    // VTXSEM_TEXN
    for( size_t i = 0; i < numStages; ++i )
    {
        mStateBindings[8+i].func = mFormat.findAttrib( sGetVtxSemFromTexStage(i) ) ? &sEnableTexArray : &sDisableTexArray;
        mStateBindings[8+i].info.self     = this;
        mStateBindings[8+i].info.texStage = i;
        mStateBindings[8+i].info.semantic = GL_TEXTURE_COORD_ARRAY;
    }

#if GN_DEBUG_BUILD
    // fill invalid data
    for( size_t i = 0; i < mStateBindings.size(); ++i )
    {
        GN_ASSERT( mStateBindings[i].func );
    }
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::sEnableClientState( const StateBindingInfo & info )
{
    GN_OGL_CHECK( glEnableClientState( info.semantic ) );
}
//
void GN::gfx::OGLVtxFmt::sDisableClientState( const StateBindingInfo & info ) { GN_OGL_CHECK( glDisableClientState( info.semantic ) ); }
//
void GN::gfx::OGLVtxFmt::sEnableVAA( const StateBindingInfo & info )
{
    GN_ASSERT( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader );
    GN_OGL_CHECK( glEnableVertexAttribArrayARB( info.attribute ) );
}
//
void GN::gfx::OGLVtxFmt::sDisableVAA( const StateBindingInfo & info )
{
    GN_ASSERT( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader );
    GN_OGL_CHECK( glDisableVertexAttribArrayARB( info.attribute ) );
}
//
void GN::gfx::OGLVtxFmt::sEnableTexArray( const StateBindingInfo & info )
{
    GN_ASSERT( info.self );
    info.self->getRenderer().chooseClientTextureStage( info.texStage );
    GN_OGL_CHECK( glEnableClientState( GL_TEXTURE_COORD_ARRAY ) );
}
//
void GN::gfx::OGLVtxFmt::sDisableTexArray( const StateBindingInfo & info )
{
    GN_ASSERT( info.self );
    info.self->getRenderer().chooseClientTextureStage( info.texStage );
    GN_OGL_CHECK( glDisableClientState( GL_TEXTURE_COORD_ARRAY ) );
}
