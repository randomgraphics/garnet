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
    template<class T, size_t N, typename SIZE_TYPE = size_t>
    class FixedArray
    {
        T mElements[N];

    public:

        typedef T ElementType; ///< element type

        static const SIZE_TYPE MAX_SIZE = (SIZE_TYPE)N; ///< maximum size

        ///
        /// convert to C pointer
        ///
        const T* cptr() const { return mElements; }

        ///
        /// convert to C pointer
        ///
        T* cptr() { return mElements; }

        ///
        /// return size of the array (always be MAX_SIZE)
        ///
        SIZE_TYPE size() const { return MAX_SIZE; }

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
    /// \todo Fix issues using with class with non-trival constructor and destructor
    ///
    template<class T, size_t N, typename SIZE_TYPE = size_t>
    class StackArray
    {
        uint8     mBuffer[sizeof(T)*N];
        SIZE_TYPE mCount;

        /// default constructor
        static inline void ctor( T * ptr, SIZE_TYPE count )
        {

            for( SIZE_TYPE i = 0; i < count; ++i, ++ptr )
            {
                new (ptr) T;
            }
        }

        /// copy constructor
        static inline void cctor( T * ptr, const T & src )
        {
            new (ptr) T(src);
        }

        /// destructor
        static inline void dtor( T * ptr )
        {
            GN_UNUSED_PARAM(ptr);
            ptr->T::~T();
        }

        void doClear()
        {
            T * p = cptr();
            for( SIZE_TYPE i = 0; i < mCount; ++i, ++p )
            {
                dtor( p );
            }
            mCount = 0;
        }

        void copyFrom( const StackArray & other )
        {
            T       * dst = cptr();
            const T * src = other.cptr();

            SIZE_TYPE mincount = math::getmin<SIZE_TYPE>( mCount, other.mCount );
            for( SIZE_TYPE i = 0; i < mincount; ++i )
            {
                dst[i] = src[i];
            }

            // destruct extra objects, only when other.mCount < mCount
            for( SIZE_TYPE i = other.mCount; i < mCount; ++i )
            {
                dtor( dst + i );
            }

            // copy-construct new objects, only when mCount < other.mCount
            for( SIZE_TYPE i = mCount; i < other.mCount; ++i )
            {
                cctor( dst + i, src[i] );
            }

            mCount = other.mCount;
        }

        void doInsert( SIZE_TYPE position, const T & t )
        {
            GN_ASSERT( mCount <= N );

            if( N == mCount )
            {
                GN_ERROR(getLogger("GN.base.StackArray"))( "Can't insert more. Stack array is full already!" );
                return;
            }

            if( position > mCount )
            {
                GN_ERROR(getLogger("GN.base.StackArray"))( "invalid insert position." );
                return;
            }

            T * p = cptr();

            // construct last element
            ctor( p + mCount, 1 );

            // move elements
            for( SIZE_TYPE i = mCount; i > position; --i )
            {
                p[i] = p[i-1];
            }

            // insert new elements
            p[position] = t;

            ++mCount;
        }

        void doErase( SIZE_TYPE position )
        {
            if( position >= mCount )
            {
                GN_ERROR(getLogger("GN.base.StackArray"))( "Invalid eraseIdx position" );
                return;
            }

            --mCount;

            T * p = cptr();

            // move elements
            for( SIZE_TYPE i = position; i < mCount; ++i )
            {
                p[i] = p[i+1];
            }

            // destruct last element
            dtor( p + mCount );
        }

        void doResize( SIZE_TYPE count )
        {
            if( count == mCount ) return; // shortcut for redundant call.

            if( count > N )
            {
                GN_ERROR(getLogger("GN.base.StackArray"))("count is too large!");
                return;
            }

            T * p = cptr();

            // destruct extra objects, only when count < mCount
            for( SIZE_TYPE i = count; i < mCount; ++i )
            {
                dtor( p + i );
            }

            // construct new objects, only when mCount < count
            for( SIZE_TYPE i = mCount; i < count; ++i )
            {
                ctor( p + i, 1 );
            }

            mCount = count;
        }

        bool equal( const StackArray & other ) const
        {
            if( mCount != other.mCount ) return false;

            const T * p1 = cptr();
            const T * p2 = other.cptr();

            for( SIZE_TYPE i = 0; i < mCount; ++i )
            {
                if( p1[i] != p2[i] ) return false;
            }
            return true;
        }

    public:

        typedef T ElementType; ///< element type

        static const SIZE_TYPE MAX_SIZE = (SIZE_TYPE)N; ///< maximum size

        ///
        /// default constructor
        ///
        StackArray() : mCount(0)
        {
        }

        ///
        /// constructor with user-defined count.
        ///
        explicit StackArray( SIZE_TYPE count ) : mCount(count)
        {
            ctor( cptr(), count );
        }

        ///
        /// copy constructor
        ///
        StackArray( const StackArray & other ) : mCount(0)
        {
            copyFrom( other );
        }

        ///
        /// dtor
        ///
        ~StackArray() { doClear(); }

        /// \name Common array operations.
        ///
        //@{
        void      append( const T & t ) { doInsert( mCount, t ); }
        const T & back() const { GN_ASSERT( mCount > 0 ); return cptr()[mCount-1]; }
        T       & back() { GN_ASSERT( mCount > 0 ); return cptr()[mCount-1]; }
        const T * begin() const { return cptr(); }
        T       * begin() { return cptr(); }
        void      clear() { doClear(); }
        const T * cptr() const { return (const T*)mBuffer; }
        T       * cptr() { return (T*)mBuffer; }
        bool      empty() const { return 0 == mCount; }
        const T * end() const { return cptr() + mCount; }
        T       * end() { return cptr() + mCount; }
        /** do nothing if position is invalid or array is empty */
        void      eraseIdx( SIZE_TYPE position ) { doErase( position ); }
        void      erasePtr( const T * ptr ) { doErase( ptr - mBuffer ); }
        const T & front() const { GN_ASSERT( mCount > 0 ); return cptr()[0]; }
        T       & front() { GN_ASSERT( mCount > 0 ); return cptr()[0]; }
        /** do nothing if position is invalid or array is full */
        void      insert( SIZE_TYPE position, const T & t ) { doInsert( position, t ); }
        void      resize( SIZE_TYPE count ) { doResize( count ); }
        void      popBack() { doErase( mCount - 1 ); }
        SIZE_TYPE size() const { return mCount; }
        //@}

        /// \name common operators
        ///
        //@{
        StackArray & operator=( const StackArray & other ) { copyFrom(other); return *this; }
        bool         operator==( const StackArray & other ) const { return equal(other); }
        bool         operator!=( const StackArray & other ) const { return !equal(other); }
        T          & operator[]( SIZE_TYPE i ) { GN_ASSERT( i < mCount ); return cptr()[i]; }
        const T    & operator[]( SIZE_TYPE i ) const { GN_ASSERT( i < mCount ); return cptr()[i]; }
        //@}
    };

    ///
    /// Resizeable array.
    ///
    template<class T, typename SIZE_TYPE = size_t, class ALLOCATOR = StlAllocator<T> >
    class DynaArray
    {
        typedef typename ALLOCATOR::template rebind<T>::other ElementAllocator;

        T              * mElements;
        SIZE_TYPE        mCount;
        SIZE_TYPE        mCapacity;
        ElementAllocator mAlloc;

        /// destruct and free memory
        void dealloc( T * ptr, SIZE_TYPE count, SIZE_TYPE capacity )
        {
            for( SIZE_TYPE i = 0; i < count; ++i )
            {
                mAlloc.destroy( ptr + i );
            }

            mAlloc.deallocate( ptr, capacity );
        }

        bool doAppend( const T * p, SIZE_TYPE count )
        {
            if( 0 == count ) return true;

            if( 0 == p )
            {
                GN_ERROR(getLogger("GN.base.DynaArray"))("non-zero count with NULL pointer is not allowed!");
                return false;
            }

            // reserve memory
            if( !doReserve( mCount + count ) ) return false;

            // copy-construct new elements
            T * dst = mElements + mCount;
            for( SIZE_TYPE i = 0; i < count; ++i, ++dst, ++p )
            {
                mAlloc.construct( dst, *p );
            }

            // update count
            mCount += count;

            return true;
        }

        void doClear()
        {
            T * p = mElements;
            for( SIZE_TYPE i = 0; i < mCount; ++i, ++p )
            {
                mAlloc.destroy( p );
            }
            mCount = 0;
        }

        bool copyFrom( const DynaArray & other )
        {
            if( !doReserve( other.mCount ) ) return false;

            SIZE_TYPE mincount = math::getmin<SIZE_TYPE>( mCount, other.mCount );

            for( SIZE_TYPE i = 0; i < mincount; ++i )
            {
                mElements[i] = other.mElements[i];
            }

            // destruct extra objects, only when other.mCount < mCount
            for( SIZE_TYPE i = other.mCount; i < mCount; ++i )
            {
                mAlloc.destroy( mElements + i );
            }

            // copy-construct new objects, only when mCount < other.mCount
            for( SIZE_TYPE i = mCount; i < other.mCount; ++i )
            {
                mAlloc.construct( mElements + i, other.mElements[i] );
            }

            mCount = other.mCount;

            return true;
        }

        bool doInsert( SIZE_TYPE position, const T & t )
        {
            if( position > mCount )
            {
                GN_WARN(getLogger("GN.base.DynaArray"))("invalid insert position");
                return false;
            }

            if( !doResize( mCount + 1 ) ) return false;

            for( SIZE_TYPE i = mCount-1; i > position; --i )
            {
                mElements[i] = mElements[i-1];
            }

            mElements[position] = t;

            return true;
        }

        void doErase( SIZE_TYPE position )
        {
            if( position >= mCount )
            {
                GN_ERROR(getLogger("GN.base.DynaArray"))("invalid erase position");
                return;
            }

            --mCount;

            // move elements forward
            for( SIZE_TYPE i = position; i < mCount; ++i )
            {
                mElements[i] = mElements[i+1];
            }

            // then destroy the last element
            mAlloc.destroy( mElements + mCount );
        }

        bool doReserve( SIZE_TYPE count )
        {
            if( mCapacity >= count ) return true;

            GN_ASSERT( count > mCount );

            // align caps to next power of 2
            SIZE_TYPE newCap = count - 1;
            if( sizeof(SIZE_TYPE) >= 8 )
            {
                newCap |= (SIZE_TYPE)( ((uint64)newCap) >> 32 );
            }
            newCap |= newCap >> 16;
            newCap |= newCap >> 8;
            newCap |= newCap >> 4;
            newCap |= newCap >> 2;
            newCap |= newCap >> 1;
            newCap += 1;

            // allocate new buffer (unconstructed raw memory)
            T * newBuf = mAlloc.allocate( newCap );
            if( NULL == newBuf )
            {
                GN_ERROR(getLogger("GN.base.DynaArray"))("out of memory!");
                return false;
            }

            // copy construct new buffer
            for( SIZE_TYPE i = 0; i < mCount; ++i )
            {
                mAlloc.construct( newBuf + i, mElements[i] );
            }

            // deallocate old buffer
            dealloc( mElements, mCount, mCapacity );

            mElements = newBuf;
            mCapacity = newCap;

            return true;
        }

        bool doResize( SIZE_TYPE count )
        {
            if( count == mCount ) return true; // shortcut for redundant call

            // reserve memory
            if( !doReserve( count ) ) return false;

            // destruct extra objects, only when count < mCount
            for( SIZE_TYPE i = count; i < mCount; ++i )
            {
                mAlloc.destroy( mElements + i );
            }

            // construct new objects, only when mCount < count
            for( SIZE_TYPE i = mCount; i < count; ++i )
            {
                mAlloc.construct( mElements + i );
            }

            mCount = count;

            return true;
        }

        bool doResize( SIZE_TYPE count, const T & t )
        {
            if( count == mCount ) return true; // shortcut for redundant call

            // reserve memory
            if( !doReserve( count ) ) return false;

            // destruct extra objects, only when count < mCount
            for( SIZE_TYPE i = count; i < mCount; ++i )
            {
                mAlloc.destroy( mElements + i );
            }

            // copy-construct new objects, only when mCount < count
            for( SIZE_TYPE i = mCount; i < count; ++i )
            {
                mAlloc.construct( mElements + i, t );
            }

            mCount = count;

            return true;
        }

        void doSwap( DynaArray & another )
        {
            T *           p = mElements;
            SIZE_TYPE        n = mCount;
            SIZE_TYPE        c = mCapacity;
            AllocatorType a = mAlloc;

            mElements = another.mElements;
            mCount    = another.mCount;
            mCapacity = another.mCapacity;
            mAlloc    = another.mAlloc;

            another.mElements = p;
            another.mCount    = n;
            another.mCapacity = c;
            another.mAlloc    = a;
        }

        bool equal( const DynaArray & other ) const
        {
            if( mCount != other.mCount ) return false;
            for( SIZE_TYPE i = 0; i < mCount; ++i )
            {
                if( mElements[i] != other.mElements[i] ) return false;
            }
            return true;
        }

    public:

        typedef T ElementType; ///< element type
        typedef ALLOCATOR AllocatorType; ///< allocator type
        typedef const T * ConstIterator;
        typedef T * Iterator;

        ///
        /// default constructor
        ///
        DynaArray() : mElements(0), mCount(0), mCapacity(0) {}

        ///
        /// constructor with user-defined count.
        ///
        explicit DynaArray( SIZE_TYPE count ) : mElements(0), mCount(0), mCapacity(0) { doResize( count ); }

        ///
        /// constructor with user-defined count and value.
        ///
        DynaArray( SIZE_TYPE count, const T & t ) : mElements(0), mCount(0), mCapacity(0) { doResize( count, t ); }

        ///
        /// construct from conventional C array
        ///
        DynaArray( const T * p, SIZE_TYPE count ) : mElements(0), mCount(0), mCapacity(0) { doAppend( p, count ); }

        ///
        /// copy constructor
        ///
        DynaArray( const DynaArray & other ) : mElements(0), mCount(0), mCapacity(0), mAlloc(other.mAlloc) { copyFrom( other ); }

        ///
        /// destructor
        ///
        ~DynaArray() { dealloc( mElements, mCount, mCapacity ); }

        /// \name Common array operations.
        ///
        //@{
        bool      append( const T & t ) { return doAppend( &t, 1 ); }
        bool      append( const T * p, SIZE_TYPE count ) { return doAppend( p, count ); }
        bool      append( const DynaArray & a ) { return doAppend( a.mElements, a.mCount ); }
        const T & back() const { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        T       & back() { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        const T * begin() const { return mElements; }
        T       * begin() { return mElements; }
        SIZE_TYPE capacity() const { return mCapacity; }
        void      clear() { doClear(); }
        const T * cptr() const { return mElements; }
        T       * cptr() { return mElements; }
        bool      empty() const { return 0 == mCount; }
        const T * end() const { return mElements + mCount; }
        T       * end() { return mElements + mCount; }
        /** do nothing if position is invalid */
        void      eraseIdx( SIZE_TYPE position ) { return doErase( position ); }
        void      erasePtr( const T * p ) { return doErase( p - mElements ); }
        const T * first() const { return mElements; }
        T       * first() { return mElements; }
        const T & front() const { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        T       & front() { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        /** do nothing if position is invalid */
        bool      insert( SIZE_TYPE position, const T & t ) { return doInsert( position, t ); }
        const T * next( const T * t ) const { return ( mCount > 0 && mElements <= t && t < (mElements+mCount-1) ) ? ( t + 1 ) : NULL; }
        T       * next( const T * t ) { return ( mCount > 0 && mElements <= t && t < (mElements+mCount-1) ) ? ( t + 1 ) : NULL; }
        bool      reserve( SIZE_TYPE count ) { return doReserve( count ); }
        bool      resize( SIZE_TYPE count ) { return doResize( count ); }
        void      popBack() { doErase( mCount - 1 ); }
        /** clear array as well as release memory */
        void      purge() { dealloc( mElements, mCount, mCapacity ); mCount = 0; mCapacity = 0; mElements = 0; }
        SIZE_TYPE size() const { return mCount; }
        void      swap( DynaArray & another ) { doSwap( another ); } ///< swap data with another array
        //@}

        /// \name common operators
        ///
        //@{
        DynaArray & operator=( const DynaArray & other ) { copyFrom(other); return *this; }
        bool        operator==( const DynaArray & other ) const { return equal(other); }
        bool        operator!=( const DynaArray & other ) const { return !equal(other); }
        T         & operator[]( SIZE_TYPE i ) { GN_ASSERT( i < mCount ); return mElements[i]; }
        const T   & operator[]( SIZE_TYPE i ) const { GN_ASSERT( i < mCount ); return mElements[i]; }
        //@}
    };

    ///
    /// array accessor with out-of-boundary check in debug build.
    ///
    template<typename T>
    class SafeArrayAccessor
    {
        T    * mBegin;
        T    * mEnd;
        T    * mPtr;

    public:

        //@{

        SafeArrayAccessor( T * data, size_t count )
            : mBegin(data), mEnd( data + count ), mPtr(data)
        {
        }

        T * subrange( size_t index, size_t length ) const
        {
            GN_ASSERT( mBegin <= (mPtr+index) );
            GN_ASSERT( (mPtr+index) < mEnd );
            GN_ASSERT( (mPtr+index+length) <= mEnd );
            GN_UNUSED_PARAM( length );
            return mPtr + index;
        }

        template<typename T2>
        void copyTo( size_t srcOffset, const SafeArrayAccessor<T2> & dest, size_t dstOffset, size_t bytes )
        {
            GN_CASSERT( sizeof(T) == sizeof(T2) );
            memcpy( dest.subrange( dstOffset, bytes ), subrange( srcOffset, bytes ), bytes );
        }

        T * operator->() const
        {
            GN_ASSERT( mBegin <= mPtr && mPtr < mEnd );
            return mPtr;
        }

        T & operator[]( size_t index ) const
        {
            GN_ASSERT( mBegin <= (mPtr+index) );
            GN_ASSERT( (mPtr+index) < mEnd );
            return mPtr[index];
        }

        SafeArrayAccessor & operator++() { ++mPtr; return *this; }

        SafeArrayAccessor & operator--() { --mPtr; return *this; }

        SafeArrayAccessor & operator+=( size_t offset )
        {
            mPtr += offset;
            return *this;
        }

        SafeArrayAccessor & operator-=( size_t offset )
        {
            mPtr -= offset;
            return *this;
        }

        //@}
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_ARRAY_H__
