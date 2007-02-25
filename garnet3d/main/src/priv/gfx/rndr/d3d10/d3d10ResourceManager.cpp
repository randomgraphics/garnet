#include "pch.h"
#include "d3d10Renderer.h"
#include "d3d10Shader.h"
#include "d3d10Texture.h"
//#include "d3d10VertexDecl.h"
#include "d3d10Buffer.h"
//#include "d3d10Font.h"
//#include "d3d10Quad.h"
//#include "d3d10Line.h"

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Renderer::resourceDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "Not _ALL_ graphics resouces are released!" );
        return false;
    }

#ifdef HAS_CG_D3D10
    GN_DX9_CHECK_RV( cgD3D10SetDevice( getDevice() ), false );
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Renderer::resourceDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "All graphics resouces MUST be released, when receiving 'destroy' signal!" );

        std::list<D3D10Resource*>::iterator i = mResourceList.begin();
        while( i != mResourceList.end() )
        {
            D3D10Resource * r = *i;
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
GN::gfx::Shader *
GN::gfx::D3D10Renderer::createShader(
    ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & hints )
{
    GN_GUARD;

    switch( type )
    {
        case SHADER_VS :
            switch( lang )
            {
                case LANG_D3D_HLSL:
                {
                    AutoRef<D3D10VtxShaderHlsl> p( new D3D10VtxShaderHlsl(*this) );
                    if( !p->init( code, hints ) ) return 0;
                    return p.detach();
                }

                default:
                    GN_ERROR(sLogger)( "unsupport shading language : %s", shadingLanguage2Str(lang) );
                    return 0;
            }

        case SHADER_PS :
            switch( lang )
            {
                case LANG_D3D_HLSL:
                {
                    AutoRef<D3D10PxlShaderHlsl> p( new D3D10PxlShaderHlsl(*this) );
                    if( !p->init( code, hints ) ) return 0;
                    return p.detach();
                }

                default:
                    GN_ERROR(sLogger)( "unsupport shading language : %s", shadingLanguage2Str(lang) );
                    return 0;
            }

        case SHADER_GS:
            switch( lang )
            {
                case LANG_D3D_HLSL:
                {
                    AutoRef<D3D10GeoShaderHlsl> p( new D3D10GeoShaderHlsl(*this) );
                    if( !p->init( code, hints ) ) return 0;
                    return p.detach();
                }

                default:
                    GN_ERROR(sLogger)( "unsupport shading language : %s", shadingLanguage2Str(lang) );
                    return 0;
            }

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
GN::gfx::D3D10Renderer::createTexture( const TextureDesc & desc )
{
    GN_GUARD;

    AutoRef<D3D10Texture> p( new D3D10Texture(*this) );
    if( !p->init( desc ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
UInt32 GN::gfx::D3D10Renderer::createVtxFmt( const VtxFmtDesc & format )
{
    return 0;/*
    GN_GUARD;

    UInt32 h = mVtxFmts.findIf( EqualFormat(format) );

    if( 0 == h )
    {
        // create new vertex decl
        D3D10VtxLayoutDesc layout;
        layout.format = format;
        layout.layout.attach( createD3D10VertexLayout( mDevice, format ) );
        if( !layout.layout ) return 0;
        h = mVtxFmts.add( layout );
    }

    // success
    return h;

    GN_UNGUARD;*/
}


//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::D3D10Renderer::createVtxBuf( const VtxBufDesc & desc )
{
    GN_GUARD;

    AutoRef<D3D10VtxBuf> buf( new D3D10VtxBuf(*this) );

    if( !buf->init( desc ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::D3D10Renderer::createIdxBuf( const IdxBufDesc & desc )
{
    GN_GUARD;

    AutoRef<D3D10IdxBuf> buf( new D3D10IdxBuf(*this) );

    if( !buf->init( desc ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

