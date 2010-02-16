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
    void SwapEndian8In16( void * out, const void * in, size_t count );

    ///
    /// endian swap 8 in 32.
    ///
    void SwapEndian8In32( void * out, const void * in, size_t count );

    ///
    /// endian swap 8 in 64
    ///
    void SwapEndian8In64( void * out, const void * in, size_t count );

    ///
    /// endian swap 16 in 32.
    ///
    void SwapEndian16In32( void * out, const void * in, size_t count );

    //@{
    inline UInt16 SwapEndian8In16( UInt16 i ) { UInt16 o; SwapEndian8In16( &o, &i, 1 ); return o; }
    inline UInt32 SwapEndian8In32( UInt32 i ) { UInt32 o; SwapEndian8In32( &o, &i, 1 ); return o; }
    inline UInt64 SwapEndian8In64( UInt64 i ) { UInt64 o; SwapEndian8In64( &o, &i, 1 ); return o; }
    inline UInt32 SwapEndian16In32( UInt32 i ) { UInt32 o; SwapEndian16In32( &o, &i, 1 ); return o; }
    //@}
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_ENDIAN_H__
