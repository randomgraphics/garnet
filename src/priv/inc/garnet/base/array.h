#ifndef __GN_BASE_ARRAY_H__
#define __GN_BASE_ARRAY_H__
// *****************************************************************************
/// \file
/// \brief   Common array (vector) class
/// \author  chenlee (2006.5.31)
// *****************************************************************************

namespace GN
{
    ///
    /// Fixed sized array, which always has N elements.
    /// Behaves like C-style array, but with bound check in debug build.
    ///
    template<class T, size_t N>
    class FixedArray
    {
        T mElements[N];

    public:

        typedef T ElementType; ///< element type

        static const size_t MAX_SIZE = N; ///< maximum size

        ///
        /// convert to C pointer
        ///
        const T* cptr() const { return mElements; }

        ///
        /// convert to C pointer
        ///
        T* cptr() { return mElements; }

        ///
        /// at operator
        ///
        T & operator[]( size_t i ) { GN_ASSERT( i < N ); return mElements[i]; }

        ///
        /// at operator
        ///
        const T & operator[]( size_t i ) const { GN_ASSERT( i < N ); return mElements[i]; }
    };

    ///
    /// Fixed size array with supporting to common array operations
    /// like push, pop, insert, remove and etc.
    ///
    template<class T, size_t N>
    class StackArray
    {
        T mElements[N];
        size_t mCount;

        static Logger * sLogger;

        void copyElements( T * dst, const T * src, size_t count )
        {
            GN_ASSERT( 0 == count || dst && src );
            for( size_t i = 0; i < count; ++i )
            {
                dst[i] = src[i];
            }
        }

        void doClone( const StackArray & other )
        {
            copyElements( mElements, other.mElements, other.mCount );
            mCount = other.mCount;
        }

        void doInsert( size_t position, const T & t )
        {
            GN_ASSERT( mCount <= N );
            if( N == mCount ) { GN_WARN(sLogger)( "Can't insert more. Stack array is full already!" ); return; }
            if( position > mCount ) { GN_WARN(sLogger)( "invalid insert position." ); return; }
            for( size_t i = mCount; i > position; --i )
            {
                mElements[i] = mElements[i-1];
            }
            mElements[position] = t;
            ++mCount;
        }

        void doErase( size_t position )
        {
            if( position >= mCount ) { GN_WARN(sLogger)( "Invalid erase position" ); return; }
            --mCount;
            for( size_t i = position; i < mCount; ++i )
            {
                mElements[i] = mElements[i+1];
            }
        }

        bool equal( const StackArray & other ) const
        {
            if( mCount != other.mCount ) return false;
            return 0 == ::memcmp( mElements, other.mElements, sizeof(T)*mCount );
        }

    public:

        typedef T ElementType; ///< element type

        static const size_t MAX_SIZE = N; ///< maximum size

        ///
        /// default constructor
        ///
        StackArray() : mCount(0) {}

        ///
        /// constructor with user-defined count.
        ///
        explicit StackArray( size_t count ) : mCount(count) {}

        ///
        /// copy constructor
        ///
        StackArray( const StackArray & other ) { doClone( other ); }

        /// \name Common array operations.
        ///
        //@{
        void      append( const T & t ) { doInsert( mCount, t ); }
        const T & back() const { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        T       & back() { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        const T * begin() const { return mElements; }
        T       * begin() { return mElements; }
        void      clear() { mCount = 0; }
        const T * cptr() const { return mElements; }
        T       * cptr() { return mElements; }
        bool      empty() const { return 0 == mCount; }
        const T * end() const { return mElements + mCount; }
        T       * end() { return mElements + mCount; }
        /** do nothing if position is invalid or array is empty */
        void      erase( size_t position ) { doErase( position ); }
        const T & front() const { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        T       & front() { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        /** do nothing if position is invalid or array is full */
        void      insert( size_t position, const T & t ) { doInsert( position, t ); }
        void      resize( size_t count ) { if( count > N ) { GN_WARN(sLogger)("count is too large!"); count = N; } mCount = count; }
        void      pushBack( const T & t ) { doInsert( size(), t ); }
        void      popBack() { if( mCount > 0 ) --mCount; }
        size_t    size() const { return mCount; }
        //@}

        /// \name common operators
        ///
        //@{
        StackArray & operator=( const StackArray & other ) { doClone(other); return *this; }
        bool         operator==( const StackArray & other ) const { return equal(other); }
        bool         operator!=( const StackArray & other ) const { return !equal(other); }
        T          & operator[]( size_t i ) { GN_ASSERT( i < mCount ); return mElements[i]; }
        const T    & operator[]( size_t i ) const { GN_ASSERT( i < mCount ); return mElements[i]; }
        //@}
    };

    template<class T, size_t N> Logger * StackArray<T,N>::sLogger = getLogger("GN.base.StackArray");

    ///
    /// Resizeable array.
    ///
    template<class T, class ALLOCATOR = StandardAllocator<T> >
    class DynaArray
    {
        T * mElements;
        size_t mCount;
        size_t mCapacity;

        static Logger * sLogger;

        void dealloc()
        {
            if( mElements )
            {
                GN_ASSERT( mCapacity > 0 );
                AllocatorType::sDealloc( mElements, mCapacity );
            }
        }

        void copyElements( T * dst, const T * src, size_t count )
        {
            GN_ASSERT( 0 == count || dst && src );
            for( size_t i = 0; i < count; ++i )
            {
                dst[i] = src[i];
            }
        }

        void doAppend( const T * p, size_t count )
        {
            if( 0 == p ) count = 0;
            if( 0 == count ) return;
            size_t oldCount = mCount;
            resize( mCount + count );
            copyElements( mElements + oldCount, p, count );
        }

        void doClone( const DynaArray & other )
        {
            resize( other.mCount );
            GN_ASSERT( mCount == other.mCount );
            copyElements( mElements, other.mElements, other.mCount );
        }

        void doInsert( size_t position, const T & t )
        {
            if( position > mCount ) { GN_WARN(sLogger)("invalid insert position"); return; }

            resize( mCount + 1 );

            for( size_t i = mCount-1; i > position; --i )
            {
                mElements[i] = mElements[i-1];
            }
            mElements[position] = t;
        }

        void doErase( size_t position )
        {
            if( position >= mCount ) { GN_WARN(sLogger)("invalid erase position"); return; }
            --mCount;
            for( size_t i = position; i < mCount; ++i )
            {
                mElements[i] = mElements[i+1];
            }
        }

        void doReserve( size_t count )
        {
            if( mCapacity < count )
            {
                size_t newCap = count;
                #if GN_X64
                newCap |= newCap >> 32;
                #endif
                newCap |= newCap >> 16;
                newCap |= newCap >> 8;
                newCap |= newCap >> 4;
                newCap |= newCap >> 2;
                newCap |= newCap >> 1;
                T * newBuf = AllocatorType::sAlloc( newCap );
                copyElements( newBuf, mElements, mCount );
                dealloc();
                mElements = newBuf;
                mCapacity = newCap;
            }
        }

        void doSwap( DynaArray & another )
        {
            T *    p = mElements;
            size_t n = mCount;
            size_t c = mCapacity;

            mElements = another.mElements;
            mCount    = another.mCount;
            mCapacity = another.mCapacity;

            another.mElements = p;
            another.mCount    = n;
            another.mCapacity = c;
        }

        bool equal( const DynaArray & other ) const
        {
            if( mCount != other.mCount ) return false;
            return 0 == ::memcmp( mElements, other.mElements, sizeof(T)*mCount );
        }

    public:

        typedef T ElementType; ///< element type
        typedef ALLOCATOR AllocatorType; ///< allocator type

        ///
        /// default constructor
        ///
        DynaArray() : mElements(0), mCount(0), mCapacity(0) {}

        ///
        /// constructor with user-defined count.
        ///
        explicit DynaArray( size_t count ) : mElements(0), mCount(0), mCapacity(0) { resize( count ); }

        ///
        /// construct from conventional C array
        ///
        DynaArray( const T * p, size_t count ) : mElements(0), mCount(0), mCapacity(0) { doAppend( p, count ); }

        ///
        /// copy constructor
        ///
        DynaArray( const DynaArray & other ) : mElements(0), mCount(0), mCapacity(0) { doClone( other ); }

        ///
        /// destructor
        ///
        ~DynaArray() { dealloc(); }

        /// \name Common array operations.
        ///
        //@{
        void      append( const T & t ) { resize( mCount + 1 ); back() = t; }
        void      append( const T * p, size_t count ) { doAppend( p, count ); }
        void      append( const DynaArray & a ) { doAppend( a.mElements, a.mCount ); }
        const T & back() const { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        T       & back() { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        const T * begin() const { return mElements; }
        T       * begin() { return mElements; }
        void      clear() { mCount = 0; }
        const T * cptr() const { return mElements; }
        T       * cptr() { return mElements; }
        bool      empty() const { return 0 == mCount; }
        const T * end() const { return mElements + mCount; }
        T       * end() { return mElements + mCount; }
        /** do nothing if position is invalid */
        void      erase( size_t position ) { return doErase( position ); }
        const T & front() const { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        T       & front() { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        /** do nothing if position is invalid */
        void      insert( size_t position, const T & t ) { doInsert( position, t ); }
        void      reserve( size_t count ) { doReserve( count ); }
        void      resize( size_t count ) { doReserve( count ); mCount = count; }
        void      popBack() { if( mCount > 0 ) --mCount; }
        size_t    size() const { return mCount; }
        void      swap( DynaArray & another ) { doSwap( another ); } ///< swap data with another array
        //@}

        /// \name common operators
        ///
        //@{
        DynaArray & operator=( const DynaArray & other ) { doClone(other); return *this; }
        bool        operator==( const DynaArray & other ) const { return equal(other); }
        bool        operator!=( const DynaArray & other ) const { return !equal(other); }
        T         & operator[]( size_t i ) { GN_ASSERT( i < mCount ); return mElements[i]; }
        const T   & operator[]( size_t i ) const { GN_ASSERT( i < mCount ); return mElements[i]; }
        //@}
    };

    template<class T, class ALLOCATOR> Logger * DynaArray<T,ALLOCATOR>::sLogger = getLogger("GN.base.DynaArray");
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_ARRAY_H__
