#ifndef __GN_BASE_ARRAY_H__
#define __GN_BASE_ARRAY_H__
// *****************************************************************************
/// \file
/// \brief   Common array (vector) class
/// \author  chenlee (2006.5.31)
// *****************************************************************************

#include <vector>

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
        const T* rawptr() const { return mElements; }

        ///
        /// convert to C pointer
        ///
        T* rawptr() { return mElements; }

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
            T * p = rawptr();
            for( SIZE_TYPE i = 0; i < mCount; ++i, ++p )
            {
                dtor( p );
            }
            mCount = 0;
        }

        void copyFrom( const StackArray & other )
        {
            T       * dst = rawptr();
            const T * src = other.rawptr();

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

            T * p = rawptr();

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

            T * p = rawptr();

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

            T * p = rawptr();

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

            const T * p1 = rawptr();
            const T * p2 = other.rawptr();

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
            ctor( rawptr(), count );
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
        const T & back() const { GN_ASSERT( mCount > 0 ); return rawptr()[mCount-1]; }
        T       & back() { GN_ASSERT( mCount > 0 ); return rawptr()[mCount-1]; }
        const T * begin() const { return rawptr(); }
        T       * begin() { return rawptr(); }
        void      clear() { doClear(); }
        const T * rawptr() const { return (const T*)mBuffer; }
        T       * rawptr() { return (T*)mBuffer; }
        bool      empty() const { return 0 == mCount; }
        const T * end() const { return rawptr() + mCount; }
        T       * end() { return rawptr() + mCount; }
        /** do nothing if position is invalid or array is empty */
        void      eraseIdx( SIZE_TYPE position ) { doErase( position ); }
        void      erasePtr( const T * ptr ) { doErase( ptr - mBuffer ); }
        const T & front() const { GN_ASSERT( mCount > 0 ); return rawptr()[0]; }
        T       & front() { GN_ASSERT( mCount > 0 ); return rawptr()[0]; }
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
        T          & operator[]( SIZE_TYPE i ) { GN_ASSERT( i < mCount ); return rawptr()[i]; }
        const T    & operator[]( SIZE_TYPE i ) const { GN_ASSERT( i < mCount ); return rawptr()[i]; }
        //@}
    };

    ///
    /// Resizeable array.
    ///
    template<class T, typename SIZE_TYPE = size_t, class OBJECT_ALLOCATOR = CxxObjectAllocator<T> >
    class DynaArray
    {
        struct Header
        {
            SIZE_TYPE count;
            SIZE_TYPE capacity;
        };

        T * mElements = nullptr;
        // SIZE_TYPE        mCount;
        // SIZE_TYPE        mCapacity;

        /// Destruct all objects, and free the memory.
        void destroyAll()
        {
            if (!mElements) return;
            Header & h = GetHeader();
            T * end = mElements + h.count;
            for( T * p = mElements; p < end; ++p )
            {
                OBJECT_ALLOCATOR::sDestruct( p );
            }
            OBJECT_ALLOCATOR::sDeallocate( (void*)&h );
            mElements = nullptr;
        }

        Header & GetHeader()
        {
            GN_ASSERT(mElements);
            return *(((Header*)mElements) - 1);
        }

        const Header & GetHeader() const
        {
            GN_ASSERT(mElements);
            return *(((const Header*)mElements) - 1);
        }

        SIZE_TYPE GetCount() const { return mElements ? GetHeader().count : 0; }

        bool doAppend( const T * p, SIZE_TYPE count )
        {
            if( 0 == count ) return true;

            if( 0 == p )
            {
                GN_ERROR(getLogger("GN.base.DynaArray"))("non-zero count with NULL pointer is not allowed!");
                return false;
            }

            // reserve memory
            if( !doReserve( GetCount() + count ) ) return false;

            // copy-construct new elements
            auto & h = GetHeader();
            T * dst = mElements + h.count;
            for( SIZE_TYPE i = 0; i < count; ++i, ++dst, ++p )
            {
                OBJECT_ALLOCATOR::sConstruct( dst, *p );
            }

            // update count
            h.count += count;

            return true;
        }

        bool doMoveAppend( T * p, SIZE_TYPE count )
        {
            if( 0 == count ) return true;

            if( 0 == p )
            {
                GN_ERROR(getLogger("GN.base.DynaArray"))("non-zero count with NULL pointer is not allowed!");
                return false;
            }

            // reserve memory
            if( !doReserve( GetCount() + count ) ) return false;

            // copy-construct new elements
            auto & h = GetHeader();
            T * dst = mElements + h.count;
            for( SIZE_TYPE i = 0; i < count; ++i, ++dst, ++p )
            {
                OBJECT_ALLOCATOR::sConstruct( dst, std::move(*p) );
            }

            // update count
            h.count += count;

            return true;
        }

        void doClear()
        {
            // Destruct all objects, but do not free memory.
            if (!mElements) return;
            T * p = mElements;
            auto & h = GetHeader();
            auto count = GetCount();
            for( SIZE_TYPE i = 0; i < count; ++i, ++p )
            {
                OBJECT_ALLOCATOR::sDestruct( p );
            }
            h.count = 0;
        }

        void moveFrom( DynaArray & other )
        {
            if (this != &other) {
                destroyAll();
                mElements = other.mElements;
                other.mElements = nullptr;
            }
        }

        bool copyFrom( const DynaArray & other )
        {
            if( !doReserve( other.GetCount() ) ) return false;

            auto myCount = GetCount();
            auto othersCount = other.GetCount();

            SIZE_TYPE mincount = math::getmin<SIZE_TYPE>( myCount, othersCount );

            for( SIZE_TYPE i = 0; i < mincount; ++i )
            {
                mElements[i] = other.mElements[i];
            }

            // destruct extra objects, only when other.h.count < h.count
            for( SIZE_TYPE i = othersCount; i < myCount; ++i )
            {
                OBJECT_ALLOCATOR::sDestruct( mElements + i );
            }

            // copy-construct new objects, only when h.count < other.h.count
            for( SIZE_TYPE i = myCount; i < othersCount; ++i )
            {
                OBJECT_ALLOCATOR::sConstruct( mElements + i, other.mElements[i] );
            }

            // update count
            if (mElements) GetHeader().count = othersCount;

            return true;
        }

        bool doInsert( SIZE_TYPE position, const T & t )
        {
            auto count = GetCount();

            if( position > count )
            {
                GN_WARN(getLogger("GN.base.DynaArray"))("invalid insert position");
                return false;
            }

            if( !doResize( count + 1 ) ) return false;

            GN_ASSERT(GetCount() == count + 1);

            for (SIZE_TYPE i = count; i > position; --i)
            {
                mElements[i] = std::move(mElements[i - 1]);
            }

            // TODO: make this a move too.
            mElements[position] = t;

            return true;
        }

        void doErase( SIZE_TYPE position )
        {
            if( position >= GetCount() )
            {
                GN_ERROR(getLogger("GN.base.DynaArray"))("invalid erase position");
                return;
            }

            auto & h = GetHeader();

            GN_ASSERT(h.count > 0);
            --h.count;

            // move elements forward
            for( SIZE_TYPE i = position; i < h.count; ++i )
            {
                mElements[i] = mElements[i+1];
            }

            // then destruct the last element
            OBJECT_ALLOCATOR::sDestruct( mElements + h.count );
        }

        bool doReserve( SIZE_TYPE count )
        {
            if (0 == count) return true;

            const Header * oldHeader = mElements ? &GetHeader() : nullptr;
            SIZE_TYPE oldCap = oldHeader ? oldHeader->capacity : 0;
            if (oldCap >= count) return true;

            SIZE_TYPE oldCount = oldHeader ? oldHeader->count : 0;
            GN_ASSERT( count > oldCount );

            // align caps to next power of 2
            GN_ASSERT(count > 0);
            uint64 newCap = count - 1;
            newCap |= newCap >> 32;
            newCap |= newCap >> 16;
            newCap |= newCap >> 8;
            newCap |= newCap >> 4;
            newCap |= newCap >> 2;
            newCap |= newCap >> 1;
            newCap += 1;

            // Cap to maximum allowable value.
            const uint64 MAX_CAPS = (uint64)(SIZE_TYPE)-1;
            if( newCap > MAX_CAPS ) newCap = MAX_CAPS;

            GN_ASSERT( count <= MAX_CAPS );

            // allocate new buffer (unconstructed raw memory)
            Header * newHeader = (Header*)OBJECT_ALLOCATOR::sAllocate( (SIZE_TYPE)(newCap + sizeof(Header)) );
            if( NULL == newHeader )
            {
                GN_ERROR(getLogger("GN.base.DynaArray"))("out of memory!");
                return false;
            }
            newHeader->capacity = (SIZE_TYPE)newCap;
            newHeader->count = oldCount;

            T * newBuf = (T*)(newHeader + 1);
            for( SIZE_TYPE i = 0; i < oldCount; ++i )
            {
                if constexpr (std::is_move_constructible<T>::value) {
                    OBJECT_ALLOCATOR::sConstruct( newBuf + i, std::move(mElements[i]) );
                } else {
                    OBJECT_ALLOCATOR::sConstruct( newBuf + i, mElements[i] );
                }
            }

            // deallocate old buffer
            destroyAll();

            // done
            mElements = newBuf;
            return true;
        }

        bool doResize( SIZE_TYPE count )
        {
            auto oldCount = GetCount();
            
            if( oldCount == count ) return true; // shortcut for redundant call

            // reserve memory
            if( !doReserve( count ) ) return false;

            // destruct extra objects, only when count < h.count
            for( SIZE_TYPE i = count; i < oldCount; ++i )
            {
                OBJECT_ALLOCATOR::sDestruct( mElements + i );
            }

            // construct new objects, only when h.count < count
            for( SIZE_TYPE i = oldCount; i < count; ++i )
            {
                OBJECT_ALLOCATOR::sConstruct( mElements + i );
            }

            if (mElements) GetHeader().count = count;

            return true;
        }

        bool doResize( SIZE_TYPE count, const T & t )
        {
            auto oldCount = GetCount();
            
            if( count == oldCount ) return true; // shortcut for redundant call

            // reserve memory
            if( !doReserve( count ) ) return false;

            // destruct extra objects, only when count < h.count
            for( SIZE_TYPE i = count; i < oldCount; ++i )
            {
                OBJECT_ALLOCATOR::sDestruct( mElements + i );
            }

            // copy-construct new objects, only when h.count < count
            for( SIZE_TYPE i = oldCount; i < count; ++i )
            {
                OBJECT_ALLOCATOR::sConstruct( mElements + i, t );
            }

            if (mElements) GetHeader().count = count;

            return true;
        }

        void doSwap( DynaArray & another )
        {
            T * p = mElements;
            mElements = another.mElements;
            another.mElements = p;
        }

        bool equal( const DynaArray & other ) const
        {
            auto c1 = GetCount();
            auto c2 = other.GetCount();
            if( c1 != c2 ) return false;
            for( SIZE_TYPE i = 0; i < c1; ++i )
            {
                if( mElements[i] != other.mElements[i] ) return false;
            }
            return true;
        }

    public:

        typedef T                ElementType;   //< element type
        typedef OBJECT_ALLOCATOR AllocatorType; //< allocator type
        typedef const T        * ConstIterator; //< Constant iterator type
        typedef T              * Iterator;      //< Iterator type.

        ///
        /// default constructor
        ///
        DynaArray() : mElements(0) {}

        ///
        /// constructor with user-defined count.
        ///
        explicit DynaArray( SIZE_TYPE count ) : mElements(0) { doResize( count ); }

        ///
        /// constructor with user-defined count and value.
        ///
        DynaArray( SIZE_TYPE count, const T & t ) : mElements(0) { doResize( count, t ); }

        ///
        /// construct from conventional C array
        ///
        DynaArray( const T * p, SIZE_TYPE count ) : mElements(0) { doAppend( p, count ); }

        ///
        /// construct from conventional C array
        ///
        DynaArray( const T * begin, const T * end ) : mElements(0) { doAppend( begin, end - begin ); }

        ///
        /// copy constructor
        ///
        DynaArray( const DynaArray & other ) : mElements(0) { copyFrom( other ); }

        ///
        /// move constructor
        ///
        DynaArray( DynaArray && other ) : mElements(0) { moveFrom( other ); }

        ///
        /// destructor
        ///
        ~DynaArray() { destroyAll(); }

        /// \name Common array operations.
        ///
        //@{
        bool      append( const T & t ) { return doAppend( &t, 1 ); }
        bool      append( T && t ) { return doMoveAppend( &t, 1 ); }
        bool      append( const T * p, SIZE_TYPE count ) { return doAppend( p, count ); }
        bool      append( const DynaArray & a ) { return doAppend( a.mElements, a.size() ); }
        bool      append( const std::vector<T> & a ) { return doAppend( a.data(), (SIZE_TYPE)a.size() ); }
        const T & back() const { auto & h = GetHeader(); GN_ASSERT( h.count > 0 ); return mElements[h.count-1]; }
        T       & back() { auto & h = GetHeader(); GN_ASSERT( h.count > 0 ); return mElements[h.count-1]; }
        const T * begin() const { return mElements; }
        T       * begin() { return mElements; }
        SIZE_TYPE capacity() const { return mElements ? GetHeader().capacity : 0; }
        void      clear() { doClear(); }
        const T * data() const { return mElements; }
        T       * data() { return mElements; }
        const T * rawptr() const { return mElements; } // obsolete
        T       * rawptr() { return mElements; } // oboslete
        bool      empty() const { return 0 == GetCount(); }
        const T * end() const { return mElements + GetCount(); }
        T       * end() { return mElements + GetCount(); }
        /** do nothing if position is invalid */
        void      eraseIdx( SIZE_TYPE position ) { return doErase( position ); }
        void      erasePtr( const T * p ) { return doErase( p - mElements ); }
        const T * first() const { return mElements; }
        T       * first() { return mElements; }
        const T & front() const { GN_ASSERT(GetCount() > 0); return mElements[0]; }
        T       & front() { GN_ASSERT(GetCount() > 0); return mElements[0]; }
        /** do nothing if position is invalid */
        bool      insert( SIZE_TYPE position, const T & t ) { return doInsert( position, t ); }
        //const T * next( const T * t ) const { return ( mCount > 0 && mElements <= t && t < (mElements+mCount-1) ) ? ( t + 1 ) : NULL; }
        //T       * next( const T * t ) { return ( mCount > 0 && mElements <= t && t < (mElements+mCount-1) ) ? ( t + 1 ) : NULL; }
        bool      reserve( SIZE_TYPE count ) { return doReserve( count ); }
        bool      resize( SIZE_TYPE count ) { return doResize( count ); }
        void      popBack() { auto c = GetCount(); if (c > 0) doErase( c - 1 ); }
        /** clear array as well as release memory */
        void      purge() { destroyAll(); GN_ASSERT(nullptr == mElements); }
        SIZE_TYPE size() const { return GetCount(); }
        void      swap( DynaArray & another ) { doSwap( another ); } ///< swap data with another array
        //@}

        /// \name common operators
        ///
        //@{
        DynaArray & operator=( const DynaArray & other ) { copyFrom(other); return *this; }
        DynaArray & operator=( DynaArray && other ) { moveFrom(other); return *this; }
        bool        operator==( const DynaArray & other ) const { return equal(other); }
        bool        operator!=( const DynaArray & other ) const { return !equal(other); }
        T         & operator[]( SIZE_TYPE i ) { GN_ASSERT( i < GetCount() ); return mElements[i]; }
        const T   & operator[]( SIZE_TYPE i ) const { GN_ASSERT( i < GetCount() ); return mElements[i]; }
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

    // TODO: replace with std::size()
    template <typename T, std::size_t N>
    constexpr std::size_t countof(T const (&)[N]) noexcept
    {
        return N;
    }
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_ARRAY_H__
