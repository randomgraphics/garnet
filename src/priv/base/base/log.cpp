#include "pch.h"

// *****************************************************************************
// local functions
// *****************************************************************************

class ConsoleColor
{
#if GN_MSWIN & GN_PC
    HANDLE       mConsole;
    WORD         mAttrib;
public:
    ConsoleColor( int level )
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
            case GN::LOGLEVEL_FATAL:
            case GN::LOGLEVEL_ERROR:
                attrib = FOREGROUND_RED;
                break;

            case GN::LOGLEVEL_WARN:
                attrib = FOREGROUND_RED | FOREGROUND_GREEN;
                break;

            case GN::LOGLEVEL_INFO:

            default: // GN_TRACE_###
                attrib = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                break;
        }
        SetConsoleTextAttribute( mConsole, attrib );
    }

    ~ConsoleColor()
    {
        // restore console attributes
        SetConsoleTextAttribute( mConsole, mAttrib );
    }


#else
public:
    ConsoleColor( int ) {}
    ~ConsoleColor()     {}
#endif
};

//
//
// -----------------------------------------------------------------------------
static inline GN::StrA sLevel2Str( int l )
{
    if( 0 <= l && l < GN::LOGLEVEL_TRACE_0 )
    {
        static const char * table[] = {
            "GN_FATAL",
            "GN_ERROR",
            "GN_WARN",
            "GN_INFO",
        };
        return table[l];
    }
    else 
    {
        return GN::strFormat( "GN_TRACE_%d", l - GN::LOGLEVEL_TRACE_0 );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sDoPrint( FILE * fp, const char * file, int line, int level, const char * msg )
{
    if( GN::LOGLEVEL_INFO > level )
    {
        // warning and error
        ::fprintf(
            fp,
            "%s(%d) : %s : %s\n",
            file, line,
            sLevel2Str(level).cptr(),
            msg );
    }
    else if( GN::LOGLEVEL_INFO == level )
    {
        ::fprintf( fp, "%s\n", msg );
    }
    else
    {
        // trace messages
        GN_ASSERT( level >= GN::LOGLEVEL_TRACE_0 );
        ::fprintf( fp, "TRACE(%d) : %s\n", level - GN::LOGLEVEL_TRACE_0 , msg );
    }
}

// *****************************************************************************
// public functions
// *****************************************************************************

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
    GN::doLog( mDesc, s.cptr() );
}

//
//
// -----------------------------------------------------------------------------
void
GN::detail::LogHelper::logc( const char * cate, const char * fmt, ... )
{
    StrA s;
    va_list arglist;
    va_start( arglist, fmt );
    s.format( fmt, arglist );
    va_end( arglist );
    mDesc.cate = cate ? cate : "";
    GN::doLog( mDesc, s.cptr() );
}

//
//
// -----------------------------------------------------------------------------
void
GN::detail::LogHelper::loglc(
    int level, const char * cate, const char * fmt, ... )
{
    StrA s;
    va_list arglist;
    va_start( arglist, fmt );
    s.format( fmt, arglist );
    va_end( arglist );
    mDesc.level = level;
    mDesc.cate = cate ? cate : "";
    GN::doLog( mDesc, s.cptr() );
}

//
//
// -----------------------------------------------------------------------------
void GN::detail::defaultLogImpl( const LogDesc & desc, const char * msg )
{
    if( 0 == msg || 0 == msg[0] ) return;

    bool logDisabled = getEnvBoolean( "GN_LOG_DISABLED" );
    if( logDisabled ) return;

    StrA logFileName = getEnv( "GN_LOG_FILENAME" );

    const char * file = desc.file ? desc.file : "UNKNOWN FILE";

    ConsoleColor cc(desc.level);

    if( !logFileName.empty() )
    {
#if GN_MSVC8
        FILE * fp;
        if( 0 == fopen_s( &fp, logFileName.cptr(), "at" ) )
#else
        FILE * fp = fopen( logFileName.cptr(), "at" );
        if( fp )
#endif
        {
            sDoPrint( fp, file, desc.line, desc.level, msg );
            fclose( fp );
        }
    }

#if !GN_XENON // Xenon has no console output
    bool logToScreen = !getEnvBoolean( "GN_LOG_QUIET" );
    if( logToScreen )
    {
        // output to console
        if( desc.level < LOGLEVEL_INFO )
            sDoPrint( stderr, file, desc.line, desc.level, msg );
        else
            sDoPrint( stdout, file, desc.line, desc.level, msg );
    }
#endif

    // output to debugger
#if GN_MSWIN
    char buf[16384];
    strPrintf(
        buf,
        16384,
        "%s(%d) : %s : %s\n",
        file, desc.line,
        sLevel2Str(desc.level).cptr(),
        msg );
    ::OutputDebugStringA( buf );
#endif
}
