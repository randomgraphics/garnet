#include "pch.h"
#include "garnet/GnBase.h"

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
#if GN_WIN32
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

    if(*ignore) *ignore = ( IDCANCEL == ret );
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
        "================================================================\n",
        file?file:"",
        line,
        msg?msg:"" );
    if( *ignore ) *ignore = false;
    int  ch = 0;
    for(;;)
    {
        ch = ::getc(stdin);
        if( 'b' == ch || 'B' == ch )
        {
            return true;
        }
        if( 'c' == ch || 'C' == ch )
        {
            return false;
        }
        if( 'i' == ch || 'I' == ch )
        {
            if( *ignore ) *ignore = true;
            return false;
        }
    }
#endif
}

//
//
// -----------------------------------------------------------------------------
#if GN_WIN32
const char *
GN::getOsErrorInfo() throw()
{
    static char info[4096];

    ::FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        ::GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        info,
        4096,
        NULL );

    // 除去信息末尾多余的回车符
    size_t n = strLen(info);
    while( n > 0 && '\n' != info[n-1] )
    {
        --n;
    }
    info[n] = 0;

    // success
    return info;
}
#endif
