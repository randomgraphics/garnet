#ifndef __GN_BASE_EXCEPTION_H__
#define __GN_BASE_EXCEPTION_H__
// *****************************************************************************
/// \file
/// \brief   exception class and handleing
/// \author  chenlee (2005.4.17)
// *****************************************************************************

#include <exception>

// *****************************************************************************
/// \name                  exception macro/class
// *****************************************************************************

//@{

#ifndef GN_TRY
    #define GN_TRY try
#endif

#ifndef GN_CATCH
    #define GN_CATCH(X) catch (X)
#endif

///
/// <b>ALWAYS</b> use this macro when throwing a exception
///
#define GN_THROW(msg, ...) throwException(GN_FUNCTION, __FILE__, __LINE__, fmt::format(msg, __VA_ARGS__))

///
/// Throw a custom exception class
///
#define GN_THROW_EX(exp) throw exp

namespace GN {
///
/// custom exception class
///
struct Exception {
    std::string  msg;
    const char * func; ///< location of the exception
    const char * file; ///< location of the exception
    int          line; ///< location of the exception

    ///
    /// constructor
    ///
    Exception(const char * ifunc, const char * ifile, int iline, std::string && imsg) throw(): msg(std::move(imsg)), func(ifunc), file(ifile), line(iline) {}

    ///
    /// copy constructor
    ///
    Exception(const GN::Exception & e) throw(): msg(e.msg), func(e.func), file(e.file), line(e.line) {}

    ///
    /// assignment
    ///
    Exception & operator=(const GN::Exception & e) throw() {
        msg  = e.msg;
        func = e.func;
        file = e.file;
        line = e.line;
        return *this;
    }
};

///
/// Dump current call stack to string
///
GN_API std::string backtrace(bool includeSourceSnippet = GN_BUILD_DEBUG_ENABLED);

///
/// Throw exception
///
GN_API void throwException(const char * ifunc, const char * ifile, int iline, std::string && imsg);

///
/// exception handler
///
GN_API void exceptionHandler(const char * msg, const char * func, const char * file, int line);

///
/// exception handler
///
inline void exceptionHandler(const Exception & e) { exceptionHandler(e.msg.data(), e.func, e.file, e.line); }

} // namespace GN

//@}

// *****************************************************************************
//
/// \name                      function guard macros
//
//
// Usage:
//
//  void foo()
//  {
//    GN_GUARD;
//
//    ...
//
//  GN_UNGUARD;
//  }
//
//  - Use xxxx_ALWAYS_xxxx, when u want to do guarding all the time.
//
//  - Use GN_GUARD/UNGARD in normal functions, and these macros are mean to
//    be enabled even in release build.
//
//  - Use GN_GUARD_SLOW/GN_UNGUARD_SLOW in time-critical-functions. These
//    macros are only enabled when GN_ENABLE_SLOW_GUARD is defined (normally
//    not, in release build).
//
//  - Use GN_UNGUARD_ALWAYS_NOTHROW, when u do NOT want to re-throw
//    the exception.
//
//  - Use GN_UNGUARD_ALWAYS_DO, when u want to perform any special
//    house-keeping job when exception occured.
//
// *****************************************************************************

//@{

///
/// unguard macro
///
#define GN_UNGUARD_ALWAYS_DO(something)                                              \
    }                                                                                \
    GN_CATCH(const GN::Exception & e) {                                              \
        GN::exceptionHandler(e);                                                     \
        something                                                                    \
    }                                                                                \
    GN_CATCH(const std::exception & e) {                                             \
        GN::exceptionHandler(e.what(), GN_FUNCTION, __FILE__, __LINE__);             \
        something                                                                    \
    }                                                                                \
    GN_CATCH(const char * e) {                                                       \
        GN::exceptionHandler(e, GN_FUNCTION, __FILE__, __LINE__);                    \
        something                                                                    \
    }                                                                                \
    GN_CATCH(...) {                                                                  \
        GN::exceptionHandler("unknown exception!", GN_FUNCTION, __FILE__, __LINE__); \
        something                                                                    \
    }

#define GN_GUARD_ALWAYS            GN_TRY {
#define GN_UNGUARD_ALWAYS          GN_UNGUARD_ALWAYS_DO(throw;)
#define GN_UNGUARD_ALWAYS_NO_THROW GN_UNGUARD_ALWAYS_DO(;)

#if GN_ENABLE_GUARD
    #define GN_GUARD   GN_GUARD_ALWAYS
    #define GN_UNGUARD GN_UNGUARD_ALWAYS
#else
    #define GN_GUARD   {
    #define GN_UNGUARD }
#endif

//
// guard macros for time-critical functions
//
#if GN_ENABLE_GUARD && GN_ENABLE_SLOW_GUARD
    #define GN_GUARD_SLOW   GN_GUARD_ALWAYS
    #define GN_UNGUARD_SLOW GN_UNGUARD_ALWAYS
#else
    #define GN_GUARD_SLOW   {
    #define GN_UNGUARD_SLOW }
#endif

//@}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_EXCEPTION_H__
