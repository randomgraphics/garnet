#pragma once
#include "vtable.h"
#include "d3d/d3d11_1.h"
#include "d3d/dxgidebug.h"

#ifndef BEGIN_INTERFACE
#define BEGIN_INTERFACE
#define END_INTERFACE
#endif

// -----------------------------------------------------------------------------
struct D3D11VTables;
extern D3D11VTables g_D3D11OriginVTables;
extern D3D11VTables g_D3D11HookedVTables;

// -----------------------------------------------------------------------------
template<size_t COUNT>
inline void RealToHooked11_General(void** vtable, void** origin, void** hooked, const char * interfaceName)
{
    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update %s vtable: changing page protection failed.", interfaceName);
        return;
    }

    if (!origin[0])
    {
        void ** vtbegin = (void**)&g_D3D11OriginVTables;
        void ** vtend   = (void**)(((const BYTE*)&g_D3D11OriginVTables) + sizeof(g_D3D11OriginVTables));

        for(size_t i = 0; i < COUNT; ++i)
        {
            if (vtable[i] >= vtbegin && vtable[i] < vtend)
            {
                // the method is pointing to a hooked method, which is unexpected
                HOOK_RIP();
            }
            else
            {
                origin[i] = vtable[i];
                if (!hooked[i]) hooked[i] = vtable[i];
            }
        }
    }

    HOOK_ASSERT( 0 == memcmp(vtable, origin, sizeof(void*) * COUNT) );
    memcpy(vtable, hooked, sizeof(void*) * COUNT);
}

// -----------------------------------------------------------------------------
template<typename T> void RealToHooked11(T * p);

#include "d3d11vtable.inl"
