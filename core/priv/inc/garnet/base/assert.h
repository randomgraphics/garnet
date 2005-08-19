#ifndef __GN_BASE_ASSERT_H__
#define __GN_BASE_ASSERT_H__
// *****************************************************************************
//! \file    assert.h
//! \brief   assert functions and macros
//! \author  chenlee (2005.4.17)
// *****************************************************************************

//!
//! µ÷ÊÔÆ÷¶Ïµã
//!
#if GN_GCC
#define GN_DEBUG_BREAK asm("int $3")
#elif GN_MSVC
#if GN_WINX64 || GN_XENON
#define GN_DEBUG_BREAK ::GN::debugBreak()
#else
#define GN_DEBUG_BREAK __asm { int 3 }
#endif
#else
#define GN_DEBUG_BREAK
#endif

//!
//! Assert failture
//!
#define GN_ASSERT_FAILURE( desc )                               \
    {                                                           \
        static bool sIgnoreAll = false;                         \
        if( !sIgnoreAll && GN::assertFunc( desc, __FILE__,      \
            __LINE__, &sIgnoreAll ) )                           \
        { GN_DEBUG_BREAK; }                                     \
    }

#if GN_DEBUG

//!
//! assert macro with a description string (debug-build)
//!
#define GN_ASSERT_EX( exp, desc ) \
        if( !(exp) ) GN_ASSERT_FAILURE(desc) else void(0)

//!
//! verify macro with a description string (debug-build)
//!
#define GN_VERIFY_EX GN_ASSERT_EX

#else // !GN_DEBUG

//!
//! assert macro with a description string (release-build)
//!
#define GN_ASSERT_EX( exp, desc )  void(0)

//!
//! assert macro with a description string (release-build)
//!
#define GN_VERIFY_EX( exp, desc )   (exp)

#endif // GN_DEBUG

//!
//! assert macro
//!
#define GN_ASSERT( exp ) GN_ASSERT_EX( exp, #exp )

//!
//! verify macro
//!
#define GN_VERIFY( exp ) GN_VERIFY_EX( exp, #exp )

//!
//! Meet unexpected value
//!
#define GN_UNEXPECTED() GN_ASSERT_FAILURE( "Unexpected value or behaviour" )

//!
//! Unimplemented functionality
//!
#define GN_UNIMPL() GN_ASSERT_FAILURE( "Unimplemented!" )

//!
//! Compile-time assert
//!
#define GN_CASSERT_EX( exp, msg ) struct GN_JOIN(__GN_cassert_, GN_JOIN( msg,__LINE__)){ int data[(exp)?1:-1]; };

//!
//! Compile-time assert
//!
#define GN_CASSERT( exp ) GN_CASSERT_EX( exp, )

// ****************************************************************************
//! \name                       windows error check macros
// ****************************************************************************

//@{

#ifdef GN_WINNT

//!
//! check return value of Windows function (general version)
//!
#define GN_WIN_CHECK_DO( RETURN_TYPE, func, something )     \
    if( true ) {                                            \
        RETURN_TYPE rr = func;                              \
        if( 0 == rr )                                       \
        {                                                   \
            GN_ERROR( GN::getOSErrorInfo() );               \
            something                                       \
        }                                                   \
    } else void(0)

//!
//! check return value of Windows function
//!
#if GN_DEBUG
#define GN_WIN_CHECK( RETURN_TYPE, func ) \
            GN_WIN_CHECK_DO( RETURN_TYPE, func, void(0); )
#else
#define GN_WIN_CHECK( RETURN_TYPE, func )      func
#endif

//!
//! check return value of Windows function, return if failed
//!
#define GN_WIN_CHECK_R( RETURN_TYPE, func ) \
            GN_WIN_CHECK_DO( RETURN_TYPE, func, return; )

//!
//! check return value of Windows function, return if failed
//!
#define GN_WIN_CHECK_RV( RETURN_TYPE, func, rval ) \
            GN_WIN_CHECK_DO( RETURN_TYPE, func, return rval; )

#endif

//@}

namespace GN
{
    // ************************************************************************
    //! \name                       Debug functions
    // ************************************************************************

    //@{

    //!
    //! break into debugger ( ASCII version )
    //!
    bool
    assertFunc(
        const char * msg,
        const char * file,
        int          line,
        bool *       ignore ) throw();

#if GN_WINX64 || GN_XENON
	//!
	//! Debug break function
	//!
	void debugBreak();
#endif

#ifdef GN_WINNT
    //!
    //! get OS error info (Windows specific)
    //!
    const char * getOSErrorInfo() throw();
#endif

    //@}
}

// *****************************************************************************
//                           End of assert.h
// *****************************************************************************
#endif // __GN_BASE_ASSERT_H__
