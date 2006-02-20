#ifndef __GN_EXTERN_FOXALL_H__
#define __GN_EXTERN_FOXALL_H__
// *****************************************************************************
//! \file    inc/foxAll.h
//! \brief   Fox header wrapper
//! \author  chenlee (2006.2.19)
// *****************************************************************************

#ifndef _XBOX_VER

// disable vc8.0 CRT warnings
#if _MSC_VER >= 1400
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif
#endif

#if defined(_WIN32) && !defined(WIN32)
#define WIN32
#endif

#if defined(__ICL)
#pragma warning(disable:193) // zero used for undefined preprocessing identifier
#endif

#if defined(_MSC_VER) && !defined(__ICL)
#pragma warning(disable:4244) // 从“int”转换到“FX::FXshort”，可能丢失数据
#endif

#include "fox/fx.h"

#if defined(__ICL)
#pragma warning(default:193)
#endif

#if defined(_MSC_VER) && !defined(__ICL)
#pragma warning(default:4244)
#endif

#endif // !_XBOX_VER

// *****************************************************************************
//                           End of foxAll.h
// *****************************************************************************
#endif // __GN_EXTERN_FOXALL_H__
