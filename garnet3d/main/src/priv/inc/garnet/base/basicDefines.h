#ifndef __GN_BASE_BASICDEFINES_H__
#define __GN_BASE_BASICDEFINES_H__
// *****************************************************************************
/// \file
/// \brief   Basic defines and types
/// \author  chenlee (2005.7.24)
// *****************************************************************************

// *****************************************************************************
// 辨识编译器
// *****************************************************************************

#define GN_MSVC  0 ///< If 1, means current compiler is msvc (or icl)
#define GN_MSVC8 0 ///< If 1, means current compiler is msvc 8+
#define GN_ICL   0 ///< If 1, means current compiler is intel c++ compiler
#define GN_MINGW 0 ///< If 1, means current compiler is MingW
#define GN_GCC   0 ///< If 1, means current compiler is gcc/g++
#define GN_BCB   0 ///< If 1, means current compiler is boland c++ compiler

/// \def GN_COMPILER
/// Indicate current compiler

#if defined(_MSC_VER) && !defined(__ICL)
#undef GN_MSVC
#undef GN_MSVC8
#define GN_MSVC 1
#define GN_MSVC8 (_MSC_VER >= 1400)
#define GN_COMPILER "msvc"

#elif defined(__ICL)
#undef GN_ICL
#undef GN_MSVC
#undef GN_MSVC8
#define GN_ICL  1
#define GN_MSVC 1                  // treat intel compiler as VC compiler
#define GN_MSVC8 (_MSC_VER >= 1400)
#define GN_COMPILER "icl"

#elif defined(__GNUC__) && defined(_WIN32)
#undef GN_MINGW
#undef GN_GCC
#define GN_MINGW 1
#define GN_GCC 1
#define GN_COMPILER "mingw"

#elif defined(__BORLANDC__)
#undef GN_BCB
#define GN_BCB 1
#define GN_COMPILER "bcb"

#elif defined(__GNUC__)
#undef GN_GCC
#define GN_GCC 1
#define GN_COMPILER "gcc"

#else
#error "Unknown compiler!"
#define GN_COMPILER "unknown"
#endif

// *****************************************************************************
// Detect Target Platform
// *****************************************************************************

#define GN_MSWIN    0 ///< Windows-based system (PC, Xbox, Phone and etc.)
#define GN_WINPC    0 ///< If 1, means current platform is Microsoft Windows on PC (not Xbox)
#define GN_WINRT    0 ///< Windows RT enabled platform (Win8 or Xbox3)
#define GN_XBOX2    0 ///< If 1, means Xbox 360
#define GN_XBOX3    0 ///< If 1, means Xbox One
#define GN_POSIX    0 ///< If 1, means POSIX compatible platform, such as linux/unix and Cygwin
#define GN_CYGWIN   0 ///< If 1, means Cygwin

// Windows platform
#if defined( _WIN32 )
#undef GN_MSWIN
#define GN_MSWIN 1
#if defined(_XBOX_VER) && _XBOX_VER == 200
#undef GN_XBOX2
#define GN_XBOX2 1
#undef GN_PLATFORM_NAME
#define GN_PLATFORM_NAME "xbox2"
#elif defined(_DURANGO)
#undef GN_XBOX3
#define GN_XBOX3 1
#undef GN_PLATFORM_NAME
#define GN_PLATFORM_NAME "xbox3"
#else
#undef GN_WINPC
#define GN_WINPC 1
#define GN_PLATFORM_NAME "mswin"
#endif

// Cygwin platform
#elif defined(__CYGWIN__)

#undef GN_CYGWIN
#define GN_CYGWIN 1

#undef GN_POSIX
#define GN_POSIX  1 // cygwin also provides some posix compabilities

#define GN_PLATFORM_NAME "cygwin"

// unix platform
#elif defined( __unix ) || defined( __unix__ )

#undef GN_POSIX
#define GN_POSIX 1

#define GN_PLATFORM_NAME "posix"

// error: unknown platform
#else
#error "unknown platform!"
#define GN_PLATFORM_NAME "unknown"
#endif

// *****************************************************************************
// 辨识CPU
// *****************************************************************************

#define GN_X86 0 ///< 32-bit x86
#define GN_X64 0 ///< 64-bit amd64
#define GN_PPC 0 ///< power pc

/// \def GN_CPU
/// Indicate current CPU

#if defined( _WIN64 ) || defined( WIN64 ) || defined(__amd64__) || defined(_M_AMD64) || defined(_AMD64_)
#undef GN_X64
#define GN_X64 1
#define GN_CPU "x64"
#elif defined( _PPC_ ) || GN_XBOX2
#undef GN_PPC
#define GN_PPC 1
#define GN_CPU "ppc"
#elif defined(_M_IX86) || defined(_X86_) || defined(i386) || defined(__i386__)
#undef GN_X86
#define GN_X86 1
#define GN_CPU "x86"
#else
#error "Unknown CPU"
#endif

// *****************************************************************************
// 辨识endian
// *****************************************************************************

#if defined(__LITTLE_ENDIAN__) || defined(__BIG_ENDIAN__)
#define GN_LITTLE_ENDIAN defined(__LITTLE_ENDIAN__) ///< true on little endian machine
#define GN_BIT_ENDIAN    defined(__BIG_ENDIAN__)    ///< true on big endian machine
#else
#define GN_LITTLE_ENDIAN  ( GN_X64 || GN_X86 ) ///< true on little endian machine
#define GN_BIG_ENDIAN     GN_PPC               ///< true on big endian machine
#endif

#if !( GN_LITTLE_ENDIAN ^ GN_BIG_ENDIAN )
#error Compile time endian detection failed!
#endif

// *****************************************************************************
// platform specific configurations
// *****************************************************************************

#if GN_XBOX3
#   include     "platform.xbox3.x64.h"
#elif GN_XBOX2
#   include     "platform.xenon.ppc.h"
#elif GN_MSWIN
#   if GN_X64
#       include "platform.mswin.x64.h"
#   elif GN_X86
#       include "platform.mswin.x86.h"
#   endif
#elif GN_POSIX
#   if GN_CYGWIN
#       include "platform.cygwin.x86.h"
#   elif GN_X64
#       include "platform.posix.x64.h"
#   elif GN_PPC
#       include "platform.posix.ppc.h"
#   elif GN_X86
#       include "platform.posix.x86.h"
#   endif
#endif

// *****************************************************************************
// 定义函数和变量声明
// *****************************************************************************

///
/// force inline macro
///
#if GN_MSVC
#define GN_FORCE_INLINE   __forceinline
#else
#define GN_FORCE_INLINE   inline
#endif

// Export/Import function/variable tag

/// \def GN_EXPORT
/// Export function tag
#if GN_MSVC
#define GN_IMPORT       __declspec(dllimport)
#else
#define GN_IMPORT
#endif

/// \def GN_IMPORT
/// Import function tag
#if GN_MSVC
#define GN_EXPORT       __declspec(dllexport)
#else
#define GN_EXPORT
#endif

///
/// Garnet public API tag.
///
/// Any public functions and classes, unless fully defined in header, should have this tag.
///
#if GN_PLATFORM_IS_STATIC
#define GN_API
#elif defined(GN_CORE_INTERNAL)
#define GN_API       GN_EXPORT
#else
#define GN_API       GN_IMPORT
#endif

///
/// Depreciated function tag
///
#define GN_DEPRECIATED

// *****************************************************************************
/// \name Call Convension
// *****************************************************************************

//@{

// Note: call convension is only avaliable for MSVC on x86 platform.
#if GN_MSVC && !GN_ICL && GN_X86
#define GN_HAS_FASTCALL 1
#define GN_FASTCALL __fastcall

#define GN_HAS_STDCALL 1
#define GN_STDCALL __stdcall

#define GN_HAS_THISCALL 1
#if _MSC_VER >= 1400
#define GN_THISCALL __thiscall
#else
#define GN_THISCALL
#endif

#define GN_HAS_CDECL 1
#define GN_CDECL __cdecl

#else
#define GN_HAS_FASTCALL 0
#define GN_FASTCALL

#define GN_HAS_STDCALL 0
#define GN_STDCALL

#define GN_HAS_THISCALL 0
#define GN_THISCALL

#define GN_HAS_CDECL 1
#define GN_CDECL
#endif

//@}

// *****************************************************************************
// Misc.
// *****************************************************************************

///
/// Define helper data and member methods, that ease defining of a "enum-like" class
///
#define GN_DEFINE_ENUM_CLASS_HELPERS( ENUM_CLASS, ENUM_TYPE ) \
    private: \
        ENUM_TYPE mValue; \
    public: \
        ENUM_CLASS() {} \
        template<typename T> \
        ENUM_CLASS( T t ) : mValue( (ENUM_TYPE)t ) {} \
        const ENUM_TYPE & toRawEnum() const { return mValue; } \
        operator const ENUM_TYPE &() const { return mValue; } \
        ENUM_CLASS & operator++() { mValue = (ENUM_TYPE)(mValue + 1); return *this; } \
        ENUM_CLASS & operator--() { mValue = (ENUM_TYPE)(mValue - 1); return *this; }

///
/// 函数名称
///
#if GN_MSVC
#define GN_FUNCTION          __FUNCTION__
#elif defined( GN_GNUC )
#define GN_FUNCTION          __PRETTY_FUNCTION__
#else
#define GN_FUNCTION          ""
#endif

#define GN_JOIN(s1, s2)          GN_JOIN_DIRECT(s1, s2) ///< join 2 symbols
#define GN_JOIN3(s1, s2, s3)     GN_JOIN( GN_JOIN( s1, s2 ), s3 ) ///< join 3 symbols
#define GN_JOIN4(s1, s2, s3, s4) GN_JOIN( GN_JOIN3( s1, s2, s3 ), s4 ) ///< join 4 symbols
#define GN_JOIN_DIRECT(s1, s2)   s1##s2 ///< Auxillary macro used by GN_JOIN

///
/// Make wide-char string
///
#define GN_WSTR(X) GN_JOIN( L, X )

///
/// Make FORCC code
///
#if GN_PPC
// big endian
#define GN_MAKE_FOURCC(ch0, ch1, ch2, ch3)                  \
    ((uint32)(uint8)(ch3) | ((uint32)(uint8)(ch2) << 8) |   \
    ((uint32)(uint8)(ch1) << 16) | ((uint32)(uint8)(ch0) << 24 ))
#else
// little endian
#define GN_MAKE_FOURCC(ch0, ch1, ch2, ch3)                  \
    ((uint32)(uint8)(ch0) | ((uint32)(uint8)(ch1) << 8) |   \
    ((uint32)(uint8)(ch2) << 16) | ((uint32)(uint8)(ch3) << 24 ))
#endif

/// Compile time assert helper templates
//@{
template<bool>   struct COMPILE_TIME_ASSERT_FAILURE;
template<>       struct COMPILE_TIME_ASSERT_FAILURE<true>{ enum { value = 1 }; };
template<int>    struct COMPILE_TIME_ASSERT_TEST {};
//@}

///
/// Compile time assert
///
#define GN_CASSERT( exp ) \
    typedef COMPILE_TIME_ASSERT_TEST<sizeof(COMPILE_TIME_ASSERT_FAILURE<exp>::value)> \
    GN_JOIN(__GN_cassert_,__LINE__)

///
/// define handle type
///
#define GN_DEFINE_HANDLE( type ) typedef struct type##Struct {} * type

namespace GN
{
    ///
    /// Disable copy semantic of all descendants.
    ///
    class NoCopy
    {
    protected:
        ///
        /// Default constructor
        ///
        NoCopy() {}
        ///
        /// Destructor
        ///
        virtual ~NoCopy() {}
    private:  // emphasize the following members are private
        NoCopy( const NoCopy & );
        const NoCopy & operator = ( const NoCopy& );
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_BASICDEFINES_H__
