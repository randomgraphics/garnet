//
//
//  ----------------------------------------------------------------------------
template<typename T> inline void
GN::MemFile<T>::reset( T * buf, size_t size, const StrA & name )
{
    mStart = (UInt8*)buf;
    mPtr   = mStart;
    mSize  = size;
    setName( name );
}

//
//
//  ----------------------------------------------------------------------------
template<typename T> inline bool
GN::MemFile<T>::read( void * buf, size_t size, size_t * readen )
{
    GN_GUARD;

    // check for no-op
    if( 0 == size )
    {
        if( readen ) *readen = 0;
        return true;
    }

    // check parameter
    if( 0 == buf )
    {
        GN_ERROR(sLogger)( "null output buf!" );
        return false;
    }

    // check for null file
    if( 0 == mPtr )
    {
        GN_ERROR(sLogger)( "null file!" );
        return false;
    }

    GN_ASSERT( mStart && mStart <= mPtr && mPtr <= (mStart+mSize) );

    size = min<size_t>( size, mStart + mSize - mPtr );

    memcpy( buf, mPtr, size );

    mPtr += size;
    GN_ASSERT( mPtr <= (mStart+mSize) );

    // success
    if( readen ) *readen = size;
    return true;

    GN_UNGUARD;
}

//
//
//  ----------------------------------------------------------------------------
template<typename T> inline bool
GN::MemFile<T>::write( const void * buf, size_t size, size_t * written )
{
    GN_GUARD;

    // T can't be a constant type
    GN_CASSERT( !IsConst<T>::value );

    // check for no-op
    if( 0 == size )
    {
        if( written ) *written = 0;
        return true;
    }

    // check parameter
    if( 0 == buf )
    {
        GN_ERROR(sLogger)( "null input buf!" );
        return false;
    }

    // check for null file
    if( 0 == mPtr )
    {
        GN_ERROR(sLogger)( "null file!" );
        return false;
    }

    GN_ASSERT( mStart && mStart <= mPtr && mPtr <= (mStart+mSize) );

    size = min<size_t>( size, mStart + mSize - mPtr );

    memcpy( mPtr, buf, size );

    mPtr += size;
    GN_ASSERT( mPtr <= (mStart+mSize) );

    // success
    if( written ) *written = size;
    return true;

    GN_UNGUARD;
}

//
//
//  ----------------------------------------------------------------------------
template<typename T> inline bool
GN::MemFile<T>::seek( int offset, FileSeekMode origin )
{
    UInt8 * end = mStart + mSize;
    UInt8 * ptr;
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
        GN_ERROR(sLogger)( "invalid seek origin!" );
        return false;
    }

    if( ptr < mStart || ptr > end )
    {
        GN_ERROR(sLogger)( "out of range" );
        return false;
    }
    mPtr = ptr;
    GN_ASSERT( mStart <= mPtr && mPtr <= (mStart+mSize) );
    return true;
}

//
//
//  ----------------------------------------------------------------------------
inline bool GN::VectorFile::read( void * buf, size_t size, size_t * readen )
{
    GN_ASSERT( mCursor <= mBuffer.size() );

    if( size + mCursor > mBuffer.size() ) size = mBuffer.size() - mCursor;

    if( 0 == size )
    {
        if( readen ) *readen = 0;
        return true;
    }

    if( 0 == buf )
    {
        GN_ERROR(sLogger)( "null output buffer!" );
        return false;
    }

    memcpy( buf, &mBuffer[mCursor], size );

    mCursor += size;
    GN_ASSERT( mCursor <= mBuffer.size() );

    if( readen ) *readen = size;
    return true;
}

//
//
//  ----------------------------------------------------------------------------
inline bool GN::VectorFile::write( const void * buf, size_t size, size_t * written )
{
    GN_ASSERT( mCursor <= mBuffer.size() );

    if( 0 == size )
    {
        if( written ) *written = 0;
        return true;
    }

    if( 0 == buf )
    {
        GN_ERROR(sLogger)( "null output buffer!" );
        return false;
    }

    // resize the vector on demond
    if( size + mCursor > mBuffer.size() )
    {
        mBuffer.resize( size + mCursor );
    }

    memcpy( &mBuffer[mCursor], buf, size );

    mCursor += size;
    GN_ASSERT( mCursor <= mBuffer.size() );

    if( written ) *written = size;
    return true;
}

//
//
//  ----------------------------------------------------------------------------
inline bool GN::VectorFile::seek( int offset, FileSeekMode origin )
{
    size_t cur;
    if( FSEEK_CUR == origin )
    {
        cur = mCursor + (size_t)offset;
    }
    else if( FSEEK_END == origin )
    {
        cur = mBuffer.size() + (size_t)offset;
    }
    else if( FSEEK_SET == origin )
    {
        cur = (size_t)offset;
    }
    else
    {
        GN_ERROR(sLogger)( "invalid seek origin!" );
        return false;
    }

    if( cur > mBuffer.size() )
    {
        GN_ERROR(sLogger)( "out of range" );
        return false;
    }
    mCursor = cur;
    return true;
}