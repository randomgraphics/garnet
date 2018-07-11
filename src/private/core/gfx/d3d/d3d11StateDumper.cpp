#include "pch.h"

#if GN_BUILD_HAS_D3D11

static GN::Logger * sLogger = GN::getLogger("GN.d3d11.statedumper");

using namespace GN;

static char sDumpFilePrefix[_MAX_PATH];

struct DumpFile
{
    FILE * fp;

    DumpFile() : fp(0)
    {
        char fname[_MAX_PATH];
        sprintf_s( fname, "%s.xml", sDumpFilePrefix );

        if( 0 != fopen_s( &fp, fname, "wt" ) ) return;

        fprintf(
            fp,
            "<?xml version=\"1.0\" standalone=\"yes\"?>\n"
            "<D3D11StateDump>\n" );
    }

    ~DumpFile()
    {
        if( fp )
        {
            fprintf( fp, "</D3D11StateDump>\n" );
            fclose(fp);
        }
    }

    operator FILE*() const { return fp; }
};

/*
// convert binary data to base64 string
// -----------------------------------------------------------------------------
static StrA sToBase64( const void * data, size_t bytes )
{
    char base64_alphabet[]=
    {
        'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
        'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
        'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
        'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/',
        '='
    };

    const uint8 * p = (const uint8*)data;

    char s[4];

    StrA result;

    size_t n = bytes / 3;
    size_t k = bytes % 3;

    for( size_t i = 0; i < n; ++i, p+=3 )
    {
        s[0] = p[0] >> 2;
        s[1] = ( (p[0]&3) << 4 ) | ( (p[1]&0xF0) >> 4 );
        s[2] = ( p[1]&0xF) << 2 ) | ( p[2] >> 6 );
    }
}*/

//
//
// -----------------------------------------------------------------------------
static const GUID & VSGUID()
{
    // {B635FED2-E52F-4f85-93EE-BA017B4E40BA}
    static const GUID guid =
    { 0xb635fed2, 0xe52f, 0x4f85, { 0x93, 0xee, 0xba, 0x1, 0x7b, 0x4e, 0x40, 0xba } };
    return guid;
}

//
//
// -----------------------------------------------------------------------------
static const GUID & GSGUID()
{
    // {BA28B475-CCFD-4603-B397-0FEDF90DC916}
    static const GUID guid =
    { 0xba28b475, 0xccfd, 0x4603, { 0xb3, 0x97, 0xf, 0xed, 0xf9, 0xd, 0xc9, 0x16 } };
    return guid;
}

//
//
// -----------------------------------------------------------------------------
static const GUID & PSGUID()
{
    // {59347569-8552-4679-B86F-9D9CDFF309B2}
    static const GUID guid =
    { 0x59347569, 0x8552, 0x4679, { 0xb8, 0x6f, 0x9d, 0x9c, 0xdf, 0xf3, 0x9, 0xb2 } };
    return guid;
}

//
//
// -----------------------------------------------------------------------------
static const GUID & IL0GUID()
{
    // {21523B5E-EB51-4092-8698-0316BDB3E8CE}
    static const GUID guid =
    { 0x21523b5e, 0xeb51, 0x4092, { 0x86, 0x98, 0x3, 0x16, 0xbd, 0xb3, 0xe8, 0xce } };
    return guid;
}

//
//
// -----------------------------------------------------------------------------
static const GUID & IL1GUID()
{
    // {EAC00B60-FAC7-4438-A652-9EAB5D6AE73F}
    static const GUID guid =
    { 0xeac00b60, 0xfac7, 0x4438, { 0xa6, 0x52, 0x9e, 0xab, 0x5d, 0x6a, 0xe7, 0x3f } };
    return guid;
}

//
//
// -----------------------------------------------------------------------------
static void sDumpBinary( const char * filename, const void * data, size_t bytes )
{
    FILE * fp;
    if( 0 != fopen_s( &fp, filename, "wb" ) )
    {
        GN_ERROR(sLogger)( "fail to open file : %s", filename );
        return;
    }

    fwrite( data, 1, bytes, fp );

    fclose( fp );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpShaderCode( FILE * fp, const void * binary, size_t bytes, const char * tag )
{
    char fname[_MAX_PATH];
    sprintf_s( fname, "%s_%s.bin", sDumpFilePrefix, tag );

    sDumpBinary( fname, binary, bytes );

    fprintf( fp, "\t<%s ref=\"%s\"/>\n", tag, fname );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpVs( ID3D11DeviceContext & devcxt, FILE * fp )
{
    AutoComPtr<ID3D11VertexShader> vs;

    AutoComPtr<ID3D11ClassInstance> classes[1];
    UINT numClasses = 1;
    devcxt.VSGetShader( &vs, &classes[0], &numClasses );

    if( !vs ) return;

    DynaArray<uint8> binbuf;
    UINT sz;
    vs->GetPrivateData( VSGUID(), &sz, 0 );
    if( 0 == sz )
    {
        GN_ERROR(sLogger)( "Vertex shader is not dumpable. Please use createDumpableVS()." );
        return;
    }
    binbuf.resize( sz );
    vs->GetPrivateData( VSGUID(), &sz, &binbuf[0] );

    sDumpShaderCode( fp, &binbuf[0], sz, "vs" );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpGs( ID3D11DeviceContext & devcxt, FILE * fp )
{
    AutoComPtr<ID3D11GeometryShader> gs;

    AutoComPtr<ID3D11ClassInstance> classes[1];
    UINT numClasses = 1;
    devcxt.GSGetShader( &gs, &classes[0], &numClasses );

    if( !gs ) return;

    DynaArray<uint8> binbuf;
    UINT sz;
    gs->GetPrivateData( GSGUID(), &sz, 0 );
    if( 0 == sz )
    {
        GN_ERROR(sLogger)( "Geometry shader is not dumpable. Please use createDumpableGS()." );
        return;
    }
    binbuf.resize( sz );
    gs->GetPrivateData( GSGUID(), &sz, &binbuf[0] );

    sDumpShaderCode( fp, &binbuf[0], sz, "gs" );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpPs( ID3D11DeviceContext & devcxt, FILE * fp )
{
    AutoComPtr<ID3D11PixelShader> ps;

    AutoComPtr<ID3D11ClassInstance> classes[1];
    UINT numClasses = 1;
    devcxt.PSGetShader( &ps, &classes[0], &numClasses );

    if( !ps ) return;

    DynaArray<uint8> binbuf;
    UINT sz;
    ps->GetPrivateData( PSGUID(), &sz, 0 );
    if( 0 == sz )
    {
        GN_ERROR(sLogger)( "Pixel shader is not dumpable. Please use createDumpablePS()." );
        return;
    }
    binbuf.resize( sz );
    ps->GetPrivateData( PSGUID(), &sz, &binbuf[0] );

    sDumpShaderCode( fp, &binbuf[0], sz, "ps" );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpBuffer( ID3D11DeviceContext & devcxt, const char * filename, ID3D11Buffer * buf )
{
    GN_ASSERT( buf );

    AutoComPtr<ID3D11Device> dev;
    devcxt.GetDevice( &dev );

    // read buffer data
    D3D11_BUFFER_DESC desc;
    buf->GetDesc( &desc );
    desc.Usage = D3D11_USAGE_STAGING;
    desc.BindFlags = 0;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
    AutoComPtr<ID3D11Buffer> syscpy;
    if( FAILED( dev->CreateBuffer( &desc, 0, &syscpy ) ) )
    {
        GN_ERROR(sLogger)( "fail to create staging buffer" );
        return;
    }
    devcxt.CopyResource( syscpy, buf );

    // map the buffer
    D3D11_MAPPED_SUBRESOURCE mapped;
    if( FAILED( devcxt.Map( syscpy, 0, D3D11_MAP_READ, 0, &mapped ) ) )
    {
        GN_ERROR(sLogger)( "fail to map staging buffer" );
        return;
    }

    // write to file
    sDumpBinary( filename, mapped.pData, desc.ByteWidth );

    // done
    devcxt.Unmap( syscpy, 0 );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpVsConsts( ID3D11DeviceContext & devcxt, FILE * fp )
{
    ID3D11Buffer * cb[14];

    memset( cb, 0, sizeof(cb) );

    devcxt.VSGetConstantBuffers( 0, 14, cb );

    for( uint32 i = 0; i < 14; ++i )
    {
        if( cb[i] )
        {
            char fname[_MAX_PATH];
            sprintf_s( fname, "%s_vsc(%02d).bin", sDumpFilePrefix, i );
            fprintf( fp, "\t<vsc slot=\"%d\" ref=\"%s\"/>\n", i, fname );
            sDumpBuffer( devcxt, fname, cb[i] );

            cb[i]->Release();
        }
    }
}


//
//
// -----------------------------------------------------------------------------
static void sDumpPsConsts( ID3D11DeviceContext & devcxt, FILE * fp )
{
    ID3D11Buffer * cb[14];

    memset( cb, 0, sizeof(cb) );

    devcxt.PSGetConstantBuffers( 0, 14, cb );

    for( uint32 i = 0; i < 14; ++i )
    {
        if( cb[i] )
        {
            char fname[_MAX_PATH];
            sprintf_s( fname, "%s_psc(%02d).bin", sDumpFilePrefix, i );
            fprintf( fp, "\t<psc slot=\"%d\" ref=\"%s\"/>\n", i, fname );
            sDumpBuffer( devcxt, fname, cb[i] );

            cb[i]->Release();
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static void sDumpGsConsts( ID3D11DeviceContext & devcxt, FILE * fp )
{
    ID3D11Buffer * cb[14];

    memset( cb, 0, sizeof(cb) );

    devcxt.GSGetConstantBuffers( 0, 14, cb );

    for( uint32 i = 0; i < 14; ++i )
    {
        if( cb[i] )
        {
            char fname[_MAX_PATH];
            sprintf_s( fname, "%s_gsc(%02d).bin", sDumpFilePrefix, i );
            fprintf( fp, "\t<gsc slot=\"%d\" ref=\"%s\"/>\n", i, fname );
            sDumpBuffer( devcxt, fname, cb[i] );

            cb[i]->Release();
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static void sDumpInputLayout( ID3D11DeviceContext & devcxt, FILE * fp )
{
    AutoComPtr<ID3D11InputLayout> il;

    devcxt.IAGetInputLayout( &il );

    if( !il ) return;

    UINT sz;

    // dump signature binary
    DynaArray<uint8> signature;
    char sname[_MAX_PATH];
    sprintf_s( sname, "%s_inputlayout_signature.bin", sDumpFilePrefix );
    il->GetPrivateData( IL1GUID(), &sz, 0 );
    if( 0 == sz )
    {
        GN_ERROR(sLogger)( "InputLayout is not dumpable. Please use createDumpableIL()." );
        return;
    }
    signature.resize( sz );
    il->GetPrivateData( IL1GUID(), &sz, &signature[0] );
    sDumpBinary( sname, &signature[0], signature.size() );

    // write IL open tag
    fprintf( fp, "\t<il signature=\"%s\">\n", sname );

    // get element array
    DynaArray<D3D11_INPUT_ELEMENT_DESC> elements;
    il->GetPrivateData( IL0GUID(), &sz, 0 );
    elements.resize( sz / sizeof(D3D11_INPUT_ELEMENT_DESC) );
    il->GetPrivateData( IL0GUID(), &sz, &elements[0] );

    // write element one by one
    for( size_t i = 0; i < sz / sizeof(D3D11_INPUT_ELEMENT_DESC); ++i )
    {
        const D3D11_INPUT_ELEMENT_DESC & e = elements[i];
        fprintf( fp,
            "\t\t<element semantic=\"%s\""
                        " index=\"%d\""
                        " format=\"%d\""
                        " slot=\"%d\""
                        " offset=\"%d\""
                        " classification=\"%d\""
                        " steprate=\"%d\"/>\n",
            e.SemanticName,
            e.SemanticIndex,
            e.Format,
            e.InputSlot,
            e.AlignedByteOffset,
            e.InputSlotClass,
            e.InstanceDataStepRate );
    }

    // write IL end tag
    fprintf( fp, "\t</il>\n" );
}

//
//
// -----------------------------------------------------------------------------
void sDumpVtxBufs( ID3D11DeviceContext & devcxt, FILE * fp )
{
    ID3D11Buffer * cb[16];
    UINT       stride[16];
    UINT       offset[16];

    devcxt.IAGetVertexBuffers( 0, 16, cb, stride, offset );

    char fname[_MAX_PATH];

    D3D11_BUFFER_DESC desc;

    for( DWORD i = 0; i < 16; ++i )
    {
        if( cb[i] )
        {
            sprintf_s( fname, "%s_vtxbuf(%02d).bin", sDumpFilePrefix, i );

            sDumpBuffer( devcxt, fname, cb[i] );

            cb[i]->GetDesc( &desc );

            fprintf( fp, "\t<vtxbuf slot=\"%d\" stride=\"%d\" offset=\"%d\" bytes=\"%d\" ref=\"%s\"/>\n",
                i, stride[i], offset[i], desc.ByteWidth, fname );

            cb[i]->Release();
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void sDumpIdxBuf( ID3D11DeviceContext & devcxt, FILE * fp )
{
    AutoComPtr<ID3D11Buffer> ib;
    DXGI_FORMAT format;
    UINT        offset;

    devcxt.IAGetIndexBuffer( &ib, &format, &offset );

    if( !ib ) return;

    D3D11_BUFFER_DESC desc;
    ib->GetDesc( &desc );

    char fname[_MAX_PATH];

    sprintf_s( fname, "%s_idxbuf.bin", sDumpFilePrefix );

    sDumpBuffer( devcxt, fname, ib );

    fprintf( fp, "\t<idxbuf format=\"%d\" offset=\"%d\" bytes=\"%u\" ref=\"%s\"/>\n",
           format, offset, desc.ByteWidth, fname );
}

//
//
// -----------------------------------------------------------------------------
static StrA sDumpResource( ID3D11DeviceContext & devcxt, const char * prefix, ID3D11Resource * res )
{
    D3D11_RESOURCE_DIMENSION dim;

    res->GetType( &dim );

    char fname[_MAX_PATH];

    if( D3D11_RESOURCE_DIMENSION_BUFFER == dim )
    {
        sprintf_s( fname, "%s_buffer.bin", prefix );
        sDumpBuffer( devcxt, fname, (ID3D11Buffer*)res );
    }
    else
    {
        sprintf_s( fname, "%s_texture.dds", prefix );
        //D3DX11SaveTextureToFileA( &devcxt, res, D3DX11_IFF_DDS, fname );
    }

    return fname;
}

//
//
// -----------------------------------------------------------------------------
static void sDumpShaderResources(
    ID3D11DeviceContext & devcxt,
    FILE * fp,
    const char * tag,
    ID3D11ShaderResourceView * srv[],
    size_t count )
{
    char descname[_MAX_PATH];
    char fname[_MAX_PATH];

    D3D11_SHADER_RESOURCE_VIEW_DESC desc;

    for( DWORD i = 0; i < count; ++i )
    {
        if( 0 == srv[i] ) continue;

        // save view descriptor
        srv[i]->GetDesc( &desc );
        sprintf_s( descname, "%s_%s_srv(%03d)_desc.bin", sDumpFilePrefix, tag, i );
        sDumpBinary( descname, &desc, sizeof(desc) );

        // save resource
        AutoComPtr<ID3D11Resource> res;
        srv[i]->GetResource( &res );
        sprintf_s( fname, "%s_%s_srv(%03d)",
            sDumpFilePrefix, tag, i );
        StrA resname = sDumpResource( devcxt, fname, res );

        fprintf( fp, "\t<%ssrv slot=\"%d\" desc=\"%s\" res=\"%s\"/>\n", tag, i, descname, resname.rawptr() );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sDumpVsSrv( ID3D11DeviceContext & devcxt, FILE * fp )
{
    ID3D11ShaderResourceView * srv[128];

    devcxt.VSGetShaderResources( 0, 128, srv );

    sDumpShaderResources( devcxt, fp, "vs", srv, 128 );

    for( int i = 0; i < 128; ++i ) safeRelease( srv[i] );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpGsSrv( ID3D11DeviceContext & devcxt, FILE * fp )
{
    ID3D11ShaderResourceView * srv[128];

    devcxt.GSGetShaderResources( 0, 128, srv );

    sDumpShaderResources( devcxt, fp, "gs", srv, 128 );

    for( int i = 0; i < 128; ++i ) safeRelease( srv[i] );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpPsSrv( ID3D11DeviceContext & devcxt, FILE * fp )
{
    ID3D11ShaderResourceView * srv[128];

    devcxt.PSGetShaderResources( 0, 128, srv );

    sDumpShaderResources( devcxt, fp, "ps", srv, 128 );

    for( int i = 0; i < 128; ++i ) safeRelease( srv[i] );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpRenderTargets( ID3D11DeviceContext & devcxt, FILE * fp )
{
    ID3D11RenderTargetView * colors[8];
    ID3D11DepthStencilView * depth;

    devcxt.OMGetRenderTargets( 8, colors, &depth );

    char descname[_MAX_PATH];
    char fname[_MAX_PATH];

    for( DWORD i = 0; i < 8; ++i )
    {
        if( 0 == colors[i] ) continue;

        D3D11_RENDER_TARGET_VIEW_DESC desc;
        colors[i]->GetDesc( &desc );
        sprintf_s( descname, "%s_rendertarget(%d)_desc.bin", sDumpFilePrefix, i );
        sDumpBinary( descname, &desc, sizeof(desc) );

        // save render target content
        AutoComPtr<ID3D11Resource> res;
        colors[i]->GetResource( &res );
        sprintf_s( fname, "%s_rendertarget(%d)",
            sDumpFilePrefix, i );
        StrA resname = sDumpResource( devcxt, fname, res );

        fprintf( fp,
            "\t<rendertarget slot=\"%d\" desc=\"%s\" res=\"%s\"/>\n",
            i, descname, resname.rawptr() );

        colors[i]->Release();
    }

    if( depth )
    {
        D3D11_DEPTH_STENCIL_VIEW_DESC desc;
        depth->GetDesc( &desc );
        sprintf_s( descname, "%s_depthstencil_desc.bin", sDumpFilePrefix );
        sDumpBinary( descname, &desc, sizeof(desc) );

        // save render target content
        AutoComPtr<ID3D11Resource> res;
        depth->GetResource( &res );
        sprintf_s( fname, "%s_depthstencil", sDumpFilePrefix );
        StrA resname = sDumpResource( devcxt, fname, res );

        fprintf( fp,
            "\t<depthstencil desc=\"%s\" res=\"%s\"/>\n",
            descname, resname.rawptr() );

        depth->Release();
    }
}

//
//
// -----------------------------------------------------------------------------
static void sDumpRenderStates( ID3D11DeviceContext & devcxt, FILE * fp )
{
    char fname[_MAX_PATH];

    // raster states
    AutoComPtr<ID3D11RasterizerState> rs;
    devcxt.RSGetState( &rs );
    if( rs )
    {
        D3D11_RASTERIZER_DESC rsdesc;
        rs->GetDesc( &rsdesc );
        sprintf_s( fname, "%s_rs.bin", sDumpFilePrefix );
        sDumpBinary( fname, &rsdesc, sizeof(rsdesc) );
        fprintf( fp, "\t<rs ref=\"%s\"/>\n", fname );
    }

    // blend states
    AutoComPtr<ID3D11BlendState> bs;
    float blendFactors[4];
    UINT sampleMask;
    devcxt.OMGetBlendState( &bs, blendFactors, &sampleMask );
    if( bs )
    {
        D3D11_BLEND_DESC desc;
        bs->GetDesc( &desc );
        sprintf_s( fname, "%s_bs.bin", sDumpFilePrefix );
        sDumpBinary( fname, &desc, sizeof(desc) );
        fprintf( fp, "\t<bs factor_r=\"%f\" factor_g=\"%f\" factor_b=\"%f\" factor_a=\"%f\" mask=\"%d\" ref=\"%s\"/>\n",
            blendFactors[0],
            blendFactors[1],
            blendFactors[2],
            blendFactors[3],
            sampleMask,
            fname );
    }

    // depth states
    AutoComPtr<ID3D11DepthStencilState> ds;
    UINT stencilRef;
    devcxt.OMGetDepthStencilState( &ds, &stencilRef );
    if( ds )
    {
        D3D11_DEPTH_STENCIL_DESC desc;
        ds->GetDesc( &desc );
        sprintf_s( fname, "%s_ds.bin", sDumpFilePrefix );
        sDumpBinary( fname, &desc, sizeof(desc) );
        fprintf( fp, "\t<ds stencilref=\"%u\" ref=\"%s\"/>\n", stencilRef, fname );
    }

    // viewport
    UINT vpcount = 1;
    D3D11_VIEWPORT vp;
    devcxt.RSGetViewports( &vpcount, &vp );
    fprintf(
        fp,
        "\t<viewport x=\"%d\" y=\"%d\" w=\"%u\" h=\"%u\" zmin=\"%f\" zmax=\"%f\"/>\n",
        vp.TopLeftX, vp.TopLeftY, vp.Width, vp.Height, vp.MinDepth, vp.MaxDepth );

    // scissor
    UINT sccount = 1;
    D3D11_RECT scissor;
    devcxt.RSGetScissorRects( &sccount, &scissor );
    fprintf(
        fp,
        "\t<scissor l=\"%d\" t=\"%d\" r=\"%d\" b=\"%d\"/>\n",
        scissor.left, scissor.top, scissor.right, scissor.bottom );
}

//
//
// -----------------------------------------------------------------------------
void sDumpD3D11States( ID3D11DeviceContext & devcxt, FILE * fp )
{
    sDumpVs( devcxt, fp );
    sDumpVsConsts( devcxt, fp );
    sDumpPs( devcxt, fp );
    sDumpPsConsts( devcxt, fp );
    sDumpGs( devcxt, fp );
    sDumpGsConsts( devcxt, fp );

    sDumpInputLayout( devcxt, fp );
    sDumpVtxBufs( devcxt, fp );
    sDumpIdxBuf( devcxt, fp );

    sDumpVsSrv( devcxt, fp );
    sDumpPsSrv( devcxt, fp );
    sDumpGsSrv( devcxt, fp );

    sDumpRenderTargets( devcxt, fp );

    sDumpRenderStates( devcxt, fp );

    // TODO: dump sampler states
}

// *****************************************************************************
// public function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API void GN::d3d11::setDumpFilePrefix( const StrA & prefix )
{
    size_t n = math::getmin<size_t>( prefix.size(), _MAX_PATH );
    memcpy( sDumpFilePrefix, prefix.rawptr(), n );
    sDumpFilePrefix[_MAX_PATH-1] = 0;
}

//
//
// -----------------------------------------------------------------------------
GN_API ID3D11VertexShader * GN::d3d11::createDumpableVS(
    ID3D11Device & device,
    const void   * binary,
    size_t         bytes )
{
    AutoComPtr<ID3D11VertexShader> shader;

    GN_DX_CHECK_RETURN( device.CreateVertexShader( binary, bytes, NULL, &shader ), 0 );

    shader->SetPrivateData( VSGUID(), (UINT)bytes, binary );

    return shader.detach();
}

//
//
// -----------------------------------------------------------------------------
GN_API ID3D11GeometryShader * GN::d3d11::createDumpableGS(
    ID3D11Device & device,
    const void   * binary,
    size_t         bytes )
{
    AutoComPtr<ID3D11GeometryShader> shader;

    GN_DX_CHECK_RETURN( device.CreateGeometryShader( binary, bytes, NULL, &shader ), 0 );

    shader->SetPrivateData( GSGUID(), (UINT)bytes, binary );

    return shader.detach();
}

//
//
// -----------------------------------------------------------------------------
GN_API ID3D11PixelShader * GN::d3d11::createDumpablePS(
    ID3D11Device & device,
    const void   * binary,
    size_t         bytes )
{
    AutoComPtr<ID3D11PixelShader> shader;

    GN_DX_CHECK_RETURN( device.CreatePixelShader( binary, bytes, NULL, &shader ), 0 );

    shader->SetPrivateData( PSGUID(), (UINT)bytes, binary );

    return shader.detach();
}

//
//
// -----------------------------------------------------------------------------
GN_API ID3D11InputLayout * GN::d3d11::createDumpableIL(
    ID3D11Device                   & device,
    const D3D11_INPUT_ELEMENT_DESC * elements,
    size_t                           count,
    const void                     * signature,
    size_t                           bytes )
{
    ID3D11InputLayout * il;

    GN_DX_CHECK_RETURN( device.CreateInputLayout( elements, (UINT)count, signature, bytes, &il ), 0 );

    il->SetPrivateData(
        IL0GUID(),
        (UINT)( sizeof(D3D11_INPUT_ELEMENT_DESC) * count ),
        elements );

    il->SetPrivateData(
        IL1GUID(),
        (UINT)bytes,
        signature );

    return il;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::d3d11::dumpDraw( ID3D11DeviceContext & devcxt, uint32 vertexCount, uint32 startVertex )
{
    DumpFile file;

    if( !file.fp ) return;

    D3D11_PRIMITIVE_TOPOLOGY prim;

    devcxt.IAGetPrimitiveTopology( &prim );

    fprintf( file.fp, "\t<draw prim=\"%d\" numvtx=\"%u\" startvtx=\"%u\"/>\n",
        prim, vertexCount, startVertex );

    sDumpD3D11States( devcxt, file.fp );
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::d3d11::dumpDrawIndexed( ID3D11DeviceContext & devcxt, uint32 indexCount, uint32 startIndex, uint32 startVertex )
{
    DumpFile file;

    if( !file.fp ) return;

    D3D11_PRIMITIVE_TOPOLOGY prim;

    devcxt.IAGetPrimitiveTopology( &prim );

    fprintf( file.fp, "\t<drawindexed prim=\"%d\" numidx=\"%u\" startidx=\"%u\" startvtx=\"%u\"/>\n",
        prim, indexCount, startIndex, startVertex );

    sDumpD3D11States( devcxt, file.fp );
}

#endif // GN_BUILD_HAS_D3D11
