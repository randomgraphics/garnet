#include "pch.h"
#include "hooks9.h"

static const InterfaceDesc sAllD3D9Interfaces[] =
{
    { __uuidof(IUnknown), "IUnknown", L"IUnknown" },
#define DECLARE_D3D9_INTERFACE(x) { __uuidof(x), #x, L#x },
#include "d3d9interfaces_meta.h"
};

const InterfaceDesc * InterfaceDesc::sGetDescFromIID(const IID & iid)
{
    for(size_t i = 0; i < _countof(sAllD3D9Interfaces); ++i)
    {
        const InterfaceDesc & id = sAllD3D9Interfaces[i];
        if (id.iid == iid) return &id;
    }
    return nullptr;
}
