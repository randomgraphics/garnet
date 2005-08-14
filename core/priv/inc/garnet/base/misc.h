#ifndef __GN_BASE_MISC_H__
#define __GN_BASE_MISC_H__
// *****************************************************************************
//! \file    misc.h
//! \brief   misc functions and types
//! \author  chenlee (2005.5.4)
// *****************************************************************************

//!
//! Execute only limited times during the entir life of the
//! application, no matter how many time it is called.
//!
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

//!
//! Do something only once. 通常用来在内层循环中输出一些调试和错误信息。
//!
#define GN_DO_ONCE(X) GN_DO_LIMITED_TIMES(1,X)

//!
//! "Use" unused parameter (to avoid compiler warnings)
//!
#define GN_UNUSED_PARAM(X) ((void)(X))

// *************************************************************************
//! \name StdCalss
// *************************************************************************

//@{

namespace GN
{
    //!
    //! Disable copy semantic of all descendants.
    //!
    class NoCopy
    {
    protected:
        //!
        //! Default constructor
        //!
        NoCopy() {}
        //!
        //! Destructor
        //!
        virtual ~NoCopy() {}
    private:  // emphasize the following members are private
        NoCopy( const NoCopy & );
        const NoCopy & operator = ( const NoCopy& );
    };

    //!
    //! type cast function
    //!
    //! perform dynamic cast in debug build, and static cast in release build.
    // ------------------------------------------------------------------------
    template < class TO, class FROM >
    GN_FORCE_INLINE TO safeCast( FROM from )
    {
    #if GN_DEBUG && ( !GN_MSVC || defined(_CPPRTTI) )
        GN_ASSERT( 0 == from || dynamic_cast<TO>(from) );
        return dynamic_cast<TO>(from);
    #else
        return static_cast<TO>(from);
    #endif
    }

    //!
    //! vector to pointer
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE T * vec2ptr( std::vector<T> & vec )
    {
        return vec.empty() ? 0 : &vec[0];
    }

    //!
    //! vector to pointer
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE const T * vec2ptr( const std::vector<T> & vec )
    {
        return vec.empty() ? 0 : &vec[0];
    }

    //!
    //! 将value限定在[vmin, vmax]区间内
    // ------------------------------------------------------------------------
    template < typename T >
    inline void clamp( T & value, const T & vmin, const T & vmax )
    {
        value = vmin > value ? vmin : vmax < value ? vmax : value;
    }

    //!
    //! general safe delLocation routine
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

    //!
    //! free one object
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void safeFree( T * & ptr )
    {
        if( ptr )
        {
            memFree( ptr );
            ptr = 0;
        }
    }

    //!
    //! delete one object
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

    //!
    //! delete object array
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

    //!
    //! Safe release COM interface
    //!
    template < typename T >
    GN_FORCE_INLINE void safeRelease( T * & ptr )
    {
        if( ptr )
        {
            ptr->Release();
            ptr = 0;
        }
    }
}

// *****************************************************************************
//                           End of misc.h
// *****************************************************************************
#endif // __GN_BASE_MISC_H__
