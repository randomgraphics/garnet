#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.d3d9.d3d9ShaderUtils");

// *****************************************************************************
// local function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static GN::StrA sAddLineCount( const GN::StrA & in )
{
    using namespace GN;

    GN::StrA out( "(  1) : " );

    int line = 1;
    for( const char * s = in.cptr(); *s; ++s )
    {
        if( '\n' == *s )
        {
            out.append( strFormat( "\n(%3d) : ", ++line ) );
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
static UInt32 sRefineFlags( UInt32 flags )
{
#if GN_BUILD_DEBUG
    flags |= D3D10_SHADER_DEBUG;
#endif
    return flags;
}

//
//
// -----------------------------------------------------------------------------
static void sPrintShaderCompileError( const char * code, ID3D10Blob * err )
{
    GN_GUARD;

    GN_ERROR(sLogger)(
        "\n================== Shader compile failure ===============\n"
        "%s\n"
        "\n---------------------------------------------------------\n"
        "%s\n"
        "\n=========================================================\n",
        code ? sAddLineCount(code).cptr() : "Shader code: <EMPTY>",
        err ? (const char*)err->GetBufferPointer() : "Error: <EMPTY>" );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static void sPrintShaderCompileInfo( const char * hlsl, ID3D10Blob * bin )
{
    GN_GUARD;

    GN_ASSERT( hlsl && bin );

    using namespace GN;

    // get ASM code
    AutoComPtr<ID3D10Blob> asm_;

    D3DX10DisassembleShader(
        bin->GetBufferPointer(),
        bin->GetBufferSize(),
        false,
        NULL,
        &asm_ );

    GN_TRACE(sLogger)(
        "\n================== Shader compile success ===============\n"
        "%s\n"
        "\n---------------------------------------------------------\n"
        "%s\n"
        "\n=========================================================\n",
        sAddLineCount(hlsl).cptr(),
        sAddLineCount((const char*)asm_->GetBufferPointer()).cptr() );

    GN_UNGUARD;
}
// *****************************************************************************
// public function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
ID3D10VertexShader * GN::d3d10::compileVS(
    ID3D10Device & dev,
    const char   * code,
    size_t         len,
    UInt32         flags,
    const char   * entry,
    const char   * profile,
    ID3D10Blob  ** signature )
{
    GN_GUARD;

    AutoComPtr<ID3D10Blob> bin, err;
    if( FAILED( D3DX10CompileFromMemory(
            code,
            (0==len) ? strLen(code) : len,
            0, 0, 0,
            entry,
            profile,
            sRefineFlags(flags),
            0,
            0,
            &bin,
            &err,
            0 ) ) )
    {
        sPrintShaderCompileError( code, err );
        return 0;
    }

    sPrintShaderCompileInfo( code, bin );

    ID3D10VertexShader * vs = createDumpableVertexShader( dev, bin->GetBufferPointer(), bin->GetBufferSize() );
    if( 0 == vs ) return 0;

    // success
    if( signature ) *signature = bin.detach();
    return vs;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
ID3D10PixelShader * GN::d3d10::compilePS(
    ID3D10Device & dev,
    const char   * code,
    size_t         len,
    UInt32         flags,
    const char   * entry,
    const char   * profile,
    ID3D10Blob  ** signature )
{
    GN_GUARD;

    AutoComPtr<ID3D10Blob> bin, err;
    if( FAILED( D3DX10CompileFromMemory(
            code,
            (0==len) ? strLen(code) : len,
            0, 0, 0,
            entry,
            profile,
            sRefineFlags(flags),
            0,
            0,
            &bin,
            &err,
            0 ) ) )
    {
        sPrintShaderCompileError( code, err );
        return 0;
    }

    sPrintShaderCompileInfo( code, bin );

    ID3D10PixelShader * ps = createDumpablePixelShader( dev, bin->GetBufferPointer(), bin->GetBufferSize() );
    if( 0 == ps ) return 0;

    // success
    if( signature ) *signature = bin.detach();
    return ps;

    GN_UNGUARD;
}
