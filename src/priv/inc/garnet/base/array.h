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
        /// return size of the array (always be MAX_SIZE)
        ///
        size_t size() const { return MAX_SIZE; }

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
    template<class T, size_t N>
    class StackArray
    {
        UInt8  mBuffer[sizeof(T)*N];
        size_t mCount;
#if GN_BUILD_DEBUG
        T *    mTypedBuffer;
#endif

        /// default constructor
        static inline void ctor( T * ptr, size_t count )
        {

            for( size_t i = 0; i < count; ++i, ++ptr )
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
            ptr->T::~T();
        }

        void updateTypedBufferPtr()
        {
#if GN_BUILD_DEBUG
            mTypedBuffer = (T*)mBuffer;
#endif
        }

        void doClear()
        {
            T * p = cptr();
            for( size_t i = 0; i < mCount; ++i, ++p )
            {
                dtor( p );
            }
            mCount = 0;
        }

        void doClone( const StackArray & other )
        {
            T       * dst = cptr();
            const T * src = other.cptr();

            size_t mincount = math::getmin<size_t>( mCount, other.mCount );
            for( size_t i = 0; i < mincount; ++i )
            {
                dst[i] = src[i];
            }

            // destruct extra objects, only when other.mCount < mCount
            for( size_t i = other.mCount; i < mCount; ++i )
            {
                dtor( dst + i );
            }

            // copy-construct new objects, only when mCount < other.mCount
            for( size_t i = mCount; i < other.mCount; ++i )
            {
                cctor( dst + i, src[i] );
            }

            mCount = other.mCount;
        }

        void doInsert( size_t position, const T & t )
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
            for( size_t i = mCount; i > position; --i )
            {
                p[i] = p[i-1];
            }

            // insert new elements
            p[position] = t;

            ++mCount;
        }

        void doErase( size_t position )
        {
            if( position >= mCount )
            {
                GN_ERROR(getLogger("GN.base.StackArray"))( "Invalid eraseIdx position" );
                return;
            }

            --mCount;

            T * p = cptr();

            // move elements
            for( size_t i = position; i < mCount; ++i )
            {
                p[i] = p[i+1];
            }

            // destruct last element
            dtor( p + mCount );
        }

        void doResize( size_t count )
        {
            if( count == mCount ) return; // shortcut for redundant call.

            if( count > N )
            {
                GN_ERROR(getLogger("GN.base.StackArray"))("count is too large!");
                return;
            }

            T * p = cptr();

            // destruct extra objects, only when count < mCount
            for( size_t i = count; i < mCount; ++i )
            {
                dtor( p + i );
            }

            // construct new objects, only when mCount < count
            for( size_t i = mCount; i < count; ++i )
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

            for( size_t i = 0; i < mCount; ++i )
            {
                if( p1[i] != p2[i] ) return false;
            }
            return true;
        }

    public:

        typedef T ElementType; ///< element type

        static const size_t MAX_SIZE = N; ///< maximum size

        ///
        /// default constructor
        ///
        StackArray() : mCount(0)
        {
            updateTypedBufferPtr();
        }

        ///
        /// constructor with user-defined count.
        ///
        explicit StackArray( size_t count ) : mCount(count)
        {
            updateTypedBufferPtr();
            ctor( cptr(), count );
        }

        ///
        /// copy constructor
        ///
        StackArray( const StackArray & other ) : mCount(0)
        {
            updateTypedBufferPtr();
            doClone( other );
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
        void      eraseIdx( size_t position ) { doErase( position ); }
        void      erasePtr( const T * ptr ) { doErase( ptr - mBuffer ); }
        const T & front() const { GN_ASSERT( mCount > 0 ); return cptr()[0]; }
        T       & front() { GN_ASSERT( mCount > 0 ); return cptr()[0]; }
        /** do nothing if position is invalid or array is full */
        void      insert( size_t position, const T & t ) { doInsert( position, t ); }
        void      resize( size_t count ) { doResize( count ); }
        void      popBack() { doErase( mCount - 1 ); }
        size_t    size() const { return mCount; }
        //@}

        /// \name common operators
        ///
        //@{
        StackArray & operator=( const StackArray & other ) { doClone(other); return *this; }
        bool         operator==( const StackArray & other ) const { return equal(other); }
        bool         operator!=( const StackArray & other ) const { return !equal(other); }
        T          & operator[]( size_t i ) { GN_ASSERT( i < mCount ); return cptr()[i]; }
        const T    & operator[]( size_t i ) const { GN_ASSERT( i < mCount ); return cptr()[i]; }
        //@}
    };

    ///
    /// Resizeable array.
    ///
    template<class T, class ALLOCATOR = StlAllocator<T> >
    class DynaArray
    {
        typedef typename ALLOCATOR::template rebind<T>::other ElementAllocator;

        T              * mElements;
        size_t           mCount;
        size_t           mCapacity;
        ElementAllocator mAlloc;

        /// destruct and free memory
        void dealloc( T * ptr, size_t count, size_t capacity )
        {
            for( size_t i = 0; i < count; ++i )
            {
                mAlloc.destroy( ptr + i );
            }

            mAlloc.deallocate( ptr, capacity );
        }

        void doAppend( const T * p, size_t count )
        {
            if( 0 == count ) return;

            if( 0 == p )
            {
                GN_ERROR(getLogger("GN.base.DynaArray"))("non-zero count with NULL pointer is not allowed!");
                return;
            }

            // reserve memory
            doReserve( mCount + count );

            // copy-construct new elements
            T * dst = mElements + mCount;
            for( size_t i = 0; i < count; ++i, ++dst, ++p )
            {
                mAlloc.construct( dst, *p );
            }

            // update count
            mCount += count;
        }

        void doClear()
        {
            T * p = mElements;
            for( size_t i = 0; i < mCount; ++i, ++p )
            {
                mAlloc.destroy( p );
            }
            mCount = 0;
        }

        void doClone( const DynaArray & other )
        {
            doReserve( other.mCount );

            size_t mincount = math::getmin<size_t>( mCount, other.mCount );

            for( size_t i = 0; i < mincount; ++i )
            {
                mElements[i] = other.mElements[i];
            }

            // destruct extra objects, only when other.mCount < mCount
            for( size_t i = other.mCount; i < mCount; ++i )
            {
                mAlloc.destroy( mElements + i );
            }

            // copy-construct new objects, only when mCount < other.mCount
            for( size_t i = mCount; i < other.mCount; ++i )
            {
                mAlloc.construct( mElements + i, other.mElements[i] );
            }

            mCount = other.mCount;
        }

        void doInsert( size_t position, const T & t )
        {
            if( position > mCount )
            {
                GN_WARN(getLogger("GN.base.DynaArray"))("invalid insert position");
                return;
            }

            doResize( mCount + 1 );

            for( size_t i = mCount-1; i > position; --i )
            {
                mElements[i] = mElements[i-1];
            }

            mElements[position] = t;
        }

        void doErase( size_t position )
        {
            if( position >= mCount )
            {
                GN_ERROR(getLogger("GN.base.DynaArray"))("invalid erase position");
                return;
            }

            --mCount;

            // move elements forward
            for( size_t i = position; i < mCount; ++i )
            {
                mElements[i] = mElements[i+1];
            }

            // then destroy the last element
            mAlloc.destroy( mElements + mCount );
        }

        void doReserve( size_t count )
        {
            if( mCapacity >= count ) return;

            GN_ASSERT( count > mCount );

            // align caps to next power of 2
            size_t newCap = count - 1;
            #if GN_X64
            newCap |= newCap >> 32;
            #endif
            newCap |= newCap >> 16;
            newCap |= newCap >> 8;
            newCap |= newCap >> 4;
            newCap |= newCap >> 2;
            newCap |= newCap >> 1;
            newCap += 1;

            // allocate new buffer (unconstructed raw memory)
            T * newBuf = mAlloc.allocate( newCap );

            // copy construct new buffer
            for( size_t i = 0; i < mCount; ++i )
            {
                mAlloc.construct( newBuf + i, mElements[i] );
            }

            // deallocate old buffer
            dealloc( mElements, mCount, mCapacity );

            mElements = newBuf;
            mCapacity = newCap;
        }

        void doResize( size_t count )
        {
            if( count == mCount ) return; // shortcut for redundant call

            // reserve memory
            doReserve( count );

            // destruct extra objects, only when count < mCount
            for( size_t i = count; i < mCount; ++i )
            {
                mAlloc.destroy( mElements + i );
            }

            // construct new objects, only when mCount < count
            for( size_t i = mCount; i < count; ++i )
            {
                mAlloc.construct( mElements + i );
            }

            mCount = count;
        }

        void doResize( size_t count, const T & t )
        {
            if( count == mCount ) return; // shortcut for redundant call

            // reserve memory
            doReserve( count );

            // destruct extra objects, only when count < mCount
            for( size_t i = count; i < mCount; ++i )
            {
                mAlloc.destroy( mElements + i );
            }

            // copy-construct new objects, only when mCount < count
            for( size_t i = mCount; i < count; ++i )
            {
                mAlloc.construct( mElements + i, t );
            }

            mCount = count;
        }

        void doSwap( DynaArray & another )
        {
            T *           p = mElements;
            size_t        n = mCount;
            size_t        c = mCapacity;
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
            for( size_t i = 0; i < mCount; ++i )
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
        explicit DynaArray( size_t count ) : mElements(0), mCount(0), mCapacity(0) { doResize( count ); }

        ///
        /// constructor with user-defined count and value.
        ///
        DynaArray( size_t count, const T & t ) : mElements(0), mCount(0), mCapacity(0) { doResize( count, t ); }

        ///
        /// construct from conventional C array
        ///
        DynaArray( const T * p, size_t count ) : mElements(0), mCount(0), mCapacity(0) { doAppend( p, count ); }

        ///
        /// copy constructor
        ///
        DynaArray( const DynaArray & other ) : mElements(0), mCount(0), mCapacity(0), mAlloc(other.mAlloc) { doClone( other ); }

        ///
        /// destructor
        ///
        ~DynaArray() { dealloc( mElements, mCount, mCapacity ); }

        /// \name Common array operations.
        ///
        //@{
        void      append( const T & t ) { doAppend( &t, 1 ); }
        void      append( const T * p, size_t count ) { doAppend( p, count ); }
        void      append( const DynaArray & a ) { doAppend( a.mElements, a.mCount ); }
        const T & back() const { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        T       & back() { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        const T * begin() const { return mElements; }
        T       * begin() { return mElements; }
        size_t    capacity() const { return mCapacity; }
        void      clear() { doClear(); }
        const T * cptr() const { return mElements; }
        T       * cptr() { return mElements; }
        bool      empty() const { return 0 == mCount; }
        const T * end() const { return mElements + mCount; }
        T       * end() { return mElements + mCount; }
        /** do nothing if position is invalid */
        void      eraseIdx( size_t position ) { return doErase( position ); }
        void      erasePtr( const T * p ) { return doErase( p - mElements ); }
        const T * first() const { return mElements; }
        T       * first() { return mElements; }
        const T & front() const { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        T       & front() { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        /** do nothing if position is invalid */
        void      insert( size_t position, const T & t ) { doInsert( position, t ); }
        const T * next( const T * t ) const { return ( mCount > 0 && mElements <= t && t < (mElements+mCount-1) ) ? ( t + 1 ) : NULL; }
        T       * next( const T * t ) { return ( mCount > 0 && mElements <= t && t < (mElements+mCount-1) ) ? ( t + 1 ) : NULL; }
        void      reserve( size_t count ) { doReserve( count ); }
        void      resize( size_t count ) { doResize( count ); }
        void      popBack() { doErase( mCount - 1 ); }
        /** clear array as well as release memory */
        void      purge() { dealloc( mElements, mCount, mCapacity ); mCount = 0; mCapacity = 0; mElements = 0; }
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
