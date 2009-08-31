#include "pch.h"
#include "d3d11Gpu.h"
#include "d3d11VtxLayout.h"
#include "garnet/GNd3d10.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D11.VtxLayout");

// *****************************************************************************
// local functions
// *****************************************************************************

///
/// convert vertdecl structure to a D3D vertex declaration array
// -----------------------------------------------------------------------------
static bool
sVtxFmt2InputLayout(
    std::vector<D3D11_INPUT_ELEMENT_DESC> & elements,
    const GN::gfx::VertexFormat           & vtxfmt )
{
    GN_GUARD;

    using namespace GN;
    using namespace GN::gfx;

    elements.clear();

    for( size_t i = 0; i < vtxfmt.numElements; ++i )
    {
        const VertexElement & ve = vtxfmt.elements[i];

        D3D11_INPUT_ELEMENT_DESC elem;

        // set attrib semantic
        elem.SemanticName  = ve.binding;
        elem.SemanticIndex = ve.bindingIndex;

        // set attrib format
        elem.Format = (DXGI_FORMAT)colorFormat2DxgiFormat( ve.format );
        if( DXGI_FORMAT_UNKNOWN == elem.Format )
        {
            GN_ERROR(sLogger)( "Unknown element format: %s", ve.format.toString().cptr() );
            return false;
        }

        // set stream index
        elem.InputSlot = ve.stream;

        // set attrib offset
        elem.AlignedByteOffset = ve.offset;

        // instancing attributes
        elem.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        elem.InstanceDataStepRate = 0;

        // add to element array
        elements.push_back( elem );
    }

    if( elements.empty() )
    {
        GN_ERROR(sLogger)( "Empty input layout is not allowed." );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

///
/// Build a fake shader that can accept the input vertex format
// -----------------------------------------------------------------------------
static ID3D10Blob *
sVtxFmt2ShaderBinary( const GN::gfx::VertexFormat & vtxfmt )
{
    GN_GUARD;

    using namespace GN;
    using namespace GN::gfx;

    StrA code= "struct VS_INPUT_OUTPUT {\n";

    for( size_t i = 0; i < vtxfmt.numElements; ++i )
    {
        const GN::gfx::VertexElement & ve = vtxfmt.elements[i];

        code += strFormat( "    float4 attr%d : %s%d;\n", i, ve.binding, ve.bindingIndex );
    }

    code += "}; VS_INPUT_OUTPUT main( in VS_INPUT_OUTPUT i ) { return i; }";

    // return compiled shader binary
    return d3d10::compileShader( "vs_4_0", code.cptr(), code.size() );

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
// create D3D input layout object from vertex format structure
// -----------------------------------------------------------------------------
static ID3D11InputLayout *
sCreateD3D11InputLayout( ID3D11Device & dev, const GN::gfx::VertexFormat & format )
{
    GN_GUARD;

    using namespace GN;
    using namespace GN::gfx;

    std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
    if( !sVtxFmt2InputLayout( elements, format ) ) return false;
    GN_ASSERT( !elements.empty() );

    AutoComPtr<ID3D10Blob> bin( sVtxFmt2ShaderBinary( format ) );
    if( !bin ) return false;

    ID3D11InputLayout * layout;
    GN_DX_CHECK_RETURN(
        dev.CreateInputLayout(
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

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11VertexLayout::init(
    ID3D11Device                & dev,
    const GN::gfx::VertexFormat & format )
{
    // create D3D11 input layout object
    il.attach( sCreateD3D11InputLayout( dev, format ) );
    if( !il ) return false;

    // calculate default strides
    memset( defaultStrides, 0, sizeof(defaultStrides) );
    for( size_t i = 0; i < format.numElements; ++i )
    {
        const VertexElement & e = format.elements[i];

        GN_ASSERT( e.format.getBytesPerBlock() > 0 );
        size_t elementsize = e.offset + e.format.getBytesPerBlock();

        if( defaultStrides[e.stream] < elementsize )
        {
            defaultStrides[e.stream] = elementsize;
        }
    }

    return true;
}
