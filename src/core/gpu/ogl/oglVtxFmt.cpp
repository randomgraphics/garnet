#include "pch.h"
#include "oglVtxFmt.h"
#include "oglGpu.h"
#include "oglVtxBuf.h"

using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL.VtxFmt");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::init( const VertexBinding & attributes, const OGLBasicGpuProgram * program )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    mFormat = attributes;

    mValid = setupStateBindings( program );

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
        safeCastPtr<const OGLVtxBufVBO>( bindings[stream].vtxbuf.rawptr() )->bind();
        ab.bind((const void*)(intptr_t)((startvtx + b.offset) * b.stride), b.stride);
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

        ab.bind( (const uint8*)data, stride );
    }

    return true;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::setupStateBindings( const OGLBasicGpuProgram * gpuProgram )
{
    GN_GUARD;

    uint32 maxAttributes = getGpu().getOGLCaps().maxVertexAttributes;
    uint32 maxTextures = getGpu().caps().maxTextures;

    bool hasVertex = false;
    bool hasNormal = false;
    bool hasColor0 = false;
    bool hasColor1 = false;
    bool hasFog = false;
    DynaArray<bool> hasAttrib( maxAttributes, false );
    DynaArray<bool> hasTexCoord( maxTextures, false );

    OGLVertexBindingDesc vbd;

    mAttribBindings.reserve( mFormat.size() );

    for( size_t i = 0; i < mFormat.size(); ++i )
    {
        const VertexElement & e = mFormat[i];

        // get binding information
        if( ( NULL == gpuProgram || !gpuProgram->getBindingDesc( vbd, e.attribute ) ) )
        {
            GN_WARN(sLogger)(
                "Attribute #%d is ignored, since it is not used by "
                "current active GPU program.",
                i );
            continue;
        }

        AttribBinding ab;
        ab.info.self = this;
        ab.info.stream = (uint8)e.stream;
        ab.info.offset = (uint8)e.offset;

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
                GN_ERROR(sLogger)("client side vertex data pointer is not supportd anymore.");
                return false;

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

            case ColorFormat::RGBA8 :
                ab.info.format = GL_UNSIGNED_BYTE;
                ab.info.components = 4;
                ab.info.normalization = true;
                break;

            case ColorFormat::USHORT4 :
                ab.info.format = GL_UNSIGNED_SHORT;
                ab.info.components = 4;
                ab.info.normalization = false;
                break;

            case ColorFormat::SHORT4 :
                ab.info.format = GL_SHORT;
                ab.info.components = 4;
                ab.info.normalization = false;
                break;

            case ColorFormat::UINT4 :
                ab.info.format = GL_UNSIGNED_INT;
                ab.info.components = 4;
                ab.info.normalization = false;
                break;

            case ColorFormat::SINT4 :
                ab.info.format = GL_INT;
                ab.info.components = 4;
                ab.info.normalization = false;
                break;

            default:
                GN_ERROR(sLogger)( "unsupport vertex format: %s", e.format.toString().rawptr() );
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
    for( uint32 i = 0; i < maxAttributes; ++i )
    {
        sb.func = hasAttrib[i] ? &sEnableVAA : &sDisableVAA;
        sb.info.attribute = i;
        mStateBindings.append( sb );
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::getStandardVertexBindingDesc(
    OGLVertexBindingDesc & vbd,
    const char           * bindingName,
    uint8                  bindingIndex )
{
#if GN_ENABLE_ASSERT
    vbd.semantic = (OGLVertexSemantic)-1;
    vbd.index = (uint8)-1;
#endif

    uint32 maxAttributes = getGpu().getOGLCaps().maxVertexAttributes;
    uint32 maxTextures = getGpu().caps().maxTextures;

    if( ( 0 == str::compareI( "position", bindingName ) ||
          0 == str::compareI( "pos", bindingName ) ||
          0 == str::compareI( "gl_vertex", bindingName ) )
        &&
        0 == bindingIndex )
    {
        vbd.semantic = VERTEX_SEMANTIC_VERTEX;
        vbd.index = 0;
    }
    else if( ( 0 == str::compareI( "attribute", bindingName ) ||
               0 == str::compareI( "VertexArrribute", bindingName ) )
             &&
             bindingIndex < maxAttributes )
    {
        vbd.semantic = VERTEX_SEMANTIC_ATTRIBUTE;
        vbd.index = bindingIndex;
    }
    else if( (0 == str::compareI( "normal", bindingName ) || 0 == str::compareI( "nml", bindingName ) ) && 0 == bindingIndex )
    {
        vbd.semantic = VERTEX_SEMANTIC_NORMAL;
        vbd.index = 0;
    }
    else if( 0 == str::compareI( "color", bindingName ) )
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
    else if( 0 == str::compareI( "fog", bindingName ) && GLEW_EXT_fog_coord )
    {
        vbd.semantic = VERTEX_SEMANTIC_FOG;
        vbd.index = 0;
    }
    else if( 0 == str::compareI( "texcoord", bindingName ) && bindingIndex < maxTextures )
    {
        vbd.semantic = VERTEX_SEMANTIC_TEXCOORD;
        vbd.index = bindingIndex;
    }
    else
    {
        return false;
    }

    // make sure vbd has valid value.
#if GN_ENABLE_ASSERT
    GN_ASSERT( (OGLVertexSemantic)-1 != vbd.semantic );
    GN_ASSERT( (uint8)-1 != vbd.index );
#endif

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::sSetVertexPointer(
    const AttribBindingInfo & info, const uint8 * buf, size_t stride )
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
    const AttribBindingInfo & info, const uint8 * buf, size_t stride )
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
    const AttribBindingInfo & info, const uint8 * buf, size_t stride )
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
    const AttribBindingInfo & info, const uint8 * buf, size_t stride )
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
    const AttribBindingInfo & info, const uint8 * buf, size_t stride )
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
void GN::gfx::OGLVtxFmt::sSetVertexAttributePointer(
    const AttribBindingInfo & info, const uint8 * buf, size_t stride )
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