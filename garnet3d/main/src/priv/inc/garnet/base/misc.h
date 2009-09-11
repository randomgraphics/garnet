#ifndef __GN_BASE_MISC_H__
#define __GN_BASE_MISC_H__
// *****************************************************************************
/// \file
/// \brief   misc functions and types
/// \author  chenlee (2005.5.4)
// *****************************************************************************

#include <vector>

///
/// Get element count of C-style array
///
#define GN_ARRAY_COUNT(x) (sizeof(x)/sizeof(x[0]))

///
/// Get byte offset of class member or struct field
///
#define GN_FIELD_OFFSET( class_, field ) \
    ( (size_t)(UIntPtr) &( ((class_*)(void*)(8))->field ) - 8 )

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

namespace GN
{
    ///
    /// General handle type
    ///
    typedef const void * HandleType;

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
    /// exists and the value is "1" or "yes" (case-insensitive).
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

        /// \name public methods
        //@{

        const char * toStr() const; // Note: this function is not thread safe

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
    /// Increase counter by one when entering the life scope, decrease the counter by one
    /// when going out of the life scope.
    ///
    template<typename T>
    struct ScopeCounter
    {
        T & mValue; ///< Reference to the integer variable.

        ///
        /// Ctor
        ///
        ScopeCounter( T & v ) : mValue(v) { ++mValue; }

        ///
        /// Dtor
        ///
        ~ScopeCounter() { --mValue; }
    };

    ///
    /// Data type with automatic-initialization.
    ///
    template<typename T, T DEFAULT_VALUE>
    class AutoInitializer
    {
        T mValue;

    public:

        ///
        /// Default constructor
        ///
        AutoInitializer() : mValue(DEFAULT_VALUE) {}

        ///
        /// Construct from T
        ///
        AutoInitializer( const T & value ) : mValue(value) {}

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
    /// Automatically execute a function at the end of life scope
    ///
    template<class FUNCTOR>
    class AutoFinalizer
    {
        FUNCTOR mFunc;
        bool    mDismissed;

    public:

        ///
        /// Ctor
        ///
        AutoFinalizer( const FUNCTOR & func ) : mFunc(func), mDismissed(false) {}

        ///
        /// Dtor
        ///
        ~AutoFinalizer() { if( !mDismissed ) mFunc(); }

        ///
        /// Tell finalizer to _NOT_ run the function by the end of life scope.
        ///
        void dismiss() { mDismissed = true; }
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
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_MISC_H__
