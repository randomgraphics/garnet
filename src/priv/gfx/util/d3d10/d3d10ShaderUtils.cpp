#include "pch.h"
#include "garnet/GNd3d10.h"

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

    D3D10DisassembleShader(
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
ID3D10Blob * GN::d3d10::compileShader(
    const char   * profile,
    const char   * source,
    size_t         len,
    UInt32         flags,
    const char   * entry )
{
    AutoComPtr<ID3D10Blob> bin, err;

    if( NULL == source )
    {
        GN_ERROR(sLogger)( "NULL source pointer." );
        return NULL;
    }

    // determine source length
    if( 0 == len ) len = strLen(source);

    // generate temporary file to store shader source
    StrA filename;
#if GN_BUILD_DEBUG
    AutoObjPtr<File> fp( createTemporaryFile( "wt" ) );
    if( fp )
    {
        filename = fp->name();
        fp->write( source, len, NULL );
        fp.clear(); // delete file insance, which will close the file on disk
    }
#endif

    // Note: D3DXCompileFromMemory() is a more up to date compiler than
    //       D3D10CompileShader(), since D3D10CompileShader() ships
    //       with runtime only, but D3DXCompileFromMemory() ships with
    //       each DXSDK update.
    if( FAILED( D3DX10CompileFromMemory(
        source,
        len,
        filename.cptr(),
        0, // defines
        0, // includes
        entry,
        profile,
        sRefineFlags(flags),
        0, // effect compile flags
        0, // thread pump
        &bin,
        &err,
        0 ) ) )
    {
        sPrintShaderCompileError( source, err );
        return NULL;
    }

    sPrintShaderCompileInfo( source, bin );

    return bin.detach();
}

//
//
// -----------------------------------------------------------------------------
ID3D10VertexShader * GN::d3d10::compileAndCreateVS(
    ID3D10Device & dev,
    const char   * source,
    size_t         len,
    UInt32         flags,
    const char   * entry,
    const char   * profile,
    ID3D10Blob  ** signature )
{
    GN_GUARD;

    AutoComPtr<ID3D10Blob> bin( compileShader( profile, source, len, flags, entry ) );
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
ID3D10GeometryShader * GN::d3d10::compileAndCreateGS(
    ID3D10Device & dev,
    const char   * source,
    size_t         len,
    UInt32         flags,
    const char   * entry,
    const char   * profile,
    ID3D10Blob  ** signature )
{
    GN_GUARD;

    AutoComPtr<ID3D10Blob> bin( compileShader( profile, source, len, flags, entry ) );
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
ID3D10PixelShader * GN::d3d10::compileAndCreatePS(
    ID3D10Device & dev,
    const char   * source,
    size_t         len,
    UInt32         flags,
    const char   * entry,
    const char   * profile,
    ID3D10Blob  ** signature )
{
    GN_GUARD;

    AutoComPtr<ID3D10Blob> bin( compileShader( profile, source, len, flags, entry ) );
    if( !bin ) return NULL;

    ID3D10PixelShader * ps = createDumpablePS( dev, bin->GetBufferPointer(), bin->GetBufferSize() );
    if( 0 == ps ) return 0;

    // success
    if( signature ) *signature = bin.detach();
    return ps;

    GN_UNGUARD;
}
