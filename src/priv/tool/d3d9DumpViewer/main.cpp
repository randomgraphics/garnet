#include "pch.h"

using namespace GN;
using namespace GN::d3d9;

///
/// define to non-zero to render directly into back buffer;
/// else, render to offscreen render target, then copy to back buffer.
///
#define RENDER_TO_BACKBUF 0

static GN::Logger * sLogger = GN::GetLogger("GN.tool.D3D9DumpViewer");

static StrA sDumpFileName;

struct D3D9VsDump
{
    StrA                               source;
    AutoComPtr<IDirect3DVertexShader9> vs;
};

struct D3D9PsDump
{
    StrA                              source;
    AutoComPtr<IDirect3DPixelShader9> ps;
};

struct D3D9VtxDeclDump
{
    D3DVERTEXELEMENT9                       elements[MAX_FVF_DECL_SIZE];
    AutoComPtr<IDirect3DVertexDeclaration9> decl;
};

struct D3D9VtxBufDump
{
    UInt32 offset;
    UInt32 stride;
    StrA   ref;
    AutoComPtr<IDirect3DVertexBuffer9> vb;
};

struct D3D9IdxBufDump
{
    UInt32 format;
    UInt32 startvtx;
    StrA   ref;
    AutoComPtr<IDirect3DIndexBuffer9> ib;
};

struct D3D9TextureDump
{
    StrA ref;
    AutoComPtr<IDirect3DBaseTexture9> tex;
};

struct D3D9RtDump
{
    bool   inuse;
    UInt32 width;
    UInt32 height;
    UInt32 format;
    UInt32 msaa;
    UInt32 quality;
    StrA   ref;
    AutoComPtr<IDirect3DSurface9> surf;
    AutoComPtr<IDirect3DSurface9> syscopy;
};

struct D3D9RsDump
{
    UInt32 type;
    UInt32 value;
};

struct D3D9OperationDump
{
    bool    indexed;

    UInt32  prim;
    UInt32  numprim;
    UInt32  startvtx;

    // for indexed draw only
    UInt32  minvtxidx;
    UInt32  numvtx;
    UInt32  startidx;

    void draw( IDirect3DDevice9 & dev )
    {
        if( indexed )
        {
            dev.DrawIndexedPrimitive(
                (D3DPRIMITIVETYPE) prim,
                startvtx,
                minvtxidx,
                numvtx,
                startidx,
                numprim );
        }
        else
        {
            dev.DrawPrimitive(
                (D3DPRIMITIVETYPE) prim,
                startvtx,
                numprim );
        }
    }
};

struct D3D9StateDump
{
    //@{

    D3D9VsDump        vs;
    float             vsconstf[256][4];
    int               vsconsti[16][4];
    int               vsconstb[16];

    D3D9PsDump        ps;
    float             psconstf[224][4];
    int               psconsti[16][4];
    int               psconstb[16];

    D3D9VtxDeclDump   vtxdecl;

    D3D9VtxBufDump    vtxbufs[16];

    D3D9IdxBufDump    idxbuf;

    D3D9TextureDump   textures[16];

    D3D9RtDump        rendertargets[4];

    D3D9RtDump        depthstencil;

    D3D9RsDump        renderstates[103];

    UInt32            samplerstates[16][14]; // samplerstates[x][0] is unused.

    D3DVIEWPORT9      viewport;

    RECT              scissorrect;

    D3D9OperationDump operation;

    //@}


    //@{

    void Clear()
    {
        onDispose();

        vs.source.Clear();
        memset( &vsconstf, 0, sizeof(vsconstf) );
        memset( &vsconsti, 0, sizeof(vsconsti) );
        memset( &vsconstb, 0, sizeof(vsconstb) );

        ps.source.Clear();
        memset( &psconstf, 0, sizeof(psconstf) );
        memset( &psconsti, 0, sizeof(psconsti) );
        memset( &psconstb, 0, sizeof(psconstb) );

        for( int i = 0; i < GN_ARRAY_COUNT(vtxbufs); ++i ) vtxbufs[i].ref.Clear();

        idxbuf.ref.Clear();

        for( int i = 0; i < GN_ARRAY_COUNT(textures); ++i ) textures[i].ref.Clear();

        for( int i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i ) rendertargets[i].inuse = false;

        depthstencil.inuse = false;
    }

    bool onRestore( IDirect3DDevice9 & dev )
    {
        // vs
        if( !vs.source.Empty() )
        {
            vs.vs.Attach( assembleAndCreateVS( &dev, vs.source.ToRawPtr(), 0 ) );
            if( vs.vs.Empty() ) return false;
        }

        // ps
        if( !ps.source.Empty() )
        {
            ps.ps.Attach( assembleAndCreatePS( &dev, ps.source.ToRawPtr(), 0 ) );
            if( ps.ps.Empty() ) return false;
        }

        // decl
        GN_DX_CHECK_RETURN( dev.CreateVertexDeclaration( vtxdecl.elements, &vtxdecl.decl ), false );

        // vb
        for( size_t i = 0; i < GN_ARRAY_COUNT(vtxbufs); ++i )
        {
            D3D9VtxBufDump & vbd = vtxbufs[i];

            if( vbd.ref.Empty() ) continue;

            AutoObjPtr<File> fp( fs::OpenFile( vbd.ref, "rb" ) );
            if( !fp ) return false;

            size_t bytes = fp->Size();

            GN_DX_CHECK_RETURN( dev.CreateVertexBuffer( (UINT32)bytes, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vtxbufs[i].vb, 0 ), false );

            void * vertices;
            GN_DX_CHECK_RETURN( vbd.vb->Lock( 0, 0, &vertices, 0 ), false );

            bool ok = fp->Read( vertices, bytes, 0 );

            vbd.vb->Unlock();

            if( !ok ) return false;
        }

        // ib
        if( !idxbuf.ref.Empty() )
        {
            AutoObjPtr<File> fp( fs::OpenFile( idxbuf.ref, "rb" ) );
            if( !fp ) return false;

            size_t bytes = fp->Size();

            GN_DX_CHECK_RETURN( dev.CreateIndexBuffer( (UINT32)bytes, D3DUSAGE_WRITEONLY, (D3DFORMAT)idxbuf.format, D3DPOOL_DEFAULT, &idxbuf.ib, 0 ), false );

            void * indices;
            GN_DX_CHECK_RETURN( idxbuf.ib->Lock( 0, 0, &indices, 0 ), false );

            bool ok = fp->Read( indices, bytes, 0 );

            idxbuf.ib->Unlock();

            if( !ok ) return false;
        }

        // tex
        for( size_t i = 0; i < GN_ARRAY_COUNT(textures); ++i )
        {
            D3D9TextureDump & td = textures[i];

            if( td.ref.Empty() ) continue;

            StrA filename = fs::ToNativeDiskFilePath( td.ref );

            D3DXIMAGE_INFO info;

            GN_DX_CHECK_RETURN( D3DXGetImageInfoFromFileA( filename.ToRawPtr(), &info ), false );

            switch( info.ResourceType )
            {
                case D3DRTYPE_TEXTURE:
                    GN_DX_CHECK_RETURN(
                        D3DXCreateTextureFromFileA( &dev, filename.ToRawPtr(), (LPDIRECT3DTEXTURE9*)&td.tex ),
                        false );
                    break;

                case D3DRTYPE_CUBETEXTURE:
                    GN_DX_CHECK_RETURN(
                        D3DXCreateCubeTextureFromFileA( &dev, filename.ToRawPtr(), (LPDIRECT3DCUBETEXTURE9*)&td.tex ),
                        false );
                    break;

                case D3DRTYPE_VOLUMETEXTURE:
                    GN_DX_CHECK_RETURN(
                        D3DXCreateVolumeTextureFromFileA( &dev, filename.ToRawPtr(), (LPDIRECT3DVOLUMETEXTURE9*)&td.tex ),
                        false );
                    break;

                default:
                    GN_ERROR(sLogger)( "unsupport resource type: %d", info.ResourceType );
                    return false;
            }
        }

        // rt
        for( size_t i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i )
        {
            D3D9RtDump & rtd = rendertargets[i];
            if( !rtd.inuse ) continue;

#if RENDER_TO_BACKBUF
            if( 0 == i )
            {
                GN_DX_CHECK_RETURN(
                    dev.GetRenderTarget( 0, &rtd.surf ),
                    false );
            }
#endif
            else
            {
                GN_DX_CHECK_RETURN(
                    dev.CreateRenderTarget(
                        rtd.width,
                        rtd.height,
                        (D3DFORMAT)rtd.format,
                        (D3DMULTISAMPLE_TYPE)rtd.msaa,
                        rtd.quality,
                        0, // non-lockable
                        &rtd.surf, 0 ),
                    false );
            }
        }

        // ds
        if( depthstencil.inuse )
        {
            GN_DX_CHECK_RETURN(
                dev.CreateDepthStencilSurface(
                    depthstencil.width,
                    depthstencil.height,
                    (D3DFORMAT)depthstencil.format,
                    (D3DMULTISAMPLE_TYPE)depthstencil.msaa,
                    depthstencil.quality,
                    TRUE, // discard
                    &depthstencil.surf, 0 ),
                false );
        }

        // success
        return true;
    }

    void onDispose()
    {
        vs.vs.Clear();
        ps.ps.Clear();
        vtxdecl.decl.Clear();
        for( int i = 0; i < GN_ARRAY_COUNT(vtxbufs); ++i ) vtxbufs[i].vb.Clear();
        idxbuf.ib.Clear();
        for( int i = 0; i < GN_ARRAY_COUNT(textures); ++i ) textures[i].tex.Clear();
        for( int i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i )
        {
            rendertargets[i].surf.Clear();
            rendertargets[i].syscopy.Clear();
        }
        depthstencil.surf.Clear();
    }

    void bind( IDirect3DDevice9 & dev ) const
    {
        PixPerfScopeEvent pixevent( 0, "Bind" );

        // vs
        dev.SetVertexShader( vs.vs );

        // vsc
        dev.SetVertexShaderConstantF( 0, vsconstf[0], GN_ARRAY_COUNT(vsconstf) );
        dev.SetVertexShaderConstantI( 0, vsconsti[0], GN_ARRAY_COUNT(vsconsti) );
        dev.SetVertexShaderConstantB( 0, vsconstb, GN_ARRAY_COUNT(vsconstb) );

        // ps
        dev.SetPixelShader( ps.ps );

        // psc
        dev.SetPixelShaderConstantF( 0, psconstf[0], GN_ARRAY_COUNT(psconstf) );
        dev.SetPixelShaderConstantI( 0, psconsti[0], GN_ARRAY_COUNT(psconsti) );
        dev.SetPixelShaderConstantB( 0, psconstb, GN_ARRAY_COUNT(psconstb) );

        // decl
        dev.SetVertexDeclaration( vtxdecl.decl );

        // vb
        for( UInt32 i = 0; i < GN_ARRAY_COUNT(vtxbufs); ++i )
        {
            const D3D9VtxBufDump & vb = vtxbufs[i];
            if( vb.vb )
            {
                dev.SetStreamSource( i, vb.vb, vb.offset, vb.stride );
            }
            else
            {
                dev.SetStreamSource( i, 0, 0, 0 );
            }
        }

        // ib
        dev.SetIndices( idxbuf.ib );

        // tex
        for( UInt32 i = 0; i < GN_ARRAY_COUNT(textures); ++i )
        {
            dev.SetTexture( i, textures[i].tex );
        }

        // rt
        for( UInt32 i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i )
        {
            dev.SetRenderTarget( i, rendertargets[i].surf );
        }

        // ds
        dev.SetDepthStencilSurface( depthstencil.surf );

        // rs
        for( UInt32 i = 0; i < GN_ARRAY_COUNT(renderstates); ++i )
        {
            const D3D9RsDump & rs = renderstates[i];
            dev.SetRenderState( (D3DRENDERSTATETYPE)rs.type, rs.value );
        }

        // ss
        for( UInt32 s = 0; s < GN_ARRAY_COUNT(samplerstates); ++s )
        for( UInt32 t = 1; t < GN_ARRAY_COUNT(samplerstates[0]); ++t )
        {
            dev.SetSamplerState( s, (D3DSAMPLERSTATETYPE)t, samplerstates[s][t] );
        }

        // viewport
        dev.SetViewport( &viewport );

        // scissor
        dev.SetScissorRect( &scissorrect );
    }

    void draw( IDirect3DDevice9 & dev )
    {
        // load RT data
        {
            PixPerfScopeEvent pixevent( 0, "Load RT data" );
            for( UInt32 i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i )
            {
                D3D9RtDump & rtd = rendertargets[i];
                if( !rtd.inuse ) continue;

                GN_DX_CHECK( D3DXLoadSurfaceFromFileA( rtd.surf, 0, 0, rtd.ref.ToRawPtr(), 0, D3DX_FILTER_NONE, 0, 0 ) );
            }
        }

        operation.draw( dev );
    }

    bool loadFromXml( const XmlNode & root, const StrA & basedir )
    {
        // check root name
        const XmlElement * e = root.ToElement();
        if( 0 == e || "D3D9StateDump" != e->name )
        {
            GN_ERROR(sLogger)( "root node must be \"<D3D9StateDump>\"." );
            return false;
        }

        for( XmlNode * n = root.child; n; n = n->next )
        {
            e = n->ToElement();
            if( !e ) continue;

            if( "vs" == e->name )
            {
                if( !sGetCdata( vs.source, *e ) ) return false;
            }
            else if( "ps" == e->name )
            {
                if( !sGetCdata( ps.source, *e ) ) return false;
            }
            else if( "vsc" == e->name )
            {
                if( !sLoadConst(
                    vsconstf[0], GN_ARRAY_COUNT(vsconstf),
                    vsconsti[0], GN_ARRAY_COUNT(vsconsti),
                    vsconstb   , GN_ARRAY_COUNT(vsconstb),
                    *e ) ) return false;
            }
            else if( "psc" == e->name )
            {
                if( !sLoadConst(
                    psconstf[0], GN_ARRAY_COUNT(psconstf),
                    psconsti[0], GN_ARRAY_COUNT(psconsti),
                    psconstb   , GN_ARRAY_COUNT(psconstb),
                    *e ) ) return false;
            }
            else if( "vtxdecl" == e->name )
            {
                if( !loadVtxDecl( *e ) ) return false;
            }
            else if( "vtxbuf" == e->name )
            {
                if( !loadVtxBuf( *e, basedir ) ) return false;
            }
            else if( "idxbuf" == e->name )
            {
                if( !loadIdxBuf( *e, basedir ) ) return false;
            }
            else if( "texture" == e->name )
            {
                if( !loadTexture( *e, basedir ) ) return false;
            }
            else if( "rendertarget" == e->name )
            {
                if( !loadRenderTarget( *e, basedir ) ) return false;
            }
            else if( "depthstencil" == e->name )
            {
                if( !loadDepthStencil( *e, basedir ) ) return false;
            }
            else if( "renderstates" == e->name )
            {
                if( !loadRenderStates( *e ) ) return false;
            }
            else if( "samplerstates" == e->name )
            {
                if( !loadSamplerStates( *e ) ) return false;
            }
            else if( "viewport" == e->name )
            {
                if( !sGetNumericAttr( *e, "x", (UInt32&)viewport.X ) ) return false;
                if( !sGetNumericAttr( *e, "y", (UInt32&)viewport.Y ) ) return false;
                if( !sGetNumericAttr( *e, "w", (UInt32&)viewport.Width ) ) return false;
                if( !sGetNumericAttr( *e, "h", (UInt32&)viewport.Height ) ) return false;
                if( !sGetNumericAttr( *e, "zmin", viewport.MinZ ) ) return false;
                if( !sGetNumericAttr( *e, "zmax", viewport.MaxZ ) ) return false;
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
                if( !sGetNumericAttr( *e, "prim", operation.prim ) ) return false;
                if( !sGetNumericAttr( *e, "startvtx", operation.startvtx ) ) return false;
                if( !sGetNumericAttr( *e, "minidx", operation.minvtxidx ) ) return false;
                if( !sGetNumericAttr( *e, "numvtx", operation.numvtx ) ) return false;
                if( !sGetNumericAttr( *e, "startidx", operation.startidx ) ) return false;
                if( !sGetNumericAttr( *e, "numprim", operation.numprim ) ) return false;
            }
            else if( "draw" == e->name )
            {
                operation.indexed = false;
                if( !sGetNumericAttr( *e, "prim", operation.prim ) ) return false;
                if( !sGetNumericAttr( *e, "startvtx", operation.startvtx ) ) return false;
                if( !sGetNumericAttr( *e, "numprim", operation.numprim ) ) return false;
            }
            else if( "operation" == e->name )
            {
                operation.indexed = true;
                if( !sGetNumericAttr( *e, "prim", operation.prim ) ) return false;
                if( !sGetNumericAttr( *e, "startvtx", operation.startvtx ) ) return false;
            }
            else
            {
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->GetLocation(), e->name.ToRawPtr() );
            }
        }

        // success
        return true;
    }

    //@}

private:

    static bool sGetCdata( StrA & result, const XmlNode & node )
    {
        for( const XmlNode * n = node.child; n; n = n->next )
        {
            const XmlCdata * c = n->ToCdata();
            if( c )
            {
                result = c->text;
                return true;
            }
        }
        GN_ERROR(sLogger)( "no cdata node found!" );
        return false;
    }

    template<typename T>
    static bool sGetNumericAttr( const XmlElement & node, const StrA & attrname, T & result )
    {
        const XmlAttrib * a = node.FindAttrib( attrname );
        if ( !a || !String2Number<T>( result, a->value.ToRawPtr() ) )
        {
            GN_ERROR(sLogger)("%s : attribute '%s' is missing!", node.GetLocation(), attrname.ToRawPtr() );
            return false;
        }
        else
        {
            return true;
        }
    }

    static bool sGetRefString( const XmlElement & node, const StrA & basedir, StrA & result )
    {
        const XmlAttrib * a = node.FindAttrib( "ref" );
        if ( !a )
        {
            GN_ERROR(sLogger)("%s : attribute 'ref' is missing!", node.GetLocation() );
            return false;
        }

        result = fs::ResolvePath( basedir, a->value );

        if( !fs::IsFile( result ) )
        {
            GN_WARN(sLogger)("%s : invalid reference :  %s!", node.GetLocation(), result.ToRawPtr() );
        }

        // success
        return true;
    }

    static bool sLoadConstF( float * result, size_t count, const XmlElement & node )
    {
        for( XmlNode * n = node.child; n; n = n->next )
        {
            const XmlElement * e = n->ToElement();
            if( !e ) continue;
            if( e->name != "f" )
            {
                GN_ERROR(sLogger)( "%s : ignore unknown node %s", e->GetLocation(), e->name.ToRawPtr() );
                continue;
            }

            size_t index;
            if( !sGetNumericAttr( *e, "index", index ) ) return false;
            if( index >= count )
            {
                GN_ERROR(sLogger)( "%s : invalid const index.", e->GetLocation() );
                continue;
            }

            float * c = result + index * 4;
            if( !sGetNumericAttr( *e, "x", c[0] ) ) return false;
            if( !sGetNumericAttr( *e, "y", c[1] ) ) return false;
            if( !sGetNumericAttr( *e, "z", c[2] ) ) return false;
            if( !sGetNumericAttr( *e, "w", c[3] ) ) return false;
        }

        // success
        return true;
    }

    static bool sLoadConstI( int * result, size_t dim, size_t count, const XmlElement & node )
    {
        for( XmlNode * n = node.child; n; n = n->next )
        {
            const XmlElement * e = n->ToElement();
            if( !e ) continue;

            size_t index;
            if( !sGetNumericAttr( *e, "index", index ) ) return false;
            if( index >= count )
            {
                GN_ERROR(sLogger)( "%s : invalid const index.", e->GetLocation() );
                continue;
            }

            int * c = result + index * dim;
            const char * names[] = { "x", "y", "z", "w" };
             for( size_t i = 0; i < dim; ++i  )
            {
                 if( !sGetNumericAttr( *e, names[i], c[i] ) ) return false;
            }
        }

        // success
        return true;
    }

    static bool sLoadConst(
            float * floats, size_t countf,
            int   * ints,   size_t counti,
            int   * bools,  size_t countb,
            const XmlElement & node )
    {
        XmlElement * e = node.FindChildElement( "float" );
        if( !e )
        {
            GN_ERROR(sLogger)( "%s : vs float constants are missing!", node.GetLocation() );
            return false;
        }
        if( !sLoadConstF( floats, countf, *e ) ) return false;

        e = node.FindChildElement( "int" );
        if( !e )
        {
            GN_ERROR(sLogger)( "%s : vs integer constants are missing!", node.GetLocation() );
            return false;
        }
        if( !sLoadConstI( ints, 4, counti, *e ) ) return false;

        e = node.FindChildElement( "bool" );
        if( !e )
        {
            GN_ERROR(sLogger)( "%s : vs boolean constants are missing!", node.GetLocation() );
            return false;
        }
        if( !sLoadConstI( bools, 1, countb, *e ) ) return false;

        // success
        return true;
    }

    bool loadVtxDecl( const XmlElement & node )
    {
        size_t count = 0;
        for( XmlNode * n = node.child; n; n = n->next )
        {
            XmlElement * e = n->ToElement();

            if( !e ) continue;

            if( e->name != "element" )
            {
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->GetLocation(), e->name.ToRawPtr() );
                continue;
            }

            if( count >= GN_ARRAY_COUNT(vtxdecl.elements) )
            {
                GN_ERROR(sLogger)( "%s : too many elements!", e->GetLocation() );
                return false;
            }

            D3DVERTEXELEMENT9 & ve = vtxdecl.elements[count];

            if( !sGetNumericAttr( *e, "stream", ve.Stream     ) ) return false;
            if( !sGetNumericAttr( *e, "offset", ve.Offset     ) ) return false;
            if( !sGetNumericAttr( *e, "type"  , ve.Type       ) ) return false;
            if( !sGetNumericAttr( *e, "method", ve.Method     ) ) return false;
            if( !sGetNumericAttr( *e, "usage" , ve.Usage      ) ) return false;
            if( !sGetNumericAttr( *e, "index" , ve.UsageIndex ) ) return false;

            ++count;
        }

        // success
        return true;
    }

    bool loadVtxBuf( const XmlElement & node, const StrA & basedir )
    {
        size_t stream;
        if( !sGetNumericAttr( node, "stream", stream ) ) return false;
        if( stream >= GN_ARRAY_COUNT(vtxbufs) )
        {
            GN_ERROR(sLogger)( "invalid stream index." );
            return false;
        }

        D3D9VtxBufDump & vb = vtxbufs[stream];

        if( !sGetNumericAttr( node, "offset", vb.offset ) ) return false;
        if( !sGetNumericAttr( node, "stride", vb.stride ) ) return false;
        if( !sGetRefString( node, basedir, vb.ref ) ) return false;

        return true;
    }

    bool loadIdxBuf( const XmlElement & node, const StrA & basedir )
    {
        if( !sGetNumericAttr( node, "format", idxbuf.format ) ) return false;
        if( !sGetNumericAttr( node, "startvtx", idxbuf.startvtx ) ) return false;
        if( !sGetRefString( node, basedir, idxbuf.ref ) ) return false;
        return true;
    }

    bool loadTexture( const XmlElement & node, const StrA & basedir )
    {
        size_t stage;
        if( !sGetNumericAttr( node, "stage", stage ) ) return false;
        if( stage >= GN_ARRAY_COUNT(textures) )
        {
            GN_ERROR(sLogger)( "invalid stage." );
            return false;
        }

        D3D9TextureDump & t = textures[stage];

        if( !sGetRefString( node, basedir, t.ref ) ) return false;

        return true;
    }

    bool loadRenderTarget( const XmlElement & node, const StrA & basedir )
    {
        size_t stage;
        if( !sGetNumericAttr( node, "stage", stage ) ) return false;
        if( stage >= GN_ARRAY_COUNT(rendertargets) )
        {
            GN_ERROR(sLogger)( "invalid stage." );
            return false;
        }

        D3D9RtDump & rt = rendertargets[stage];

        if( !sGetNumericAttr( node, "width", rt.width ) ) return false;
        if( !sGetNumericAttr( node, "height", rt.height ) ) return false;
        if( !sGetNumericAttr( node, "format", rt.format ) ) return false;
        if( !sGetNumericAttr( node, "msaa", rt.msaa ) ) return false;
        if( !sGetNumericAttr( node, "quality", rt.quality ) ) return false;
        if( !sGetRefString( node, basedir, rt.ref ) ) return false;

        rt.inuse = true;
        return true;
    }

    bool loadDepthStencil( const XmlElement & node, const StrA & basedir )
    {
        if( !sGetNumericAttr( node, "width", depthstencil.width ) ) return false;
        if( !sGetNumericAttr( node, "height", depthstencil.height ) ) return false;
        if( !sGetNumericAttr( node, "format", depthstencil.format ) ) return false;
        if( !sGetNumericAttr( node, "msaa", depthstencil.msaa ) ) return false;
        if( !sGetNumericAttr( node, "quality", depthstencil.quality ) ) return false;
        if( !sGetRefString( node, basedir, depthstencil.ref ) ) return false;

        depthstencil.inuse = true;
        return true;
    }

    bool loadRenderStates( const XmlElement & node )
    {
        size_t count = 0;
        for( XmlNode * n = node.child; n; n = n->next )
        {
            XmlElement * e = n->ToElement();
            if( !e ) continue;

            if( "rs" != e->name )
            {
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->GetLocation(), e->name.ToRawPtr() );
                continue;
            }

            if( count >= GN_ARRAY_COUNT(renderstates) )
            {
                GN_WARN(sLogger)( "%s : too many render states", e->GetLocation() );
                return true;
            }

            D3D9RsDump & rs = renderstates[count];

            if( !sGetNumericAttr( *e, "type", rs.type ) ) return false;
            if( !sGetNumericAttr( *e, "value", rs.value ) ) return false;

            ++count;
        }

        if( count != GN_ARRAY_COUNT(renderstates) )
        {
            GN_WARN(sLogger)( "%s : too little render states", node.GetLocation() );
        }

        // success
        return true;
    }

    bool loadSamplerStates( const XmlElement & node )
    {
        for( XmlNode * n = node.child; n; n = n->next )
        {
            XmlElement * e = n->ToElement();
            if( !e ) continue;

            if( "ss" != e->name )
            {
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->GetLocation(), e->name.ToRawPtr() );
                continue;
            }

            UInt32 stage, type;
            if( !sGetNumericAttr( *e, "stage", stage ) ) return false;
            if( !sGetNumericAttr( *e, "type", type ) ) return false;

            if( stage > 15 || type > 13 )
            {
                GN_ERROR(sLogger)( "%s : invalid SS stage and/or type", e->GetLocation() );
                return false;
            }

            if( !sGetNumericAttr( *e, "value", samplerstates[stage][type] ) ) return false;
        }

        // success
        return true;
    }
};

struct D3D9StateResource
{
    //@}
};

class MyApp : public D3D9Application
{
    D3D9StateDump mState;
    AutoComPtr<IDirect3DSurface9> mBackBuf;

protected:

    bool onInit( D3D9AppOption & o )
    {
        mState.Clear();
        if( !LoadFromXmlFile( mState, sDumpFileName ) ) return false;

#if RENDER_TO_BACKBUF
        o.windowedWidth  = mState.rendertargets[0].width;
        o.windowedHeight = mState.rendertargets[0].height;
#else
        o.windowedWidth  = mState.viewport.Width;
        o.windowedHeight = mState.viewport.Height;
#endif

        // success
        return true;
    }

    bool onRestore()
    {
        if( !mState.onRestore( d3d9dev() ) ) return false;
        d3d9dev().GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &mBackBuf );
        return true;
    }

    void onDispose()
    {
        mState.onDispose();
        mBackBuf.Clear();
    }

    void onDraw()
    {
        IDirect3DDevice9 & dev = d3d9dev();

        // clear back buffer
        dev.SetRenderTarget( 0, mBackBuf );
        dev.SetRenderTarget( 1, 0 );
        dev.SetRenderTarget( 2, 0 );
        dev.SetRenderTarget( 3, 0 );
        dev.SetDepthStencilSurface( 0 );
        dev.Clear( 0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0 );

        if( D3D_OK == dev.BeginScene() )
        {
            mState.bind( dev );
            mState.draw( dev );

#if !RENDER_TO_BACKBUF
            // copy RT0 content to backbuffer
            RECT srcrc = {
                (int)mState.viewport.X,
                (int)mState.viewport.Y,
                (int)( mState.viewport.X + mState.viewport.Width ),
                (int)( mState.viewport.Y + mState.viewport.Height ),
            };
            RECT dstrc = {
                0,
                0,
                (int)mState.viewport.Width,
                (int)mState.viewport.Height,
            };
            dev.StretchRect( mState.rendertargets[0].surf, &srcrc, mBackBuf, &dstrc, D3DTEXF_NONE );
#endif

            dev.EndScene();
        }

        dev.Present( 0, 0, 0, 0 );
    }
};

void printhelp( const char * appname )
{
    printf( "Usage: %s [dumpname]\n", (fs::BaseName(appname) + fs::ExtName(appname)).ToRawPtr() );
}

int main( int argc, const char * argv [] )
{
    GN_GUARD_ALWAYS;

    // parse command line
    if( argc < 2 )
    {
        printhelp( argv[0] );
        return -1;
    }
    sDumpFileName = argv[1];

    MyApp app;
    return app.run();

    GN_UNGUARD_ALWAYS_NO_THROW;
    return -1;
}
