#include "pch.h"
#include <d3dcompiler.h>

#if GN_MSVC
#pragma comment( lib, "d3dcompiler.lib" )
#endif // GN_MSVC

static GN::Logger * sLogger = GN::getLogger("GN.d3d9.d3d9ShaderUtils");

// *****************************************************************************
// local function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static GN::StrA sAddLineCountD3D10( const GN::StrA & in )
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
static uint32 sRefineFlagsD3D10( uint32 flags )
{
#if GN_ENABLE_DEBUG
    flags |= D3D10_SHADER_DEBUG;
#endif
    return flags;
}

//
//
// -----------------------------------------------------------------------------
static void sPrintShaderCompileErrorD3D10( const char * code, ID3D10Blob * err )
{
    GN_GUARD;

    GN_ERROR(sLogger)(
        "\n================== Shader compile failure ===============\n"
        "%s\n"
        "\n---------------------------------------------------------\n"
        "%s\n"
        "\n=========================================================\n",
        code ? sAddLineCountD3D10(code).rawptr() : "Shader code: <EMPTY>",
        err ? (const char*)err->GetBufferPointer() : "Error: <EMPTY>" );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static void sPrintShaderCompileInfoD3D10( const char * hlsl, ID3D10Blob * bin )
{
    GN_GUARD;

    GN_ASSERT( hlsl && bin );

    using namespace GN;

    // get ASM code
    AutoComPtr<ID3D10Blob> asm_;

    D3D10DisassembleShader(
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
        sAddLineCountD3D10(hlsl).rawptr(),
        sAddLineCountD3D10((const char*)asm_->GetBufferPointer()).rawptr() );

    GN_UNGUARD;
}

// *****************************************************************************
// public function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API ID3D10Blob * GN::d3d10::compileShader(
    const char   * profile,
    const char   * source,
    size_t         len,
    uint32         flags,
    const char   * entry )
{
    AutoComPtr<ID3D10Blob> bin, err;

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
    if( file.open( "D3D10_shader_source", "wt", TempFile::MANUAL_DELETE ) )
    {
        filename = file.name();
        file.write( source, len, NULL );
        file.close();
    }
#endif

    // Note: D3DXCompileFromMemory() is a more up to date compiler than
    //       D3D10CompileShader(), since D3D10CompileShader() ships
    //       with runtime only, but D3DXCompileFromMemory() ships with
    //       each DXSDK update.
    if( FAILED( D3DCompile(
        source,
        len,
        filename.rawptr(),
        0, // defines
        0, // includes
        entry,
        profile,
        sRefineFlagsD3D10(flags),
        0, // effect compile flags
        &bin,
        &err ) ) )
    {
        sPrintShaderCompileErrorD3D10( source, err );
        return NULL;
    }

    sPrintShaderCompileInfoD3D10( source, bin );

    return bin.detach();
}

//
//
// -----------------------------------------------------------------------------
GN_API ID3D10VertexShader * GN::d3d10::compileAndCreateVS(
    ID3D10Device & dev,
    const char   * source,
    size_t         len,
    uint32         flags,
    const char   * entry,
    const char   * profile,
    ID3D10Blob  ** signature )
{
    GN_GUARD;

    AutoComPtr<ID3D10Blob> bin = AutoComPtr<ID3D10Blob>::sAttach(compileShader( profile, source, len, flags, entry ));
    if( !bin ) return NULL;

    ID3D10VertexShader * vs = createDumpableVS( dev, bin->GetBufferPointer(), bin->GetBufferSize() );
    if( 0 == vs ) return NULL;

    // success
    if( signature ) *signature = bin.detach();
    return vs;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API ID3D10GeometryShader * GN::d3d10::compileAndCreateGS(
    ID3D10Device & dev,
    const char   * source,
    size_t         len,
    uint32         flags,
    const char   * entry,
    const char   * profile,
    ID3D10Blob  ** signature )
{
    GN_GUARD;

    AutoComPtr<ID3D10Blob> bin = AutoComPtr<ID3D10Blob>::sAttach( compileShader( profile, source, len, flags, entry ) );
    if( !bin ) return NULL;

    ID3D10GeometryShader * gs = createDumpableGS( dev, bin->GetBufferPointer(), bin->GetBufferSize() );
    if( 0 == gs ) return NULL;

    // success
    if( signature ) *signature = bin.detach();
    return gs;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API ID3D10PixelShader * GN::d3d10::compileAndCreatePS(
    ID3D10Device & dev,
    const char   * source,
    size_t         len,
    uint32         flags,
    const char   * entry,
    const char   * profile,
    ID3D10Blob  ** signature )
{
    GN_GUARD;

    AutoComPtr<ID3D10Blob> bin = AutoComPtr<ID3D10Blob>::sAttach( compileShader( profile, source, len, flags, entry ) );
    if( !bin ) return NULL;

    ID3D10PixelShader * ps = createDumpablePS( dev, bin->GetBufferPointer(), bin->GetBufferSize() );
    if( 0 == ps ) return 0;

    // success
    if( signature ) *signature = bin.detach();
    return ps;

    GN_UNGUARD;
}