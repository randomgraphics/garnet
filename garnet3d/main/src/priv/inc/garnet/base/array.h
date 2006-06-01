#ifndef __GN_BASE_ARRAY_H__
#define __GN_BASE_ARRAY_H__
// *****************************************************************************
//! \file    base/array.h
//! \brief   Common array (vector) class
//! \author  chenlee (2006.5.31)
// *****************************************************************************

namespace GN
{
    //!
    //! Fixed sized array, which always has N elements.
    //! Behaves like C-style array, but with bound check in debug build.
    //!
    template<class T, size_t N>
    class FixedArray
    {
        T mElements[N];

    public:

        typedef T ElementType; //!< element type

        static const size_t MAX_SIZE = N; //!< maximum size

        //!
        //! at operator
        //!
        T & operator[]( size_t i ) { GN_ASSERT( i < N ); return mElements[i]; }

        //!
        //! at operator
        //!
        const T & operator[]( size_t i ) const { GN_ASSERT( i < N ); return mElements[i]; }

        //!
        //! convert to C pointer
        //!
        operator const T*() const { return mElements; }

        //!
        //! convert to C pointer
        //!
        operator T*() { return mElements; }
    };

    //!
    //! Fixed size array with supporting to common array operations
    //! like push, pop, insert, remove and etc.
    //!
    template<class T, size_t N>
    class StackArray
    {
        T mElements[N];
        size_t mCount;

        void doClone( const StackArray & other )
        {
            for( size_t i = 0; i < mCount; ++i )
            {
                mElements[i] = other.mElements[i];
            }
            mCount = other.mCount;
        }

        void doInsert( size_t position, const T & t )
        {
            GN_ASSERT( mCount < N );
            GN_ASSERT( position <= mCount );
            for( size_t i = mCount; i > position; --i )
            {
                mElements[i] = mElements[i-1];
            }
            mElements[position] = t;
            ++mCount;
        }

        T doErase( size_t position )
        {
            GN_ASSERT( mCount > 0 );
            GN_ASSERT( position < mCount );
            --mCount;
            for( size_t i = position; i < mCount; ++i )
            {
                mElements[i] = mElements[i+1];
            }
        }

    public:

        typedef T ElementType; //!< element type

        static const size_t MAX_SIZE = N; //!< maximum size

        //!
        //! default constructor
        //!
        StackArray() : mCount(0) {}

        //!
        //! constructor with user-defined count.
        //!
        explicit StackArray( size_t count ) : mCount(count) {}

        //!
        //! copy constructor
        //!
        StackArray( const StackArray & other ) { doClone( other ); }

        //! \name Common array operations.
        //!
        //@{
        void      append( const T & t ) { GN_ASSERT( mCount < N ); mElements[mCount] = t; ++mCount; }
        const T & back() const { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        T       & back() { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        const T * begin() const { return mElements; }
        T       * begin() { return mElements; }
        void      clear() { mCount = 0; }
        const T * cptr() const { return mElements; }
        T       * cptr() { return mElements; }
        void      empty() { return 0 == mCount; }
        const T * end() const { return mElements + mCount; }
        T       * end() { return mElements + mCount; }
        T         erase( size_t position ) { return doErase( position ); }
        const T & front() const { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        T       & front() { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        void      insert( size_t position, const T & t ) { doInsert( position, t ); }
        void      resize( size_t count ) { GN_ASSERT( count <= N ); mCount = count; }
        T         popBack() { GN_ASSERT( mCount > 0 ); --mCount; return mElements[mCount]; }
        void      size() { return mCount; }
        //@}

        //! \name common operators
        //!
        //@{
        StackArray & operator=( const StackArray & other ) { doClone(other); return *this; }
        T          & operator[]( size_t i ) { GN_ASSERT( i < mCount ); return mElements[i]; }
        const T    & operator[]( size_t i ) const { GN_ASSERT( i < mCount ); return mElements[i]; }
        operator const T*() const { return mElements; }
        operator T*() { return mElements; }
        //@}
    };

    //!
    //! Resizeable array.
    //!
    template<class T, class ALLOCATOR = StandardAllocator<T> >
    class DynamicArray
    {
        T * mElements;
        size_t mCount;
        size_t mCapacity;

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
            for( size_t i = 0; i < mCount; ++i )
            {
                dst[i] = src[i];
            }
        }

        void doClone( const DynamicArray & other )
        {
            GN_ASSERT( mCount == other.mCount );
            resize( other.mCount );
            copyElements( mElements, other.mElements, other.mCount );
        }

        void doInsert( size_t position, const T & t )
        {
            GN_ASSERT( position <= mCount );

            resize( mCount + 1 );

            for( size_t i = mCount-1; i > position; --i )
            {
                mElements[i] = mElements[i-1];
            }
            mElements[position] = t;
        }

        T doErase( size_t position )
        {
            GN_ASSERT( mCount > 0 );
            GN_ASSERT( position < mCount );
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

    public:

        typedef T ElementType; //!< element type
        typedef ALLOCATOR AllocatorType; //!< allocator type

        //!
        //! default constructor
        //!
        DynamicArray() : mElements(0), mCount(0), mCapacity(0) {}

        //!
        //! constructor with user-defined count.
        //!
        explicit DynamicArray( size_t count ) : mElements(0), mCount(0), mCapacity(0) { resize( count ); }

        //!
        //! copy constructor
        //!
        DynamicArray( const DynamicArray & other ) : mElements(0), mCount(0), mCapacity(0) { doClone( other ); }

        //!
        //! destructor
        //!
        ~DynamicArray() { dealloc(); }

        //! \name Common array operations.
        //!
        //@{
        void      append( const T & t ) { resize( mCount + 1 ); back() = t; }
        const T & back() const { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        T       & back() { GN_ASSERT( mCount > 0 ); return mElements[mCount-1]; }
        const T * begin() const { return mElements; }
        T       * begin() { return mElements; }
        void      clear() { mCount = 0; }
        const T * cptr() const { return mElements; }
        T       * cptr() { return mElements; }
        void      empty() { return 0 == mCount; }
        const T * end() const { return mElements + mCount; }
        T       * end() { return mElements + mCount; }
        T         erase( size_t position ) { return doErase( position ); }
        const T & front() const { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        T       & front() { GN_ASSERT( mCount > 0 ); return mElements[0]; }
        void      insert( size_t position, const T & t ) { doInsert( position, t ); }
        void      reserve( size_t count ) { doReserve( count ); }
        void      resize( size_t count ) { doReserve( count ); mCount = count; }
        T         popBack() { GN_ASSERT( mCount > 0 ); --mCount; return mElements[mCount]; }
        void      size() { return mCount; }
        //@}

        //! \name common operators
        //!
        //@{
        DynamicArray & operator=( const DynamicArray & other ) { doClone(other); return *this; }
        T            & operator[]( size_t i ) { GN_ASSERT( i < mCount ); return mElements[i]; }
        const T      & operator[]( size_t i ) const { GN_ASSERT( i < mCount ); return mElements[i]; }
        operator const T*() const { return mElements; }
        operator T*() { return mElements; }
        //@}
    };
}

// *****************************************************************************
//                           End of array.h
// *****************************************************************************
#endif // __GN_BASE_ARRAY_H__
