#include "pch.h"

namespace GN { namespace gfx
{
    const Guid GPU_DLL_GUID = { 0xa397a51a, 0xa008, 0x4ffd, { 0xbd, 0x8a, 0x7f, 0xd4, 0xa3, 0xaf, 0x59, 0xd7 } };

    //
    //
    // -------------------------------------------------------------------------
    GN_API void deleteGpu( Gpu * r )
    {
        GN_GUARD;

        if( r )
        {
            bool hasdll = r->hasUserData( GPU_DLL_GUID );
            SharedLib * dll = NULL;
            if( hasdll ) r->getUserData( GPU_DLL_GUID, &dll, sizeof(dll) );
            delete r;
            if( hasdll ) delete dll;
        }

        GN_UNGUARD;
    }
}}
