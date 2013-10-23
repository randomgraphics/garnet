#include "pch.h"
#include "interfacebase.h"
#include "d3d/d3d11_1.h"

static const InterfaceDesc sAllD3D11Interfaces[] =
{
    { __uuidof(IUnknown), "IUnknown", L"IUnknown" },
#define DECLARE_D3D11_INTERFACE(x) { __uuidof(x), #x, L#x },
#include "d3d11interfaces_meta.h"
};

const InterfaceDesc * InterfaceDesc::sGetDescFromIID(const IID & iid)
{
    for(size_t i = 0; i < _countof(sAllD3D11Interfaces); ++i)
    {
        const InterfaceDesc & id = sAllD3D11Interfaces[i];
        if (id.iid == iid) return &id;
    }
    return nullptr;
}
