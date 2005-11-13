#include "pch.h"

//
//
// -----------------------------------------------------------------------------
bool
GN::assertFunc(
    const char * msg,
    const char * file,
    int          line,
    bool *       ignore ) throw()
{
#if GN_MSWIN && GN_PC
    char buf[1024];
    ::_snprintf( buf, 1023,
        "%s(%d)\n"
        "%s\n\n"
        "转入调试器？\n"
        "（如果选择Cancel，则以后这个Assert失败时将不再弹出对话框）",
        file?file:"", line, msg?msg:"" );
    buf[1023] = 0;
    int ret = ::MessageBoxA(
        0,
        buf,
        "Assert失败",
        MB_YESNOCANCEL|MB_ICONQUESTION
        );

    if(ignore) *ignore = ( IDCANCEL == ret );
    return IDYES == ret;
#else
    ::fprintf(
        stderr,
        "\n"
        "======================== ASSERT FAILURE ========================\n"
        "%s(%d)\n"
        "----------------------------------------------------------------\n"
        "%s\n"
        "----------------------------------------------------------------\n"
        "              Break(B), Continue(C), Ignore(I)\n"
        "      (Choose Ignore to suppress further assert warnings)\n"
        "================================================================\n",
        file?file:"",
        line,
        msg?msg:"" );
    if( *ignore ) *ignore = false;
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
