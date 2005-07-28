#include "pch.h"

//
//
// -----------------------------------------------------------------------------
void
GN::detail::LogHelper::log( const char * fmt, ... )
{
    StrA s;
    va_list arglist;
    va_start( arglist, fmt );
    s.format( fmt, arglist );
    va_end( arglist );
    GN::doLog( mLevel, mCate, s.cstr(), mFunc, mFile, mLine );
}

//
//
// -----------------------------------------------------------------------------
void
GN::detail::LogHelper::logc( const char * category, const char * fmt, ... )
{
    StrA s;
    va_list arglist;
    va_start( arglist, fmt );
    s.format( fmt, arglist );
    va_end( arglist );
    GN::doLog( mLevel, category, s.cstr(), mFunc, mFile, mLine );
}

//
//
// -----------------------------------------------------------------------------
void
GN::detail::LogHelper::loglc(
    LogLevel level, const char * category, const char * fmt, ... )
{
    StrA s;
    va_list arglist;
    va_start( arglist, fmt );
    s.format( fmt, arglist );
    va_end( arglist );
    GN::doLog( level, category, s.cstr(), mFunc, mFile, mLine );
}

//
//
// -----------------------------------------------------------------------------
static GN::StrA levelStr( GN::LogLevel l )
{
    const char * s;
    if ( 0 <= l && l < GN::NUM_LOGLEVELS )
    {
        static const char * table[] = {
            "GN_FATAL",
            "GN_ERROR",
            "GN_WARN",
            "GN_INFO",
            "GN_TRACE"
        };
        s = table[l];
    }
    else
    {
        s = "UNKNOWN_LOG_LEVEL";
    }
    return GN::formatStr( "%s(%d)",s,l);
}

class ConsoleColor
{
#if GN_WIN32
    HANDLE       mConsole;
    WORD         mAttrib;
public:
    ConsoleColor( GN::LogLevel level )
    {
        // store console attributes
        mConsole = GetStdHandle(
            GN::LOGLEVEL_INFO <= level ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE );
        CONSOLE_SCREEN_BUFFER_INFO csbf;
        GetConsoleScreenBufferInfo( mConsole, &csbf );
        mAttrib = csbf.wAttributes;

        // change console color
        WORD attrib;
        switch( level )
        {
            default:
            case GN::LOGLEVEL_FATAL:
            case GN::LOGLEVEL_ERROR:
                attrib = FOREGROUND_RED;
                break;

            case GN::LOGLEVEL_WARN:
                attrib = FOREGROUND_RED | FOREGROUND_GREEN;
                break;

            case GN::LOGLEVEL_INFO:
            case GN::LOGLEVEL_TRACE:
                attrib = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                break;
        }
        SetConsoleTextAttribute( mConsole, attrib | FOREGROUND_INTENSITY );
    }

    ~ConsoleColor()
    {
        // restore console attributes
        SetConsoleTextAttribute( mConsole, mAttrib );
    }


#else
public:
    ConsoleColor( GN::LogLevel ) {}
    ~ConsoleColor()              {}
#endif
};

//
//
// -----------------------------------------------------------------------------
void GN::detail::defaultLogImpl(
    LogLevel     level,
    const char * /*category*/,
    const char * msg,
    const char * /*func*/,
    const char * file,
    int          line )
{
    using namespace ::GN;

    if( 0 == msg || 0 == msg[0] ) return;

    ConsoleColor cc(level);

    if( LOGLEVEL_INFO == level )
    {
        ::fprintf( stdout, "%s\n", msg );
    }
    else
    {
        // output to console
        ::fprintf(
            level > GN::LOGLEVEL_INFO ? stdout : stderr,
            "%s(%d) : %s : %s\n",
            file?file:"UNKNOWN_FILE",
            line,
            levelStr(level).cstr(),
            msg );

        // output to debugger
        #if GN_WIN32
        char buf[4096];
        _snprintf( buf, 4096,
            "%s(%d) : %s : %s\n",
            file?file:"UNKNOWN_FILE",
            line,
            levelStr(level).cstr(),
            msg );
        ::OutputDebugStringA( buf );
        #endif
    }
}
