#include "pch.h"
#include "d3dRenderer.h"
#include "d3dVtxBuf.h"
#include "d3dIdxBuf.h"

// *****************************************************************************
// local functions
// *****************************************************************************

//!
//! d3d vertex semantic structure
//!
struct VtxSemDesc
{
    D3DDECLUSAGE usage; //!< D3D vertex usage
    BYTE         index; //!< register index
};

//!
//! vertex semantic convert table
//!
static VtxSemDesc sVtxSem2D3D[GN::gfx::NUM_VTXSEMS] =
{
    #define GNGFX_DEFINE_VTXSEM( tag, d3ddecl, d3dindex, glname, glindex, cgname ) \
        { static_cast<D3DDECLUSAGE>(d3ddecl), d3dindex },
    #include "garnet/gfx/vertexSemanticMeta.h"
    #undef  GNGFX_DEFINE_VTXSEM
};

//!
//! convert vertex format to d3d-decl usage
// -----------------------------------------------------------------------------
static inline D3DDECLTYPE sClrFmt2D3D( GN::gfx::ClrFmt fmt )
{
    GN_ASSERT( 0 <= fmt && fmt < GN::gfx::NUM_CLRFMTS );
    switch ( fmt )
    {
        case GN::gfx::FMT_FLOAT1   : return D3DDECLTYPE_FLOAT1;
        case GN::gfx::FMT_FLOAT2   : return D3DDECLTYPE_FLOAT2;
        case GN::gfx::FMT_FLOAT3   : return D3DDECLTYPE_FLOAT3;
        case GN::gfx::FMT_FLOAT4   : return D3DDECLTYPE_FLOAT4;
        case GN::gfx::FMT_D3DCOLOR : return D3DDECLTYPE_D3DCOLOR;
        default:
            GNGFX_ERROR( "Invalid color format: %d!", fmt );
            return D3DDECLTYPE(MAXD3DDECLTYPE+1);
    }
};

//!
//! element sorting function
// -----------------------------------------------------------------------------
static bool
sSortByOffset( const D3DVERTEXELEMENT9 & a, const D3DVERTEXELEMENT9 & b )
{
    return a.Offset < b.Offset;
}

//!
//! convert vertdecl structure to a D3D vertex declaration array
// -----------------------------------------------------------------------------
static bool
sVtxFmtDesc2D3DDecl( std::vector<D3DVERTEXELEMENT9> & elements, const GN::gfx::VtxFmtDesc & decl )
{
    GN_GUARD;

    elements.clear();

    for( uint8_t i = 0; i < GN::gfx::NUM_VTXSEMS; ++i )
    {
        const GN::gfx::VtxFmtDesc::AttribDesc & va = decl.attribs[i];

        // ignore unused attribute
        if ( !va.used ) continue;

        D3DVERTEXELEMENT9 elem;

        // set stream index
        elem.Stream = va.stream;

        // set attrib offset
        elem.Offset = va.offset;

        // set method ( no tessellator is used )
        elem.Method = D3DDECLMETHOD_DEFAULT;

        // set attrib semantic
        elem.Usage      = (BYTE)sVtxSem2D3D[i].usage;
        elem.UsageIndex = (BYTE)sVtxSem2D3D[i].index;

        // set attrib format
        elem.Type = (BYTE)sClrFmt2D3D( va.format );

        // add to element array
        elements.push_back( elem );
    }

    // sort elements by offset
    std::sort( elements.begin(), elements.end(), &sSortByOffset );

    // end tag
    D3DVERTEXELEMENT9 endtag = D3DDECL_END();
    elements.push_back( endtag );

    // success
    return true;

    GN_UNGUARD;
}

//!
//! create D3D decl from vertex format structure
// -----------------------------------------------------------------------------
static LPDIRECT3DVERTEXDECLARATION9
sCreateD3DVertexDecl( LPDIRECT3DDEVICE9 dev, const GN::gfx::VtxFmtDesc & format )
{
    GN_GUARD;

    GN_ASSERT( dev );

    std::vector<D3DVERTEXELEMENT9> elements;
    if( !sVtxFmtDesc2D3DDecl( elements, format ) ) return false;
    GN_ASSERT( !elements.empty() );

    LPDIRECT3DVERTEXDECLARATION9 decl;
    GN_DX_CHECK_RV( dev->CreateVertexDeclaration( &elements[0], &decl ), 0 );

    // success
    return decl;

    GN_UNGUARD;
}

struct EqualFormat
{
    const GN::gfx::VtxFmtDesc & format;
    EqualFormat( const GN::gfx::VtxFmtDesc & f ) : format(f) {}
    bool operator()( const GN::gfx::D3DVtxBindingDesc & vbd ) const { return format == vbd.format; }
};

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::bufferDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // recreate all D3D decls
    uint32_t h = mVtxBindings.first();
    while( h )
    {
        mVtxBindings[h].decl.attach( sCreateD3DVertexDecl( mDevice, mVtxBindings[h].format ) );
        if( !mVtxBindings[h].decl ) return false;

        h = mVtxBindings.next( h );
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::bufferDeviceDispose()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // release all D3D decls
    uint32_t h = mVtxBindings.first();
    while( h )
    {
        mVtxBindings[h].decl.reset();
        h = mVtxBindings.next( h );
    }

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
uint32_t GN::gfx::D3DRenderer::createVtxBinding( const VtxFmtDesc & format )
{
    GN_GUARD;

    uint32_t h = mVtxBindings.findIf( EqualFormat(format) );

    if( 0 == h )
    {
        // create new vertex decl
        D3DVtxBindingDesc vbd;
        vbd.format = format;
        vbd.decl.attach( sCreateD3DVertexDecl( mDevice, format ) );
        if( !vbd.decl ) return 0;

        h = mVtxBindings.add( vbd );
    }

    // success
    return h;

    GN_UNGUARD;
}


//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::D3DRenderer::createVtxBuf(
    size_t numVtx, size_t stride, ResourceUsage usage, bool sysCopy )
{
    GN_GUARD;

    AutoRef<D3DVtxBuf> buf( new D3DVtxBuf(*this) );

    if( !buf->init( numVtx, stride, usage, sysCopy ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::D3DRenderer::createIdxBuf(
    size_t numIdx, ResourceUsage usage, bool sysCopy )
{
    GN_GUARD;

    AutoRef<D3DIdxBuf> buf( new D3DIdxBuf(*this) );

    if( !buf->init( numIdx, usage, sysCopy ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::bindVtxBinding( uint32_t handle )
{
    GN_GUARD;

    if( !mVtxBindings.validHandle(handle) )
    {
        GNGFX_ERROR( "invalid vertex binding handle : %d", handle );
        return;
    }

    GN_ASSERT( mVtxBindings[handle].decl );
    GN_DX_CHECK( mDevice->SetVertexDeclaration( mVtxBindings[handle].decl ) );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::bindVtxBufs( const VtxBuf * const buffers[], size_t start, size_t count )
{
    GN_GUARD_SLOW;

    size_t stage = start;

    for( size_t i = 0; i < count; ++i, ++stage )
    {
        if( buffers[i] )
        {
            GN_DX_CHECK( mDevice->SetStreamSource(
                (UINT)stage,
                safeCast<const D3DVtxBuf*>(buffers[i])->getD3DVb(),
                0,
                (UINT)buffers[i]->getStride() ) );
        }
        else
        {
            GN_DX_CHECK( mDevice->SetStreamSource( (UINT)stage, 0, 0, 0 ) );
        }
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::bindVtxBuf( size_t index, const VtxBuf * buffer, size_t stride )
{
    GN_GUARD_SLOW;

    GN_DX_CHECK( mDevice->SetStreamSource(
        (UINT)index,
        buffer ? safeCast<const D3DVtxBuf*>(buffer)->getD3DVb() : 0,
        0,
        (UINT)stride ) );

     GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::bindIdxBuf( const IdxBuf * buf )
{
    GN_GUARD_SLOW;

    GN_DX_CHECK( mDevice->SetIndices( buf ? safeCast<const D3DIdxBuf*>(buf)->getD3DIb() : 0 ) );

    GN_UNGUARD_SLOW;
}
