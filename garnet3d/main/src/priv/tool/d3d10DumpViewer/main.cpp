#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::d3d10;

///
/// define to non-zero to render directly into back buffer;
/// else, render to offscreen render target, then copy to back buffer.
///

static GN::Logger * sLogger = GN::GetLogger("GN.tool.D3D10DumpViewer");

static StrA sDumpFileName;

/* *****************************************************************************
// shader function templates
// *****************************************************************************

template<typename T> struct ShaderFunc {};

template<> struct ShaderFunc<ID3D10VertexShader>
{
    static ID3D10VertexShader * sCreate( ID3D10Device & dev, const void * binary, size_t bytes )
    {
        ID3D10VertexShader * result;
        GN_DX_CHECK_RETURN( dev.CreateVertexShader( binary, bytes, &result ), NULL );
        return result;
    }

    static void sSetConstBufs( ID3D10Device & dev, UINT start, UINT count, ID3D10Buffer * const * buffers )
    {
        dev.VSSetConstantBuffers( start, count, buffers );
    }

    static void sSetShaderResources( ID3D10Device & dev, UINT start, UINT count, ID3D10ShaderResourceView * const * views )
    {
        dev.VSSetShaderResources( start, count, views );
    }
};

template<> struct ShaderFunc<ID3D10PixelShader>
{
    static ID3D10PixelShader * sCreate( ID3D10Device & dev, const void * binary, size_t bytes )
    {
        ID3D10PixelShader * result;
        GN_DX_CHECK_RETURN( dev.CreatePixelShader( binary, bytes, &result ), NULL );
        return result;
    }

    static void sSetConstBufs( ID3D10Device & dev, UINT start, UINT count, ID3D10Buffer * const * buffers )
    {
        dev.PSSetConstantBuffers( start, count, buffers );
    }

    static void sSetShaderResources( ID3D10Device & dev, UINT start, UINT count, ID3D10ShaderResourceView * const * views )
    {
        dev.PSSetShaderResources( start, count, views );
    }
};

template<> struct ShaderFunc<ID3D10GeometryShader>
{
    static ID3D10GeometryShader * sCreate( ID3D10Device & dev, const void * binary, size_t bytes )
    {
        ID3D10GeometryShader * result;
        GN_DX_CHECK_RETURN( dev.CreateGeometryShader( binary, bytes, &result ), NULL );
        return result;
    }

    static void sSetConstBufs( ID3D10Device & dev, UINT start, UINT count, ID3D10Buffer * const * buffers )
    {
        dev.GSSetConstantBuffers( start, count, buffers );
    }

    static void sSetShaderResources( ID3D10Device & dev, UINT start, UINT count, ID3D10ShaderResourceView * const * views )
    {
        dev.GSSetShaderResources( start, count, views );
    }
};*/

// *****************************************************************************
// device state dump structures
// *****************************************************************************

template<typename T>
static bool sLoadBinary( const XmlElement & node, const StrA & attr, const StrA & basedir, DynaArray<T> & result )
{
    const XmlAttrib * a = node.findAttrib( attr );
    if ( !a )
    {
        GN_ERROR(sLogger)("%s : attribute '%s' is missing!", node.getLocation(), attr.GetRawPtr() );
        return false;
    }

    StrA fullname = fs::resolvePath( basedir, a->value );

    if( !fs::isFile( fullname ) )
    {
        GN_WARN(sLogger)("%s : binary file not found :  %s!", node.getLocation(), fullname.GetRawPtr() );
    }

    AutoObjPtr<File> fp( fs::openFile( fullname, "rb" ) );
    if( !fp ) return false;

    result.resize( fp->size() / sizeof(T) );

    return fp->read( result.GetRawPtr(), result.size() * sizeof(T), 0 );
}

template<typename T>
static bool sGetNumericAttr( const XmlElement & node, const StrA & attrname, T & result )
{
    const XmlAttrib * a = node.findAttrib( attrname );
    if ( !a || !String2Number<T>( result, a->value.GetRawPtr() ) )
    {
        GN_ERROR(sLogger)("%s : attribute '%s' is missing!", node.getLocation(), attrname.GetRawPtr() );
        return false;
    }
    else
    {
        return true;
    }
}

template< UINT MAX_SLOT >
static bool sGetSlot( const XmlElement & node, UINT & slot )
{
    if( !sGetNumericAttr( node, "slot", slot ) ) return false;

    if( slot >= MAX_SLOT )
    {
        GN_ERROR(sLogger)( "%s: slot is too large" );
        return false;
    }

    return true;
}

template<typename T>
struct BinaryComDump
{
    DynaArray<UInt8> binary;
    AutoComPtr<T>    comptr;

    bool load( const XmlElement & node, const StrA & attr, const StrA & basedir )
    {
        return sLoadBinary( node, attr, basedir, binary );
    }

    void clear() { comptr.clear(); }
};

template<typename T>
struct ShaderDump : public BinaryComDump<T>
{
};

struct D3D10BufferDump : BinaryComDump<ID3D10Buffer>
{
    bool create( ID3D10Device & dev, UINT bind )
    {
        D3D10_BUFFER_DESC desc = {
            (UINT)binary.size(),
            D3D10_USAGE_IMMUTABLE,
            bind,
            0, // no CPU access
            0  // no misc. flags
        };
        D3D10_SUBRESOURCE_DATA data = {
            binary.GetRawPtr(),
            (UINT)binary.size(),
            (UINT)binary.size()
        };

        return S_OK == dev.CreateBuffer( &desc, &data, &comptr );
    }
};

struct D3D10ConstBufferDump : public D3D10BufferDump
{
};

struct D3D10InputLayoutDump
{
    StackArray<StrA,256>                semantics;
    DynaArray<D3D10_INPUT_ELEMENT_DESC> elements;
    DynaArray<UInt8>                    signature;
    AutoComPtr<ID3D10InputLayout>       comptr;

    bool load( const XmlElement & node, const StrA & basedir )
    {
        XmlElement * e;

        D3D10_INPUT_ELEMENT_DESC desc;

        for( XmlNode * n = node.child; n; n = n->next )
        {
            e = n->toElement();

            if( !e ) continue;
            else if( "element" == e->name )
            {
                XmlAttrib * sem = e->findAttrib( "semantic" );
                if( 0 == sem )
                {
                    GN_ERROR(sLogger)( "%s : 'semantic' attribute is missing.", e->getLocation() );
                    return false;
                }
                semantics.append( sem->value );
                desc.SemanticName = semantics.back().GetRawPtr();

                if( !sGetNumericAttr( *e, "index", desc.SemanticIndex ) ) return false;
                if( !sGetNumericAttr( *e, "format", (INT&)desc.Format ) ) return false;
                if( !sGetNumericAttr( *e, "slot", desc.InputSlot ) ) return false;
                if( !sGetNumericAttr( *e, "offset", desc.AlignedByteOffset ) ) return false;
                if( !sGetNumericAttr( *e, "classification", (INT&)desc.InputSlotClass ) ) return false;
                if( !sGetNumericAttr( *e, "steprate", desc.InstanceDataStepRate ) ) return false;

                elements.append( desc );
            }
            else
            {
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->getLocation(), e->name.GetRawPtr() );
            }
        }

        return sLoadBinary( node, "signature", basedir, signature );
    }

    void clear() { comptr.clear(); }
};

struct D3D10VtxBufDump : public D3D10BufferDump
{
    UInt32 stride;
    UInt32 offset;
};

struct D3D10IdxBufDump : public D3D10BufferDump
{
    SInt32 format;
    UInt32 offset;
};

template<typename T>
struct D3D10ViewDump
{
    UInt32                     width;
    UInt32                     height;
    DynaArray<UInt8>           desc;
    DynaArray<UInt8>           content;
    AutoComPtr<T>              view;
    AutoComPtr<ID3D10Resource> original; ///< store original resource data loaded from file
    AutoComPtr<ID3D10Resource> res;

    bool load( const XmlElement & node, const StrA & basedir )
    {
        if( !sLoadBinary( node, "desc", basedir, desc ) ) return false;
        if( !sLoadBinary( node, "res", basedir, content ) ) return false;
        return true;
    }

    bool createBuffer( ID3D10Device & dev, UINT bind )
    {
        width = (UInt32)content.size();
        height = 1;

        D3D10_BUFFER_DESC bd =
        {
            width,
            D3D10_USAGE_DEFAULT,
            bind,
            0,
            0,
        };

		D3D10_SUBRESOURCE_DATA sd = { content.GetRawPtr(), width, width };

        ID3D10Buffer * buf;

        GN_DX_CHECK_RETURN( dev.CreateBuffer( &bd, &sd, &buf ), false );

        res.attach( buf );
        return true;
    }

    bool createTexture( ID3D10Device & dev, UINT bind, DXGI_FORMAT format )
    {
        // get image information
        D3DX10_IMAGE_INFO info;
        GN_DX_CHECK_RETURN( D3DX10GetImageInfoFromMemory( content.GetRawPtr(), content.size(), 0, &info, 0 ), false );
        width  = info.Width;
        height = info.Height;

        if( DXGI_FORMAT_R32G8X24_TYPELESS == info.Format ||
            DXGI_FORMAT_D32_FLOAT_S8X24_UINT == info.Format ||
            DXGI_FORMAT_R24G8_TYPELESS == info.Format ||
            DXGI_FORMAT_D24_UNORM_S8_UINT == info.Format )
        {
            // This is a depth format that is not supported by current D3D10X library.
            // We'll have to use our custom loader

            MemFile<UInt8> file( content.GetRawPtr(), content.size() );

            ImageReader      ir;
            ImageDesc        id;
            DynaArray<UInt8> data;

            if( !ir.reset( file ) ) return false;
            if( !ir.readHeader( id ) ) return false;
            data.resize( id.getTotalBytes() );
            if( !ir.readImage( data.GetRawPtr() ) ) return false;

            switch( info.ResourceDimension )
            {
                case D3D10_RESOURCE_DIMENSION_TEXTURE1D :
                    GN_UNIMPL();
                    break;

                case D3D10_RESOURCE_DIMENSION_TEXTURE2D :
                {
                    D3D10_TEXTURE2D_DESC desc2d = {
                        id.mipmaps[0].width,
                        id.mipmaps[0].height,
                        id.numLevels,
                        id.numFaces,
                        (DXGI_FORMAT_UNKNOWN == format ) ? info.Format : format,
                        { 1, 0 }, // samples
                        D3D10_USAGE_STAGING, // usage
                        0, // bind
                        D3D10_CPU_ACCESS_WRITE, // CPU access
                        6 == id.numFaces ? D3D10_RESOURCE_MISC_TEXTURECUBE : 0
                    };

                    DynaArray<D3D10_SUBRESOURCE_DATA> subdata;
                    subdata.resize( id.numLevels * id.numFaces );
                    for( UInt32 f = 0; f < id.numFaces; ++f )
                    for( UInt32 l = 0; l < id.numLevels; ++l )
                    {
                        const MipmapDesc       & m = id.getMipmap( f, l );
                        D3D10_SUBRESOURCE_DATA & d = subdata[D3D10CalcSubresource( l, f, id.numLevels )];

                        d.pSysMem          = &data[id.getMipmapOffset( f, l )];
                        d.SysMemPitch      = m.rowPitch;
                        d.SysMemSlicePitch = m.slicePitch;
                    }

                    ID3D10Texture2D * tex2d;

                    GN_DX_CHECK_RETURN( dev.CreateTexture2D( &desc2d, subdata.GetRawPtr(), &tex2d ), false );
                    original.attach( tex2d );

                    desc2d.Usage = D3D10_USAGE_DEFAULT;
                    desc2d.BindFlags = bind;
                    desc2d.CPUAccessFlags = 0;
                    GN_DX_CHECK_RETURN( dev.CreateTexture2D( &desc2d, subdata.GetRawPtr(), &tex2d ), false );
                    res.attach( tex2d );

                    break;
                }

                case D3D10_RESOURCE_DIMENSION_TEXTURE3D :
                    GN_UNIMPL();
                    break;

                case D3D10_RESOURCE_DIMENSION_BUFFER :
                    GN_ERROR(sLogger)( "not support loading buffer from DDS" );
                    return false;

                default :
                    GN_ERROR(sLogger)( "invalid resource dimension" );
                    return false;
            }
        }
        else
        {
            D3DX10_IMAGE_LOAD_INFO load;

            // reset all to default
            memset( &load, 0xFF, sizeof(load) );
            load.pSrcInfo = NULL;

            load.FirstMipLevel = 0;
            load.MipLevels = info.MipLevels;
            if( DXGI_FORMAT_UNKNOWN != format )
            {
                load.Format = format;
            }

            GN_DX_CHECK_RETURN(
                D3DX10CreateTextureFromMemory(
                    &dev,
                    content.GetRawPtr(),
                    content.size(),
                    &load,
                    0,
                    &original,
                    0 ),
                false );

            load.BindFlags = bind;
            load.CpuAccessFlags = 0;

            GN_DX_CHECK_RETURN(
                D3DX10CreateTextureFromMemory(
                    &dev,
                    content.GetRawPtr(),
                    content.size(),
                    &load,
                    0,
                    &res,
                    0 ),
                false );
        }

        // success
        return true;
    }

    // restore resource content
    void restoreContent( ID3D10Device & dev )
    {
        if( res )
        {
            GN_ASSERT( original );
            dev.CopyResource( res, original );
        }
    }

    void clear() { view.clear(); res.clear(); }
};

struct D3D10SrvDump : public D3D10ViewDump<ID3D10ShaderResourceView>
{
    bool create( ID3D10Device & dev )
    {
        GN_ASSERT( sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC) == desc.size() );

        const D3D10_SHADER_RESOURCE_VIEW_DESC * srvdesc = (const D3D10_SHADER_RESOURCE_VIEW_DESC*)desc.GetRawPtr();

        if( D3D10_SRV_DIMENSION_BUFFER == srvdesc->ViewDimension )
        {
            if( !createBuffer( dev, D3D10_BIND_SHADER_RESOURCE ) ) return false;
        }
        else
        {
            if( !createTexture( dev, D3D10_BIND_SHADER_RESOURCE, srvdesc->Format ) ) return false;
        }

        // create view
        GN_DX_CHECK_RETURN(
            dev.CreateShaderResourceView( res, srvdesc, &view ),
            false );

        // success
        return true;
    }
};

struct D3D10RtvDump : public D3D10ViewDump<ID3D10RenderTargetView>
{
    AutoComPtr<ID3D10ShaderResourceView> srv;

    bool create( ID3D10Device & dev )
    {
        GN_ASSERT( sizeof(D3D10_RENDER_TARGET_VIEW_DESC) == desc.size() );

        const D3D10_RENDER_TARGET_VIEW_DESC * rtvdesc = (const D3D10_RENDER_TARGET_VIEW_DESC *)desc.GetRawPtr();

        if( !createTexture( dev, D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE, rtvdesc->Format ) ) return false;

        GN_DX_CHECK_RETURN(
            dev.CreateRenderTargetView( res, rtvdesc, &view ),
            false );

        D3D10_SHADER_RESOURCE_VIEW_DESC srvdesc = { rtvdesc->Format };
        switch( rtvdesc->ViewDimension )
        {
            case D3D10_RTV_DIMENSION_BUFFER:
                srvdesc.ViewDimension        = D3D10_SRV_DIMENSION_BUFFER;
                srvdesc.Buffer.ElementOffset = rtvdesc->Buffer.ElementOffset;
                srvdesc.Buffer.ElementWidth  = rtvdesc->Buffer.ElementWidth;
                break;

            case D3D10_RTV_DIMENSION_TEXTURE2D:
                srvdesc.ViewDimension             = D3D10_SRV_DIMENSION_TEXTURE2D;
                srvdesc.Texture2D.MostDetailedMip = rtvdesc->Texture2D.MipSlice;
                srvdesc.Texture2D.MipLevels       = 1;
                break;

            default:
                GN_UNIMPL();
                break;
        };

        GN_DX_CHECK_RETURN(
            dev.CreateShaderResourceView( res, &srvdesc, &srv ),
            false );

        // success
        return true;
    }

    void clear()
    {
        D3D10ViewDump<ID3D10RenderTargetView>::clear();
        srv.clear();
    }
};

struct D3D10DsvDump : public D3D10ViewDump<ID3D10DepthStencilView>
{
    bool create( ID3D10Device & dev )
    {
        GN_ASSERT( sizeof(D3D10_DEPTH_STENCIL_VIEW_DESC) == desc.size() );

        const D3D10_DEPTH_STENCIL_VIEW_DESC * dsvdesc = (const D3D10_DEPTH_STENCIL_VIEW_DESC *)desc.GetRawPtr();

        if( !createTexture( dev, D3D10_BIND_DEPTH_STENCIL, dsvdesc->Format ) ) return false;

        // create view
        GN_DX_CHECK_RETURN(
            dev.CreateDepthStencilView( res, dsvdesc, &view ),
            false );

        // success
        return true;
    }

    // CopyResource() does not support depth stencil texture.
    void restoreContent( ID3D10Device & dev )
    {
        if( content.empty() ) return;

        // D3D10 does not support loading data to depth/stencil texture.
        // We'll have to delete and recreate.
        view.clear();
        original.clear();
        res.clear();
        create( dev );
    }
};

struct D3D10RasterizerStateDump : BinaryComDump<ID3D10RasterizerState>
{
};

struct D3D10BlendStateDump : BinaryComDump<ID3D10BlendState>
{
    float factors[4];
    UINT  sampleMask;
};

struct D3D10DepthStencilStateDump : BinaryComDump<ID3D10DepthStencilState>
{
    UINT stencilRef;
};

struct D3D10SamplerStateDump : BinaryComDump<ID3D10SamplerState>
{
    bool create( ID3D10Device & dev )
    {
        static const D3D10_SAMPLER_DESC DEFAULT_DESC = {
            D3D10_FILTER_MIN_MAG_MIP_LINEAR,
            D3D10_TEXTURE_ADDRESS_CLAMP,
            D3D10_TEXTURE_ADDRESS_CLAMP,
            D3D10_TEXTURE_ADDRESS_CLAMP,
            0,
            1,
            D3D10_COMPARISON_NEVER,
            { 1.0f, 1.0f, 1.0f, 1.0f },
            -FLT_MAX,
            FLT_MAX
        };

        const D3D10_SAMPLER_DESC * desc = binary.empty() ? &DEFAULT_DESC : (const D3D10_SAMPLER_DESC *)binary.GetRawPtr();

        return S_OK == dev.CreateSamplerState( desc, &comptr );
    }
};

struct D3D10OperationDump
{
    bool   indexed;
	bool   instanced;
    SInt32 prim;
    UInt32 startidx; // StartIndexLocation
    UInt32 numidx;   // IndexCountPerInstance
    UInt32 startvtx; // BaseVertexLocation
    UInt32 numvtx;
	UInt32 numinst;
	UInt32 startinst;

	D3D10OperationDump()
	{
		memset( this, sizeof(*this), 0 );
	}

    void draw( ID3D10Device & dev )
    {
        dev.IASetPrimitiveTopology( (D3D10_PRIMITIVE_TOPOLOGY)prim );
        if( indexed )
        {
			if( instanced )
			{
	            dev.DrawIndexedInstanced( numidx, numinst, startidx, startvtx, startinst );
			}
			else
			{
	            //setDumpFilePrefix( fs::joinPath( "a", fs::baseName(sDumpFileName) ) );
	            //dumpDrawIndexed( dev, numidx, startidx, startvtx );

	            dev.DrawIndexed( numidx, startidx, startvtx );
			}

        }
        else
        {
			if( instanced )
			{
				GN_DO_ONCE( GN_FATAL(sLogger)("Do not support DrawInstanced(...) yet.") );
			}
			else
			{
	            //setDumpFilePrefix( fs::joinPath( "a", fs::baseName(sDumpFileName) ) );
	            //dumpDraw( dev, numvtx, startvtx );

	            dev.Draw( numvtx, startvtx );
			}
        }
    }
};

struct D3D10StateDump
{
    //@{

    BinaryComDump<ID3D10VertexShader>   vs;
    BinaryComDump<ID3D10PixelShader>    ps;
    BinaryComDump<ID3D10GeometryShader> gs;

    D3D10BufferDump                     vsc[14];
    D3D10BufferDump                     psc[14];
    D3D10BufferDump                     gsc[14];

    D3D10SrvDump                        vssrv[128];
    D3D10SrvDump                        pssrv[128];
    D3D10SrvDump                        gssrv[128];

    D3D10SamplerStateDump               vssamp[16];
    D3D10SamplerStateDump               pssamp[16];
    D3D10SamplerStateDump               gssamp[16];

    D3D10InputLayoutDump                il;
    D3D10VtxBufDump                     vtxbufs[16];
    D3D10IdxBufDump                     idxbuf;

    D3D10RtvDump                        rendertargets[8];
    D3D10DsvDump                        depthstencil;

    D3D10RasterizerStateDump            rs;
    D3D10BlendStateDump                 bs;
    D3D10DepthStencilStateDump          ds;

    D3D10_VIEWPORT                      viewport;
    D3D10_RECT                          scissorrect;

    D3D10OperationDump                  operation;

    //@}


    //@{

    bool loadFromXml( const XmlNode & root, const StrA & basedir )
    {
        GN_GUARD;

        // check root name
        const XmlElement * e = root.toElement();
        if( 0 == e || "D3D10StateDump" != e->name )
        {
            GN_ERROR(sLogger)( "root node must be \"<D3D10StateDump>\"." );
            return false;
        }

        for( XmlNode * n = root.child; n; n = n->next )
        {
            e = n->toElement();

            if( !e ) continue;

            else if( "vs" == e->name )
            {
                if( !vs.load( *e, "ref", basedir ) ) return false;
            }
            else if( "ps" == e->name )
            {
                if( !ps.load( *e, "ref", basedir ) ) return false;
            }
            else if( "gs" == e->name )
            {
                if( !gs.load( *e, "ref", basedir ) ) return false;
            }
            else if( "vsc" == e->name )
            {
                UINT slot;
                if( !sGetSlot<GN_ARRAY_COUNT(vsc)>( *e, slot ) ) return false;
                if( !vsc[slot].load( *e, "ref", basedir ) ) return false;
            }
            else if( "psc" == e->name )
            {
                UINT slot;
                if( !sGetSlot<GN_ARRAY_COUNT(psc)>( *e, slot ) ) return false;
                if( !psc[slot].load( *e, "ref", basedir ) ) return false;
            }
            else if( "gsc" == e->name )
            {
                UINT slot;
                if( !sGetSlot<GN_ARRAY_COUNT(gsc)>( *e, slot ) ) return false;
                if( !gsc[slot].load( *e, "ref", basedir ) ) return false;
            }
            else if( "il" == e->name )
            {
                if( !il.load( *e, basedir ) ) return false;
            }
            else if( "vtxbuf" == e->name )
            {
                UINT slot;
                if( !sGetSlot<GN_ARRAY_COUNT(vtxbufs)>( *e, slot ) ) return false;
                if( !vtxbufs[slot].load( *e, "ref", basedir ) ) return false;
                if( !sGetNumericAttr( *e, "stride", vtxbufs[slot].stride ) ) return false;
                if( !sGetNumericAttr( *e, "offset", vtxbufs[slot].offset ) ) return false;
            }
            else if( "idxbuf" == e->name )
            {
                if( !idxbuf.load( *e, "ref", basedir ) ) return false;
                if( !sGetNumericAttr( *e, "format", idxbuf.format ) ) return false;
                if( !sGetNumericAttr( *e, "offset", idxbuf.offset ) ) return false;
            }
            else if( "vssrv" == e->name )
            {
                UINT slot;
                if( !sGetSlot<GN_ARRAY_COUNT(vssrv)>( *e, slot ) ) return false;
                if( !vssrv[slot].load( *e, basedir ) ) return false;
            }
            else if( "pssrv" == e->name )
            {
                UINT slot;
                if( !sGetSlot<GN_ARRAY_COUNT(pssrv)>( *e, slot ) ) return false;
                if( !pssrv[slot].load( *e, basedir ) ) return false;
            }
            else if( "gssrv" == e->name )
            {
                UINT slot;
                if( !sGetSlot<GN_ARRAY_COUNT(gssrv)>( *e, slot ) ) return false;
                if( !gssrv[slot].load( *e, basedir ) ) return false;
            }
            else if( "rendertarget" == e->name )
            {
                UINT slot;
                if( !sGetSlot<GN_ARRAY_COUNT(rendertargets)>( *e, slot ) ) return false;
                if( !rendertargets[slot].load( *e, basedir ) ) return false;
            }
            else if( "depthstencil" == e->name )
            {
                if( !depthstencil.load( *e, basedir ) ) return false;
            }
            else if( "rs" == e->name )
            {
                if( !rs.load( *e, "ref", basedir ) ) return false;
            }
            else if( "bs" == e->name )
            {
                if( !bs.load( *e, "ref", basedir ) ) return false;
                if( !sGetNumericAttr( *e, "factor_r", bs.factors[0] ) ) return false;
                if( !sGetNumericAttr( *e, "factor_g", bs.factors[1] ) ) return false;
                if( !sGetNumericAttr( *e, "factor_b", bs.factors[2] ) ) return false;
                if( !sGetNumericAttr( *e, "factor_a", bs.factors[3] ) ) return false;
                if( !sGetNumericAttr( *e, "mask", bs.sampleMask ) ) return false;
            }
            else if( "ds" == e->name )
            {
                if( !ds.load( *e, "ref", basedir ) ) return false;
                if( !sGetNumericAttr( *e, "stencilref", ds.stencilRef ) ) return false;
            }
            else if( "vssamp" == e->name )
            {
                UINT slot;
                if( !sGetSlot<GN_ARRAY_COUNT(vssamp)>( *e, slot ) ) return false;
                if( !vssamp[slot].load( *e, "desc", basedir ) ) return false;
            }
            else if( "gssamp" == e->name )
            {
                UINT slot;
                if( !sGetSlot<GN_ARRAY_COUNT(gssamp)>( *e, slot ) ) return false;
                if( !gssamp[slot].load( *e, "desc", basedir ) ) return false;
            }
            else if( "pssamp" == e->name )
            {
                UINT slot;
                if( !sGetSlot<GN_ARRAY_COUNT(pssamp)>( *e, slot ) ) return false;
                if( !pssamp[slot].load( *e, "desc", basedir ) ) return false;
            }
            else if( "viewport" == e->name )
            {
                if( !sGetNumericAttr( *e, "x", (UInt32&)viewport.TopLeftX ) ) return false;
                if( !sGetNumericAttr( *e, "y", (UInt32&)viewport.TopLeftY ) ) return false;
                if( !sGetNumericAttr( *e, "w", (UInt32&)viewport.Width ) ) return false;
                if( !sGetNumericAttr( *e, "h", (UInt32&)viewport.Height ) ) return false;
                if( !sGetNumericAttr( *e, "zmin", viewport.MinDepth ) ) return false;
                if( !sGetNumericAttr( *e, "zmax", viewport.MaxDepth ) ) return false;
            }
            else if( "scissor" == e->name )
            {
                if( !sGetNumericAttr( *e, "l", (SInt32&)scissorrect.left ) ) return false;
                if( !sGetNumericAttr( *e, "t", (SInt32&)scissorrect.top ) ) return false;
                if( !sGetNumericAttr( *e, "r", (SInt32&)scissorrect.right ) ) return false;
                if( !sGetNumericAttr( *e, "b", (SInt32&)scissorrect.bottom ) ) return false;
            }
            else if( "drawindexed" == e->name )
            {
                operation.indexed = true;
                operation.instanced = false;
                if( !sGetNumericAttr( *e, "prim", operation.prim ) ) return false;
                if( !sGetNumericAttr( *e, "startidx", operation.startidx ) ) return false;
                if( !sGetNumericAttr( *e, "startvtx", operation.startvtx ) ) return false;
                if( !sGetNumericAttr( *e, "numidx", operation.numidx ) ) return false;
            }
            else if( "draw" == e->name )
            {
                operation.indexed = false;
                operation.instanced = false;
                if( !sGetNumericAttr( *e, "prim", operation.prim ) ) return false;
                if( !sGetNumericAttr( *e, "startvtx", operation.startvtx ) ) return false;
                if( !sGetNumericAttr( *e, "numvtx", operation.numvtx ) ) return false;
            }
			else if( "drawindexedinstanced" == e->name )
			{
                operation.indexed = true;
                operation.instanced = true;
                if( !sGetNumericAttr( *e, "prim", operation.prim ) ) return false;
                if( !sGetNumericAttr( *e, "IndexCountPerInstance", operation.numidx ) ) return false;
                if( !sGetNumericAttr( *e, "InstanceCount", operation.numinst ) ) return false;
                if( !sGetNumericAttr( *e, "StartIndexLocation", operation.startidx ) ) return false;
                if( !sGetNumericAttr( *e, "BaseVertexLocation", operation.startvtx ) ) return false;
                if( !sGetNumericAttr( *e, "StartInstanceLocation", operation.startinst ) ) return false;
			}
            else
            {
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->getLocation(), e->name.GetRawPtr() );
            }
        }

        // success
        return true;

        GN_UNGUARD;
    }

    bool onCreate( ID3D10Device & dev )
    {
        GN_GUARD;

        // vs
        if( !vs.binary.empty() )
        {
            GN_DX_CHECK_RETURN( dev.CreateVertexShader( vs.binary.GetRawPtr(), vs.binary.size(), &vs.comptr ), false );
        }

        // ps
        if( !ps.binary.empty() )
        {
            GN_DX_CHECK_RETURN( dev.CreatePixelShader( ps.binary.GetRawPtr(), ps.binary.size(), &ps.comptr ), false );
        }

        // gs
        if( !gs.binary.empty() )
        {
            GN_DX_CHECK_RETURN( dev.CreateGeometryShader( gs.binary.GetRawPtr(), gs.binary.size(), &gs.comptr ), false );
        }

        // const buffers
        for( size_t i = 0; i < GN_ARRAY_COUNT(vsc); ++i )
        {
            if( !vsc[i].binary.empty() && !vsc[i].create( dev, D3D10_BIND_CONSTANT_BUFFER ) ) return false;
            if( !psc[i].binary.empty() && !psc[i].create( dev, D3D10_BIND_CONSTANT_BUFFER ) ) return false;
            if( !gsc[i].binary.empty() && !gsc[i].create( dev, D3D10_BIND_CONSTANT_BUFFER ) ) return false;
        }

        // il
        if( !il.elements.empty() )
        {
	        GN_DX_CHECK_RETURN(
	            dev.CreateInputLayout(
	                il.elements.GetRawPtr(),
	                (UINT)il.elements.size(),
	                il.signature.GetRawPtr(),
	                (UINT)il.signature.size(),
	                &il.comptr ),
	            false );
        }

        // vb
        for( size_t i = 0; i < GN_ARRAY_COUNT(vtxbufs); ++i )
        {
            D3D10VtxBufDump & vbd = vtxbufs[i];

            if( vbd.binary.empty() ) continue;

            if( !vbd.create( dev, D3D10_BIND_VERTEX_BUFFER ) ) return false;
        }

        // ib
        if( !idxbuf.binary.empty() )
        {
            if( !idxbuf.create( dev, D3D10_BIND_INDEX_BUFFER ) ) return false;
        }

        // srv
        for( size_t i = 0; i < GN_ARRAY_COUNT(vssrv); ++i )
        {
            if( !vssrv[i].content.empty() && !vssrv[i].create( dev ) ) return false;
            if( !pssrv[i].content.empty() && !pssrv[i].create( dev ) ) return false;
            if( !gssrv[i].content.empty() && !gssrv[i].create( dev ) ) return false;
        }

        // samplers
        for( size_t i = 0; i < GN_ARRAY_COUNT(vssamp); ++i )
        {
            if( !vssamp[i].create( dev ) ) return false;
            if( !pssamp[i].create( dev ) ) return false;
            if( !gssamp[i].create( dev ) ) return false;
        }

        // rtv
        for( size_t i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i )
        {
            if( !rendertargets[i].content.empty() && !rendertargets[i].create( dev ) ) return false;
        }

        // dsv
        if( !depthstencil.content.empty() && !depthstencil.create( dev ) ) return false;

        // rs
        GN_ASSERT( sizeof(D3D10_RASTERIZER_DESC) == rs.binary.size() );
        GN_DX_CHECK_RETURN( dev.CreateRasterizerState( (const D3D10_RASTERIZER_DESC *)rs.binary.GetRawPtr(), &rs.comptr ), false );

        // bs
        GN_ASSERT( sizeof(D3D10_BLEND_DESC) == bs.binary.size() );
        GN_DX_CHECK_RETURN( dev.CreateBlendState( (const D3D10_BLEND_DESC *)bs.binary.GetRawPtr(), &bs.comptr ), false );

        // ds
        GN_ASSERT( sizeof(D3D10_DEPTH_STENCIL_DESC) == ds.binary.size() );
        GN_DX_CHECK_RETURN( dev.CreateDepthStencilState( (const D3D10_DEPTH_STENCIL_DESC *)ds.binary.GetRawPtr(), &ds.comptr ), false );

        // success
        return true;

        GN_UNGUARD;
    }

    void onDestroy()
    {
        GN_GUARD;

        vs.clear();
        ps.clear();
        gs.clear();

        for( int i = 0; i < GN_ARRAY_COUNT(vsc); ++i ) vsc[i].clear();
        for( int i = 0; i < GN_ARRAY_COUNT(psc); ++i ) psc[i].clear();
        for( int i = 0; i < GN_ARRAY_COUNT(gsc); ++i ) gsc[i].clear();

        for( int i = 0; i < GN_ARRAY_COUNT(vssrv); ++i ) vssrv[i].clear();
        for( int i = 0; i < GN_ARRAY_COUNT(pssrv); ++i ) pssrv[i].clear();
        for( int i = 0; i < GN_ARRAY_COUNT(gssrv); ++i ) gssrv[i].clear();

        for( int i = 0; i < GN_ARRAY_COUNT(vssamp); ++i ) vssamp[i].clear();
        for( int i = 0; i < GN_ARRAY_COUNT(pssamp); ++i ) pssamp[i].clear();
        for( int i = 0; i < GN_ARRAY_COUNT(gssamp); ++i ) gssamp[i].clear();

        il.clear();

        for( int i = 0; i < GN_ARRAY_COUNT(vtxbufs); ++i ) vtxbufs[i].clear();

        idxbuf.clear();

        for( int i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i ) rendertargets[i].clear();

        depthstencil.clear();

        rs.clear();
        bs.clear();
        ds.clear();

        GN_UNGUARD;
    }

    /// restore render target cotent
    void restoreContent( ID3D10Device & dev )
    {
        for( UInt32 i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i )
        {
            rendertargets[i].restoreContent( dev );
        }
        depthstencil.restoreContent( dev );
    }

    void bind( ID3D10Device & dev, ID3D10RenderTargetView * backbuf ) const
    {
        GN_GUARD_SLOW;

        PixPerfScopeEvent pixevent( 0, L"Bind" );

        // shaders
        dev.VSSetShader( vs.comptr );
        dev.GSSetShader( gs.comptr );
        dev.PSSetShader( ps.comptr );

        // constants
        ID3D10Buffer * cb[GN_ARRAY_COUNT(vsc)];

        for( UINT i = 0; i < GN_ARRAY_COUNT(cb); ++i ) cb[i] = vsc[i].comptr;
        dev.VSSetConstantBuffers( 0, GN_ARRAY_COUNT(cb), cb );

        for( UINT i = 0; i < GN_ARRAY_COUNT(cb); ++i ) cb[i] = psc[i].comptr;
        dev.PSSetConstantBuffers( 0, GN_ARRAY_COUNT(cb), cb );

        for( UINT i = 0; i < GN_ARRAY_COUNT(cb); ++i ) cb[i] = gsc[i].comptr;
        dev.GSSetConstantBuffers( 0, GN_ARRAY_COUNT(cb), cb );

        // inputlayout
        dev.IASetInputLayout( il.comptr );

        // vb
        for( UInt32 i = 0; i < GN_ARRAY_COUNT(vtxbufs); ++i )
        {
            const D3D10VtxBufDump & vb = vtxbufs[i];
            if( vb.comptr )
            {
                dev.IASetVertexBuffers( i, 1, &vb.comptr, &vb.stride, &vb.offset );
            }
        }

        // ib
        if( idxbuf.comptr )
        {
            dev.IASetIndexBuffer( idxbuf.comptr, (DXGI_FORMAT)idxbuf.format, idxbuf.offset );
        }

        // srv
        for( UInt32 i = 0; i < GN_ARRAY_COUNT(vssrv); ++i )
        {
            if( vssrv[i].view ) dev.VSSetShaderResources( i, 1, &vssrv[i].view );
            if( pssrv[i].view ) dev.PSSetShaderResources( i, 1, &pssrv[i].view );
            if( gssrv[i].view ) dev.GSSetShaderResources( i, 1, &gssrv[i].view );
        }

        // samplers
        ID3D10SamplerState * samplers[GN_ARRAY_COUNT(vssamp)];

        for( size_t i = 0; i < GN_ARRAY_COUNT(vssamp); ++i ) samplers[i] = vssamp[i].comptr;
        dev.VSSetSamplers( 0, GN_ARRAY_COUNT(vssamp), samplers );

        for( size_t i = 0; i < GN_ARRAY_COUNT(pssamp); ++i ) samplers[i] = pssamp[i].comptr;
        dev.PSSetSamplers( 0, GN_ARRAY_COUNT(pssamp), samplers );

        for( size_t i = 0; i < GN_ARRAY_COUNT(gssamp); ++i ) samplers[i] = gssamp[i].comptr;
        dev.GSSetSamplers( 0, GN_ARRAY_COUNT(gssamp), samplers );

        // rtv
        ID3D10RenderTargetView * rtv[GN_ARRAY_COUNT(rendertargets)];
        for( UInt32 i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i )
        {
            rtv[i] = rendertargets[i].view;
        }
        if( backbuf ) rtv[0] = backbuf;
        dev.OMSetRenderTargets( 8, rtv, depthstencil.view );

        // rs, bs, ds
        dev.RSSetState( rs.comptr );
        dev.RSSetViewports( 1, &viewport );
        dev.RSSetScissorRects( 1, &scissorrect );
        dev.OMSetBlendState( bs.comptr, bs.factors, bs.sampleMask );
        dev.OMSetDepthStencilState( ds.comptr, ds.stencilRef );

        GN_UNGUARD_SLOW;
    }

    void draw( ID3D10Device & dev )
    {
        GN_GUARD_SLOW;

        operation.draw( dev );

        GN_UNGUARD_SLOW;
    }

    //@}
};

#define DRAW_TO_BACKBUF 0

class MyApp : public D3D10Application
{
    D3D10StateDump            mState;
    AutoComPtr<ID3DX10Sprite> mSprite;

    void copyRt0ToBackbuf()
    {
        PixPerfScopeEvent pixevent( 0, L"Copy RT0 to back buffer" );

        ID3D10Device & dev = device();

        static const D3DXCOLOR PURE_WHITE( 1.0f, 1.0f, 1.0f, 1.0f );

        D3DX10_SPRITE s;
        D3DXMatrixScaling( &s.matWorld, 2.0f, 2.0f, 1.0f );
        s.TexCoord.x    = 0;
        s.TexCoord.y    = 0;
        s.TexSize.x     = 1.0f;
        s.TexSize.y     = 1.0f;
        s.ColorModulate = PURE_WHITE;
        s.pTexture      = mState.rendertargets[0].srv;
        s.TextureIndex  = 0;

        ID3D10RenderTargetView * rtv = backrtv();
        dev.ClearState();
        D3D10_VIEWPORT vp = { 0, 0, mState.rendertargets[0].width, mState.rendertargets[0].height, 0.0f, 1.0f };
        dev.RSSetViewports( 1, &vp );
        dev.OMSetRenderTargets( 1, &rtv, 0 );

        mSprite->Begin( 0 );
        mSprite->DrawSpritesImmediate( &s, 1, 0, 0 );
        mSprite->End();
    }

protected:

    bool onInit( D3D10AppOption & o )
    {
        GN_GUARD;

        if( !loadFromXmlFile( mState, sDumpFileName ) ) return false;

#if DRAW_TO_BACKBUF
        o.width  = mState.rendertargets[0].width;
        o.height = mState.rendertargets[0].height;
#else
        o.width  = mState.viewport.Width;
        o.height = mState.viewport.Height;
#endif

        // success
        return true;

        GN_UNGUARD;
    }

    bool onCreate()
    {
        GN_GUARD;

        if( !mState.onCreate( device() ) ) return false;

        GN_DX_CHECK_RETURN( D3DX10CreateSprite( &device(), 1, &mSprite ), false );

        return true;

        GN_UNGUARD;
    }

    void onDestroy()
    {
        GN_GUARD;

        mState.onDestroy();
        mSprite.clear();

        GN_UNGUARD;
    }

    void onDraw()
    {
        GN_GUARD;

        ID3D10Device & dev = device();

        mState.restoreContent( dev );
#if DRAW_TO_BACKBUF
        copyRt0ToBackbuf();
        mState.bind( dev, backrtv() );
        mState.draw( dev );
#else
        mState.bind( dev, 0 );
        mState.draw( dev );
        copyRt0ToBackbuf();
#endif

        GN_UNGUARD;
    }
};

void printhelp( const char * appname )
{
    printf( "Usage: %s <ref|hal|refd|hald> [dumpname]\n", (fs::baseName(appname) + fs::extName(appname)).GetRawPtr() );
}

int main( int argc, const char * argv [] )
{
    GN_GUARD;

    D3D10AppOption opt;

    // parse command line
    if( argc < 2 )
    {
        printhelp( argv[0] );
        return -1;
    }
    else if( argc == 2 )
    {
        sDumpFileName = argv[1];
    }
    else
    {
        opt.ref = ( 0 == StringCompareI( "ref", argv[1] ) ) || ( 0 == StringCompareI( "refd", argv[1] ) );
        opt.debug = ( 0 == StringCompareI( "hald", argv[1] ) ) || ( 0 == StringCompareI( "refd", argv[1] ) );
        sDumpFileName = argv[2];
    }

    MyApp app;
    return app.run( opt );

    GN_UNGUARD;
}
