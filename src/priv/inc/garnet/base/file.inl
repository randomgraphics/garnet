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

    size = min<size_t>( size, mStart + mSize - mPtr );

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

    // T can't be a constant type
    GN_CASSERT( !IsConst<T>::value );

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

    size = min<size_t>( size, mStart + mSize - mPtr );

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


//
//
//  ----------------------------------------------------------------------------
inline size_t GN::VectorFile::read( void * buf, size_t size )
{
    GN_ASSERT( mCursor <= mBuffer.size() );

    if( size + mCursor > mBuffer.size() ) size = mBuffer.size() - mCursor;

    if( 0 == size ) return 0;

    if( 0 == buf )
    {
        GN_ERROR( "null output buffer!" );
        return size_t(-1);
    }

    memcpy( buf, &mBuffer[mCursor], size );

    mCursor += size;
    GN_ASSERT( mCursor <= mBuffer.size() );

    return size;
}

//
//
//  ----------------------------------------------------------------------------
inline size_t GN::VectorFile::write( const void * buf, size_t size )
{
    GN_ASSERT( mCursor <= mBuffer.size() );

    if( 0 == size ) return 0;

    if( 0 == buf )
    {
        GN_ERROR( "null output buffer!" );
        return size_t(-1);
    }

    // resize the vector on demond
    if( size + mCursor > mBuffer.size() )
    {
        mBuffer.resize( size + mCursor );
    }

    memcpy( &mBuffer[mCursor], buf, size );

    mCursor += size;
    GN_ASSERT( mCursor <= mBuffer.size() );

    return size;
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
        GN_ERROR( "invalid seek origin!" );
        return false;
    }

    if( cur > mBuffer.size() )
    {
        GN_ERROR( "out of range" );
        return false;
    }
    mCursor = cur;
    return true;
}
