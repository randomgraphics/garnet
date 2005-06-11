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

namespace GN
{
    //!
    //! type cast function
    //!
    //! perform dynamic cast in debug build, and static cast in release build.
    // ------------------------------------------------------------------------
    template < class TO, class FROM >
    GN_FORCE_INLINE TO safe_cast( FROM from )
    {
    #if defined(GN_DEBUG) && ( !defined(GN_MSVC) || defined(_CPPRTTI) )
        GN_ASSERT( dynamic_cast<TO>(from) );
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

    //! \cond
    namespace misc_detail
    {
        //!
        //! delete one object instance
        //!
        template < typename T >
        struct delete_element
        {
            //!
            //! delete one object instance
            //!
            static void deallocate( T * & ptr )
            {
                delete ptr;
            }
        };

        //!
        //! delete an object array
        //!
        template < typename T >
        struct delete_array
        {
            //!
            //! delete an object array
            //!
            static void deallocate( T * & ptr )
            {
                delete [] ptr;
            }
        };

        //template < typename T >
        //struct release_object
        //{
        //    static void deallocate( T * & ptr )
        //    {
        //        ptr->release();
        //    }
        //};
    }
    //! \endcond

    //!
    //! general safe delocation routine
    // ------------------------------------------------------------------------
    template < typename T, typename deallocate_pilocy >
    GN_FORCE_INLINE void safe_deallocate( T * & ptr )
    {
        if( ptr )
        {
            deallocate_pilocy::deallocate( ptr );
            ptr = NULL;
        }
    }

    //!
    //! delete one object
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void safe_delete( T * & ptr )
    {
        safe_deallocate<T,misc_detail::delete_element<T> >(ptr);
    }

    //!
    //! delete object array
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void safe_delete_array( T * & ptr )
    {
        safe_deallocate<T,misc_detail::delete_array<T> >(ptr);
    }
}

// *****************************************************************************
//                           End of misc.h
// *****************************************************************************
#endif // __GN_BASE_MISC_H__
