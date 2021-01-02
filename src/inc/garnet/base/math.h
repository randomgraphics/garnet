#ifndef __GN_BASE_MATH_H__
#define __GN_BASE_MATH_H__
// *****************************************************************************
/// \file
/// \brief   basic mathmatic functions
/// \author  chenlee (2005.4.17)
// *****************************************************************************

#include <math.h>

///
/// 圆周率
///
#define GN_PI       3.1415926535897932385f

///
/// 圆周率/2
///
#define GN_HALF_PI  (GN_PI / 2.0f)

///
/// 圆周率*2
///
#define GN_TWO_PI   (GN_PI * 2.0f)

namespace GN
{
    ///
    /// namespace for math helpers
    ///
    namespace math
    {

        ///
        /// floating equality check with epsilon
        // --------------------------------------------------------------------
        template<typename T>
        GN_FORCE_INLINE bool
        floatEqual( const T & a, const T & b, const T & epsilon = (T)0.0000001 )
        {
            T diff = a - b;
            return -epsilon < diff && diff < epsilon;
        }

        ///
        /// 角度->弧度
        // --------------------------------------------------------------------
        template<typename T>
        GN_FORCE_INLINE T deg2rad( T a ) { return a*(T)0.01745329252f; }

        ///
        /// 弧度->角度
        // --------------------------------------------------------------------
        template<typename T>
        GN_FORCE_INLINE T rad2deg( T a ) { return a*(T)57.29577951f;   }

        ///
        /// 检查n是否为2^n
        // --------------------------------------------------------------------
        template<typename T>
        GN_FORCE_INLINE constexpr bool isPowerOf2( T n )
        {
            return ( 0 == (n & (n - 1)) ) && ( 0 != n );
        }

        ///
        /// 返回不小于n的最小的2的整幂
        // --------------------------------------------------------------------
        GN_FORCE_INLINE constexpr uint32 ceilPowerOf2( uint32 n )
        {
            n -= 1;

            n |= n >> 16;
            n |= n >> 8;
            n |= n >> 4;
            n |= n >> 2;
            n |= n >> 1;

            return n + 1;
        }

        ///
        /// 返回不大于n的最大的2的整幂
        // --------------------------------------------------------------------
        GN_FORCE_INLINE constexpr uint32 floorPowerOf2( uint32 n )
        {
            n |= n >> 16;
            n |= n >> 8;
            n |= n >> 4;
            n |= n >> 2;
            n |= n >> 1;

            return (n + 1) >> 1;
        }

#if GN_X64

        ///
        /// 返回不小于n的最小的2的整幂
        // --------------------------------------------------------------------
        GN_FORCE_INLINE size_t ceilPowerOf2( size_t n )
        {
            n -= 1;

            n |= n >> 32;
            n |= n >> 16;
            n |= n >> 8;
            n |= n >> 4;
            n |= n >> 2;
            n |= n >> 1;

            return n + 1;
        }

        ///
        /// 返回不大于n的最大的2的整幂
        // --------------------------------------------------------------------
        GN_FORCE_INLINE size_t floorPowerOf2( size_t n )
        {
            n |= n >> 32;
            n |= n >> 16;
            n |= n >> 8;
            n |= n >> 4;
            n |= n >> 2;
            n |= n >> 1;

            return (n + 1) >> 1;
        }

#endif

        ///
        /// 将value限定在[vmin, vmax]区间内
        // --------------------------------------------------------------------
        template < typename T >
        inline T clamp( T value, const T & vmin, const T & vmax )
        {
            value = vmin > value ? vmin : vmax < value ? vmax : value;
            return value;
        }

        ///
        /// align numeric value up to next multiple of alignment.
        /// Note that the alignment must be 2^N.
        // --------------------------------------------------------------------
        template< typename T >
        inline T alignToPowerOf2( const T & value, const T & alignment )
        {
            GN_ASSERT( isPowerOf2( alignment ) );
            return ( value + (alignment-1) ) & ~(alignment-1);
        }

        ///
        /// get minimal value
        ///
        template<typename T> inline const T & getmin( const T & a, const T & b ) { return a < b ? a : b; }

        ///
        /// get minimal value
        ///
        template<typename T> inline const T & getmin( const T & a, const T & b, const T & c )
        {
            return a < b ? ( c < a ? c : a ) : ( c < b ? c : b );
        }

        ///
        /// get maximal value
        ///
        template<typename T> inline const T & getmax( const T & a, const T & b ) { return a > b ? a : b; }

        ///
        /// get maximal value
        ///
        template<typename T> inline const T & getmax( const T & a, const T & b, const T & c )
        {
            return a > b ? ( c > a ? c : a ) : ( c > b ? c : b );
        }
    }
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_MATH_H__
