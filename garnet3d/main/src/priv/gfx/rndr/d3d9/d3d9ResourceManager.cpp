#include "pch.h"
#include "d3d9Renderer.h"
#include "d3d9Shader.h"
#include "d3d9Texture.h"
#include "d3d9VertexDecl.h"
#include "d3d9VtxBuf.h"
#include "d3d9IdxBuf.h"
#include "d3d9Font.h"
#include "d3d9Quad.h"
#include "d3d9Line.h"

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
    bool operator()( const GN::gfx::D3D9VtxDeclDesc & vbd ) const { return format == vbd.format; }
};

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::resourceDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "Not _ALL_ graphics resouces are released!" );
        return false;
    }

    mFont = new D3D9Font(*this);
    if( !mFont->init() ) return false;

    mQuad = new D3D9Quad(*this);
    if( !mQuad->init() ) return false;

    mLine = new D3D9Line(*this);
    if( !mLine->init() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::resourceDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::list<D3D9Resource*>::iterator i = mResourceList.begin();
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
void GN::gfx::D3D9Renderer::resourceDeviceDispose()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    std::list<D3D9Resource*>::reverse_iterator i = mResourceList.rbegin();
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
void GN::gfx::D3D9Renderer::resourceDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    safeDelete( mFont );
    safeDelete( mQuad );
    safeDelete( mLine );

    // release vertex formats
    mVtxFmts.clear();

    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "All graphics resouces MUST be released, after receiving 'destroy' signal!" );

        std::list<D3D9Resource*>::iterator i = mResourceList.begin();
        while( i != mResourceList.end() )
        {
            D3D9Resource * r = *i;
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
GN::gfx::D3D9Renderer::createShader(
    ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & hints )
{
    GN_GUARD;

    switch( type )
    {
        case SHADER_VS :
            switch( lang )
            {
                case LANG_D3D_ASM :
                {
                    AutoRef<D3D9VtxShaderAsm> p( new D3D9VtxShaderAsm(*this) );
                    if( !p->init( code ) ) return 0;
                    return p.detach();
                }

                case LANG_D3D_HLSL:
                {
                    AutoRef<D3D9VtxShaderHlsl> p( new D3D9VtxShaderHlsl(*this) );
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
                case LANG_D3D_ASM :
                {
                    AutoRef<D3D9PxlShaderAsm> p( new D3D9PxlShaderAsm(*this) );
                    if( !p->init( code ) ) return 0;
                    return p.detach();
                }

                case LANG_D3D_HLSL:
                {
                    AutoRef<D3D9PxlShaderHlsl> p( new D3D9PxlShaderHlsl(*this) );
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
GN::gfx::D3D9Renderer::createTexture( const TextureDesc & desc,
                                     const TextureLoader & loader )
{
    GN_GUARD;

    AutoRef<D3D9Texture> p( new D3D9Texture(*this) );
    p->setLoader( loader );
    if( !p->init( desc ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
uint32_t GN::gfx::D3D9Renderer::createVtxFmt( const VtxFmtDesc & format )
{
    GN_GUARD;

    uint32_t h = mVtxFmts.findIf( EqualFormat(format) );

    if( 0 == h )
    {
        // create new vertex decl
        D3D9VtxDeclDesc vbd;
        vbd.format = format;
        vbd.decl.attach( createD3D9VertexDecl( mDevice, format ) );
        if( !vbd.decl ) return 0;

        h = mVtxFmts.add( vbd );
    }

    // success
    return h;

    GN_UNGUARD;
}


//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::D3D9Renderer::createVtxBuf(
    size_t bytes, bool dynamic, bool sysCopy, const VtxBufLoader & loader )
{
    GN_GUARD;

    AutoRef<D3D9VtxBuf> buf( new D3D9VtxBuf(*this) );

    if( !buf->init( bytes, dynamic, sysCopy, loader ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::D3D9Renderer::createIdxBuf(
    size_t numIdx, bool dynamic, bool sysCopy, const IdxBufLoader & loader )
{
    GN_GUARD;

    AutoRef<D3D9IdxBuf> buf( new D3D9IdxBuf(*this) );

    if( !buf->init( numIdx, dynamic, sysCopy, loader ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

