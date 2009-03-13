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
    void swap8in16( UInt16 * out, const UInt16 * in, size_t count );

    ///
    /// endian swap 8 in 32.
    ///
    void swap8in32( UInt32 * out, const UInt32 * in, size_t count );

    ///
    /// endian swap 8 in 64
    ///
    void swap8in64( UInt64 * out, const UInt64 * in, size_t count );

    ///
    /// endian swap 16 in 32.
    ///
    void swap16in32( UInt32 * out, const UInt32 * in, size_t count );

    //@{
    inline UInt16 swap8in16( UInt16 i ) { UInt16 o; swap8in16( &o, &i, 1 ); return o; }
    inline UInt32 swap8in32( UInt32 i ) { UInt32 o; swap8in32( &o, &i, 1 ); return o; }
    inline UInt64 swap8in64( UInt64 i ) { UInt64 o; swap8in64( &o, &i, 1 ); return o; }
    inline UInt32 swap16in32( UInt32 i ) { UInt32 o; swap16in32( &o, &i, 1 ); return o; }
    //@}
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_ENDIAN_H__
