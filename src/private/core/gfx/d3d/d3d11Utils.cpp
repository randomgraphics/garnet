#include "pch.h"

#if GN_BUILD_HAS_D3D11

#include <d3dcompiler.h>

using namespace GN;
using namespace GN::d3d11;
static Logger * sLogger = GN::getLogger("dxutils");

#ifndef IFC
#define IFC(x) hr = (x); if (FAILED(hr)) { GN_ERROR(sLogger)(#x " Failed: hr=0x%08X", hr); goto Cleanup; }
#endif

// *****************************************************************************
// Local utilities
// *****************************************************************************

// -----------------------------------------------------------------------------
struct AutoFile
{
    FILE * fp;

    AutoFile() : fp(nullptr)
    {
    }

    ~AutoFile()
    {
        Close();
    }

    void Close()
    {
        if(fp) fclose(fp), fp = 0;
    }
};

// -----------------------------------------------------------------------------
static const char * sGetDefaultShaderProfile(ShaderType type)
{
    switch(type)
    {
        case SHADER_TYPE_VS : return "vs_4_0";
        case SHADER_TYPE_PS : return "ps_4_0";
        case SHADER_TYPE_GS : return "gs_4_0";
        case SHADER_TYPE_HS : return "hs_4_0";
        case SHADER_TYPE_DS : return "ds_4_0";
        case SHADER_TYPE_CS : return "cs_4_0";
        default             :
            GN_ERROR(sLogger)("Invalid shader type: %d", type);
            return nullptr;
    }
}

// -----------------------------------------------------------------------------
static std::string sAddLineCountD3D11( const char * source )
{
    std::string out;
    if( nullptr != source && 0 != *source )
    {
        out = "(  1) : ";
        int line = 1;
        for( const char * s = source; *s; ++s )
        {
            if( '\n' == *s )
            {
                out += (char*)GN::str::format( "\n(%3d) : ", ++line );
            }
            else
            {
                out += *s;
            }
        }
    }
    return out;
}

// -----------------------------------------------------------------------------
static UINT sRefineFlagsD3D11( UINT flags )
{
    flags |= D3DCOMPILE_DEBUG;
    return flags;
}

// -----------------------------------------------------------------------------
static void sPrintShaderCompileErrorD3D11( const char * code, ID3DBlob * err )
{
    GN_ERROR(sLogger)(
        "\n================== Shader compile failure ===============\n"
        "%s\n"
        "\n---------------------------------------------------------\n"
        "%s\n"
        "\n=========================================================\n",
        code ? sAddLineCountD3D11(code).c_str() : "Shader code: <EMPTY>",
        err ? (const char*)err->GetBufferPointer() : "Error: <EMPTY>" );
}

// -----------------------------------------------------------------------------
static void sPrintShaderCompileInfoD3D11( const char * hlsl, ID3DBlob * bin )
{
    GN_ASSERT( hlsl && bin );

    // get ASM code
    AutoComPtr<ID3DBlob> asm_;

    D3DDisassemble(
        bin->GetBufferPointer(),
        bin->GetBufferSize(),
        false,
        nullptr,
        &asm_ );

    GN_VTRACE(sLogger)(
        "\n================== Shader compile success ===============\n"
        "%s\n"
        "\n---------------------------------------------------------\n"
        "%s\n"
        "\n=========================================================\n",
        sAddLineCountD3D11(hlsl).c_str(),
        sAddLineCountD3D11((const char*)asm_->GetBufferPointer()).c_str() );
}

// *****************************************************************************
// public functions
// *****************************************************************************

#include <oleauto.h>
#pragma comment(lib, "OleAut32.lib")

// -----------------------------------------------------------------------------
GN_API GN::StrW GN::d3d11::hresult2string(HRESULT hr)
{
    StrW strMessage;
    WORD facility = HRESULT_FACILITY(hr);
    AutoComPtr<IErrorInfo> iei;
    if (S_OK == GetErrorInfo(0, &iei) && iei)
    {
        // get the error description from the IErrorInfo
        BSTR bstr = NULL;
        if (SUCCEEDED(iei->GetDescription(&bstr)))
        {
            // append the description to our label
            strMessage = bstr;

            // done with BSTR, do manual cleanup
            SysFreeString(bstr);
        }
    }
    else if (facility == FACILITY_ITF)
    {
        // interface specific - no standard mapping available
        strMessage = L"FACILITY_ITF - This error is interface specific.  No further information is available.";
    }
    else
    {
        // attempt to treat as a standard, system error, and ask FormatMessage to explain it.
        strMessage = getWin32ErrorInfoW(hr);
    }
    return strMessage;
}

// -----------------------------------------------------------------------------
GN_API ID3DBlob * GN::d3d11::compileShader(
    const char   * profile,
    const char   * source,
    size_t         len,
    UINT           flags,
    const char   * entry )
{
    AutoComPtr<ID3DBlob> bin, err;

    if( nullptr == source || 0 == *source )
    {
        GN_ERROR(sLogger)( "empty source code." );
        return nullptr;
    }

    // determine source length
    if( 0 == len ) len = strlen(source);

#if 0
    // TODO: generate temporary file to store shader source
    StrA filename;
#if GN_BUILD_DEBUG_ENABLED
    TempFile file;
    if( file.open( "D3D11_shader_source", "wt", TempFile::MANUAL_DELETE ) )
    {
        filename = file.name();
        file.write( source, len, nullptr );
        file.close();
    }
#endif
#endif

    if( FAILED( D3DCompile(
        source,
        len,
        nullptr, // filename
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
        return nullptr;
    }

    sPrintShaderCompileInfoD3D11( source, bin );

    return bin.detach();
}

// -----------------------------------------------------------------------------
GN_API ID3D11DeviceChild * GN::d3d11::createShader(
    ID3D11Device & dev,
    ShaderType     type,
    const char   * source,
    size_t         len,
    UINT           flags,
    const char   * entry,
    const char   * profile,
    ID3DBlob    ** signature )
{
    if( nullptr == profile || 0 == *profile )
    {
        profile = sGetDefaultShaderProfile(type);
        if( nullptr == profile)
        {
            return nullptr;
        }
    }

    AutoComPtr<ID3DBlob> bin = AutoComPtr<ID3DBlob>::sAttach( compileShader( profile, source, len, flags, entry ) );
    if( !bin ) return nullptr;

    const void * binary = bin->GetBufferPointer();
    size_t bytes = bin->GetBufferSize();

    ID3D11DeviceChild * shader = nullptr;
    switch(type)
    {
        case SHADER_TYPE_VS:
        {
            ID3D11VertexShader * vs;
            GN_RETURN_NULL_ON_HR_FAILED( dev.CreateVertexShader( binary, bytes, NULL, &vs ) );
            shader = vs;
            break;
        }

        case SHADER_TYPE_PS:
        {
            ID3D11PixelShader * ps;
            GN_RETURN_NULL_ON_HR_FAILED( dev.CreatePixelShader( binary, bytes, NULL, &ps ) );
            shader = ps;
            break;
        }

        case SHADER_TYPE_GS:
        {
            ID3D11GeometryShader * gs;
            GN_RETURN_NULL_ON_HR_FAILED( dev.CreateGeometryShader( binary, bytes, NULL, &gs ) );
            shader = gs;
            break;
        }

        case SHADER_TYPE_HS:
        {
            ID3D11HullShader * hs;
            GN_RETURN_NULL_ON_HR_FAILED( dev.CreateHullShader( binary, bytes, NULL, &hs ) );
            shader = hs;
            break;
        }

        case SHADER_TYPE_DS:
        case SHADER_TYPE_CS:
            GN_ASSERT_EX(0, "Not implemented");
            return nullptr;

        default:
            GN_ERROR(sLogger)("Invalid shader type: %d", type);
            return nullptr;
    }

    if( nullptr != shader && nullptr != signature )
    {
        *signature = bin.detach();
    }

    // success
    return shader;
}

// -----------------------------------------------------------------------------
GN_API ID3D11DeviceChild * GN::d3d11::loadShaderFromFile(
    ID3D11Device  & dev,
    ShaderType      type,
    const wchar_t * fileName,
    UINT            flags,
    const char    * entry,
    const char    * profile,
    ID3DBlob     ** signature )
{
    if(nullptr == fileName || 0 == *fileName)
    {
        GN_ERROR(sLogger)("Empty/Null shader file name is not allowed.");
        return nullptr;
    }

    AutoFile file;
    if(_wfopen_s(&file.fp, fileName, L"rt"))
    {
        GN_ERROR(sLogger)("Fail to open shader file: %S.", fileName);
        return nullptr;
    }

    fseek(file.fp, 0, SEEK_END);
    long fileSize = ftell(file.fp);
    fseek(file.fp, 0, SEEK_SET);
    if(fileSize <= 0)
    {
        GN_ERROR(sLogger)("Fail to determine shader file size: %S", fileName);
        return nullptr;
    }

    size_t bytesToRead = (size_t)fileSize;
    std::vector<char> buffer(bytesToRead + 1);
    size_t readen = fread(&buffer[0], 1, bytesToRead, file.fp);
    if(0 == readen)
    {
        GN_ERROR(sLogger)("Fail to read shader file: %S", fileName);
        return nullptr;
    }
    buffer[readen] = 0;

    file.Close();

    return createShader(dev, type, &buffer[0], buffer.size(), flags, entry, profile, signature);
}

//
//
// -----------------------------------------------------------------------------
bool GN::d3d11::SimpleMesh::init( ID3D11DeviceContext & cxt )
{
    quit();

    cxt.GetDevice(&mDevice);

    // create a mini-shader (for its signature)
    static const char * vscode =
        "struct VSInput \n"
        "{ \n"
        "   float4 p : POSITION0; \n"
        "   float3 n : NORMAL0; \n"
        "   float2 t : TEXCOORD0; \n"
        "   float4 c : COLOR0; \n"
        "   float4 u : USER0; \n"
        "};\n"
        "float4 main( VSInput i ) : SV_Position0 { return 0; }";
    AutoComPtr<ID3D11VertexShader> vs;
    AutoComPtr<ID3DBlob>  signature;
    vs.attach( createVS( *mDevice, vscode, 0, 0, "main", "vs_4_0", &signature ) );
    if( nullptr == vs )
    {
        quit();
        return false;
    }

    // create input layout
    static const D3D11_INPUT_ELEMENT_DESC elements[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT      , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "USER"    , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    GN_RETURN_ON_HR_FAILED(
        mDevice->CreateInputLayout( elements, (uint32_t)countof(elements), signature->GetBufferPointer(), signature->GetBufferSize(), &mLayout ),
        (quit(), false) );

    // success
    mContext = &cxt;
    mContext->AddRef();
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::quit()
{
    safeRelease( mDevice );
    safeRelease( mContext );
    safeRelease( mLayout );
    safeRelease( mVtxBuf );
    safeRelease( mIdxBuf );
    clear();
}
// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::beginVertices( size_t reservedVertexCount )
{
    mVertices.clear();
    mVertices.reserve(reservedVertexCount);
}

// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::pos( float x, float y, float z )
{
    mNewVertex.pos[0] = x;
    mNewVertex.pos[1] = y;
    mNewVertex.pos[2] = z;
    mVertices.push_back( mNewVertex );
}

// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::normal( float x, float y, float z )
{
    mNewVertex.normal[0] = x;
    mNewVertex.normal[1] = y;
    mNewVertex.normal[2] = z;
}

// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::tex( float x, float y )
{
    mNewVertex.tex[0] = x;
    mNewVertex.tex[1] = y;
}

// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::color( float r, float g, float b, float a )
{
    mNewVertex.color[0] = r;
    mNewVertex.color[1] = g;
    mNewVertex.color[2] = b;
    mNewVertex.color[3] = a;
}

// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::user(float x, float y, float z, float w)
{
    mNewVertex.user[0] = x;
    mNewVertex.user[1] = y;
    mNewVertex.user[2] = z;
    mNewVertex.user[3] = w;
}

// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::endVertices()
{
    if( mVertices.empty() ) return;

    UINT bytes = (UINT)( mVertices.size() * sizeof(Vertex) );

    if( mVtxBufCapacity < mVertices.size() )
    {
        safeRelease( mVtxBuf );
        mVtxBufCapacity = 0;

        D3D11_BUFFER_DESC desc = {
            bytes,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_VERTEX_BUFFER,
            0,
            0,
        };

        GN_RETURN_VOID_ON_HR_FAILED( mDevice->CreateBuffer( &desc, 0, &mVtxBuf ) );

        mVtxBufCapacity = mVertices.size();
    }

    D3D11_BOX box = { 0, 0, 0, bytes, 1, 1 };

    mContext->UpdateSubresource( mVtxBuf, 0, &box, &mVertices[0], bytes, bytes );

    mNumVertices = mVertices.size();
}

// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::setVertices( const Vertex * vertices, size_t count )
{
    beginVertices(count);
    mVertices.assign( vertices, vertices + count );
    endVertices();
}

// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::beginTopology(D3D11_PRIMITIVE_TOPOLOGY topology, size_t reservedIndexCount)
{
    mIndices.clear();
    mIndices.reserve(reservedIndexCount);
    mTopology = topology;
}

// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::triangle( size_t i0, size_t i1, size_t i2 )
{
    mIndices.push_back( (USHORT)i0 );
    mIndices.push_back( (USHORT)i1 );
    mIndices.push_back( (USHORT)i2 );
}


//
//
// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::endTopology()
{
    if( mIndices.empty() ) return;

    UINT bytes = (UINT)( mIndices.size() * sizeof(USHORT) );

    if( mIdxBufCapacity < mIndices.size() )
    {
        safeRelease( mIdxBuf );
        mIdxBufCapacity = 0;

        D3D11_BUFFER_DESC desc = {
            bytes,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_INDEX_BUFFER,
            0,
            0,
        };

        GN_RETURN_VOID_ON_HR_FAILED( mDevice->CreateBuffer( &desc, 0, &mIdxBuf ) );

        mIdxBufCapacity = mIndices.size();
    }

    D3D11_BOX box = { 0, 0, 0, bytes, 1, 1 };

    mContext->UpdateSubresource( mIdxBuf, 0, &box, &mIndices[0], bytes, bytes );

    mNumIndices = mIndices.size();
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::setTriangles( const USHORT * triangles, size_t triangleCount )
{
    size_t indexCount = triangleCount * 3;
    beginTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indexCount);
    mIndices.assign(triangles, triangles + indexCount);
    endTopology();
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::draw() const
{
    if( mNumVertices > 0 )
    {
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        mContext->IASetInputLayout( mLayout );
        mContext->IASetVertexBuffers( 0, 1, &mVtxBuf, &stride, &offset );
        mContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
        mContext->Draw( (UINT)mNumVertices, 0 );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d11::SimpleMesh::drawIndexed() const
{
    if( mNumIndices > 0 )
    {
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        mContext->IASetInputLayout( mLayout );
        mContext->IASetVertexBuffers( 0, 1, &mVtxBuf, &stride, &offset );
        mContext->IASetIndexBuffer( mIdxBuf, DXGI_FORMAT_R16_UINT, 0 );
        mContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
        mContext->DrawIndexed( (UINT)mNumIndices, 0, 0 );
    }
}

// -----------------------------------------------------------------------------

void GN::d3d11::D3D11Resource::clear()
{
    resource = nullptr;
    buffer = nullptr;
    tex1d = nullptr;
    tex2d = nullptr;
    tex3d = nullptr;
    srv1 = nullptr;
    srv2 = nullptr;
    rtv = nullptr;
    dsv = nullptr;
    uav = nullptr;
}

// -----------------------------------------------------------------------------

HRESULT GN::d3d11::D3D11Resource::createBuffer(
    ID3D11Device * dev,
    const D3D11_BUFFER_DESC & bufdesc,
    const D3D11_SUBRESOURCE_DATA * data,
    const D3D11_SHADER_RESOURCE_VIEW_DESC * srvDesc,
    const D3D11_UNORDERED_ACCESS_VIEW_DESC * uavDesc)
{
    HRESULT hr = S_OK;
    ViewDescriptors viewDesc = {};
    viewDesc.srv1 = srvDesc;
    viewDesc.uav  = uavDesc;
    clear();
    IFC(dev->CreateBuffer(&bufdesc, data, &this->buffer));
    IFC(createViews(dev, viewDesc));
Cleanup:
    return hr;
}

// -----------------------------------------------------------------------------

HRESULT GN::d3d11::D3D11Resource::createTexture(
    ID3D11Device * dev,
    const D3D11_TEXTURE1D_DESC & texdesc,
    const ViewDescriptors & viewDesc)
{
    HRESULT hr = S_OK;
    clear();
    IFC(dev->CreateTexture1D(&texdesc, nullptr, &this->tex1d));
    IFC(this->tex1d.as(&this->resource));
    IFC(createViews(dev, viewDesc));
Cleanup:
    return hr;
}

// -----------------------------------------------------------------------------

HRESULT GN::d3d11::D3D11Resource::createTexture(
    ID3D11Device * dev,
    const D3D11_TEXTURE2D_DESC & texdesc,
    const ViewDescriptors & viewDesc)
{
    HRESULT hr = S_OK;
    clear();
    IFC(dev->CreateTexture2D(&texdesc, nullptr, &this->tex2d));
    IFC(this->tex2d.as(&this->resource));
    IFC(createViews(dev, viewDesc));
Cleanup:
    return hr;
}

// -----------------------------------------------------------------------------

HRESULT GN::d3d11::D3D11Resource::createTexture(
    ID3D11Device * dev,
    const D3D11_TEXTURE3D_DESC & texdesc,
    const ViewDescriptors & viewDesc)
{
    HRESULT hr = S_OK;
    clear();
    IFC(dev->CreateTexture3D(&texdesc, nullptr, &this->tex3d));
    IFC(this->tex3d.as(&this->resource));
    IFC(createViews(dev, viewDesc));
Cleanup:
    return hr;
}

// -----------------------------------------------------------------------------

HRESULT GN::d3d11::D3D11Resource::createVB(ID3D11Device * dev, UINT sizeBytes, const void * data)
{
    HRESULT hr = S_OK;
    clear();
    D3D11_BUFFER_DESC desc = {0};
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = (UINT)sizeBytes;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA sub = {data, (UINT)sizeBytes, (UINT)sizeBytes};
    IFC(createBuffer(dev, desc, data ? &sub : nullptr));
Cleanup:
    return hr;
}

// -----------------------------------------------------------------------------

HRESULT GN::d3d11::D3D11Resource::createIB(ID3D11Device * dev, UINT sizeBytes, const void * data)
{
    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC desc = {0};
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = (UINT)sizeBytes;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA sub = {data, (UINT)sizeBytes, (UINT)sizeBytes};
    IFC(createBuffer(dev, desc, data ? &sub : nullptr));
Cleanup:
    return hr;
}

// -----------------------------------------------------------------------------

HRESULT GN::d3d11::D3D11Resource::createCB(ID3D11Device * dev, UINT sizeBytes, const void * data)
{
    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC desc = {0};
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = (UINT)sizeBytes;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    D3D11_SUBRESOURCE_DATA sub = {data, (UINT)sizeBytes, (UINT)sizeBytes};
    IFC(createBuffer(dev, desc, data ? &sub : nullptr));
Cleanup:
    return hr;
}

// -----------------------------------------------------------------------------

HRESULT GN::d3d11::D3D11Resource::create1D(ID3D11Device * dev, UINT width, DXGI_FORMAT resourceFormat, DXGI_FORMAT srvFormat)
{
    HRESULT hr = S_OK;
    D3D11_TEXTURE1D_DESC desc = {};
    desc.Width = width;
    desc.MipLevels = 0;
    desc.ArraySize = 1;
    desc.Format = resourceFormat;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = srvFormat;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
    srvDesc.Texture1D.MostDetailedMip = 0;
    srvDesc.Texture1D.MipLevels = (UINT)-1;
    ViewDescriptors viewDesc;
    viewDesc.srv1 = &srvDesc;
    IFC(createTexture(dev, desc, viewDesc));
Cleanup:
    return hr;
}

// -----------------------------------------------------------------------------

HRESULT GN::d3d11::D3D11Resource::create2D(ID3D11Device * dev, UINT width, UINT height, DXGI_FORMAT resourceFormat, DXGI_FORMAT srvFormat)
{
    HRESULT hr = S_OK;
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 0;
    desc.ArraySize = 1;
    desc.Format = resourceFormat;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = srvFormat;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = (UINT)-1;
    ViewDescriptors viewDesc = {};
    viewDesc.srv1 = &srvDesc;
    IFC(createTexture(dev, desc, viewDesc));
Cleanup:
    return hr;
}

// -----------------------------------------------------------------------------
HRESULT GN::d3d11::D3D11Resource::create3D(ID3D11Device * dev, UINT width, UINT height, UINT depth, DXGI_FORMAT resourceFormat, DXGI_FORMAT srvFormat)
{
    HRESULT hr = S_OK;
    D3D11_TEXTURE3D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.Depth = depth;
    desc.MipLevels = 0;
    desc.Format = resourceFormat;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = srvFormat;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
    srvDesc.Texture3D.MostDetailedMip = 0;
    srvDesc.Texture3D.MipLevels = (UINT)-1;
    ViewDescriptors viewDesc = {};
    viewDesc.srv1 = &srvDesc;
    IFC(createTexture(dev, desc, viewDesc));
Cleanup:
    return hr;
}

// -----------------------------------------------------------------------------

HRESULT GN::d3d11::D3D11Resource::createRT(ID3D11Device * dev, UINT width, UINT height, DXGI_FORMAT resourceFormat, DXGI_FORMAT rtvFormat, DXGI_FORMAT srvFormat)
{
    HRESULT hr = S_OK;
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 0;
    desc.ArraySize = 1;
    desc.Format = resourceFormat;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | ((DXGI_FORMAT_UNKNOWN != srvFormat) ? D3D11_BIND_SHADER_RESOURCE : 0);
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = srvFormat;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = (UINT)-1;
    D3D11_RENDER_TARGET_VIEW_DESC rtvdesc = {};
    rtvdesc.Format = rtvFormat;
    rtvdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvdesc.Texture2D.MipSlice = 0;
    ViewDescriptors viewDesc = {};
    viewDesc.srv1 = (DXGI_FORMAT_UNKNOWN == srvFormat) ? nullptr : &srvDesc;
    viewDesc.rtv  = (DXGI_FORMAT_UNKNOWN == rtvFormat) ? nullptr : &rtvdesc;
    IFC(createTexture(dev, desc, viewDesc));
Cleanup:
    return hr;
}

// -----------------------------------------------------------------------------

HRESULT GN::d3d11::D3D11Resource::createDS(ID3D11Device * dev, UINT width, UINT height, DXGI_FORMAT resourceFormat, DXGI_FORMAT dsvFormat, DXGI_FORMAT srv1Format, DXGI_FORMAT srv2Format)
{
    HRESULT hr = S_OK;
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 0;
    desc.ArraySize = 1;
    desc.Format = resourceFormat;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL
                   | ((DXGI_FORMAT_UNKNOWN != srv1Format) ? D3D11_BIND_SHADER_RESOURCE : 0)
                   | ((DXGI_FORMAT_UNKNOWN != srv2Format) ? D3D11_BIND_SHADER_RESOURCE : 0)
                   ;
    D3D11_SHADER_RESOURCE_VIEW_DESC srv1Desc = {};
    srv1Desc.Format = srv1Format;
    srv1Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv1Desc.Texture2D.MostDetailedMip = 0;
    srv1Desc.Texture2D.MipLevels = (UINT)-1;
    D3D11_SHADER_RESOURCE_VIEW_DESC srv2Desc = {};
    srv2Desc.Format = srv1Format;
    srv2Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv2Desc.Texture2D.MostDetailedMip = 0;
    srv2Desc.Texture2D.MipLevels = (UINT)-1;
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvdesc = {};
    dsvdesc.Format = dsvFormat;
    dsvdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvdesc.Texture2D.MipSlice = 0;
    ViewDescriptors viewDesc = {};
    viewDesc.srv1 = (DXGI_FORMAT_UNKNOWN == srv1Format) ? nullptr : &srv1Desc;
    viewDesc.srv2 = (DXGI_FORMAT_UNKNOWN == srv2Format) ? nullptr : &srv2Desc;
    viewDesc.dsv  = (DXGI_FORMAT_UNKNOWN == dsvFormat)  ? nullptr : &dsvdesc;
    IFC(createTexture(dev, desc, viewDesc));
Cleanup:
    return hr;
}
// -----------------------------------------------------------------------------

HRESULT GN::d3d11::D3D11Resource::createViews(ID3D11Device * dev, const ViewDescriptors & viewDesc)
{
    HRESULT hr = S_OK;
    if (viewDesc.srv1)
    {
        IFC(dev->CreateShaderResourceView(this->resource.get(), viewDesc.srv1, &this->srv1));
    }
    if (viewDesc.srv2)
    {
        IFC(dev->CreateShaderResourceView(this->resource.get(), viewDesc.srv2, &this->srv2));
    }
    if (viewDesc.rtv)
    {
        IFC(dev->CreateRenderTargetView(this->resource.get(), viewDesc.rtv, &this->rtv));
    }
    if (viewDesc.dsv)
    {
        IFC(dev->CreateDepthStencilView(this->resource.get(), viewDesc.dsv, &this->dsv));
    }
    if (viewDesc.uav)
    {
        IFC(dev->CreateUnorderedAccessView(this->resource.get(), viewDesc.uav, &this->uav));
    }
Cleanup:
    return hr;
}

#endif // GN_BUILD_HAS_D3D11
