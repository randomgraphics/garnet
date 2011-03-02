#ifndef __GN_BASE_VEC128_H__
#define __GN_BASE_VEC128_H__
// *****************************************************************************
/// \file
/// \brief   SIMD optimized vector opertions
/// \author  chen@@CHENLI-OLDPC (2011.2.24)
// *****************************************************************************

#if GN_PLATFORM_HAS_XNAMATH
#if GN_XENON
#include <xtl.h>
#else
#include <windows.h>
#endif
#include <xnamath.h>
#endif

/*
#if GN_MSVC
#define GN_MSVC_ALIGN16 __declspec( align(16) )
#else
#define GN_MSVC_ALIGN16
#endif

#if GN_GCC
#define GN_GCC_ALIGN16 __attribute__ ((aligned (16)))
#else
#define GN_GCC_ALIGN16
#endif
*/

namespace GN
{
    namespace simd
    {
        struct InstructionSet
        {
            enum ENUM
            {
                SSE  = 1<<0,
                SSE2 = 1<<1,
                SSE3 = 1<<2,
                SSE4 = 1<<2,
            };
        };

        uint32 isCPUSupportSIMD();

    #if GN_PLATFORM_HAS_XNAMATH
        typedef XMVECTOR     vec4;
        typedef const vec4   cvec4;
    #else
        struct vec4
        {
            union
            {
                struct { float x, y, z, w; };
                float  f32[4];
                sint8  s8[16];
                sint16 s16[8];
                sint32 s32[4];
                sint64 s64[2];
                uint8  u8[16];
                uint16 u16[8];
                uint32 u32[4];
                uint64 u64[2];
            };

            vec4 operator+( const vec4 & v ) const
            {
                vec4 r;
                r.x = x + v.x;
                r.y = y + v.y;
                r.z = z + v.z;
                r.w = w + v.w;
                return r;
            }
        };
        typedef const vec4 & cvec4;
    #endif

        inline float getx( cvec4 v )
        {
        #if GN_PLATFORM_HAS_XNAMATH
            return XMVectorGetX( v );
        #else
            return v.x;
        #endif
        }

        inline float gety( cvec4 v )
        {
        #if GN_PLATFORM_HAS_XNAMATH
            return XMVectorGetY( v );
        #else
            return v.y;
        #endif
        }

        inline float getz( cvec4 v )
        {
        #if GN_PLATFORM_HAS_XNAMATH
            return XMVectorGetZ( v );
        #else
            return v.z;
        #endif
        }

        inline float getw( cvec4 v )
        {
        #if GN_PLATFORM_HAS_XNAMATH
            return XMVectorGetW( v );
        #else
            return v.w;
        #endif
        }

        inline vec4 setf4( float x, float y, float z, float w )
        {
        #if GN_PLATFORM_HAS_XNAMATH
            return XMVectorSet( x, y, z, w );
        #else
            vec4 v = { { { x, y, z, w } } };
            return v;
        #endif
        }

        /// Row major matrix using 128bit optimized instruction set.
        struct _mat44_
        {
            /// Data members
            //@{
            union
            {
                vec4     r[4];  //< 4 rows

                // While using optimized instruction set, like SSE or VMX,
                // you should not access these fields directly. You can,
                // however, see these types in the debugger.
                float    m[16]; //< 16 elements
                struct
                {
                    float m11, m12, m13, m14;
                    float m21, m22, m23, m24;
                    float m31, m32, m33, m34;
                    float m41, m42, m43, m44;
                };
            };
            //@}
        };

        /// Row major matrix using 128bit optimized instruction set.
        struct mat44 : public _mat44_
        {
        };

    }
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_VEC128_H__
