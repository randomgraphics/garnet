#include "pch.h"
#include "d3d10VtxLayout.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D10.VtxLayout");

// *****************************************************************************
// local functions
// *****************************************************************************

///
/// d3d vertex semantic structure
///
struct D3D10VtxSemDesc
{
    const char * name;  ///< semantic name
    UINT         index; ///< semantic index
};

///
/// convert vertex format to D3D10 semantic
// -----------------------------------------------------------------------------
static inline const D3D10VtxSemDesc * sVtxSem2D3D10( GN::gfx::VtxSem sem )
{
    using namespace GN;
    using namespace GN::gfx;

    static D3D10VtxSemDesc sDesc;
    sDesc.index = 0;

    switch( sem.u32 )
    {
        case GN_MAKE_FOURCC('P','O','S','0') :
            sDesc.name = "POSITION";
            break;

        case GN_MAKE_FOURCC('W','G','H','T') :
            sDesc.name = "BLENDWEIGHT";
            break;

        case GN_MAKE_FOURCC('N','M','L','0') :
            sDesc.name = "NORMAL";
            break;

        case GN_MAKE_FOURCC('C','L','R','0') :
            sDesc.name = "COLOR";
            break;

        case GN_MAKE_FOURCC('C','L','R','1') :
            sDesc.name = "COLOR";
            sDesc.index = 1;
            break;

        case GN_MAKE_FOURCC('F','O','G','0') :
            sDesc.name = "FOG";
            break;

        case GN_MAKE_FOURCC('T','A','N','G') :
            sDesc.name = "TANGENT";
            break;

        case GN_MAKE_FOURCC('B','N','M','L') :
            sDesc.name = "BINORMAL";
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
            sDesc.name = "TEXCOORD";
            sDesc.index = sem.u8[3] - VTXSEM_TEX0.u8[3];
            break;

        case GN_MAKE_FOURCC('T','E','X','A') :
        case GN_MAKE_FOURCC('T','E','X','B') :
        case GN_MAKE_FOURCC('T','E','X','C') :
        case GN_MAKE_FOURCC('T','E','X','D') :
        case GN_MAKE_FOURCC('T','E','X','E') :
        case GN_MAKE_FOURCC('T','E','X','F') :
            sDesc.name = "TEXCOORD";
            sDesc.index = sem.u8[3] - VTXSEM_TEXA.u8[3] + 10;
            break;

        default :
            GN_ERROR(sLogger)( "unsupport vertex semantic: %s", sem.toStr() );
            return NULL;
    };

    // success
    return &sDesc;
}   

///
/// convert vertdecl structure to a D3D vertex declaration array
// -----------------------------------------------------------------------------
static bool
sVtxFmt2InputLayout(
    std::vector<D3D10_INPUT_ELEMENT_DESC> & elements,
    const GN::gfx::VtxFmtDesc & vtxfmt )
{
    GN_GUARD;

    using namespace GN;
    using namespace GN::gfx;

    elements.clear();

    for( size_t i = 0; i < vtxfmt.count; ++i )
    {
        const VtxFmtDesc::AttribDesc & va = vtxfmt.attribs[i];

        D3D10_INPUT_ELEMENT_DESC elem;

        // set attrib semantic
        const D3D10VtxSemDesc * desc = sVtxSem2D3D10( va.semantic );
        if( !desc ) return false;
        elem.SemanticName  = desc->name;
        elem.SemanticIndex = desc->index;

        // set attrib format
        elem.Format = d3d10::clrFmt2DxgiFormat( va.format );

        // set stream index
        elem.InputSlot = va.stream;

        // set attrib offset
        elem.AlignedByteOffset = va.offset;

        // add to element array
        elements.push_back( elem );
    }

    // success
    return true;

    GN_UNGUARD;
}

///
/// Build a fake shader that can accept the input vertex format
// -----------------------------------------------------------------------------
static ID3D10Blob *
sVtxFmt2ShaderBinary( const GN::gfx::VtxFmtDesc & vtxfmt )
{
    GN_GUARD;

    using namespace GN;
    using namespace GN::gfx;

    StrA code= "struct VS_INPUT {\n";

    for( size_t i = 0; i < vtxfmt.count; ++i )
    {
        const GN::gfx::VtxFmtDesc::AttribDesc & va = vtxfmt.attribs[i];

        // get attrib semantic
        const D3D10VtxSemDesc * desc = sVtxSem2D3D10( va.semantic );
        if( !desc ) return false;

        code += strFormat( "    float4 attr%d : %s%d;\n", i, desc->name, desc->index );
    }

    code += "}; float4 main( VS_INPUT in ) : POSITION { return float(0,0,0,1); }";

    // compile shader
    AutoComPtr<ID3D10Blob> bin, err;
    if( FAILED( D3D10CompileShader(
        code.cptr(), (UINT)code.size(),
        "", // filename
        0,  // defines
        0,  // includes
        "main",
        "vs_4_0",
        0, // flag
        &bin,
        &err ) ) )
    {
        GN_ERROR(sLogger)(
            "\n================== Shader compile failure ===============\n"
            "%s\n"
            "\n---------------------------------------------------------\n"
            "%s\n"
            "\n=========================================================\n",
            code.cptr(),
            err ? (const char*)err->GetBufferPointer() : "Unknown error." );
        return false;
    }

    // success
    return bin.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
// create D3D vtxfmt from vertex format structure
// -----------------------------------------------------------------------------
ID3D10InputLayout *
GN::gfx::createD3D10VtxLayout( ID3D10Device * dev, const VtxFmtDesc & format )
{
    GN_GUARD;

    GN_ASSERT( dev );

    std::vector<D3D10_INPUT_ELEMENT_DESC> elements;
    if( !sVtxFmt2InputLayout( elements, format ) ) return false;
    GN_ASSERT( !elements.empty() );

    AutoComPtr<ID3D10Blob> bin( sVtxFmt2ShaderBinary( format ) );
    if( !bin ) return false;

    ID3D10InputLayout * layout;
    GN_DX10_CHECK_RV(
        dev->CreateInputLayout(
            &elements[0],
            (UINT)elements.size(),
            bin->GetBufferPointer(),
            bin->GetBufferSize(),
            &layout ),
        0 );

    // success
    return layout;

    GN_UNGUARD;
}

