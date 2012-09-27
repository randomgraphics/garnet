#include "pch.h"
#include "d3d11hook.h"

static GN::Logger * sLogger = GN::getLogger("GN.tool.d3d11hook");

void STDMETHODCALLTYPE D3D11DeviceHook::CustomCreateBuffer_PRE(
    const D3D11_BUFFER_DESC * &,
    const D3D11_SUBRESOURCE_DATA * &)
{
    GN_INFO(sLogger)("ID3D11Device::CreateBuffer() is called.");
}
