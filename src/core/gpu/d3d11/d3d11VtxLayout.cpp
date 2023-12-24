#include "pch.h"
#include "d3d11Gpu.h"
#include "d3d11VtxLayout.h"
#include "d3d11shader.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D11.VtxLayout");

// *****************************************************************************
// local functions
// *****************************************************************************

///
/// convert vertdecl structure to a D3D vertex declaration array
// -----------------------------------------------------------------------------
static bool sVtxBind2D3D11InputLayout(GN::DynaArray<D3D11_INPUT_ELEMENT_DESC> & elements, const GN::gfx::VertexBinding & vtxbind,
                                      const GN::gfx::D3D11GpuProgram & gpuProgram) {
    GN_GUARD;

    using namespace GN;
    using namespace GN::gfx;

    elements.clear();

    for (size_t i = 0; i < vtxbind.size(); ++i) {
        const VertexElement & ve = vtxbind[i];

        D3D11_INPUT_ELEMENT_DESC elem;

        // set attribute semantic
        elem.SemanticName = gpuProgram.getAttributeSemantic(ve.attribute, &elem.SemanticIndex);
        if (NULL == elem.SemanticName) return false;

        // set attrib format
        elem.Format = (DXGI_FORMAT) ve.format.toDXGI();
        if (DXGI_FORMAT_UNKNOWN == elem.Format) {
            GN_ERROR(sLogger)("Unknown element format: %s", ve.format.toString().c_str());
            return false;
        }

        // set stream index
        elem.InputSlot = ve.stream;

        // set attrib offset
        elem.AlignedByteOffset = ve.offset;

        // instancing attributes
        elem.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
        elem.InstanceDataStepRate = 0;

        // add to element array
        elements.append(elem);
    }

    if (elements.empty()) {
        GN_ERROR(sLogger)("Empty input layout is not allowed.");
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11VertexLayout::init(ID3D11Device & dev, const GN::gfx::VertexBinding & vtxbind, const D3D11GpuProgram & gpuProgram) {
    DynaArray<D3D11_INPUT_ELEMENT_DESC> elements;
    if (!sVtxBind2D3D11InputLayout(elements, vtxbind, gpuProgram)) return false;
    GN_ASSERT(!elements.empty());

    size_t       signatureSize;
    const void * signature = gpuProgram.getInputSignature(&signatureSize);
    if (NULL == signature) return false;

    GN_DX_CHECK_RETURN(dev.CreateInputLayout(&elements[0], (UINT) elements.size(), signature, signatureSize, &il), false);

    return true;
}
