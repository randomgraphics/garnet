#ifndef __GN_BASE_DEBUG_H__
#define __GN_BASE_DEBUG_H__
// *****************************************************************************
/// \file
/// \brief   debug functions and macros
/// \author  chenlee (2005.4.17)
// *****************************************************************************

///
/// Assert failture
///
#define GN_ASSERT_FAILURE( desc )                                \
    if( true ) {                                                 \
        static bool sIgnoredForever = false;                     \
        if( !sIgnoredForever ) {                                 \
            GN::internal::handleAssertFailure(                   \
                desc, __FILE__, __LINE__, &sIgnoredForever );    \
        }                                                        \
    } else void(0)

///
/// Perform runtime assert.
///
/// This macro will perform assertion in all builds, in case you want assert in
/// release build. Normally, you don't need this.
///
#define GN_DO_ASSERT( exp, desc ) if( !(exp) ) GN_ASSERT_FAILURE(desc); else void(0)


///
/// Assert macro with description
///
#if GN_ENABLE_ASSERT
#define GN_ASSERT_EX( exp, desc ) GN_DO_ASSERT( exp, desc )
#else
#define GN_ASSERT_EX( exp, desc ) void(0)
#endif

///
/// Verification macro with description
///
#define GN_VERIFY_EX( exp, desc ) GN_DO_ASSERT( exp, desc )

///
/// assert macro
///
#define GN_ASSERT( exp ) GN_ASSERT_EX( exp, #exp )

///
/// verify macro (do check even in release build.)
///
#define GN_VERIFY( exp ) GN_VERIFY_EX( exp, #exp )

///
/// Meet unexpected value
///
#define GN_UNEXPECTED_EX( message ) GN_ASSERT_FAILURE( message )

///
/// Meet unexpected value
///
#define GN_UNEXPECTED() GN_UNEXPECTED_EX( "Unexpected value or behaviour" )

///
/// Assert for unimplemented functionality
///
#define GN_UNIMPL() GN_ASSERT_FAILURE( "Unimplemented!" )

///
/// Output a warning message for unimplemented functionality
///
#define GN_UNIMPL_WARNING() GN_DO_ONCE( GN_WARN(GN::getLogger("GN.base.todo"))( "TODO: function %s is not implmented yet.", GN_FUNCTION ) );

///
/// Output a todo message.
///
#define GN_TODO(msg) GN_DO_ONCE( GN_WARN(GN::getLogger("GN.base.todo"))( "TODO: %s", msg ) );

// *****************************************************************************
/// \name error check macros
// *****************************************************************************

//@{

#define GN_OGL_CHECK_DO( func, ... )                                        \
    if( true ) {                                                            \
        func;                                                               \
        GLenum err = glGetError();                                          \
        if( GL_NO_ERROR != err )                                            \
        {                                                                   \
            static GN::Logger * sOglLogger = GN::getLogger("GN.gfx.OGLError"); \
            GN_ERROR(sOglLogger)( "error=0x%x", err );                      \
            __VA_ARGS__                                                     \
        }                                                                   \
    } else void(0)
#if GN_BUILD_DEBUG_ENABLED
#define GN_OGL_CHECK( func, ... ) GN_OGL_CHECK_DO( func, __VA_ARGS__ )
#else
#define GN_OGL_CHECK( func, ... ) func
#endif
#define GN_OGL_CHECK_R( X, ... ) GN_OGL_CHECK_DO( X, return __VA_ARGS__; )

///
/// check return value of Windows function (general version)
///
#define GN_MSW_CHECK_DO( func, something )                                      \
    if( true ) {                                                                \
        intptr_t rr = (intptr_t)(func);                                         \
        if( 0 == rr )                                                           \
        {                                                                       \
            static GN::Logger * sMswLogger = GN::getLogger("GN.base.MSWError"); \
            GN_ERROR(sMswLogger)( ::GN::getWin32LastErrorInfo() );              \
            something                                                           \
        }                                                                       \
    } else void(0)

///
/// check return value of Windows function
///
#if GN_BUILD_DEBUG_ENABLED
#define GN_MSW_CHECK( func ) GN_MSW_CHECK_DO( func, void(0); )
#else
#define GN_MSW_CHECK( func ) func
#endif

///
/// check return value of Windows function, return if failed
///
#define GN_MSW_CHECK_RETURN( func, ... ) GN_MSW_CHECK_DO( func, return __VA_ARGS__; )

///
/// DX9 error check routine
///
#ifdef D3DCOMPILE_USEVOIDS
#define GN_DX_CHECK_DO( func, something ) func
#else
#define GN_DX_CHECK_DO( func, something )                                     \
    if( true ) {                                                              \
        HRESULT hr = func;                                                    \
        if( FAILED(hr) )                                                      \
        {                                                                     \
            static GN::Logger * sDxLogger = GN::getLogger("GN.gfx.DXError");  \
            GN_ERROR(sDxLogger)( GN::getDXErrorInfo(hr) );                    \
            something                                                         \
        }                                                                     \
    } else void(0)
#endif // D3DCOMPILE_USEVOIDS

///
/// DX error check routine
///
#if GN_BUILD_DEBUG_ENABLED
#define GN_DX_CHECK( func )         GN_DX_CHECK_DO( func, )
#else
#define GN_DX_CHECK( func )         func
#endif

///
/// DX error check routine
///
#define GN_DX_CHECK_RETURN( func, ... ) GN_DX_CHECK_DO( func, return __VA_ARGS__; )

///
/// check return value of XLib function (general version)
///
#define GN_X_CHECK_DO( func, something )                    \
    if( true ) {                                            \
        Status rr = (func);                                 \
        if( 0 == rr )                                       \
        {                                                   \
            static GN::Logger * sXLogger = GN::getLogger("GN.gfx.XLibError"); \
            GN_ERROR(sXLogger)( "XLib function %s failed: return(0x%X).", #func, rr );  \
            something                                       \
        }                                                   \
    } else void(0)

///
/// check return value of XLib function
///
#if GN_BUILD_DEBUG_ENABLED
#define GN_X_CHECK( func ) GN_X_CHECK_DO( func, void(0); )
#else
#define GN_X_CHECK( func ) func
#endif

///
/// check return value of XLib function, return if failed
///
#define GN_X_CHECK_RETURN( func, ... ) GN_X_CHECK_DO( func, return __VA_ARGS__; )

//@}

namespace GN
{
    // ************************************************************************
    /// \name                       Debug functions
    // ************************************************************************

    //@{

    enum RuntimeAssertBehavior
    {
        RAB_ASK_USER,           ///< Ask user how to respond assert failure. This is default behavior
        RAB_BREAK_ALWAYS,       ///< Always break into debugger.
        RAB_LOG_ONLY,           ///< Ignore assert failure, output log message only.
        RAB_SILENCE,            ///< Silence ignore assert failure. No break, No message.
        RAB_CALL_USER_ROUTINE,  ///< Call a user specified routine when assert failed.
    };

    ///
    /// Define user routine that will be called when assert fails.
    ///
    typedef void (*AssertFailuerUserRoutine)(
        void       * userContext,
        const char * msg,
        const char * file,
        int          line,
        bool       * ignoreTheFailureForEver );

    ///
    /// Change runtime assert behavior. Default is RAB_ASK_USER.
    ///
    /// \return
    ///     Return old behavior.
    ///
    GN_API RuntimeAssertBehavior setRuntimeAssertBehavior( RuntimeAssertBehavior );

    ///
    /// Set the assert failure routine. Return the current routine.
    ///
    GN_API void setAssertFailerUserRoutine(
        AssertFailuerUserRoutine   newRoutine,
        void                     * newUserContext,
        AssertFailuerUserRoutine * oldRoutine = NULL,
        void                    ** oldUserContext = NULL );

    ///
    /// check if there's debugger attached to current process.
    ///
    GN_API bool isDebuggerAttached();

    ///
    /// Debug break function
    ///
    GN_API void breakIntoDebugger();

    ///
    /// Print to debugger
    ///
    GN_API void printToDebugger(const char *);

#if GN_MSWIN

    ///
    /// get OS error info (Windows specific)
    ///
    GN_API const char * getWin32ErrorInfo( sint32 win32ErrorCode ) throw();

    ///
    /// get OS error info (Windows specific)
    ///
    GN_API const wchar_t * getWin32ErrorInfoW( sint32 win32ErrorCode ) throw();

    ///
    /// get OS error info (Windows specific)
    ///
    GN_API const char * getWin32LastErrorInfo() throw();

#endif

    ///
    /// get DX error string
    ///
    GN_API const char * getDXErrorInfo( sint32 hr ) throw();

    ///
    /// convert errno value to string
    ///
    GN_API const char * errno2str( int );

    namespace internal
    {
        ///
        /// Handle assert failure
        ///
        GN_API void
        handleAssertFailure(
            const char * msg,
            const char * file,
            int          line,
            bool *       ignoreForever ) throw();
    };

    //@}
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_DEBUG_H__
