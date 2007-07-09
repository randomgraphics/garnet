#include "pch.h"
#ifdef HAS_D3D9

static GN::Logger * sLogger = GN::getLogger("GN.gfx.base.d3d9ShaderUtils");

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
#if GN_DEBUG_BUILD
    flags |= D3DXSHADER_DEBUG;
#endif
    return flags;
}

//
//
// -----------------------------------------------------------------------------
static void sPrintShaderCompileError( HRESULT hr, const char * code, LPD3DXBUFFER err )
{
    GN_GUARD;

    GN_ERROR(sLogger)(
        "\n================== Shader compile failure ===============\n"
        "%s\n"
        "\n---------------------------------------------------------\n"
        "Error Code(08x%X) : %s\n"
        "\n---------------------------------------------------------\n"
        "%s\n"
        "\n=========================================================\n",
        code ? sAddLineCount(code).cptr() : "Shader code: <EMPTY>",
        hr, DXGetErrorDescription9A(hr),
        err ? (const char*)err->GetBufferPointer() : "Error: <EMPTY>" );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static void sPrintShaderCompileInfo( const char * hlsl, ID3DXBuffer * bin )
{
    GN_GUARD;

    GN_ASSERT( hlsl && bin );

    using namespace GN;

    // get ASM code
    AutoComPtr<ID3DXBuffer> asm_;

    D3DXDisassembleShader(
        (const DWORD*)bin->GetBufferPointer(),
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

#if 0
#include <io.h>
//
// save shader code to temporary file
// -----------------------------------------------------------------------------
static GN::StrA sSaveCodeToTemporaryFile( const char * code, size_t len )
{
    using namespace GN;

    const char * templ = "app::/XXXXXX";
    char fname[256];
    memcpy( fname, templ, 13 );
    if( 0 != _mktemp_s( fname, 13 ) )
    {
        GN_ERROR(sLogger)( "fail to generate temporary file name" );
        return StrA::EMPTYSTR;
    }

    AutoObjPtr<File> fp( core::openFile( fname, "wt" ) );
    if( 0 == fp )
    {
        GN_ERROR(sLogger)( "fail to open temporary file." );
        return StrA::EMPTYSTR;
    }

    if( !fp->write( code, len ? len : strLen(code), 0 ) )
    {
        GN_ERROR(sLogger)( "fail to write to temporary file." );
        return StrA::EMPTYSTR;
    }

    GN_INFO(sLogger)( "save shader code to file '%s'", fname );
    return core::toNative(fname);
}
#endif

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::gfx::d3d9::compileVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, UInt32 flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShader(
            code, (UINT)( len ? len : strLen(code) ),
            NULL, NULL, // no macros, no includes,
            entry,
            strEmpty(profile) ? D3DXGetVertexShaderProfile( dev ) : profile,
            sRefineFlags(flags),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileError( hr, code, err );
        return 0;
    }

    // print compile info
    sPrintShaderCompileInfo( code, bin );

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX9_CHECK_RV(
        dev->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::gfx::d3d9::compileVSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, UInt32 flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShaderFromFileA(
            core::toNative(file).cptr(),
            NULL, NULL, // no macros, no includes,
            entry,
            strEmpty(profile) ? D3DXGetVertexShaderProfile( dev ) : profile,
            sRefineFlags(flags),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileError( hr, file, err );
        return 0;
    }

    // print compile info
    sPrintShaderCompileInfo( file, bin );

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX9_CHECK_RV(
        dev->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::gfx::d3d9::assembleVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, UInt32 flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // trim leading spaces in shader code
    if( 0 == len ) len = strLen( code );
    while( len > 0 && ( ' '==*code || '\t' == *code || '\n' == *code ) )
    {
        ++code;
        --len;
    }

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShader(
            code, (UINT)len,
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags),
            &bin,
            &err )) )
    {
        sPrintShaderCompileError( hr, code, err );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX9_CHECK_RV(
        dev->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::gfx::d3d9::assembleVSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, UInt32 flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShaderFromFileA(
            core::toNative(file).cptr(),
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags),
            &bin,
            &err )) )
    {
        sPrintShaderCompileError( hr, file, err );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX9_CHECK_RV(
        dev->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::gfx::d3d9::compilePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, UInt32 flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShader(
            code, (UINT)( len ? len : strLen(code) ),
            NULL, NULL, // no macros, no includes,
            entry,
            strEmpty(profile) ? D3DXGetPixelShaderProfile( dev ) : profile,
            sRefineFlags(flags),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileError( hr, code, err );
        return 0;
    };

    // print compile info
    sPrintShaderCompileInfo( code, bin );

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX9_CHECK_RV(
        dev->CreatePixelShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::gfx::d3d9::compilePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, UInt32 flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShaderFromFileA(
            core::toNative(file).cptr(),
            NULL, NULL, // no macros, no includes,
            entry,
            strEmpty(profile) ? D3DXGetPixelShaderProfile( dev ) : profile,
            sRefineFlags(flags),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileError( hr, file, err );
        return 0;
    };

    // print compile info
    sPrintShaderCompileInfo( file, bin );

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX9_CHECK_RV(
        dev->CreatePixelShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::gfx::d3d9::assemblePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, UInt32 flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // trim leading spaces in shader code
    if( 0 == len ) len = strLen( code );
    while( len > 0 && ( ' '==*code || '\t' == *code || '\n' == *code ) )
    {
        ++code;
        --len;
    }

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShader(
            code, (UINT)len,
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags),
            &bin,
            &err )) )
    {
        sPrintShaderCompileError( hr, code, err );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX9_CHECK_RV(
        dev->CreatePixelShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::gfx::d3d9::assemblePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, UInt32 flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShaderFromFileA(
            core::toNative(file).cptr(),
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags),
            &bin,
            &err )) )
    {
        sPrintShaderCompileError( hr, file, err );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX9_CHECK_RV(
        dev->CreatePixelShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPD3DXEFFECT GN::gfx::d3d9::compileEffect( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, UInt32 flags, LPD3DXEFFECTPOOL pool )
{
    GN_GUARD;

    GN_ASSERT( dev );

    //StrA tmpfile = sSaveCodeToTemporaryFile( code, len );
    //if( tmpfile.empty() ) return 0;

    LPD3DXEFFECT result;
    AutoComPtr<ID3DXBuffer> err;

    HRESULT hr;
    if( FAILED(hr = D3DXCreateEffect(
            dev,
            code, (UINT)(len?len:strLen(code)), //tmpfile.cptr(),
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags),
            pool,
            &result,
            &err )) )
    {
        sPrintShaderCompileError( hr, code, err );
        return 0;
    };

    // success
    return result;

    GN_UNGUARD;
}

#endif
