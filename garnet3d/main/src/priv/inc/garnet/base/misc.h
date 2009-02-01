#ifndef __GN_BASE_MISC_H__
#define __GN_BASE_MISC_H__
// *****************************************************************************
/// \file
/// \brief   misc functions and types
/// \author  chenlee (2005.5.4)
// *****************************************************************************

#include <vector>

///
/// Execute only limited times during the entir life of the
/// application, no matter how many time it is called.
///
#define GN_DO_LIMITED_TIMES(n,X)      \
    if (true)                         \
    {                                 \
        static size_t s_counter = n;  \
        if( s_counter > 0 )           \
        {                             \
            --s_counter;              \
            X;                        \
        }                             \
    } else void(0)

///
/// Do something only once. 通常用来在内层循环中输出一些调试和错误信息。
///
#define GN_DO_ONCE(X) GN_DO_LIMITED_TIMES(1,X)

///
/// "Use" unused parameter (to avoid compiler warnings)
///
#define GN_UNUSED_PARAM(X) ((void)(X))

///
/// Get element count of C-style array
///
#define GN_ARRAY_COUNT(x) (sizeof(x)/sizeof(x[0]))

///
/// Get byte offset of class member or struct field
///
#define GN_FIELD_OFFSET( class_, field ) \
    ( (size_t)(UIntPtr) &( ((class_*)(void*)(8))->field ) - 8 )

namespace GN
{
    ///
    /// General handle type
    ///
    typedef const void * HandleType;

    ///
    /// min
    ///
    template<typename T> inline const T & min( const T & a, const T & b ) { return a < b ? a : b; }

    ///
    /// max
    ///
    template<typename T> inline const T & max( const T & a, const T & b ) { return a > b ? a : b; }

    ///
    /// Swap endian. Do "in-place" swap, if dst == src.
    ///
    GN_FORCE_INLINE void swapEndian8In32( void * dst, const void * src, size_t countInInt32 )
    {
        GN_ASSERT( dst && src );
        UInt32 * d = (UInt32*)dst;
        const UInt32 * s = (const UInt32*)src;
        for( size_t i = 0; i < countInInt32; ++i, ++s, ++d )
        {
            *d = ( ((*s)         ) << 24 ) |
                 ( ((*s)&0xFF00  ) << 8  ) |
                 ( ((*s)&0xFF0000) >> 8  ) |
                 ( ((*s)         ) >> 24 );
        }
    }

    ///
    /// Swap endian. Do "in-place" swap, if dst == src.
    ///
    GN_FORCE_INLINE void swapEndian16In32( void * dst, const void * src, size_t countInInt32 )
    {
        GN_ASSERT( dst && src );
        UInt32 * d = (UInt32*)dst;
        const UInt32 * s = (const UInt32*)src;
        for( size_t i = 0; i < countInInt32; ++i, ++s, ++d )
        {
            *d = ( (*s) >> 16 ) |
                 ( (*s) << 16 );
        }
    }

    ///
    /// Swap endian. Do "in-place" swap, if dst == src.
    ///
    GN_FORCE_INLINE void swapEndian8In16( void * dst, const void * src, size_t countInInt16 )
    {
        GN_ASSERT( dst && src );
        UInt16 * d = (UInt16*)dst;
        const UInt16 * s = (const UInt16*)src;
        for( size_t i = 0; i < countInInt16; ++i, ++s, ++d )
        {
            *d = ( (*s) >> 8 ) |
                 ( (*s) << 8 );
        }
    }

    ///
    /// type cast function
    ///
    /// perform dynamic cast in debug build, and reinterpret cast in release build.
    // ------------------------------------------------------------------------
    template < class TO, class FROM >
    GN_FORCE_INLINE TO & safeCastRef( FROM & from )
    {
    #if GN_BUILD_DEBUG && ( !GN_MSVC || defined(_CPPRTTI) )
        return dynamic_cast<TO&>(from);
    #else
        return reinterpret_cast<TO&>(from);
    #endif
    }

    ///
    /// type cast function
    ///
    /// perform dynamic cast in debug build, and reinterpret cast in release build.
    // ------------------------------------------------------------------------
    template < class TO, class FROM >
    GN_FORCE_INLINE TO * safeCastPtr( FROM * from )
    {
    #if GN_BUILD_DEBUG && ( !GN_MSVC || defined(_CPPRTTI) )
        TO * to = dynamic_cast<TO*>(from);
    #else
        TO * to = reinterpret_cast<TO*>(from);
    #endif

        GN_ASSERT( 0 == from || 0 != to );

        return to;
    }

    ///
    /// vector to pointer
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE T * vec2ptr( std::vector<T> & vec )
    {
        return vec.empty() ? 0 : &vec[0];
    }

    ///
    /// vector to pointer
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE const T * vec2ptr( const std::vector<T> & vec )
    {
        return vec.empty() ? 0 : &vec[0];
    }

    ///
    /// 将value限定在[vmin, vmax]区间内
    // ------------------------------------------------------------------------
    template < typename T >
    inline void clamp( T & value, const T & vmin, const T & vmax )
    {
        value = vmin > value ? vmin : vmax < value ? vmax : value;
    }

    ///
    /// align numeric value. The alignment must be 2^N.
    // ------------------------------------------------------------------------
    template< typename T >
    inline T align( const T & value, const T & alignment )
    {
        GN_ASSERT( math::isPowerOf2( alignment ) );
        return ( value + (alignment-1) ) & ~(alignment-1);
    }

    ///
    /// general safe deallocation routine
    // ------------------------------------------------------------------------
    template < typename T, typename DEALLOC_FUNC >
    GN_FORCE_INLINE void safeDealloc( T * & ptr )
    {
        if( ptr )
        {
            DEALLOC_FUNC( ptr );
            ptr = 0;
        }
    }

    ///
    /// free a C-style heap pointer
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void safeHeapFree( T * & ptr )
    {
        if( ptr )
        {
            heapFree( ptr );
            ptr = 0;
        }
    }

    ///
    /// delete one object
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void safeDelete( T * & ptr )
    {
        if( ptr )
        {
            delete ptr;
            ptr = 0;
        }
    }

    ///
    /// delete object array
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void safeDeleteArray( T * & ptr )
    {
        if( ptr )
        {
            delete [] ptr;
            ptr = 0;
        }
    }

    ///
    /// Safe release COM interface
    ///
    template < typename T >
    GN_FORCE_INLINE void safeRelease( T * & ptr )
    {
        if( ptr )
        {
            ptr->Release();
            ptr = 0;
        }
    }

    ///
    /// Safe release RefCounter class
    ///
    template < typename T >
    GN_FORCE_INLINE void safeDecref( T * & ptr )
    {
        if( ptr )
        {
            ptr->decref();
            ptr = 0;
        }
    }

    ///
    /// Set environment variable. Set value to empty or NULL to delete
    /// that environment variable.
    ///
    void putEnv( const char * name, const char * value );

    ///
    /// Get environment variable.
    ///
    void getEnv( StrA & result, const char * name );

    ///
    /// Get environment variable.
    ///
    inline StrA getEnv( const char * name )
    {
        StrA result;
        getEnv( result, name );
        return result;
    }

    ///
    /// Get environment variable as boolean variable. Return true only when the variable
    /// exists and the value is "1" or "yes".
    ///
    inline bool getEnvBoolean( const char * name )
    {
        StrA result;
        getEnv( result, name );
        return "1" == result ||
               0 == strCmpI( "yes", result.cptr() ) ||
               0 == strCmpI( "true", result.cptr() );
    }

    ///
    /// FOURCC structure
    ///
    union FOURCC
    {
        UInt32        u32;   ///< FOURCC as unsigned 32-bit integer
        SInt32        i32;   ///< FOURCC as 32-bit integer
        char          c8[4]; ///< FOURCC as 4 characters
        unsigned char u8[4]; ///< FOURCC as 4 unsigned characters

        ///
        /// conver to string
        ///
        const char * toStr() const
        {
            static char s[5];
            s[0] = c8[0];
            s[1] = c8[1];
            s[2] = c8[2];
            s[3] = c8[3];
            s[4] = 0;
            return s;
        }

        ///
        /// convert from string
        ///
        void fromStr(const char * str )
        {
            u32 = 0;
            if( str )
            {
                int i = 0;
                while( *str && i < 4 )
                {
                    c8[i] = *str;
                    ++str;
                    ++i;
                }
            }
        }

        ///
        /// Make FOURCC from string
        ///
        static FOURCC sMake( const char * str )
        {
            FOURCC r;
            r.fromStr( str );
            return r;
        }

        ///
        /// Make FOURCC by 4 characters
        ///
        static FOURCC sMake( char c0, char c1, char c2, char c3 )
        {
            GN_CASSERT( 4 == sizeof(FOURCC) );
            FOURCC r;
            r.c8[0] = c0;
            r.c8[1] = c1;
            r.c8[2] = c2;
            r.c8[3] = c3;
            return r;
        }

        /// \name commonly used operators
        //@{
        bool operator ==( const FOURCC & rhs ) const { return u32 == rhs.u32; }
        bool operator !=( const FOURCC & rhs ) const { return u32 == rhs.u32; }
        bool operator < ( const FOURCC & rhs ) const { return u32 <  rhs.u32; }
        //@}
    };

    ///
    /// GUID class
    ///
    struct Guid
    {
        /// \name data members
        //@{
        UInt32 data1;
        UInt16 data2;
        UInt16 data3;
        UInt8  data4[8];
        //@}

        /// \name operators
        //@{

        Guid & operator=( const Guid & rhs )
        {
            UInt64       * a = (UInt64*)this;
            const UInt64 * b = (const UInt64*)&rhs;
            a[0] = b[0];
            a[1] = b[1];
            return *this;
        }

        bool operator==( const Guid & rhs ) const
        {
            const UInt64 * a = (const UInt64*)this;
            const UInt64 * b = (const UInt64*)&rhs;
            return a[0] == b[0] && a[1] == b[1];
        }

        bool operator!=( const Guid & rhs ) const
        {
            const UInt64 * a = (const UInt64*)this;
            const UInt64 * b = (const UInt64*)&rhs;
            return a[0] != b[0] || a[1] != b[1];
        }

        bool operator < ( const Guid & rhs ) const
        {
            // Note: may produce different result on machine with different endian.
            const UInt64 * a = (const UInt64*)this;
            const UInt64 * b = (const UInt64*)&rhs;
            if( a[0] != b[0] )
                return a[0] < b[0];
            else
                return a[1] < b[1];
        }

        //@}
    };
    GN_CASSERT( sizeof(Guid) == 128/8 );

    ///
    /// Hard to explain in English, please see code by your self :)
    ///
    struct ScopeBool
    {
        bool & mValue; ///< Reference to the boolean variable.

        ///
        /// Ctor
        ///
        ScopeBool( bool & v ) : mValue(v) { mValue = true; }

        ///
        /// Dtor
        ///
        ~ScopeBool() { mValue = false; }
    };

    ///
    /// Hard to explain in English, please see code by your self :)
    ///
    template<typename T>
    struct ScopeInteger
    {
        T & mValue; ///< Reference to the integer variable.

        ///
        /// Ctor
        ///
        ScopeInteger( T & v ) : mValue(v) { ++mValue; }

        ///
        /// Dtor
        ///
        ~ScopeInteger() { --mValue; }
    };

    ///
    /// Automatically execute a function at the end of life scope
    ///
    template<class FUNCTOR>
    class AutoScope
    {
        FUNCTOR mFunc;
        bool    mDismissed;

    public:

        ///
        /// Ctor
        ///
        AutoScope( const FUNCTOR & func ) : mFunc(func), mDismissed(false) {}

        ///
        /// Dtor
        ///
        ~AutoScope() { if( !mDismissed ) mFunc(); }

        ///
        /// Tell AutoScope to _NOT_ run the function by the end of life scope.
        ///
        void dismiss() { mDismissed = true; }
    };

    ///
    /// Data type with automatic-initialization.
    ///
    template<typename T, T DEFAULT_VALUE>
    class AutoInit
    {
        T mValue;

    public:

        ///
        /// Default constructor
        ///
        AutoInit() : mValue(DEFAULT_VALUE) {}

        ///
        /// Construct from T
        ///
        AutoInit( const T & value ) : mValue(value) {}

        ///
        /// get internal value
        ///
        T & value() { return mValue; }

        ///
        /// get internal value
        ///
        const T & value() const { return mValue; }

        ///
        /// Convert to T
        ///
        operator T & () { return mValue; }

        ///
        /// Convert to constant T
        ///
        operator const T & () const { return mValue; }
    };

    ///
    /// Disable copy semantic of all descendants.
    ///
    class NoCopy
    {
    protected:
        ///
        /// Default constructor
        ///
        NoCopy() {}
        ///
        /// Destructor
        ///
        virtual ~NoCopy() {}
    private:  // emphasize the following members are private
        NoCopy( const NoCopy & );
        const NoCopy & operator = ( const NoCopy& );
    };

    namespace detail
    {
        ///
        /// Basic auto pointer class. Can NOT be used in STL containers.
        ///
        template<typename T, typename CLASS>
        class BaseAutoPtr : public NoCopy
        {
            T * mPtr;

            typedef BaseAutoPtr<T,CLASS> MyType;

            void release()
            {
                CLASS::sDoRelease(mPtr);
                mPtr = 0;
            }

        public:

            ///
            /// Construct from C-style
            ///
            explicit BaseAutoPtr( T * p ) throw() : mPtr(p) {}

            ///
            /// Destructor
            ///
            ~BaseAutoPtr() { release(); }

            ///
            /// Is pointer empty or not.
            ///
            bool empty() const { return 0 == mPtr; }

            ///
            /// Get internal pointer
            ///
            T * get() const { return mPtr; }

            ///
            /// clear internal pointer. Same as attach(0)
            ///
            void clear() { if( mPtr ) release(), mPtr = 0; }

            ///
            /// attach to new pointer (release the old one)
            ///
            void attach( T * p )
            {
                if( p != mPtr )
                {
                    release();
                    mPtr = p;
                }
            }

            ///
            /// Release ownership of private pointer
            ///
            T * detach() throw()
            {
                T * tmp = mPtr;
                mPtr = 0;
                return tmp;
            }

            ///
            /// dereference the pointer
            ///
            T & dereference() const { GN_ASSERT( mPtr ); return *mPtr; }

            ///
            /// Convert to T *
            ///
            operator T*() const { return mPtr; }

            ///
            /// dereference operator
            ///
            T & operator*() const { GN_ASSERT( mPtr ); return *mPtr; }

            ///
            /// arrow operator
            ///
            T * operator->() const { return mPtr; }
        };
    }

#if GN_POSIX
    ///
    /// Automatic X resource pointer
    ///
    template<typename T>
    class AutoXPtr : public detail::BaseAutoPtr< T, AutoXPtr<T> >
    {
        typedef detail::BaseAutoPtr< T, AutoXPtr<T> > ParentType;
        friend class detail::BaseAutoPtr< T, AutoXPtr<T> >;

        static void sDoRelease( T * p )
        {
            if( p ) XFree(p);
        }

    public:

        ///
        /// Construct from C-style pointer
        ///
        explicit AutoXPtr( T * p = 0 ) throw() : ParentType(p) {}
    };
#endif

    ///
    /// Automatic object pointer. Can NOT be used in STL containers.
    ///
    template<typename T>
    class AutoObjPtr : public detail::BaseAutoPtr< T, AutoObjPtr<T> >
    {
        typedef detail::BaseAutoPtr< T, AutoObjPtr<T> > ParentType;
#if GN_GCC
        friend class detail::BaseAutoPtr< T, AutoObjPtr<T> >;
#else
        friend class ParentType;
#endif

        static void sDoRelease( T * p )
        {
            if( p ) delete p;
        }

    public:

        ///
        /// Construct from C-style pointer
        ///
        explicit AutoObjPtr( T * p = 0 ) throw() : ParentType(p) {}
    };

    ///
    /// Automatic object array. Can NOT be used in STL containers.
    ///
    template<typename T>
    class AutoObjArray : public detail::BaseAutoPtr< T, AutoObjArray<T> >
    {
        typedef detail::BaseAutoPtr< T, AutoObjArray<T> > ParentType;
#if GN_GCC
        friend class detail::BaseAutoPtr< T, AutoObjArray<T> >;
#else
        friend class ParentType;
#endif

        static void sDoRelease( T * p )
        {
            if( p ) delete [] p;
        }

    public:

        ///
        /// Construct from C-style pointer
        ///
        explicit AutoObjArray( T * p = 0 ) throw() : ParentType(p) {}
    };

    ///
    /// Automatic C-style array created by heapAlloc. Can NOT be used in STL containers.
    ///
    template<typename T>
    class AutoTypePtr : public detail::BaseAutoPtr< T, AutoTypePtr<T> >
    {
        typedef detail::BaseAutoPtr< T, AutoTypePtr<T> > ParentType;
#if GN_GCC
        friend class detail::BaseAutoPtr< T, AutoTypePtr<T> >;
#else
        friend class ParentType;
#endif

        static void sDoRelease( T * p )
        {
            if( p ) heapFree(p);
        }

    public:

        ///
        /// Construct from C-style pointer
        ///
        explicit AutoTypePtr( T * p = 0 ) throw() : ParentType(p) {}
    };

    ///
    /// Automatic COM pointer class. CAN be used in STL containers.
    ///
    template <class T>
    class AutoComPtr
    {
        T * mPtr;
    public:

        ///
        /// Default constructor
        ///
        AutoComPtr() throw() : mPtr(0) {}

        ///
        /// Construct from normal pointer
        ///
        explicit AutoComPtr( T * p ) throw() : mPtr(p) {}

        ///
        /// Copy constructor
        ///
        AutoComPtr( const AutoComPtr & other ) throw() : mPtr(other.mPtr)
        {
            if( mPtr ) mPtr->AddRef();
        }

        ///
        /// Destructor
        ///
        ~AutoComPtr() throw()
        {
            if (mPtr) mPtr->Release();
        }

        ///
        /// Assignment operator
        ///
        AutoComPtr & operator=( const AutoComPtr & rhs )
        {
            set( rhs.mPtr );
            return *this;
        }

        ///
        /// Convert to T *
        ///
        operator T*() const throw()
        {
            return mPtr;
        }

        ///
        /// dereference operator
        ///
        T & operator*() const throw()
        {
            GN_ASSERT( mPtr );
            return *mPtr;
        }

        ///
        /// Get address of pointer.
        ///
        /// This operator is normally used during resource creation.
        /// This function will ensure that the internal pointer is NULL.
        ///
        T ** operator&() throw()
        {
            GN_ASSERT( NULL == mPtr );
            return &mPtr;
        }

        ///
        /// Get address of pointer.
        ///
        T * const * operator&() const throw()
        {
            GN_ASSERT( mPtr );
            return &mPtr;
        }

        ///
        /// self explain.
        ///
        T * operator->() const throw()
        {
            GN_ASSERT( mPtr );
            return mPtr;
        }

        ///
        /// NOT operator
        ///
        bool operator!() const throw()
        {
            return mPtr == NULL;
        }

        ///
        /// LESS operator
        ///
        bool operator<( T * pT ) const throw()
        {
            return mPtr < pT;
        }

        ///
        /// EQ operator
        ///
        bool operator==( T * pT ) const throw()
        {
            return mPtr == pT;
        }

        ///
        /// Is empty pointer?
        ///
        bool empty() const { return 0 == mPtr; }

        ///
        /// Clear to empty. Same as set(NULL).
        ///
        void clear() { if( mPtr ) mPtr->Release(); mPtr = 0; }

        ///
        /// Release existing interface, then hold new interface
        ///
        void set( T * p ) throw()
        {
            if( p ) p->AddRef();
            if( mPtr ) mPtr->Release();
            mPtr = p;
        }

        ///
        /// Attach to an existing interface (does not AddRef)
        ///
        void attach( T * p2 ) throw()
        {
            if (mPtr) mPtr->Release();
            mPtr = p2;
        }

        ///
        /// Detach the interface (does not Release)
        ///
        T * detach() throw()
        {
            T * pt = mPtr;
            mPtr = NULL;
            return pt;
        }
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
#endif // __GN_BASE_MISC_H__
