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
        const T* GetRawPtr() const { return mElements; }

        ///
        /// convert to C pointer
        ///
        T* GetRawPtr() { return mElements; }

        ///
        /// return Size of the array (always be MAX_SIZE)
        ///
        size_t Size() const { return MAX_SIZE; }

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

        /// default constructor
        static inline void Ctor( T * ptr, size_t count )
        {

            for( size_t i = 0; i < count; ++i, ++ptr )
            {
                new (ptr) T;
            }
        }

        /// copy constructor
        static inline void CCtor( T * ptr, const T & src )
        {
            new (ptr) T(src);
        }

        /// destructor
        static inline void Dtor( T * ptr )
        {
            ptr->T::~T();
        }

        void DoClear()
        {
            T * p = GetRawPtr();
            for( size_t i = 0; i < mCount; ++i, ++p )
            {
                Dtor( p );
            }
            mCount = 0;
        }

        void DoClone( const StackArray & other )
        {
            T       * dst = GetRawPtr();
            const T * src = other.GetRawPtr();

            size_t mincount = math::GetMin<size_t>( mCount, other.mCount );
            for( size_t i = 0; i < mincount; ++i )
            {
                dst[i] = src[i];
            }

            // destruct extra objects, only when other.mCount < mCount
            for( size_t i = other.mCount; i < mCount; ++i )
            {
                Dtor( dst + i );
            }

            // copy-construct new objects, only when mCount < other.mCount
            for( size_t i = mCount; i < other.mCount; ++i )
            {
                CCtor( dst + i, src[i] );
            }

            mCount = other.mCount;
        }

        void DoInsert( size_t position, const T & t )
        {
            GN_ASSERT( mCount <= N );

            if( N == mCount )
            {
                GN_ERROR(GetLogger("GN.base.StackArray"))( "Can't Insert more. Stack array is full already!" );
                return;
            }

            if( position > mCount )
            {
                GN_ERROR(GetLogger("GN.base.StackArray"))( "invalid Insert position." );
                return;
            }

            T * p = GetRawPtr();

            // construct last element
            Ctor( p + mCount, 1 );

            // move elements
            for( size_t i = mCount; i > position; --i )
            {
                p[i] = p[i-1];
            }

            // Insert new elements
            p[position] = t;

            ++mCount;
        }

        void DoErase( size_t position )
        {
            if( position >= mCount )
            {
                GN_ERROR(GetLogger("GN.base.StackArray"))( "Invalid Erase position" );
                return;
            }

            --mCount;

            T * p = GetRawPtr();

            // move elements
            for( size_t i = position; i < mCount; ++i )
            {
                p[i] = p[i+1];
            }

            // destruct last element
            Dtor( p + mCount );
        }

        void DoResize( size_t count )
        {
            if( count == mCount ) return; // shortcut for redundant call.

            if( count > N )
            {
                GN_ERROR(GetLogger("GN.base.StackArray"))("count is too large!");
                return;
            }

            T * p = GetRawPtr();

            // destruct extra objects, only when count < mCount
            for( size_t i = count; i < mCount; ++i )
            {
                Dtor( p + i );
            }

            // construct new objects, only when mCount < count
            for( size_t i = mCount; i < count; ++i )
            {
                Ctor( p + i, 1 );
            }

            mCount = count;
        }

        bool Equal( const StackArray & other ) const
        {
            if( mCount != other.mCount ) return false;

            const T * p1 = GetRawPtr();
            const T * p2 = other.GetRawPtr();

            for( size_t i = 0; i < mCount; ++i )
            {
                if( p1[i] != p2[i] ) return false;
            }
            return true;
        }

    public:

        typedef T ElementType; ///< element type

        static const size_t MAX_SIZE = N; ///< maximum Size

        ///
        /// default constructor
        ///
        StackArray() : mCount(0) {}

        ///
        /// constructor with user-defined count.
        ///
        explicit StackArray( size_t count ) : mCount(count) { Ctor( GetRawPtr(), count ); }

        ///
        /// copy constructor
        ///
        StackArray( const StackArray & other ) : mCount(0) { DoClone( other ); }

        ///
        /// Dtor
        ///
        virtual ~StackArray() { DoClear(); }

        /// \name Common array operations.
        ///
        //@{
        void      Append( const T & t ) { DoInsert( mCount, t ); }
        const T & Back() const { GN_ASSERT( mCount > 0 ); return GetRawPtr()[mCount-1]; }
        T       & Back() { GN_ASSERT( mCount > 0 ); return GetRawPtr()[mCount-1]; }
        const T * Begin() const { return GetRawPtr(); }
        T       * Begin() { return GetRawPtr(); }
        void      Clear() { DoClear(); }
        const T * GetRawPtr() const { return (const T*)mBuffer; }
        T       * GetRawPtr() { return (T*)mBuffer; }
        bool      Empty() const { return 0 == mCount; }
        const T * End() const { return GetRawPtr() + mCount; }
        T       * End() { return GetRawPtr() + mCount; }
        /** do nothing if position is invalid or array is Empty */
        void      Erase( size_t position ) { DoErase( position ); }
        const T & Front() const { GN_ASSERT( mCount > 0 ); return GetRawPtr()[0]; }
        T       & Front() { GN_ASSERT( mCount > 0 ); return GetRawPtr()[0]; }
        /** do nothing if position is invalid or array is full */
        void      Insert( size_t position, const T & t ) { DoInsert( position, t ); }
        void      Resize( size_t count ) { DoResize( count ); }
        void      PushBack( const T & t ) { DoInsert( Size(), t ); }
        void      PopBack() { DoErase( mCount - 1 ); }
        size_t    Size() const { return mCount; }
        //@}

        /// \name common operators
        ///
        //@{
        StackArray & operator=( const StackArray & other ) { DoClone(other); return *this; }
        bool         operator==( const StackArray & other ) const { return Equal(other); }
        bool         operator!=( const StackArray & other ) const { return !Equal(other); }
        T          & operator[]( size_t i ) { GN_ASSERT( i < mCount ); return GetRawPtr()[i]; }
        const T    & operator[]( size_t i ) const { GN_ASSERT( i < mCount ); return GetRawPtr()[i]; }
        //@}
    };

    ///
    /// Resizeable array.
    ///
    template<class T, class ALLOCATOR = StlAllocator<T> >
    class DynaArray
    {
        T           * mElements;
        size_t        mCount;
        size_t        mCapacity;
        ALLOCATOR     mAlloc;

        /// destruct and free memory
        void Dealloc( T * ptr, size_t count, size_t capacity )
        {
            for( size_t i = 0; i < count; ++i )
            {
                mAlloc.destroy( ptr + i );
            }

            mAlloc.deallocate( ptr, capacity );
        }

        void DoAppend( const T * p, size_t count )
        {
            if( 0 == count ) return;

            if( 0 == p )
            {
                GN_ERROR(GetLogger("GN.base.DynaArray"))("non-zero count with NULL pointer is not allowed!");
                return;
            }

            // Reserve memory
            DoReserve( mCount + count );

            // copy-construct new elements
            T * dst = mElements + mCount;
            for( size_t i = 0; i < count; ++i, ++dst, ++p )
            {
                mAlloc.construct( dst, *p );
            }

            // update count
            mCount += count;
        }

        void DoClear()
        {
            T * p = mElements;
            for( size_t i = 0; i < mCount; ++i, ++p )
            {
                mAlloc.destroy( p );
            }
            mCount = 0;
        }

        void DoClone( const DynaArray & other )
        {
            DoReserve( other.mCount );

            size_t mincount = math::GetMin<size_t>( mCount, other.mCount );

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

        void DoInsert( size_t position, const T & t )
        {
            if( position > mCount )
            {
                GN_WARN(GetLogger("GN.base.DynaArray"))("invalid Insert position");
                return;
            }

            DoResize( mCount + 1 );

            for( size_t i = mCount-1; i > position; --i )
            {
                mElements[i] = mElements[i-1];
            }

            mElements[position] = t;
        }

        void DoErase( size_t position )
        {
            if( position >= mCount )
            {
                GN_ERROR(GetLogger("GN.base.DynaArray"))("invalid Erase position");
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

        void DoReserve( size_t count )
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
            Dealloc( mElements, mCount, mCapacity );

            mElements = newBuf;
            mCapacity = newCap;
        }

        void DoResize( size_t count )
        {
            if( count == mCount ) return; // shortcut for redundant call

            // Reserve memory
            DoReserve( count );

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

        void DoSwap( DynaArray & another )
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

        bool Equal( const DynaArray & other ) const
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

        ///
        /// default constructor
        ///
        DynaArray() : mElements(0), mCount(0), mCapacity(0) {}

        ///
        /// constructor with user-defined count.
        ///
        explicit DynaArray( size_t count ) : mElements(0), mCount(0), mCapacity(0) { Resize( count ); }

        ///
        /// construct from conventional C array
        ///
        DynaArray( const T * p, size_t count ) : mElements(0), mCount(0), mCapacity(0) { DoAppend( p, count ); }

        ///
        /// copy constructor
        ///
        DynaArray( const DynaArray & other ) : mElements(0), mCount(0), mCapacity(0), mAlloc(other.mAlloc) { DoClone( other ); }

        ///
        /// destructor
        ///
        ~DynaArray() { Dealloc( mElements, mCount, mCapacity ); }

        /// \name Common array operations.
        ///
        //@{
        void      Append( const T & t ) { DoAppend( &t, 1 ); }
        void      Append( const T * p, size_t count ) { DoAppend( p, count ); }
        void      Append( const DynaArray & a ) { DoAppend( a.mElements, a.mCount ); }
        const T & Back() const { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        T       & Back() { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        const T * Begin() const { return mElements; }
        T       * Begin() { return mElements; }
        void      Clear() { DoClear(); }
        const T * GetRawPtr() const { return mElements; }
        T       * GetRawPtr() { return mElements; }
        bool      Empty() const { return 0 == mCount; }
        const T * End() const { return mElements + mCount; }
        T       * End() { return mElements + mCount; }
        /** do nothing if position is invalid */
        void      Erase( size_t position ) { return DoErase( position ); }
        const T & Front() const { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        T       & Front() { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        /** do nothing if position is invalid */
        void      Insert( size_t position, const T & t ) { DoInsert( position, t ); }
        void      Reserve( size_t count ) { DoReserve( count ); }
        void      Resize( size_t count ) { DoResize( count ); }
        void      PopBack() { DoErase( mCount - 1 ); }
        /** clear array as well as release memory */
        void      Purge() { Dealloc( mElements, mCount, mCapacity ); mCount = 0; mCapacity = 0; mElements = 0; }
        size_t    Size() const { return mCount; }
        void      Swap( DynaArray & another ) { DoSwap( another ); } ///< Swap data with another array
        //@}

        /// \name common operators
        ///
        //@{
        DynaArray & operator=( const DynaArray & other ) { DoClone(other); return *this; }
        bool        operator==( const DynaArray & other ) const { return Equal(other); }
        bool        operator!=( const DynaArray & other ) const { return !Equal(other); }
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

        T * SubRange( size_t index, size_t length ) const
        {
            GN_ASSERT( mBegin <= (mPtr+index) );
            GN_ASSERT( (mPtr+index) < mEnd );
            GN_ASSERT( (mPtr+index+length) <= mEnd );
            GN_UNUSED_PARAM( length );
            return mPtr + index;
        }

        template<typename T2>
        void CopyTo( size_t srcOffset, const SafeArrayAccessor<T2> & dest, size_t dstOffset, size_t bytes )
        {
            GN_CASSERT( sizeof(T) == sizeof(T2) );
            memcpy( dest.SubRange( dstOffset, bytes ), SubRange( srcOffset, bytes ), bytes );
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
