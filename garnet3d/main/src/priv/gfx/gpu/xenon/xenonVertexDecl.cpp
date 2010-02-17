#include "pch.h"
#include "xenonVertexDecl.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.xenon");

// *****************************************************************************
// local functions
// *****************************************************************************

static const D3DDECLUSAGE D3DDECLUSAGE_ERROR = (D3DDECLUSAGE)-1;

///
/// convert vertex format to d3d-decl usage
// -----------------------------------------------------------------------------
static inline D3DDECLUSAGE sVertexBindingToXenon( const char * binding )
{
    using namespace GN;

    struct BindingMap
    {
        struct MapItem { const char * binding; D3DDECLUSAGE usage; };

        std::map<StrA,D3DDECLUSAGE> map;

        BindingMap()
        {
            MapItem table[] =
            {
                { "position", D3DDECLUSAGE_POSITION },
                { "pos",      D3DDECLUSAGE_POSITION },
                { "weight",   D3DDECLUSAGE_BLENDWEIGHT },
                { "normal",   D3DDECLUSAGE_NORMAL },
                { "nml",      D3DDECLUSAGE_NORMAL },
                { "color",    D3DDECLUSAGE_COLOR },
                { "fog",      D3DDECLUSAGE_FOG },
                { "tangent",  D3DDECLUSAGE_TANGENT },
                { "tang",     D3DDECLUSAGE_TANGENT },
                { "texcoord", D3DDECLUSAGE_TEXCOORD },
                { "tex",      D3DDECLUSAGE_TEXCOORD },
            };

            for( size_t i = 0; i < GN_ARRAY_COUNT(table); ++i )
            {
                map[table[i].binding] = table[i].usage;
            }
        }
    };

    static const BindingMap sConvertTable;

    StrA lowerCaseBinding( binding );
    lowerCaseBinding.ToLowerCase();

    std::map<StrA,D3DDECLUSAGE>::const_iterator iter = sConvertTable.map.find( lowerCaseBinding );

    return ( sConvertTable.map.end() == iter ) ? D3DDECLUSAGE_ERROR : iter->second;
}

static const D3DDECLTYPE D3DDECLTYPE_ERROR = D3DDECLTYPE(MAXD3DDECLTYPE+1);

///
/// convert vertex format to d3d-decl usage
// -----------------------------------------------------------------------------
static inline D3DDECLTYPE
sColorFormatToXenon( GN::gfx::ColorFormat fmt )
{
    switch ( fmt.alias )
    {
        case GN::gfx::ColorFormat::FLOAT1    : return D3DDECLTYPE_FLOAT1;
        case GN::gfx::ColorFormat::FLOAT2    : return D3DDECLTYPE_FLOAT2;
        case GN::gfx::ColorFormat::FLOAT3    : return D3DDECLTYPE_FLOAT3;
        case GN::gfx::ColorFormat::FLOAT4    : return D3DDECLTYPE_FLOAT4;
        case GN::gfx::ColorFormat::SHORT2    : return D3DDECLTYPE_SHORT2;
        case GN::gfx::ColorFormat::UBYTE4N   : return D3DDECLTYPE_UBYTE4N;
        case GN::gfx::ColorFormat::SHORT2N   : return D3DDECLTYPE_SHORT2N;
        case GN::gfx::ColorFormat::SHORT4N   : return D3DDECLTYPE_SHORT4N;
        case GN::gfx::ColorFormat::USHORT2N  : return D3DDECLTYPE_USHORT2N;
        case GN::gfx::ColorFormat::USHORT4N  : return D3DDECLTYPE_USHORT4N;
        //case GN::gfx::ColorFormat::UDEC3     : return D3DDECLTYPE_UDEC3;
        //case GN::gfx::ColorFormat::DEC3N     : return D3DDECLTYPE_DEC3N;
        //case GN::gfx::ColorFormat::FLOAT16_2 : return D3DDECLTYPE_FLOAT16_2;
        //case GN::gfx::ColorFormat::FLOAT16_4 : return D3DDECLTYPE_FLOAT16_4;
        case GN::gfx::ColorFormat::INT1      : return D3DDECLTYPE_INT1;
        //case GN::gfx::ColorFormat::INT2      : return D3DDECLTYPE_INT2;
        //case GN::gfx::ColorFormat::INT4      : return D3DDECLTYPE_INT4;
        case GN::gfx::ColorFormat::UINT1     : return D3DDECLTYPE_UINT1;
        //case GN::gfx::ColorFormat::UINT2     : return D3DDECLTYPE_UINT2;
        //case GN::gfx::ColorFormat::UINT4     : return D3DDECLTYPE_UINT4;
        case GN::gfx::ColorFormat::INT1N     : return D3DDECLTYPE_INT1N;
        //case GN::gfx::ColorFormat::INT2N     : return D3DDECLTYPE_INT2N;
        //case GN::gfx::ColorFormat::INT4N     : return D3DDECLTYPE_INT4N;
        case GN::gfx::ColorFormat::UINT1N    : return D3DDECLTYPE_UINT1N;
        //case GN::gfx::ColorFormat::UINT2N    : return D3DDECLTYPE_UINT2N;
        //case GN::gfx::ColorFormat::UINT4N    : return D3DDECLTYPE_UINT4N;
        //case GN::gfx::ColorFormat::UBYTE4    : return D3DDECLTYPE_UBYTE4;
        //case GN::gfx::ColorFormat::BYTE4     : return D3DDECLTYPE_BYTE4;
        //case GN::gfx::ColorFormat::BYTE4N    : return D3DDECLTYPE_BYTE4N;
        case GN::gfx::ColorFormat::SHORT4    : return D3DDECLTYPE_SHORT4;
        case GN::gfx::ColorFormat::USHORT2   : return D3DDECLTYPE_USHORT2;
        case GN::gfx::ColorFormat::USHORT4   : return D3DDECLTYPE_USHORT4;
        //case GN::gfx::ColorFormat::DEC3      : return D3DDECLTYPE_DEC3;
        //case GN::gfx::ColorFormat::UDEC3N    : return D3DDECLTYPE_UDEC3N;
        //case GN::gfx::ColorFormat::UDEC4     : return D3DDECLTYPE_UDEC4;
        //case GN::gfx::ColorFormat::DEC4      : return D3DDECLTYPE_DEC4;
        //case GN::gfx::ColorFormat::UDEC4N    : return D3DDECLTYPE_UDEC4N;
        //case GN::gfx::ColorFormat::DEC4N     : return D3DDECLTYPE_DEC4N;
        //case GN::gfx::ColorFormat::UHEND3    : return D3DDECLTYPE_UHEND3;
        //case GN::gfx::ColorFormat::HEND3     : return D3DDECLTYPE_HEND3;
        //case GN::gfx::ColorFormat::UHEND3N   : return D3DDECLTYPE_UHEND3N;
        //case GN::gfx::ColorFormat::HEND3N    : return D3DDECLTYPE_HEND3N;
        //case GN::gfx::ColorFormat::UDHEN3    : return D3DDECLTYPE_UDHEN3;
        //case GN::gfx::ColorFormat::DHEN3     : return D3DDECLTYPE_DHEN3;
        //case GN::gfx::ColorFormat::UDHEN3N   : return D3DDECLTYPE_UDHEN3N;
        //case GN::gfx::ColorFormat::DHEN3N    : return D3DDECLTYPE_DHEN3N;

        default:
            GN_ERROR(sLogger)( "Invalid color format: %d!", fmt );
            return D3DDECLTYPE_ERROR;
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
sVtxFmtDesc2D3DDecl( std::vector<D3DVERTEXELEMENT9> & elements, const GN::gfx::VertexFormat & vtxfmt )
{
    GN_GUARD;

    elements.clear();

    for( size_t i = 0; i < vtxfmt.numElements; ++i )
    {
        const GN::gfx::VertexElement & ve = vtxfmt.elements[i];

        D3DVERTEXELEMENT9 elem;

        // set stream index
        elem.Stream = ve.stream;

        // set attrib offset
        elem.Offset = ve.offset;

        // set method ( no tessellator is used )
        elem.Method = D3DDECLMETHOD_DEFAULT;

        // set attrib semantic
        elem.Usage = (BYTE)sVertexBindingToXenon( ve.binding );
        if( (BYTE)D3DDECLUSAGE_ERROR == elem.Usage ) return false;

        elem.UsageIndex = ve.bindingIndex;

        // set attrib vtxfmt
        elem.Type = sColorFormatToXenon( ve.format );
        if( D3DDECLTYPE_ERROR == elem.Type ) return false;

        // add to element array
        elements.push_back( elem );
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
// create D3D decl from vertex format structure
// -----------------------------------------------------------------------------
IDirect3DVertexDeclaration9 *
GN::gfx::createXenonVertexDecl( IDirect3DDevice9 & dev, const GN::gfx::VertexFormat & format )
{
    GN_GUARD;

    std::vector<D3DVERTEXELEMENT9> elements;
    if( !sVtxFmtDesc2D3DDecl( elements, format ) ) return NULL;
    GN_ASSERT( !elements.empty() );

    IDirect3DVertexDeclaration9 * decl;
    GN_DX_CHECK_RETURN( dev.CreateVertexDeclaration( &elements[0], &decl ), NULL );

    // success
    return decl;

    GN_UNGUARD;
}
