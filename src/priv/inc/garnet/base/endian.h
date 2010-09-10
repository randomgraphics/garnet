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
    void swap8in16( void * out, const void * in, size_t count );

    ///
    /// endian swap 8 in 32.
    ///
    void swap8in32( void * out, const void * in, size_t count );

    ///
    /// endian swap 8 in 64
    ///
    void swap8in64( void * out, const void * in, size_t count );

    ///
    /// endian swap 16 in 32.
    ///
    void swap16in32( void * out, const void * in, size_t count );

    //@{
    inline uint16 swap8in16( uint16 i ) { uint16 o; swap8in16( &o, &i, 1 ); return o; }
    inline uint32 swap8in32( uint32 i ) { uint32 o; swap8in32( &o, &i, 1 ); return o; }
    inline uint64 swap8in64( uint64 i ) { uint64 o; swap8in64( &o, &i, 1 ); return o; }
    inline uint32 swap16in32( uint32 i ) { uint32 o; swap16in32( &o, &i, 1 ); return o; }
    //@}
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_ENDIAN_H__
