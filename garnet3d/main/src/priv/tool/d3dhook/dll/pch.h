#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   Precompiled header
// \author  chenlee (2005.8.7)
// *****************************************************************************

#include "garnet/GNbase.h"

#if GN_XBOX2
#include <xtl.h>
#elif GN_WINPC
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
#include <stdio.h>

//
//
// -----------------------------------------------------------------------------
inline const wchar_t * GetRealDllPath(const wchar_t * dllName)
{
    static __declspec(thread) wchar_t dllpath[256];
    GetSystemDirectoryW(dllpath, _countof(dllpath));
    wcscat_s(dllpath, L"\\");
    wcscat_s(dllpath, dllName);
    return dllpath;
}

#undef _In_reads_opt_
#define _In_reads_opt_(x)

#undef _Inexpressible_
#define _Inexpressible_(x)

#undef _In_reads_
#define _In_reads_(x)

#undef _In_reads_bytes_
#define _In_reads_bytes_(x)

#undef _In_reads_bytes_opt_
#define _In_reads_bytes_opt_(x)

#undef _Inout_updates_bytes_
#define _Inout_updates_bytes_(x)

#undef _Out_writes_
#define _Out_writes_(x)

#undef _Out_writes_opt_
#define _Out_writes_opt_(x)

#undef _Out_writes_bytes_to_
#define _Out_writes_bytes_to_(x,y)

#undef _Out_writes_all_opt_
#define _Out_writes_all_opt_(x)

#undef _Out_writes_to_opt_
#define _Out_writes_to_opt_(x,y)

#undef _Out_writes_bytes_
#define _Out_writes_bytes_(x)

#undef _Out_writes_bytes_opt_
#define _Out_writes_bytes_opt_(x)

#undef _Outptr_
#define _Outptr_

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_PCH_H__
