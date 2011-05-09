#include "pch.h"
#include <d3dcompiler.h>

#if GN_MSVC
#pragma comment( lib, "d3dcompiler.lib" )
#endif // GN_MSVC


static GN::Logger * sLogger = GN::getLogger("GN.d3d11.d3d11ShaderUtils");

// *****************************************************************************
// local function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static GN::StrA sAddLineCountD3D11( const GN::StrA & in )
{
    using namespace GN;

    GN::StrA out( "(  1) : " );

    int line = 1;
    for( const char * s = in.rawptr(); *s; ++s )
    {
        if( '\n' == *s )
        {
            out.append( stringFormat( "\n(%3d) : ", ++line ) );
        }
        else
        {
            out.append( *s );
        }
    }

    return out;
}

//
//
// -----------------------------------------------------------------------------
static uint32 sRefineFlagsD3D11( uint32 flags )
{
#ifndef D3DCOMPILE_DEBUG
#define D3DCOMPILE_DEBUG D3D10_SHADER_DEBUG
#endif

#if GN_ENABLE_DEBUG
    flags |= D3DCOMPILE_DEBUG;
#endif
    return flags;
}

//
//
// -----------------------------------------------------------------------------
static void sPrintShaderCompileErrorD3D11( const char * code, ID3DBlob * err )
{
    GN_GUARD;

    GN_ERROR(sLogger)(
        "\n================== Shader compile failure ===============\n"
        "%s\n"
        "\n---------------------------------------------------------\n"
        "%s\n"
        "\n=========================================================\n",
        code ? sAddLineCountD3D11(code).rawptr() : "Shader code: <EMPTY>",
        err ? (const char*)err->GetBufferPointer() : "Error: <EMPTY>" );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static void sPrintShaderCompileInfoD3D11( const char * hlsl, ID3DBlob * bin )
{
    GN_GUARD;

    GN_ASSERT( hlsl && bin );

    using namespace GN;

    // get ASM code
    AutoComPtr<ID3DBlob> asm_;

    D3DDisassemble(
        bin->GetBufferPointer(),
        bin->GetBufferSize(),
        false,
        NULL,
        &asm_ );

    GN_VTRACE(sLogger)(
        "\n================== Shader compile success ===============\n"
        "%s\n"
        "\n---------------------------------------------------------\n"
        "%s\n"
        "\n=========================================================\n",
        sAddLineCountD3D11(hlsl).rawptr(),
        sAddLineCountD3D11((const char*)asm_->GetBufferPointer()).rawptr() );

    GN_UNGUARD;
}

// *****************************************************************************
// public function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API ID3DBlob * GN::d3d11::compileShader(
    const char   * profile,
    const char   * source,
    size_t         len,
    uint32         flags,
    const char   * entry )
{
    AutoComPtr<ID3DBlob> bin, err;

    if( NULL == source )
    {
        GN_ERROR(sLogger)( "NULL source pointer." );
        return NULL;
    }

    // determine source length
    if( 0 == len ) len = stringLength(source);

    // generate temporary file to store shader source
    StrA filename;
#if GN_ENABLE_DEBUG
    TempFile file;
    if( file.open( "D3D11_shader_source", "wt", TempFile::MANUAL_DELETE ) )
    {
        filename = file.name();
        file.write( source, len, NULL );
        file.close();
    }
#endif

    if( FAILED( D3DCompile(
        source,
        len,
        filename.rawptr(),
        0, // defines
        0, // includes
        entry,
        profile,
        sRefineFlagsD3D11(flags),
        0, // effect compile flags
        &bin,
        &err ) ) )
    {
        sPrintShaderCompileErrorD3D11( source, err );
        return NULL;
    }

    sPrintShaderCompileInfoD3D11( source, bin );

    return bin.detach();
}

//
//
// -----------------------------------------------------------------------------
GN_API ID3D11VertexShader * GN::d3d11::compileAndCreateVS(
    ID3D11Device & dev,
    const char   * source,
    size_t         len,
    uint32         flags,
    const char   * entry,
    const char   * profile,
    ID3DBlob  ** signature )
{
    GN_GUARD;

    AutoComPtr<ID3DBlob> bin( compileShader( profile, source, len, flags, entry ) );
    if( !bin ) return NULL;

    ID3D11VertexShader * vs = createDumpableVS( dev, bin->GetBufferPointer(), bin->GetBufferSize() );
    if( 0 == vs ) return NULL;

    // success
    if( signature ) *signature = bin.detach();
    return vs;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API ID3D11GeometryShader * GN::d3d11::compileAndCreateGS(
    ID3D11Device & dev,
    const char   * source,
    size_t         len,
    uint32         flags,
    const char   * entry,
    const char   * profile,
    ID3DBlob  ** signature )
{
    GN_GUARD;

    AutoComPtr<ID3DBlob> bin( compileShader( profile, source, len, flags, entry ) );
    if( !bin ) return NULL;

    ID3D11GeometryShader * gs = createDumpableGS( dev, bin->GetBufferPointer(), bin->GetBufferSize() );
    if( 0 == gs ) return NULL;

    // success
    if( signature ) *signature = bin.detach();
    return gs;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API ID3D11PixelShader * GN::d3d11::compileAndCreatePS(
    ID3D11Device & dev,
    const char   * source,
    size_t         len,
    uint32         flags,
    const char   * entry,
    const char   * profile,
    ID3DBlob  ** signature )
{
    GN_GUARD;

    AutoComPtr<ID3DBlob> bin( compileShader( profile, source, len, flags, entry ) );
    if( !bin ) return NULL;

    ID3D11PixelShader * ps = createDumpablePS( dev, bin->GetBufferPointer(), bin->GetBufferSize() );
    if( 0 == ps ) return 0;

    // success
    if( signature ) *signature = bin.detach();
    return ps;

    GN_UNGUARD;
}
