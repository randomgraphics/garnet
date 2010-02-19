#include "pch.h"
#include "oglVtxFmt.h"
#include "oglGpu.h"
#include "oglVtxBuf.h"

using namespace GN::gfx;

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.OGL.VtxFmt");

// *****************************************************************************
// Local classes and functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static size_t inline sCalcNumStreams( const VertexFormat & vf )
{
    size_t n = 0;
    for( size_t i = 0; i < vf.numElements; ++i )
    {
        const VertexElement & e =  vf.elements[i];
        if( e.stream >= n ) n = e.stream + 1;
    }
    return n;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::Init( const VertexFormat & format, const OGLBasicGpuProgram * program )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLVtxFmt, () );

    mFormat = format;
    mProgram = program;

    mValid = setupStateBindings();

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::Quit()
{
    GN_GUARD;

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::bindStates() const
{
    GN_GUARD_SLOW;

    if( !mValid ) return false;

    for( size_t i = 0; i < mStateBindings.size(); ++i )
    {
        const StateBinding & sb = mStateBindings[i];
        GN_ASSERT( sb.func );
        sb.func( sb.info );
    }

    return true;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::OGLVtxFmt::bindBuffers(
     const VertexBufferBinding * bindings,
     size_t                      numbufs,
     size_t                      startvtx ) const
{
    GN_GUARD_SLOW;

    if( !mValid ) return false;

    for( size_t i = 0; i < mAttribBindings.size(); ++i )
    {
        const AttribBinding & ab = mAttribBindings[i];

        size_t stream = ab.info.stream;

        if( stream >= numbufs )
        {
            GN_ERROR(sLogger)(
                "Current vertex format requires at least %u vertex buffers. But only %u are provided.",
                stream+1, numbufs );
            return false;
        }

        const VertexBufferBinding & b = bindings[stream];
        const OGLBasicVtxBuf * vb = SafeCastPtr<const OGLBasicVtxBuf>( b.vtxbuf.get() );

        const UInt8 * vtxdata = vb ? (const UInt8*)vb->getVtxData() : NULL;
        size_t       stride  = b.stride;
        size_t       offset  = b.offset;

        ab.bind( vtxdata + (startvtx+offset) * stride, stride );
    }

    return true;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::OGLVtxFmt::bindRawMemoryBuffer( const void * data, size_t stride ) const
{
    if( !mValid ) return false;

    for( size_t i = 0; i < mAttribBindings.size(); ++i )
    {
        const AttribBinding & ab = mAttribBindings[i];

        if( ab.info.stream > 0 )
        {
            GN_ERROR(sLogger)(
                "Current vertex format requires at least %u vertex buffers. But only 1 are provided.",
                ab.info.stream+1 );
            return false;
        }

        ab.bind( (const UInt8*)data, stride );
    }

    return true;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::setupStateBindings()
{
    GN_GUARD;

    UInt32 maxAttributes = getGpu().getOGLCaps().maxVertexAttributes;
    UInt32 maxTextures = getGpu().GetCaps().maxTextures;

    bool hasVertex = false;
    bool hasNormal = false;
    bool hasColor0 = false;
    bool hasColor1 = false;
    bool hasFog = false;
    std::vector<bool> hasAttrib( maxAttributes, false );
    std::vector<bool> hasTexCoord( maxTextures, false );

    OGLVertexBindingDesc vbd;

    mAttribBindings.reserve( mFormat.numElements );

    for( size_t i = 0; i < mFormat.numElements; ++i )
    {
        const VertexElement & e = mFormat.elements[i];

        AttribBinding ab;
        ab.info.self = this;
        ab.info.stream = e.stream;
        ab.info.offset = e.offset;

        // get binding information
        if( !getVertexBindingDesc( vbd, e.binding, e.bindingIndex ) )
        {
            GN_WARN(sLogger)(
                "Vertex element (name=%s index=%d) is ignored, since it is neither used by "
                "current active GPU program nor binding to any of conventional OpenGL attributes.",
                e.binding,
                e.bindingIndex );
            continue;
        }

        switch( vbd.semantic )
        {
            case VERTEX_SEMANTIC_VERTEX:
                ab.func = &sSetVertexPointer;
                hasVertex = true;
                break;

            case VERTEX_SEMANTIC_ATTRIBUTE:
                ab.func = &sSetVertexAttributePointer;
                ab.info.index = vbd.index;
                hasAttrib[vbd.index] = true;
                break;

            case VERTEX_SEMANTIC_NORMAL:
                ab.func = &sSetNormalPointer;
                hasNormal = true;
                break;

            case VERTEX_SEMANTIC_TEXCOORD:
                ab.func = &sSetTexCoordPointer;
                ab.info.index = vbd.index;
                hasTexCoord[vbd.index] = true;
                break;

            case VERTEX_SEMANTIC_COLOR:
                if( 0 == vbd.index )
                {
                    ab.func = &sSetColorPointer;
                    hasColor0 = true;
                }
                else
                {
                    GN_ASSERT( 1 == vbd.index );
                    ab.func = &sSetSecondaryColorPointer;
                    hasColor1 = true;
                }
                break;

            case VERTEX_SEMANTIC_FOG:
                ab.func = &sSetFogPointer;
                hasFog = true;
                break;

            default:
                GN_UNEXPECTED();
                return false;
        }

        switch ( e.format.alias )
        {
            case ColorFormat::FLOAT1 :
                ab.info.format = GL_FLOAT;
                ab.info.components = 1;
                ab.info.normalization = false;
                break;

            case ColorFormat::FLOAT2 :
                ab.info.format = GL_FLOAT;
                ab.info.components = 2;
                ab.info.normalization = false;
                break;

            case ColorFormat::FLOAT3 :
                ab.info.format = GL_FLOAT;
                ab.info.components = 3;
                ab.info.normalization = false;
                break;

            case ColorFormat::FLOAT4 :
                ab.info.format = GL_FLOAT;
                ab.info.components = 4;
                ab.info.normalization = false;
                break;

            case ColorFormat::RGBA32 :
                ab.info.format = GL_UNSIGNED_BYTE;
                ab.info.components = 4;
                ab.info.normalization = true;
                break;

            default:
                GN_ERROR(sLogger)( "unsupport vertex format: %s", e.format.toString().GetRawPtr() );
                return false;
        }

        GN_ASSERT( ab.func );

        mAttribBindings.append( ab );
    }

    // ===================
    // setup state binding
    // ===================

    StateBinding sb;
    sb.info.self = this;

    // position
    sb.func = hasVertex ? &sEnableClientState : &sDisableClientState;
    sb.info.semantic = GL_VERTEX_ARRAY;
    mStateBindings.append( sb );

    // normal
    sb.func = hasNormal ? &sEnableClientState : &sDisableClientState;
    sb.info.semantic = GL_NORMAL_ARRAY;
    mStateBindings.append( sb );

    // color0
    sb.func = hasColor0 ? &sEnableClientState : &sDisableClientState;
    sb.info.semantic = GL_COLOR_ARRAY;
    mStateBindings.append( sb );

    // color1
    if( GLEW_EXT_secondary_color )
    {
        sb.func = hasColor1 ? &sEnableClientState : &sDisableClientState;
        sb.info.semantic = GL_SECONDARY_COLOR_ARRAY_EXT;
        mStateBindings.append( sb );
    }

    // has fog
    sb.func = hasFog ? &sEnableClientState : &sDisableClientState;
    sb.info.semantic = GL_FOG_COORDINATE_ARRAY_EXT;
    mStateBindings.append( sb );

    // vertex attributes
    for( UInt32 i = 0; i < maxAttributes; ++i )
    {
        sb.func = hasAttrib[i] ? &sEnableVAA : &sDisableVAA;
        sb.info.attribute = i;
        mStateBindings.append( sb );
    }

    // texture coordinates
    for( UInt32 i = 0; i < maxTextures; ++i )
    {
        sb.func          = hasTexCoord[i] ? &sEnableTexArray : &sDisableTexArray;
        sb.info.self     = this;
        sb.info.texStage = i;
        sb.info.semantic = GL_TEXTURE_COORD_ARRAY;
        mStateBindings.append( sb );
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::getVertexBindingDesc(
    OGLVertexBindingDesc & vbd,
    const char           * bindingName,
    UInt8                  bindingIndex )
{
#if GN_BUILD_ENABLE_ASSERT
    vbd.semantic = (OGLVertexSemantic)-1;
    vbd.index = (UInt8)-1;
#endif

    //
    // try get binding information from GPU program
    //
    if( mProgram && mProgram->getBindingDesc( vbd, bindingName, bindingIndex ) )
    {
        return true;
    }

    //
    // then try stanadard/predefined bindings
    //

    UInt32 maxAttributes = getGpu().getOGLCaps().maxVertexAttributes;
    UInt32 maxTextures = getGpu().GetCaps().maxTextures;

    if( ( 0 == StringCompareI( "position", bindingName ) ||
          0 == StringCompareI( "pos", bindingName ) ||
          0 == StringCompareI( "gl_vertex", bindingName ) )
        &&
        0 == bindingIndex )
    {
        vbd.semantic = VERTEX_SEMANTIC_VERTEX;
        vbd.index = 0;
    }
    else if( ( 0 == StringCompareI( "attribute", bindingName ) ||
               0 == StringCompareI( "VertexArrribute", bindingName ) )
             &&
             bindingIndex < maxAttributes )
    {
        vbd.semantic = VERTEX_SEMANTIC_ATTRIBUTE;
        vbd.index = bindingIndex;
    }
    else if( (0 == StringCompareI( "normal", bindingName ) || 0 == StringCompareI( "nml", bindingName ) ) && 0 == bindingIndex )
    {
        vbd.semantic = VERTEX_SEMANTIC_NORMAL;
        vbd.index = 0;
    }
    else if( 0 == StringCompareI( "color", bindingName ) )
    {
        if( 0 == bindingIndex )
        {
            vbd.semantic = VERTEX_SEMANTIC_COLOR;
            vbd.index = 0;
        }
        else if( 1 == bindingIndex && GLEW_EXT_secondary_color )
        {
            vbd.semantic = VERTEX_SEMANTIC_COLOR;
            vbd.index = 1;
        }
        else
        {
            return false;
        }
    }
    else if( 0 == StringCompareI( "fog", bindingName ) && GLEW_EXT_fog_coord )
    {
        vbd.semantic = VERTEX_SEMANTIC_FOG;
        vbd.index = 0;
    }
    else if( 0 == StringCompareI( "texcoord", bindingName ) && bindingIndex < maxTextures )
    {
        vbd.semantic = VERTEX_SEMANTIC_TEXCOORD;
        vbd.index = bindingIndex;
    }
    else
    {
        return false;
    }

    // make sure vbd has valid value.
#if GN_BUILD_ENABLE_ASSERT
    GN_ASSERT( (OGLVertexSemantic)-1 != vbd.semantic );
    GN_ASSERT( (UInt8)-1 != vbd.index );
#endif

    // success
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
    OGLGpu & r = info.self->getGpu();
    r.chooseClientTextureStage( info.index );
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
    GN_ASSERT( info.index < 16 );
    GN_OGL_CHECK( glVertexAttribPointerARB(
                    info.index,
                    info.components,
                    info.format,
                    info.normalization,
                    (GLsizei)stride,
                    buf + info.offset ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::sEnableClientState( const StateBindingInfo & info )
{
    GN_OGL_CHECK( glEnableClientState( info.semantic ) );
}
//
void GN::gfx::OGLVtxFmt::sDisableClientState( const StateBindingInfo & info )
{
    GN_OGL_CHECK( glDisableClientState( info.semantic ) );
}
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
    info.self->getGpu().chooseClientTextureStage( info.texStage );
    GN_OGL_CHECK( glEnableClientState( GL_TEXTURE_COORD_ARRAY ) );
}
//
void GN::gfx::OGLVtxFmt::sDisableTexArray( const StateBindingInfo & info )
{
    GN_ASSERT( info.self );
    info.self->getGpu().chooseClientTextureStage( info.texStage );
    GN_OGL_CHECK( glDisableClientState( GL_TEXTURE_COORD_ARRAY ) );
}
