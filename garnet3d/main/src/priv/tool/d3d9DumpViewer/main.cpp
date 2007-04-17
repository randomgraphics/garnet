#include "pch.h"
#include "d3d9app.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.tool.D3D9DumpViewer");

static StrA sDumpFileName;

struct D3D9VsDump
{
    StrA                               code;
    AutoComPtr<IDirect3DVertexShader9> vs;
};

struct D3D9PsDump
{
    StrA                              code;
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
    UInt32 basevtx;
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
    AutoComPtr<IDirect3DSurface9> surf;
};

struct D3D9RsDump
{
    UInt32 type;
    UInt32 value;
};

struct D3D9StateDump
{
    //@{

    D3D9VsDump        vs;
    Vector4f          vsconstf[256];
    Vector4i          vsconsti[16];
    int               vsconstb[16];

    D3D9PsDump        ps;
    Vector4f          psconstf[224];
    Vector4i          psconsti[16];
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

    //@}


    //@{

    void clear()
    {
        onDispose();

        vs.code.clear();
        memset( &vsconstf, 0, sizeof(vsconstf) );
        memset( &vsconsti, 0, sizeof(vsconsti) );
        memset( &vsconstb, 0, sizeof(vsconstb) );

        ps.code.clear();
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
        if( !vs.code.empty() )
        {
            vs.vs.attach( d3d9::assembleVS( &dev, vs.code.cptr(), 0 ) );
            if( vs.vs.empty() ) return false;
        }

        // ps
        if( !ps.code.empty() )
        {
            ps.ps.attach( d3d9::assemblePS( &dev, ps.code.cptr(), 0 ) );
            if( ps.ps.empty() ) return false;
        }

        // decl
        GN_DX9_CHECK_RV( dev.CreateVertexDeclaration( vtxdecl.elements, &vtxdecl.decl ), false );

        // vb
        for( size_t i = 0; i < GN_ARRAY_COUNT(vtxbufs); ++i )
        {
            D3D9VtxBufDump & vbd = vtxbufs[i];

            if( vbd.ref.empty() ) continue;

            AutoObjPtr<File> fp( core::openFile( vbd.ref, "rb" ) );
            if( !fp ) return false;

            size_t bytes = fp->size();

            GN_DX9_CHECK_RV( dev.CreateVertexBuffer( bytes, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vtxbufs[i].vb, 0 ), false );

            void * vertices;
            GN_DX9_CHECK_RV( vbd.vb->Lock( 0, 0, &vertices, 0 ), false );

            bool ok = fp->read( vertices, bytes, 0 );

            vbd.vb->Unlock();

            if( !ok ) return false;
        }

        // ib
        if( !idxbuf.ref.empty() )
        {
            AutoObjPtr<File> fp( core::openFile( idxbuf.ref, "rb" ) );
            if( !fp ) return false;

            size_t bytes = fp->size();

            GN_DX9_CHECK_RV( dev.CreateIndexBuffer( bytes, D3DUSAGE_WRITEONLY, (D3DFORMAT)idxbuf.format, D3DPOOL_DEFAULT, &idxbuf.ib, 0 ), false );

            void * indices;
            GN_DX9_CHECK_RV( idxbuf.ib->Lock( 0, 0, &indices, 0 ), false );

            bool ok = fp->read( indices, bytes, 0 );

            idxbuf.ib->Unlock();

            if( !ok ) return false;
        }

        // tex
        for( size_t i = 0; i < GN_ARRAY_COUNT(textures); ++i )
        {
            D3D9TextureDump & td = textures[i];

            if( td.ref.empty() ) continue;

            StrA filename = core::toNative( td.ref );

            D3DXIMAGE_INFO info;

            GN_DX9_CHECK_RV( D3DXGetImageInfoFromFileA( filename.cptr(), &info ), false );

            switch( info.ResourceType )
            {
                case D3DRTYPE_TEXTURE:
                    GN_DX9_CHECK_RV(
                        D3DXCreateTextureFromFileA( &dev, filename.cptr(), (LPDIRECT3DTEXTURE9*)&td.tex ),
                        false );
                    break;

                case D3DRTYPE_CUBETEXTURE:
                    GN_DX9_CHECK_RV(
                        D3DXCreateCubeTextureFromFileA( &dev, filename.cptr(), (LPDIRECT3DCUBETEXTURE9*)&td.tex ),
                        false );
                    break;

                case D3DRTYPE_VOLUMETEXTURE:
                    GN_DX9_CHECK_RV(
                        D3DXCreateVolumeTextureFromFileA( &dev, filename.cptr(), (LPDIRECT3DVOLUMETEXTURE9*)&td.tex ),
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

            GN_DX9_CHECK_RV(
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

        // ds
        if( depthstencil.inuse )
        {
            GN_DX9_CHECK_RV(
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
        for( int i = 0; i < GN_ARRAY_COUNT(rendertargets); ++i ) rendertargets[i].surf.clear();
        depthstencil.surf.clear();
    }

    void bind( IDirect3DDevice9 & dev ) const
    {
        // vs
        dev.SetVertexShader( vs.vs );

        // vsc

        // ps
        dev.SetPixelShader( ps.ps );

        // psc

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

    bool loadFromXmlNode( const XmlNode & root, const StrA & /*basedir*/ )
    {
        // check root name
        const XmlElement * e = root.toElement();
        if( 0 == e || "D3D9StateDump" != e->name )
        {
            GN_ERROR(sLogger)( "root node must be \"<D3D9StateDump>\"." );
            return false;
        }

        for( XmlNode * n = root.child; n; n = n->sibling )
        {
            e = n->toElement();
            if( !e ) continue;

            if( "vs" == e->name )
            {
                if( !sGetCdata( vs.code, *e ) ) return false;
            }
            else if( "ps" == e->name )
            {
                if( !sGetCdata( ps.code, *e ) ) return false;
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
                if( !loadVtxBuf( *e ) ) return false;
            }
            else if( "idxbuf" == e->name )
            {
                if( !loadIdxBuf( *e ) ) return false;
            }
            else if( "texture" == e->name )
            {
                if( !loadTexture( *e ) ) return false;
            }
            else if( "rendertarget" == e->name )
            {
                if( !loadRenderTarget( *e ) ) return false;
            }
            else if( "depthstencil" == e->name )
            {
                if( !loadDepthStencil( *e ) ) return false;
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
                if( !sGetNumbericAttr( *e, "x", (UInt32&)viewport.X ) ) return false;
                if( !sGetNumbericAttr( *e, "y", (UInt32&)viewport.Y ) ) return false;
                if( !sGetNumbericAttr( *e, "w", (UInt32&)viewport.Width ) ) return false;
                if( !sGetNumbericAttr( *e, "h", (UInt32&)viewport.Height ) ) return false;
                if( !sGetNumbericAttr( *e, "zmin", viewport.MinZ ) ) return false;
                if( !sGetNumbericAttr( *e, "zmax", viewport.MaxZ ) ) return false;
            }
            else if( "scissor" == e->name )
            {
                if( !sGetNumbericAttr( *e, "l", (SInt32&)scissorrect.left ) ) return false;
                if( !sGetNumbericAttr( *e, "t", (SInt32&)scissorrect.top ) ) return false;
                if( !sGetNumbericAttr( *e, "r", (SInt32&)scissorrect.right ) ) return false;
                if( !sGetNumbericAttr( *e, "b", (SInt32&)scissorrect.bottom ) ) return false;
            }
            else
            {
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->getLocation(), e->name.cptr() );
            }
        }

        // success
        return true;
    }

    //@}

private:

    static bool sGetCdata( StrA & result, const XmlNode & node )
    {
        for( const XmlNode * n = node.child; n; n = n->sibling )
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
    static bool sGetNumbericAttr( const XmlElement & node, const StrA & attrname, T & result )
    {
        const XmlAttrib * a = node.findAttrib( attrname );
        if ( !a || !str2Number<T>( result, a->value.cptr() ) )
        {
            GN_ERROR(sLogger)("%s : attribute '%s' is missing!", node.getLocation(), attrname.cptr() );
            return false;
        }
        else
        {
            return true;
        }
    }

    static bool sGetRefString( const XmlElement & node, StrA & result )
    {
        const XmlAttrib * a = node.findAttrib( "ref" );
        if ( !a )
        {
            GN_ERROR(sLogger)("%s : attribute 'ref' is missing!", node.getLocation() );
            return false;
        }

        if( !core::isFile( a->value ) )
        {
            GN_WARN(sLogger)("%s : invalid reference :  %s!", node.getLocation(), a->value.cptr() );
        }

        // success
        result = a->value;
        return true;
    }

    static bool sLoadConstF( float * result, size_t count, const XmlElement & node )
    {
        for( XmlNode * n = node.child; n; n = n->sibling )
        {
            const XmlElement * e = n->toElement();
            if( !e ) continue;
            if( e->name != "f" )
            {
                GN_ERROR(sLogger)( "%s : ignore unknown node %s", e->getLocation(), e->name.cptr() );
                continue;
            }

            size_t index;
            if( !sGetNumbericAttr( *e, "index", index ) ) return false;
            if( index >= count )
            {
                GN_ERROR(sLogger)( "%s : invalid const index.", e->getLocation() );
                continue;
            }

            float * c = result + index * 4;
            if( !sGetNumbericAttr( *e, "x", c[0] ) ) return false;
            if( !sGetNumbericAttr( *e, "y", c[1] ) ) return false;
            if( !sGetNumbericAttr( *e, "z", c[2] ) ) return false;
            if( !sGetNumbericAttr( *e, "w", c[3] ) ) return false;
        }

        // success
        return true;
    }

    static bool sLoadConstI( int * result, size_t dim, size_t count, const XmlElement & node )
    {
        for( XmlNode * n = node.child; n; n = n->sibling )
        {
            const XmlElement * e = n->toElement();
            if( !e ) continue;

            size_t index;
            if( !sGetNumbericAttr( *e, "index", index ) ) return false;
            if( index >= count )
            {
                GN_ERROR(sLogger)( "%s : invalid const index.", e->getLocation() );
                continue;
            }

            int * c = result + index * dim;
            const char * names[] = { "x", "y", "z", "w" };
             for( size_t i = 0; i < dim; ++i  )
            {
                 if( !sGetNumbericAttr( *e, names[i], c[i] ) ) return false;
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
        for( XmlNode * n = node.child; n; n = n->sibling )
        {
            XmlElement * e = n->toElement();

            if( !e ) continue;

            if( e->name != "element" )
            {
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->getLocation(), e->name.cptr() );
                continue;
            }

            if( count >= GN_ARRAY_COUNT(vtxdecl.elements) )
            {
                GN_ERROR(sLogger)( "%s : too many elements!", e->getLocation() );
                return false;
            }

            D3DVERTEXELEMENT9 & ve = vtxdecl.elements[count];

            if( !sGetNumbericAttr( *e, "stream", ve.Stream     ) ) return false;
            if( !sGetNumbericAttr( *e, "offset", ve.Offset     ) ) return false;
            if( !sGetNumbericAttr( *e, "type"  , ve.Type       ) ) return false;
            if( !sGetNumbericAttr( *e, "method", ve.Method     ) ) return false;
            if( !sGetNumbericAttr( *e, "usage" , ve.Usage      ) ) return false;
            if( !sGetNumbericAttr( *e, "index" , ve.UsageIndex ) ) return false;

            ++count;
        }

        // success
        return true;
    }

    bool loadVtxBuf( const XmlElement & node )
    {
        size_t stream;
        if( !sGetNumbericAttr( node, "stream", stream ) ) return false;
        if( stream >= GN_ARRAY_COUNT(vtxbufs) )
        {
            GN_ERROR(sLogger)( "invalid stream index." );
            return false;
        }

        D3D9VtxBufDump & vb = vtxbufs[stream];

        if( !sGetNumbericAttr( node, "offset", vb.offset ) ) return false;
        if( !sGetNumbericAttr( node, "stride", vb.stride ) ) return false;
        if( !sGetRefString( node, vb.ref ) ) return false;

        return true;
    }

    bool loadIdxBuf( const XmlElement & node )
    {
        if( !sGetNumbericAttr( node, "format", idxbuf.format ) ) return false;
        if( !sGetNumbericAttr( node, "basevtx", idxbuf.basevtx ) ) return false;
        if( !sGetRefString( node, idxbuf.ref ) ) return false;
        return true;
    }

    bool loadTexture( const XmlElement & node )
    {
        size_t stage;
        if( !sGetNumbericAttr( node, "stage", stage ) ) return false;
        if( stage >= GN_ARRAY_COUNT(textures) )
        {
            GN_ERROR(sLogger)( "invalid stage." );
            return false;
        }

        D3D9TextureDump & t = textures[stage];

        if( !sGetRefString( node, t.ref ) ) return false;

        return true;
    }

    bool loadRenderTarget( const XmlElement & node )
    {
        size_t stage;
        if( !sGetNumbericAttr( node, "stage", stage ) ) return false;
        if( stage >= GN_ARRAY_COUNT(rendertargets) )
        {
            GN_ERROR(sLogger)( "invalid stage." );
            return false;
        }

        D3D9RtDump & rt = rendertargets[stage];

        if( !sGetNumbericAttr( node, "width", rt.width ) ) return false;
        if( !sGetNumbericAttr( node, "height", rt.height ) ) return false;
        if( !sGetNumbericAttr( node, "format", rt.format ) ) return false;
        if( !sGetNumbericAttr( node, "msaa", rt.msaa ) ) return false;
        if( !sGetNumbericAttr( node, "quality", rt.quality ) ) return false;

        rt.inuse = true;
        return true;
    }

    bool loadDepthStencil( const XmlElement & node )
    {
        if( !sGetNumbericAttr( node, "width", depthstencil.width ) ) return false;
        if( !sGetNumbericAttr( node, "height", depthstencil.height ) ) return false;
        if( !sGetNumbericAttr( node, "format", depthstencil.format ) ) return false;
        if( !sGetNumbericAttr( node, "msaa", depthstencil.msaa ) ) return false;
        if( !sGetNumbericAttr( node, "quality", depthstencil.quality ) ) return false;

        depthstencil.inuse = true;
        return true;
    }

    bool loadRenderStates( const XmlElement & node )
    {
        size_t count = 0;
        for( XmlNode * n = node.child; n; n = n->sibling )
        {
            XmlElement * e = n->toElement();
            if( !e ) continue;

            if( "rs" != e->name )
            {
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->getLocation(), e->name.cptr() );
                continue;
            }

            if( count >= GN_ARRAY_COUNT(renderstates) )
            {
                GN_WARN(sLogger)( "%s : too many render states", e->getLocation() );
                return true;
            }

            D3D9RsDump & rs = renderstates[count];

            if( !sGetNumbericAttr( *e, "type", rs.type ) ) return false;
            if( !sGetNumbericAttr( *e, "value", rs.value ) ) return false;

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
        for( XmlNode * n = node.child; n; n = n->sibling )
        {
            XmlElement * e = n->toElement();
            if( !e ) continue;

            if( "ss" != e->name )
            {
                GN_WARN(sLogger)( "%s : ignore unknown node %s", e->getLocation(), e->name.cptr() );
                continue;
            }

            UInt32 stage, type;
            if( !sGetNumbericAttr( *e, "stage", stage ) ) return false;
            if( !sGetNumbericAttr( *e, "type", type ) ) return false;

            if( stage > 15 || type > 13 )
            {
                GN_ERROR(sLogger)( "%s : invalid SS stage and/or type", e->getLocation() );
                return false;
            }

            if( !sGetNumbericAttr( *e, "value", samplerstates[stage][type] ) ) return false;
        }

        // success
        return true;
    }
};

struct D3D9StateResource
{
    //@}
};

class MyApp : public GN::gfx::d3d9::D3D9Application
{
    D3D9StateDump mState;

protected:

    bool onInit()
    {
        mState.clear();

        return scene::loadFromXmlFile( mState, sDumpFileName );
    }

    bool onRestore()
    {
        if( !mState.onRestore( d3d9dev() ) ) return false;
        mState.bind( d3d9dev() );
        return true;
    }

    void onDispose()
    {
        mState.onDispose();
    }

    void onDraw()
    {
        IDirect3DDevice9 & dev = d3d9dev();

        dev.Clear( 0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0, 1.0f, 0 );

        dev.Present( 0, 0, 0, 0 );
    }
};

void printhelp( const char * appname )
{
    printf( "Usage: %s [dumpname]\n", (baseName(appname) + extName(appname)).cptr() );
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
