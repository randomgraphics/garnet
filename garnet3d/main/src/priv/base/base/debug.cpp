#include "pch.h"

#if GN_BUILD_GPU_API_HAS_XENON

#include "dxerr9.h"
#if GN_MSVC
#pragma comment( lib, "dxerr9.lib" )
#endif
#define DXERR_FUNC DXGetErrorDescription9A

#elif GN_BUILD_GPU_API_HAS_D3D10 || GN_BUILD_GPU_API_HAS_D3D11

#include "dxerr.h"
#if GN_MSVC
#pragma comment( lib, "dxerr.lib" )
#endif
#define DXERR_FUNC DXGetErrorDescriptionA

#else

static const char * DXERR_FUNC( sint32 ) { return "unknown error code."; }

#endif


namespace GN
{
    // Global runtime assert behavior flag. Implemented in core module.
    extern GN_PUBLIC RuntimeAssertBehavior gRuntimeAssertBehavior;
}
//
//
// -----------------------------------------------------------------------------
GN::RuntimeAssertBehavior GN::setRuntimeAssertBehavior( RuntimeAssertBehavior rab )
{
    RuntimeAssertBehavior old = gRuntimeAssertBehavior;
    gRuntimeAssertBehavior = rab;
    return old;
}

//
//
// -----------------------------------------------------------------------------
bool
GN::assertFunc(
    const char * msg,
    const char * file,
    int          line,
    bool *       ignoreFromNowOn ) throw()
{
    if( RAB_SILENCE == gRuntimeAssertBehavior ) return false;

    ::fprintf(
        stderr,
        "\n"
        "======================== ASSERT FAILURE ========================\n"
        "%s(%d)\n"
        "----------------------------------------------------------------\n"
        "%s\n"
        "================================================================\n",
        file?file:"",
        line,
        msg?msg:"" );

    if( RAB_LOG_ONLY == gRuntimeAssertBehavior ) return false;

    if( RAB_BREAK_ALWAYS == gRuntimeAssertBehavior ) return true;

#if GN_MSWIN
    char buf[1024];
    stringPrintf( buf, 1024,
        "%s(%d)\n"
        "%s\n\n"
        "Break into debugger?\n"
        "(If canceled, this specific assert failure will not be triggered again)",
        file?file:"", line, msg?msg:"" );
    int ret = ::MessageBoxA(
        0,
        buf,
        "Assert Failure",
        MB_YESNOCANCEL|MB_ICONQUESTION
        );

    if(ignoreFromNowOn) *ignoreFromNowOn = ( IDCANCEL == ret );
    return IDYES == ret;
#else
    if( *ignoreFromNowOn ) *ignoreFromNowOn = false;
    return true;
#endif
}

//
//
// -----------------------------------------------------------------------------
void GN::breakIntoDebugger()
{
#if GN_MSVC
	::DebugBreak();
#elif GN_GCC
    asm("int $3");
#else
#error "Unsupport compiler!"
#endif
}

#if GN_MSWIN || GN_XENON

//
//
// -----------------------------------------------------------------------------
const char *
GN::getWin32ErrorInfo( uint32 win32ErrorCode ) throw()
{
    static char info[4096];

#if GN_XENON
    // TODO: unimplemented
    info[0] = 0;
#else
    ::FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        win32ErrorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        info,
        4096,
        NULL );
    info[4095] = 0;

    // 除去信息末尾多余的回车符
    size_t n = stringLength(info);
    while( n > 0 && '\n' != info[n-1] )
    {
        --n;
    }
    info[n] = 0;
#endif

    // success
    return info;
}

//
//
// -----------------------------------------------------------------------------
const char *
GN::getWin32LastErrorInfo() throw()
{
    return getWin32ErrorInfo( ::GetLastError() );
}

#endif


//
//
// -----------------------------------------------------------------------------
const char *
GN::getDXErrorInfo( sint32 hr ) throw()
{
    return DXERR_FUNC( hr );
}

//
//
// -----------------------------------------------------------------------------
const char *
GN::errno2str( int err )
{
    switch( err )
    {
        case EPERM        : return "Operation not permitted";
        case ENOENT       : return "No such file or directory";
        case ESRCH        : return "No such process";
        case EINTR        : return "Interrupted function";
        case EIO          : return "I/O error";
        case ENXIO        : return "No such device or address";
        case E2BIG        : return "Argument list too long";
        case ENOEXEC      : return "Exec format error";
        case EBADF        : return "Bad file number";
        case ECHILD       : return "No spawned processes";
        case EAGAIN       : return "No more processes or not enough memory or maximum nesting level reached";
        case ENOMEM       : return "Not enough memory";
        case EACCES       : return "Permission denied";
        case EFAULT       : return "Bad address";
        case EBUSY        : return "Device or resource busy";
        case EEXIST       : return "File exists";
        case EXDEV        : return "Cross-device link";
        case ENODEV       : return "No such device";
        case ENOTDIR      : return "Not a directory";
        case EISDIR       : return "Is a directory";
        case EINVAL       : return "Invalid argument";
        case ENFILE       : return "Too many files open in system";
        case EMFILE       : return "Too many open files";
        case ENOTTY       : return "Inappropriate I/O control operation";
        case EFBIG        : return "File too large";
        case ENOSPC       : return "No space left on device";
        case ESPIPE       : return "Invalid seek";
        case EROFS        : return "Read-only file system";
        case EMLINK       : return "Too many links";
        case EPIPE        : return "Broken pipe";
        case EDOM         : return "Math argument";
        case ERANGE       : return "Result too large";
        case EDEADLK      : return "Resource deadlock would occur";
        case ENAMETOOLONG : return "Filename too long";
        case ENOLCK       : return "No locks available";
        case ENOSYS       : return "Function not supported";
        case ENOTEMPTY    : return "Directory not empty";
        case EILSEQ       : return "Illegal byte sequence";
        default           : return "Unknown error.";
    }
}
