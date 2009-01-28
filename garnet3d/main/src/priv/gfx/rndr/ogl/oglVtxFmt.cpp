#include "pch.h"
#include "oglVtxFmt.h"
#include "oglRenderer.h"

using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL.VtxFmt");

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
bool GN::gfx::OGLVtxFmt::init( const VertexFormat & format )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLVtxFmt, () );

    mFormat = format;

    // calculate default strides
    memset( mDefaultStrides, 0, sizeof(mDefaultStrides) );
    for( size_t i = 0; i < format.numElements; ++i )
    {
        const VertexElement & e = format.elements[i];

        GN_ASSERT( e.format.getBytesPerBlock() > 0 );
        size_t elementsize = e.offset + e.format.getBytesPerBlock();

        if( mDefaultStrides[e.stream] < elementsize )
        {
            mDefaultStrides[e.stream] = elementsize;
        }
    }

    if( !setupStateBindings() ) return failure();

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
void GN::gfx::OGLVtxFmt::bindStates() const
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
bool
GN::gfx::OGLVtxFmt::bindBuffers(
     const void * const * buffers,
     const UInt16       * strides,
     size_t               numbufs,
     size_t               startvtx ) const
{
    GN_GUARD_SLOW;

    for( size_t i = 0; i < mAttribBindings.size(); ++i )
    {
        const AttribBinding & ab = mAttribBindings[i];

        size_t stream = ab.info.stream;

        if( stream >= numbufs )
        {
            GN_ERROR(sLogger)(
                "Current vertex format requires %u vertex buffers. But only %u are provided.",
                stream, numbufs );
            return false;
        }

        size_t stride = strides[stream];
        if( 0 == stride ) stride = mDefaultStrides[stream];

        const UInt8 * ptr = (const UInt8*)buffers[stream];

        ab.bind( ptr + startvtx * stride, stride );
    }

    return true;

    GN_UNGUARD_SLOW;
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

    UInt32 maxAttributes = getRenderer().getOGLCaps().maxVertexAttributes;
    UInt32 maxTextures = getRenderer().getCaps().maxTextures;

    bool hasPos = false;
    bool hasNormal = false;
    bool hasC0 = false;
    bool hasC1 = false;
    bool hasFog = false;
    std::vector<bool> hasvaa( maxAttributes, false );
    std::vector<bool> hastex( maxTextures, false );

    mAttribBindings.resize( mFormat.numElements );

    for( size_t i = 0; i < mFormat.numElements; ++i )
    {
        const VertexElement & e = mFormat.elements[i];

        AttribBinding & ab = mAttribBindings[i];

        ab.info.self = this;
        ab.info.stream = e.stream;
        ab.info.offset = e.offset;

        if( 0 == strCmpI( "position", e.binding ) ||
            0 == strCmpI( "pos", e.binding ) )
        {
            if( 0 != e.bindingIndex )
            {
                GN_ERROR(sLogger)( "bindingIndex of \"position\" must be 0." );
                return false;
            }
            ab.func = &sSetVertexPointer;
            hasPos = true;
        }
        else if( 0 == strCmpI( "attribute", e.binding ) ||
                 0 == strCmpI( "VertexArrribute", e.binding ) )
        {
            if( e.bindingIndex >= maxAttributes )
            {
                GN_ERROR(sLogger)(
                    "vertex attribute index (%u) is larger than maximum allowed value (%u).",
                    e.bindingIndex, maxAttributes );
                return false;
            }

            ab.func = &sSetVertexAttributePointer;
            ab.info.index = e.bindingIndex;
            hasvaa[e.bindingIndex] = true;
        }
        else if( 0 == strCmpI( "normal", e.binding ) )
        {
            ab.func = &sSetNormalPointer;
            hasNormal = true;
        }
        else if( 0 == strCmpI( "color", e.binding ) )
        {
            if( 0 == e.bindingIndex )
            {
                ab.func = &sSetColorPointer;
                hasC0 = true;
            }
            else if( 1 == e.bindingIndex )
            {
                if( GLEW_EXT_secondary_color )
                {
                    ab.func = &sSetSecondaryColorPointer;
                    hasC1 = true;
                }
                else
                {
                    GN_ERROR(sLogger)("current hardware does not support EXT_secondary_color" );
                    return false;
                }
            }
        }
        else if( 0 == strCmpI( "fog", e.binding ) )
        {
            if( GLEW_EXT_fog_coord )
            {
                ab.func = &sSetFogPointer;
                hasFog = true;
            }
            else
            {
                GN_ERROR(sLogger)("current hardware does not support EXT_fog_coord" );
                return false;
            }
        }
        else if( 0 == strCmpI( "texcoord", e.binding ) )
        {
            if( e.bindingIndex >= maxTextures )
            {
                GN_ERROR(sLogger)(
                    "texcoordinate index (%u) is larger than maximum allowed value (%u).",
                    e.bindingIndex, maxTextures );
                return false;
            }

            ab.func = &sSetTexCoordPointer;
            ab.info.self = this;
            ab.info.index = e.bindingIndex;
            hastex[e.bindingIndex] = true;
        }
        else
        {
            GN_ERROR(sLogger)( "unsupport vertex binding : %s(%d)", e.binding, e.bindingIndex );
            return false;
        };

        switch ( e.format.alias )
        {
            case COLOR_FORMAT_FLOAT1 :
                ab.info.format = GL_FLOAT;
                ab.info.components = 1;
                ab.info.normalization = false;
                break;

            case COLOR_FORMAT_FLOAT2 :
                ab.info.format = GL_FLOAT;
                ab.info.components = 2;
                ab.info.normalization = false;
                break;

            case COLOR_FORMAT_FLOAT3 :
                ab.info.format = GL_FLOAT;
                ab.info.components = 3;
                ab.info.normalization = false;
                break;

            case COLOR_FORMAT_FLOAT4 :
                ab.info.format = GL_FLOAT;
                ab.info.components = 4;
                ab.info.normalization = false;
                break;

            case COLOR_FORMAT_RGBA32 :
                ab.info.format = GL_UNSIGNED_BYTE;
                ab.info.components = 4;
                ab.info.normalization = true;
                break;

            default:
                GN_ERROR(sLogger)( "unsupport vertex format: %s", e.format.toString().cptr() );
                return false;
        }

        GN_ASSERT( ab.func );
    }

    // ===================
    // setup state binding
    // ===================

    StateBinding sb;
    sb.info.self = this;

    // position
    sb.func = hasPos ? &sEnableClientState : &sDisableClientState;
    sb.info.semantic = GL_VERTEX_ARRAY;
    mStateBindings.append( sb );

    // normal
    sb.func = hasNormal ? &sEnableClientState : &sDisableClientState;
    sb.info.semantic = GL_NORMAL_ARRAY;
    mStateBindings.append( sb );

    // color0
    sb.func = hasC0 ? &sEnableClientState : &sDisableClientState;
    sb.info.semantic = GL_COLOR_ARRAY;
    mStateBindings.append( sb );

    // color1
    if( GLEW_EXT_secondary_color )
    {
        sb.func = hasC1 ? &sEnableClientState : &sDisableClientState;
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
        sb.func = hasvaa[i] ? &sEnableVAA : &sDisableVAA;
        sb.info.attribute = i;
        mStateBindings.append( sb );
    }

    // texture coordinates
    for( UInt32 i = 0; i < maxTextures; ++i )
    {
        sb.func          = hastex[i] ? &sEnableTexArray : &sDisableTexArray;
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
    GN_ASSERT( 0 <= info.index && info.index < 16 );
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
