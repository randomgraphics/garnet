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

#undef _In_reads_opt_
#define _In_reads_opt_(x)

#undef _Inexpressible_
#define _Inexpressible_(x)

#undef _In_reads_
#define _In_reads_(x)

#undef _In_reads_bytes_
#define _In_reads_bytes_(x)

#undef _Out_writes_
#define _Out_writes_(x)

#undef _Out_writes_opt_
#define _Out_writes_opt_(x)

#undef _Out_writes_to_opt_
#define _Out_writes_to_opt_(x,y)

#undef _Out_writes_bytes_
#define _Out_writes_bytes_(x)

#undef _Out_writes_bytes_opt_
#define _Out_writes_bytes_opt_(x)

#undef _In_reads_bytes_opt_
#define _In_reads_bytes_opt_(x)

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_PCH_H__
