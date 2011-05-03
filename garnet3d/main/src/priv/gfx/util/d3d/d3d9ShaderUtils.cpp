#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.d3d9.d3d9ShaderUtils");

//
//
// -----------------------------------------------------------------------------
static GN::StrA sAddLineCountD3D9( const GN::StrA & in )
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
static uint32 sRefineFlagsD3D9( uint32 flags )
{
#if GN_ENABLE_DEBUG
    flags |= D3DXSHADER_DEBUG;
#endif
    return flags;
}

//
//
// -----------------------------------------------------------------------------
static void sPrintShaderCompileErrorD3D9( HRESULT hr, const char * code, LPD3DXBUFFER err )
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
        code ? sAddLineCountD3D9(code).rawptr() : "Shader code: <EMPTY>",
        hr, GN::getDXErrorInfo(hr),
        err ? (const char*)err->GetBufferPointer() : "Error: <EMPTY>" );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static void sPrintShaderCompileInfoD3D9( const char * hlsl, ID3DXBuffer * bin )
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

    GN_VTRACE(sLogger)(
        "\n================== Shader compile success ===============\n"
        "%s\n"
        "\n---------------------------------------------------------\n"
        "%s\n"
        "\n=========================================================\n",
        sAddLineCountD3D9(hlsl).rawptr(),
        sAddLineCountD3D9((const char*)asm_->GetBufferPointer()).rawptr() );

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

    if( !fp->write( code, len ? len : stringLength(code), 0 ) )
    {
        GN_ERROR(sLogger)( "fail to write to temporary file." );
        return StrA::EMPTYSTR;
    }

    GN_INFO(sLogger)( "save shader code to file '%s'", fname );
    return fs::toNativeDiskFilePath(fname);
}
#endif

//
//
// -----------------------------------------------------------------------------
GN_API LPDIRECT3DVERTEXSHADER9 GN::d3d9::compileAndCreateVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, uint32 flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable, LPD3DXBUFFER * binary )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShader(
            code, (UINT)( len ? len : stringLength(code) ),
            NULL, NULL, // no macros, no includes,
            entry,
            stringEmpty(profile) ? D3DXGetVertexShaderProfile( dev ) : profile,
            sRefineFlagsD3D9(flags),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileErrorD3D9( hr, code, err );
        return 0;
    }

    // print compile info
    sPrintShaderCompileInfoD3D9( code, bin );

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX_CHECK_RETURN(
        dev->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    if( binary ) *binary = bin.detach();

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API LPDIRECT3DVERTEXSHADER9 GN::d3d9::compileAndCreateVSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32 flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable, LPD3DXBUFFER * binary )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShaderFromFileA(
            fs::toNativeDiskFilePath(file).rawptr(),
            NULL, NULL, // no macros, no includes,
            entry,
            stringEmpty(profile) ? D3DXGetVertexShaderProfile( dev ) : profile,
            sRefineFlagsD3D9(flags),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileErrorD3D9( hr, file, err );
        return 0;
    }

    // print compile info
    sPrintShaderCompileInfoD3D9( file, bin );

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX_CHECK_RETURN(
        dev->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    if( binary ) *binary = bin.detach();

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API LPDIRECT3DVERTEXSHADER9 GN::d3d9::assembleAndCreateVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, uint32 flags, LPD3DXBUFFER * binary )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // trim leading spaces in shader code
    if( 0 == len ) len = stringLength( code );
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
            sRefineFlagsD3D9(flags),
            &bin,
            &err )) )
    {
        sPrintShaderCompileErrorD3D9( hr, code, err );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX_CHECK_RETURN(
        dev->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    if( binary ) *binary = bin.detach();

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API LPDIRECT3DVERTEXSHADER9 GN::d3d9::assembleAndCreateVSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32 flags, LPD3DXBUFFER * binary )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShaderFromFileA(
            fs::toNativeDiskFilePath(file).rawptr(),
            NULL, NULL, // no macros, no includes,
            sRefineFlagsD3D9(flags),
            &bin,
            &err )) )
    {
        sPrintShaderCompileErrorD3D9( hr, file, err );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX_CHECK_RETURN(
        dev->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    if( binary ) *binary = bin.detach();

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API LPDIRECT3DPIXELSHADER9 GN::d3d9::compileAndCreatePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, uint32 flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShader(
            code, (UINT)( len ? len : stringLength(code) ),
            NULL, NULL, // no macros, no includes,
            entry,
            stringEmpty(profile) ? D3DXGetPixelShaderProfile( dev ) : profile,
            sRefineFlagsD3D9(flags),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileErrorD3D9( hr, code, err );
        return 0;
    };

    // print compile info
    sPrintShaderCompileInfoD3D9( code, bin );

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX_CHECK_RETURN(
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
GN_API LPDIRECT3DPIXELSHADER9 GN::d3d9::compileAndCreatePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32 flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShaderFromFileA(
            fs::toNativeDiskFilePath(file).rawptr(),
            NULL, NULL, // no macros, no includes,
            entry,
            stringEmpty(profile) ? D3DXGetPixelShaderProfile( dev ) : profile,
            sRefineFlagsD3D9(flags),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileErrorD3D9( hr, file, err );
        return 0;
    };

    // print compile info
    sPrintShaderCompileInfoD3D9( file, bin );

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX_CHECK_RETURN(
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
GN_API LPDIRECT3DPIXELSHADER9 GN::d3d9::assembleAndCreatePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, uint32 flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // trim leading spaces in shader code
    if( 0 == len ) len = stringLength( code );
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
            sRefineFlagsD3D9(flags),
            &bin,
            &err )) )
    {
        sPrintShaderCompileErrorD3D9( hr, code, err );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX_CHECK_RETURN(
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
GN_API LPDIRECT3DPIXELSHADER9 GN::d3d9::assembleAndCreatePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32 flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShaderFromFileA(
            fs::toNativeDiskFilePath(file).rawptr(),
            NULL, NULL, // no macros, no includes,
            sRefineFlagsD3D9(flags),
            &bin,
            &err )) )
    {
        sPrintShaderCompileErrorD3D9( hr, file, err );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX_CHECK_RETURN(
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
GN_API LPD3DXEFFECT GN::d3d9::compileAndCreateEffect( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, uint32 flags, LPD3DXEFFECTPOOL pool )
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
            code, (UINT)(len?len:stringLength(code)), //tmpfile.rawptr(),
            NULL, NULL, // no macros, no includes,
            sRefineFlagsD3D9(flags),
            pool,
            &result,
            &err )) )
    {
        sPrintShaderCompileErrorD3D9( hr, code, err );
        return 0;
    };

    // success
    return result;

    GN_UNGUARD;
}
