#include "pch.h"
#include "d3dRenderer.h"
#include "d3dShader.h"
#include "d3dTexture.h"
#include "d3dVertexDecl.h"
#include "d3dVtxBuf.h"
#include "d3dIdxBuf.h"

// *****************************************************************************
// local functions
// *****************************************************************************

//
// Functor to compare vertex format
//
struct EqualFormat
{
    const GN::gfx::VtxFmtDesc & format;
    EqualFormat( const GN::gfx::VtxFmtDesc & f ) : format(f) {}
    bool operator()( const GN::gfx::D3DVtxBindingDesc & vbd ) const { return format == vbd.format; }
};

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::resourceDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::list<D3DResource*>::iterator i = mResourceList.begin();
    while( i != mResourceList.end() )
    {
        if( !(*i)->deviceCreate() ) return false;
        ++i;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::resourceDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::list<D3DResource*>::iterator i = mResourceList.begin();
    while( i != mResourceList.end() )
    {
        if( !(*i)->deviceRestore() ) return false;
        ++i;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::resourceDeviceDispose()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::list<D3DResource*>::reverse_iterator i = mResourceList.rbegin();
    while( i != mResourceList.rend() )
    {
        (*i)->deviceDispose();
        ++i;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::resourceDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::list<D3DResource*>::reverse_iterator i = mResourceList.rbegin();
    while( i != mResourceList.rend() )
    {
        (*i)->deviceDestroy();
        ++i;
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
GN::gfx::D3DRenderer::createShader(
    ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & entry )
{
    GN_GUARD;

    if( !supportShader( type, lang ) )
    {
        GN_ERROR( "Current renderer do not support shading language '%s' for shader type '%s'.",
            shadingLanguage2Str(lang), shaderType2Str(type) );
        return 0;
    }

    switch( type )
    {
        case VERTEX_SHADER :
            switch( lang )
            {
                case LANG_D3D_ASM :
                {
                    AutoRef<D3DVtxShaderAsm> p( new D3DVtxShaderAsm(*this) );
                    if( !p->init( code ) ) return 0;
                    return p.detach();
                }

                case LANG_D3D_HLSL:
                {
                    AutoRef<D3DVtxShaderHlsl> p( new D3DVtxShaderHlsl(*this) );
                    if( !p->init( code, entry ) ) return 0;
                    return p.detach();
                }

                default:
                    GN_ERROR( "unsupport shading language : %s", shadingLanguage2Str(lang) );
                    return 0;
            }

        case PIXEL_SHADER :
            switch( lang )
            {
                case LANG_D3D_ASM :
                {
                    AutoRef<D3DPxlShaderAsm> p( new D3DPxlShaderAsm(*this) );
                    if( !p->init( code ) ) return 0;
                    return p.detach();
                }

                case LANG_D3D_HLSL:
                {
                    AutoRef<D3DPxlShaderHlsl> p( new D3DPxlShaderHlsl(*this) );
                    if( !p->init( code, entry ) ) return 0;
                    return p.detach();
                }

                default:
                    GN_ERROR( "unsupport shading language : %s", shadingLanguage2Str(lang) );
                    return 0;
            }

        default:
            GN_UNEXPECTED(); // program should not reach here
            GN_ERROR( "invalid shader type: %d", type );
            return 0;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::D3DRenderer::createTexture( const TextureDesc & desc,
                                     const TextureLoader & loader )
{
    GN_GUARD;

    AutoRef<D3DTexture> p( new D3DTexture(*this) );
    p->setLoader( loader );
    if( !p->init( desc ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
uint32_t GN::gfx::D3DRenderer::createVtxBinding( const VtxFmtDesc & format )
{
    GN_GUARD;

    uint32_t h = mVtxBindings.findIf( EqualFormat(format) );

    if( 0 == h )
    {
        // create new vertex decl
        D3DVtxBindingDesc vbd;
        vbd.format = format;
        vbd.decl.attach( createD3DVertexDecl( mDevice, format ) );
        if( !vbd.decl ) return 0;

        h = mVtxBindings.add( vbd );
    }

    // success
    return h;

    GN_UNGUARD;
}


//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::D3DRenderer::createVtxBuf(
    size_t bytes, bool dynamic, bool sysCopy, const VtxBufLoader & loader )
{
    GN_GUARD;

    AutoRef<D3DVtxBuf> buf( new D3DVtxBuf(*this) );

    if( !buf->init( bytes, dynamic, sysCopy, loader ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::D3DRenderer::createIdxBuf(
    size_t numIdx, bool dynamic, bool sysCopy, const IdxBufLoader & loader )
{
    GN_GUARD;

    AutoRef<D3DIdxBuf> buf( new D3DIdxBuf(*this) );

    if( !buf->init( numIdx, dynamic, sysCopy, loader ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

