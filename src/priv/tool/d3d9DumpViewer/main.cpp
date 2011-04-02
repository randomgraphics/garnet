#include "pch.h"

using namespace GN;
using namespace GN::d3d9;

///
/// define to non-zero to render directly into back buffer;
/// else, render to offscreen render target, then copy to back buffer.
///
#define RENDER_TO_BACKBUF 0

static GN::Logger * sLogger = GN::getLogger("GN.tool.D3D9DumpViewer");

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
    uint32 offset;
    uint32 stride;
    StrA   ref;
    AutoComPtr<IDirect3DVertexBuffer9> vb;
};

struct D3D9IdxBufDump
{
    uint32 format;
    uint32 startvtx;
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
    uint32 width;
    uint32 height;
    uint32 format;
    uint32 msaa;
    uint32 quality;
    StrA   ref;
    AutoComPtr<IDirect3DSurface9> surf;
    AutoComPtr<IDirect3DSurface9> syscopy;
};

struct D3D9RsDump
{
    uint32 type;
    uint32 value;
};

struct D3D9OperationDump
{
    bool    indexed;

    uint32  prim;
    uint32  numprim;
    uint32  startvtx;

    // for indexed draw only
    uint32  minvtxidx;
    uint32  numvtx;
    uint32  startidx;

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

    uint32            samplerstates[16][14]; // samplerstates[x][0] is unused.

    D3DVIEWPORT9      viewport;

    RECT              scissorrect;

    D3D9OperationDump operation;

    //@}


    //@{

    void clear()
    {
        onDispose();

        vs.source.clear();
        memset( &vsconstf, 0, sizeof(vsconstf) );
        memset( &vsconsti, 0, sizeof(vsconsti) );
        memset( &vsconstb, 0, sizeof(vsconstb) );

        ps.source.clear();
        memset( &psconstf, 0, sizeof(psconstf) );
        memset( &psconsti, 0, sizeof(psconsti) );
        memset( &psconstb, 0, sizeof(psconstb) );

        for( int i = 0; i < GN_ARRAY_COUNT(vtxbufs); ++i ) vtxbufs[i].ref.clear();

        idxbuf.ref.clear();

        for( int i = 0; i < GN_ARRAY_COUNT(textures); ++i ) textures[i].ref.clear();

        for( int i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i ) rendertargets[i].inuse = false;

        depthstencil.inuse = false;
    }

    bool onRestore( IDirect3DDevice9 & dev )
    {
        // vs
        if( !vs.source.empty() )
        {
            vs.vs.attach( assembleAndCreateVS( &dev, vs.source.rawptr(), 0 ) );
            if( vs.vs.empty() ) return false;
        }

        // ps
        if( !ps.source.empty() )
        {
            ps.ps.attach( assembleAndCreatePS( &dev, ps.source.rawptr(), 0 ) );
            if( ps.ps.empty() ) return false;
        }

        // decl
        GN_DX_CHECK_RETURN( dev.CreateVertexDeclaration( vtxdecl.elements, &vtxdecl.decl ), false );

        // vb
        for( size_t i = 0; i < GN_ARRAY_COUNT(vtxbufs); ++i )
        {
            D3D9VtxBufDump & vbd = vtxbufs[i];

            if( vbd.ref.empty() ) continue;

            AutoObjPtr<File> fp( fs::openFile( vbd.ref, "rb" ) );
            if( !fp ) return false;

            size_t bytes = fp->size();

            GN_DX_CHECK_RETURN( dev.CreateVertexBuffer( (UINT32)bytes, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vtxbufs[i].vb, 0 ), false );

            void * vertices;
            GN_DX_CHECK_RETURN( vbd.vb->Lock( 0, 0, &vertices, 0 ), false );

            bool ok = fp->read( vertices, bytes, 0 );

            vbd.vb->Unlock();

            if( !ok ) return false;
        }

        // ib
        if( !idxbuf.ref.empty() )
        {
            AutoObjPtr<File> fp( fs::openFile( idxbuf.ref, "rb" ) );
            if( !fp ) return false;

            size_t bytes = fp->size();

            GN_DX_CHECK_RETURN( dev.CreateIndexBuffer( (UINT32)bytes, D3DUSAGE_WRITEONLY, (D3DFORMAT)idxbuf.format, D3DPOOL_DEFAULT, &idxbuf.ib, 0 ), false );

            void * indices;
            GN_DX_CHECK_RETURN( idxbuf.ib->Lock( 0, 0, &indices, 0 ), false );

            bool ok = fp->read( indices, bytes, 0 );

            idxbuf.ib->Unlock();

            if( !ok ) return false;
        }

        // tex
        for( size_t i = 0; i < GN_ARRAY_COUNT(textures); ++i )
        {
            D3D9TextureDump & td = textures[i];

            if( td.ref.empty() ) continue;

            StrA filename = fs::toNativeDiskFilePath( td.ref );

            D3DXIMAGE_INFO info;

            GN_DX_CHECK_RETURN( D3DXGetImageInfoFromFileA( filename.rawptr(), &info ), false );

            switch( info.ResourceType )
            {
                case D3DRTYPE_TEXTURE:
                    GN_DX_CHECK_RETURN(
                        D3DXCreateTextureFromFileA( &dev, filename.rawptr(), (LPDIRECT3DTEXTURE9*)&td.tex ),
                        false );
                    break;

                case D3DRTYPE_CUBETEXTURE:
                    GN_DX_CHECK_RETURN(
                        D3DXCreateCubeTextureFromFileA( &dev, filename.rawptr(), (LPDIRECT3DCUBETEXTURE9*)&td.tex ),
                        false );
                    break;

                case D3DRTYPE_VOLUMETEXTURE:
                    GN_DX_CHECK_RETURN(
                        D3DXCreateVolumeTextureFromFileA( &dev, filename.rawptr(), (LPDIRECT3DVOLUMETEXTURE9*)&td.tex ),
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
        vs.vs.clear();
        ps.ps.clear();
        vtxdecl.decl.clear();
        for( int i = 0; i < GN_ARRAY_COUNT(vtxbufs); ++i ) vtxbufs[i].vb.clear();
        idxbuf.ib.clear();
        for( int i = 0; i < GN_ARRAY_COUNT(textures); ++i ) textures[i].tex.clear();
        for( int i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i )
        {
            rendertargets[i].surf.clear();
            rendertargets[i].syscopy.clear();
        }
        depthstencil.surf.clear();
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
        for( uint32 i = 0; i < GN_ARRAY_COUNT(vtxbufs); ++i )
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
        for( uint32 i = 0; i < GN_ARRAY_COUNT(textures); ++i )
        {
            dev.SetTexture( i, textures[i].tex );
        }

        // rt
        for( uint32 i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i )
        {
            dev.SetRenderTarget( i, rendertargets[i].surf );
        }

        // ds
        dev.SetDepthStencilSurface( depthstencil.surf );

        // rs
        for( uint32 i = 0; i < GN_ARRAY_COUNT(renderstates); ++i )
        {
            const D3D9RsDump & rs = renderstates[i];
            dev.SetRenderState( (D3DRENDERSTATETYPE)rs.type, rs.value );
        }

        // ss
        for( uint32 s = 0; s < GN_ARRAY_COUNT(samplerstates); ++s )
        for( uint32 t = 1; t < GN_ARRAY_COUNT(samplerstates[0]); ++t )
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
            for( uint32 i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i )
            {
                D3D9RtDump & rtd = rendertargets[i];
                if( !rtd.inuse ) continue;

                GN_DX_CHECK( D3DXLoadSurfaceFromFileA( rtd.surf, 0, 0, rtd.ref.rawptr(), 0, D3DX_FILTER_NONE, 0, 0 ) );
            }
        }

        operation.draw( dev );
    }

    bool loadFromXml( const XmlNode & root, const StrA & basedir )
    {
        // check root name
        const XmlElement * e = root.toElement();
        if( 0 == e || "D3D9StateDump" != e->name )
        {
            GN_ERROR(sLogger)( "root node must be \"<D3D9StateDump>\"." );
            return false;
        }

        for( XmlNode * n = root.firstc; n; n = n->nexts )
        {
            e = n->toElement();
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
                if( !sGetNumericAttr( *e, "x", (uint32&)viewport.X ) ) return false;
                if( !sGetNumericAttr( *e, "y", (uint32&)viewport.Y ) ) return false;
                if( !sGetNumericAttr( *e, "w", (uint32&)viewport.Width ) ) return false;
                if( !sGetNumericAttr( *e, "h", (uint32&)viewport.Height ) ) return false;
                if( !sGetNumericAttr( *e, "zmin", viewport.MinZ ) ) return false;
                if( !sGetNumericAttr( *e, "zmax", viewport.MaxZ ) ) return false;
            }
            else if( "scissor" == e->name )
            {
                if( !sGetNumericAttr( *e, "l", (sint32&)scissorrect.left ) ) return false;
                if( !sGetNumericAttr( *e, "t", (sint32&)scissorrect.top ) ) return false;
                if( !sGetNumericAttr( *e, "r", (sint32&)scissorrect.right ) ) return false;
                if( !sGetNumericAttr( *e, "b", (sint32&)scissorrect.bottom ) ) return false;
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
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->getLocation(), e->name.rawptr() );
            }
        }

        // success
        return true;
    }

    //@}

private:

    static bool sGetCdata( StrA & result, const XmlNode & node )
    {
        for( const XmlNode * n = node.firstc; n; n = n->nexts )
        {
            const XmlCdata * c = n->toCdata();
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
        const XmlAttrib * a = node.findAttrib( attrname );
        if ( !a || !string2Number<T>( result, a->value.rawptr() ) )
        {
            GN_ERROR(sLogger)("%s : attribute '%s' is missing!", node.getLocation(), attrname.rawptr() );
            return false;
        }
        else
        {
            return true;
        }
    }

    static bool sGetRefString( const XmlElement & node, const StrA & basedir, StrA & result )
    {
        const XmlAttrib * a = node.findAttrib( "ref" );
        if ( !a )
        {
            GN_ERROR(sLogger)("%s : attribute 'ref' is missing!", node.getLocation() );
            return false;
        }

        result = fs::resolvePath( basedir, a->value );

        if( !fs::isFile( result ) )
        {
            GN_WARN(sLogger)("%s : invalid reference :  %s!", node.getLocation(), result.rawptr() );
        }

        // success
        return true;
    }

    static bool sLoadConstF( float * result, size_t count, const XmlElement & node )
    {
        for( XmlNode * n = node.firstc; n; n = n->nexts )
        {
            const XmlElement * e = n->toElement();
            if( !e ) continue;
            if( e->name != "f" )
            {
                GN_ERROR(sLogger)( "%s : ignore unknown node %s", e->getLocation(), e->name.rawptr() );
                continue;
            }

            size_t index;
            if( !sGetNumericAttr( *e, "index", index ) ) return false;
            if( index >= count )
            {
                GN_ERROR(sLogger)( "%s : invalid const index.", e->getLocation() );
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
        for( XmlNode * n = node.firstc; n; n = n->nexts )
        {
            const XmlElement * e = n->toElement();
            if( !e ) continue;

            size_t index;
            if( !sGetNumericAttr( *e, "index", index ) ) return false;
            if( index >= count )
            {
                GN_ERROR(sLogger)( "%s : invalid const index.", e->getLocation() );
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
        XmlElement * e = node.findChildElement( "float" );
        if( !e )
        {
            GN_ERROR(sLogger)( "%s : vs float constants are missing!", node.getLocation() );
            return false;
        }
        if( !sLoadConstF( floats, countf, *e ) ) return false;

        e = node.findChildElement( "int" );
        if( !e )
        {
            GN_ERROR(sLogger)( "%s : vs integer constants are missing!", node.getLocation() );
            return false;
        }
        if( !sLoadConstI( ints, 4, counti, *e ) ) return false;

        e = node.findChildElement( "bool" );
        if( !e )
        {
            GN_ERROR(sLogger)( "%s : vs boolean constants are missing!", node.getLocation() );
            return false;
        }
        if( !sLoadConstI( bools, 1, countb, *e ) ) return false;

        // success
        return true;
    }

    bool loadVtxDecl( const XmlElement & node )
    {
        size_t count = 0;
        for( XmlNode * n = node.firstc; n; n = n->nexts )
        {
            XmlElement * e = n->toElement();

            if( !e ) continue;

            if( e->name != "element" )
            {
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->getLocation(), e->name.rawptr() );
                continue;
            }

            if( count >= GN_ARRAY_COUNT(vtxdecl.elements) )
            {
                GN_ERROR(sLogger)( "%s : too many elements!", e->getLocation() );
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
        for( XmlNode * n = node.firstc; n; n = n->nexts )
        {
            XmlElement * e = n->toElement();
            if( !e ) continue;

            if( "rs" != e->name )
            {
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->getLocation(), e->name.rawptr() );
                continue;
            }

            if( count >= GN_ARRAY_COUNT(renderstates) )
            {
                GN_WARN(sLogger)( "%s : too many render states", e->getLocation() );
                return true;
            }

            D3D9RsDump & rs = renderstates[count];

            if( !sGetNumericAttr( *e, "type", rs.type ) ) return false;
            if( !sGetNumericAttr( *e, "value", rs.value ) ) return false;

            ++count;
        }

        if( count != GN_ARRAY_COUNT(renderstates) )
        {
            GN_WARN(sLogger)( "%s : too little render states", node.getLocation() );
        }

        // success
        return true;
    }

    bool loadSamplerStates( const XmlElement & node )
    {
        for( XmlNode * n = node.firstc; n; n = n->nexts )
        {
            XmlElement * e = n->toElement();
            if( !e ) continue;

            if( "ss" != e->name )
            {
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->getLocation(), e->name.rawptr() );
                continue;
            }

            uint32 stage, type;
            if( !sGetNumericAttr( *e, "stage", stage ) ) return false;
            if( !sGetNumericAttr( *e, "type", type ) ) return false;

            if( stage > 15 || type > 13 )
            {
                GN_ERROR(sLogger)( "%s : invalid SS stage and/or type", e->getLocation() );
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
        mState.clear();
        if( !loadFromXmlFile( mState, sDumpFileName ) ) return false;

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
        mBackBuf.clear();
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
    printf( "Usage: %s [dumpname]\n", (fs::baseName(appname) + fs::extName(appname)).rawptr() );
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
