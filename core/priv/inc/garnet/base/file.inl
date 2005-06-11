#include <boost/type_traits/is_const.hpp>
#include <boost/static_assert.hpp>

//
//
//  ----------------------------------------------------------------------------
template<typename T> inline void
GN::MemFile<T>::reset( T * buf, size_t size, const StrA & name )
{
    mStart = (uint8_t*)buf;
    mPtr   = mStart;
    mSize  = size;
    setName( name );
}

//
//
//  ----------------------------------------------------------------------------
template<typename T> inline size_t
GN::MemFile<T>::read( void * buf, size_t size )
{
    GN_GUARD;

    // check for no-op
    if( 0 == size ) return 0;

    // check parameter
    if( 0 == buf )
    {
        GN_ERROR( "null output buf!" );
        return size_t(-1);
    }

    // check for null file
    if( 0 == mPtr )
    {
        GN_ERROR( "null file!" );
        return size_t(-1);
    }

    GN_ASSERT( mStart && mStart <= mPtr && mPtr <= (mStart+mSize) );

    size = std::min<size_t>( size, mStart + mSize - mPtr );

    memcpy( buf, mPtr, size );

    mPtr += size;
    GN_ASSERT( mPtr <= (mStart+mSize) );

    // success
    return size;

    GN_UNGUARD;
}

//
//
//  ----------------------------------------------------------------------------
template<typename T> inline size_t
GN::MemFile<T>::write( const void * buf, size_t size )
{
    GN_GUARD;

    // check for constness
    BOOST_STATIC_ASSERT( !boost::is_const<T>::value );

    // check for no-op
    if( 0 == size ) return 0;

    // check parameter
    if( 0 == buf )
    {
        GN_ERROR( "null input buf!" );
        return size_t(-1);
    }

    // check for null file
    if( 0 == mPtr )
    {
        GN_ERROR( "null file!" );
        return size_t(-1);
    }

    GN_ASSERT( mStart && mStart <= mPtr && mPtr <= (mStart+mSize) );

    size = std::min<size_t>( size, mStart + mSize - mPtr );

    memcpy( mPtr, buf, size );

    mPtr += size;
    GN_ASSERT( mPtr <= (mStart+mSize) );

    // success
    return size;

    GN_UNGUARD;
}

//
//
//  ----------------------------------------------------------------------------
template<typename T> inline bool
GN::MemFile<T>::seek( int offset, FileSeekMode origin )
{
    uint8_t * end = mStart + mSize;
    uint8_t * ptr;
    if( FSEEK_CUR == origin )
    {
        ptr = mPtr + offset;
    }
    else if( FSEEK_END == origin )
    {
        ptr = end + offset;
    }
    else if( FSEEK_SET == origin )
    {
        ptr = mStart + offset;
    }
    else
    {
        GN_ERROR( "invalid seek origin!" );
        return false;
    }

    if( ptr < mStart || ptr > end )
    {
        GN_ERROR( "out of range" );
        return false;
    }
    mPtr = ptr;
    GN_ASSERT( mStart <= mPtr && mPtr <= (mStart+mSize) );
    return true;
}
