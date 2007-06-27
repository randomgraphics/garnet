#include "pch.h"
#include "d3d9VtxDecl.h"

using namespace GN::gfx;
using namespace GN::gfx2;

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9VtxDecl");

// *****************************************************************************
// local functions
// *****************************************************************************

///
/// d3d vertex semantic structure
///
struct D3D9VtxSemDesc
{
    D3DDECLUSAGE usage; ///< D3D vertex usage
    BYTE         index; ///< register index
};

///
/// convert vertex format to d3d-decl usage
// -----------------------------------------------------------------------------
static inline const D3D9VtxSemDesc * sVtxSem2D3D( GN::gfx2::SurfaceAttributeSemantic sem )
{
    using namespace GN;
    using namespace GN::gfx;

    static D3D9VtxSemDesc sDesc;
    sDesc.index = 0;

    switch( sem.u64 )
    {
        case GN_MAKE_FOURCC('P','O','S','0') :
            sDesc.usage = D3DDECLUSAGE_POSITION;
            break;

        case GN_MAKE_FOURCC('W','G','H','T') :
            sDesc.usage = D3DDECLUSAGE_BLENDWEIGHT;
            break;

        case GN_MAKE_FOURCC('N','M','L','0') :
            sDesc.usage = D3DDECLUSAGE_NORMAL;
            break;

        case GN_MAKE_FOURCC('C','L','R','0') :
            sDesc.usage = D3DDECLUSAGE_COLOR;
            break;

        case GN_MAKE_FOURCC('C','L','R','1') :
            sDesc.usage = D3DDECLUSAGE_COLOR;
            sDesc.index = 1;
            break;

        case GN_MAKE_FOURCC('F','O','G','0') :
            sDesc.usage = D3DDECLUSAGE_FOG;
            break;

        case GN_MAKE_FOURCC('T','A','N','G') :
            sDesc.usage = D3DDECLUSAGE_TANGENT;
            break;

        case GN_MAKE_FOURCC('B','N','M','L') :
            sDesc.usage = D3DDECLUSAGE_BINORMAL;
            break;

        case GN_MAKE_FOURCC('T','E','X','0') :
        case GN_MAKE_FOURCC('T','E','X','1') :
        case GN_MAKE_FOURCC('T','E','X','2') :
        case GN_MAKE_FOURCC('T','E','X','3') :
        case GN_MAKE_FOURCC('T','E','X','4') :
        case GN_MAKE_FOURCC('T','E','X','5') :
        case GN_MAKE_FOURCC('T','E','X','6') :
        case GN_MAKE_FOURCC('T','E','X','7') :
        case GN_MAKE_FOURCC('T','E','X','8') :
        case GN_MAKE_FOURCC('T','E','X','9') :
            sDesc.usage = D3DDECLUSAGE_TEXCOORD;
            sDesc.index = sem.c8[3] - '0';
            break;

        case GN_MAKE_FOURCC('T','E','X','A') :
        case GN_MAKE_FOURCC('T','E','X','B') :
        case GN_MAKE_FOURCC('T','E','X','C') :
        case GN_MAKE_FOURCC('T','E','X','D') :
        case GN_MAKE_FOURCC('T','E','X','E') :
        case GN_MAKE_FOURCC('T','E','X','F') :
            sDesc.usage = D3DDECLUSAGE_TEXCOORD;
            sDesc.index = sem.c8[3] - '0' + 10;
            break;

        default :
            GN_ERROR(sLogger)( "unsupport vertex semantic: %s", sem.str() );
            return NULL;
    };

    // success
    return &sDesc;
}   

///
/// convert vertex format to d3d-decl usage
// -----------------------------------------------------------------------------
static inline D3DDECLTYPE sClrFmt2D3D( GN::gfx::ClrFmt fmt )
{
    GN_ASSERT( 0 <= fmt && fmt < GN::gfx::NUM_CLRFMTS );
    switch ( fmt )
    {
        case GN::gfx::FMT_FLOAT1    : return D3DDECLTYPE_FLOAT1;
        case GN::gfx::FMT_FLOAT2    : return D3DDECLTYPE_FLOAT2;
        case GN::gfx::FMT_FLOAT3    : return D3DDECLTYPE_FLOAT3;
        case GN::gfx::FMT_FLOAT4    : return D3DDECLTYPE_FLOAT4;
        case GN::gfx::FMT_SHORT2    : return D3DDECLTYPE_SHORT2;
        case GN::gfx::FMT_UBYTE4N   : return D3DDECLTYPE_UBYTE4N;
        case GN::gfx::FMT_SHORT2N   : return D3DDECLTYPE_SHORT2N;
        case GN::gfx::FMT_SHORT4N   : return D3DDECLTYPE_SHORT4N;
        case GN::gfx::FMT_USHORT2N  : return D3DDECLTYPE_USHORT2N;
        case GN::gfx::FMT_USHORT4N  : return D3DDECLTYPE_USHORT4N;
        case GN::gfx::FMT_UDEC3     : return D3DDECLTYPE_UDEC3;
        case GN::gfx::FMT_DEC3N     : return D3DDECLTYPE_DEC3N;
        case GN::gfx::FMT_FLOAT16_2 : return D3DDECLTYPE_FLOAT16_2;
        case GN::gfx::FMT_FLOAT16_4 : return D3DDECLTYPE_FLOAT16_4;
#if GN_XENON
        case GN::gfx::FMT_INT1      : return D3DDECLTYPE_INT1;
        case GN::gfx::FMT_INT2      : return D3DDECLTYPE_INT2;
        case GN::gfx::FMT_INT4      : return D3DDECLTYPE_INT4;
        case GN::gfx::FMT_UINT1     : return D3DDECLTYPE_UINT1;
        case GN::gfx::FMT_UINT2     : return D3DDECLTYPE_UINT2;
        case GN::gfx::FMT_UINT4     : return D3DDECLTYPE_UINT4;
        case GN::gfx::FMT_INT1N     : return D3DDECLTYPE_INT1N;
        case GN::gfx::FMT_INT2N     : return D3DDECLTYPE_INT2N;
        case GN::gfx::FMT_INT4N     : return D3DDECLTYPE_INT4N;
        case GN::gfx::FMT_UINT1N    : return D3DDECLTYPE_UINT1N;
        case GN::gfx::FMT_UINT2N    : return D3DDECLTYPE_UINT2N;
        case GN::gfx::FMT_UINT4N    : return D3DDECLTYPE_UINT4N;
        case GN::gfx::FMT_UBYTE4    : return D3DDECLTYPE_UBYTE4;
        case GN::gfx::FMT_BYTE4     : return D3DDECLTYPE_BYTE4;
        case GN::gfx::FMT_BYTE4N    : return D3DDECLTYPE_BYTE4N;
        case GN::gfx::FMT_SHORT4    : return D3DDECLTYPE_SHORT4;
        case GN::gfx::FMT_USHORT2   : return D3DDECLTYPE_USHORT2;
        case GN::gfx::FMT_USHORT4   : return D3DDECLTYPE_USHORT4;
        case GN::gfx::FMT_DEC3      : return D3DDECLTYPE_DEC3;
        case GN::gfx::FMT_UDEC3N    : return D3DDECLTYPE_UDEC3N;
        case GN::gfx::FMT_UDEC4     : return D3DDECLTYPE_UDEC4;
        case GN::gfx::FMT_DEC4      : return D3DDECLTYPE_DEC4;
        case GN::gfx::FMT_UDEC4N    : return D3DDECLTYPE_UDEC4N;
        case GN::gfx::FMT_DEC4N     : return D3DDECLTYPE_DEC4N;
        case GN::gfx::FMT_UHEND3    : return D3DDECLTYPE_UHEND3;
        case GN::gfx::FMT_HEND3     : return D3DDECLTYPE_HEND3;
        case GN::gfx::FMT_UHEND3N   : return D3DDECLTYPE_UHEND3N;
        case GN::gfx::FMT_HEND3N    : return D3DDECLTYPE_HEND3N;
        case GN::gfx::FMT_UDHEN3    : return D3DDECLTYPE_UDHEN3;
        case GN::gfx::FMT_DHEN3     : return D3DDECLTYPE_DHEN3;
        case GN::gfx::FMT_UDHEN3N   : return D3DDECLTYPE_UDHEN3N;
        case GN::gfx::FMT_DHEN3N    : return D3DDECLTYPE_DHEN3N;
#endif
        default:
            GN_ERROR(sLogger)( "Invalid color format: %d!", fmt );
            return D3DDECLTYPE(MAXD3DDECLTYPE+1);
    }
}

///
/// element sorting function, first by stream, then by offset.
// -----------------------------------------------------------------------------
static bool
sElementSorting( const D3DVERTEXELEMENT9 & a, const D3DVERTEXELEMENT9 & b )
{
	if( a.Stream < b.Stream ) return true;
	if( a.Stream > b.Stream ) return false;
    return a.Offset < b.Offset;
}

///
/// convert vertdecl structure to a D3D vertex declaration array
// -----------------------------------------------------------------------------
static bool
sSurfaceElementFormatToD3D9(
    std::vector<D3DVERTEXELEMENT9>               & elements,
    const GN::gfx2::SurfaceElementFormat * const * streams,
    size_t                                         count )
{
    GN_GUARD;

    elements.clear();

    for( size_t i = 0; i < count; ++i )
    {
        const GN::gfx2::SurfaceElementFormat & stream = *streams[i];

        for( size_t j = 0; j < stream.count; ++j )
        {
            const GN::gfx2::SurfaceAttribute & va = stream.attribs[j];

            D3DVERTEXELEMENT9 elem;

            // set stream index
            elem.Stream = (WORD)i;

            // set attrib offset
            elem.Offset = (WORD)va.offset;

            // set method ( no tessellator is used )
            elem.Method = D3DDECLMETHOD_DEFAULT;

            // set attrib semantic
            const D3D9VtxSemDesc * desc = sVtxSem2D3D( va.semantic );
            if( !desc ) return false;
            elem.Usage      = (BYTE) desc->usage;
            elem.UsageIndex = desc->index;

            // set attrib format
            elem.Type = (BYTE)sClrFmt2D3D( va.format );

            // add to element array
            elements.push_back( elem );
        }
    }

    // sort elements by offset
    std::sort( elements.begin(), elements.end(), &sElementSorting );

    // end tag
    D3DVERTEXELEMENT9 endtag = D3DDECL_END();
    elements.push_back( endtag );

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
// create D3D decl from surface format structure
// -----------------------------------------------------------------------------
LPDIRECT3DVERTEXDECLARATION9
GN::gfx2::createD3D9VtxDecl(
    LPDIRECT3DDEVICE9                    dev,
    const SurfaceElementFormat * const * streams,
    size_t                               count )
{
    GN_GUARD;

    GN_ASSERT( dev );

    std::vector<D3DVERTEXELEMENT9> elements;
    if( !sSurfaceElementFormatToD3D9( elements, streams, count ) ) return false;
    GN_ASSERT( !elements.empty() );

    LPDIRECT3DVERTEXDECLARATION9 decl;
    GN_DX9_CHECK_RV( dev->CreateVertexDeclaration( &elements[0], &decl ), 0 );

    // success
    return decl;

    GN_UNGUARD;
}

