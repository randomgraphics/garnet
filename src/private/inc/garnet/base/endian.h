#ifndef __GN_BASE_ENDIAN_H__
#define __GN_BASE_ENDIAN_H__
// *****************************************************************************
/// \file
/// \brief   endian tag and routine
/// \author  chen@@CHENLI-HOMEPC (2007.4.5)
// *****************************************************************************

namespace GN
{
    //
    // Note:
    //  1. in and out could be same for in-place endian swap
    //  2. count is in unit of elements, not bytes.
    //

    ///
    /// endian swap 8 in 16.
    ///
    GN_API void Swap8In16( void * out, const void * in, size_t count );

    ///
    /// endian swap 8 in 32.
    ///
    GN_API void Swap8In32( void * out, const void * in, size_t count );

    ///
    /// endian swap 8 in 64
    ///
    GN_API void Swap8In64( void * out, const void * in, size_t count );

    ///
    /// endian swap 16 in 32.
    ///
    GN_API void Swap16In32( void * out, const void * in, size_t count );

    //@{
    inline uint16 Swap8In16( uint16 i ) { uint16 o; Swap8In16( &o, &i, 1 ); return o; }
    inline uint32 Swap8In32( uint32 i ) { uint32 o; Swap8In32( &o, &i, 1 ); return o; }
    inline uint64 Swap8In64( uint64 i ) { uint64 o; Swap8In64( &o, &i, 1 ); return o; }
    inline uint32 Swap16In32( uint32 i ) { uint32 o; Swap16In32( &o, &i, 1 ); return o; }
    //@}
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_ENDIAN_H__
