#include <garnet/GNbase.h>

#if GN_PLATFORM_HAS_XNAMATH
#include <xnamath.h>
#endif

namespace GN
{
    /// 128bit vector class for optimized instruction set.
    __declspec( align(16) )
    union VEC128
    {
        // While using optimized instruction set, like SSE or VMX,
        // you should not access these fields directly. You can,
        // however, see these types in the debugger.
        private char             i8[16];
        private short            i16[8];
        private int              i32[4];
        private int64_t          i64[2];
        private unsigned char    u8[16];
        private unsigned short   u16[8];
        private unsigned int     u32[4];
        private unsigned int64_t u64[2];
        #if GN_PLATFORM_HAS_XNAMATH
        private XMVECTOR         xna;
        #endif

        float x() const
        {
        }

        float y() const
        {
        }

        float z() const
        {
        }

        float w() const
        {
        }
    };
};
