#include "pch.h"

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

#if GN_MSWIN && GN_PC
    char buf[1024];
    strPrintf( buf, 1024,
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

#if !GN_X86
//
//
// -----------------------------------------------------------------------------
void GN::debugBreak()
{
#if GN_MSVC
	::DebugBreak();
#else
#error "Debug break unimplemented!"
#endif
}
#endif

//
//
// -----------------------------------------------------------------------------
#if GN_MSWIN
const char *
GN::getOSErrorInfo() throw()
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
        ::GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        info,
        4096,
        NULL );
    info[4095] = 0;

    // 除去信息末尾多余的回车符
    size_t n = strLen(info);
    while( n > 0 && '\n' != info[n-1] )
    {
        --n;
    }
    info[n] = 0;
#endif

    // success
    return info;
}
#endif
