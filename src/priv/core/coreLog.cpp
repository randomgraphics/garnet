#include "pch.h"
#include <set>

// Note: to prevent circle referencing, this file should try to avoid
// referencing other garnet components as much as possible.

// *****************************************************************************
// local classes and utils
// *****************************************************************************

///
/// setup and restore console color
///
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
            ( level >= GN::Logger::INFO )
                ? STD_OUTPUT_HANDLE
                : STD_ERROR_HANDLE );
        CONSOLE_SCREEN_BUFFER_INFO csbf;
        GetConsoleScreenBufferInfo( mConsole, &csbf );
        mAttrib = csbf.wAttributes;

        // change console color
        WORD attrib;
        switch( level )
        {
            case GN::Logger::FATAL:
            case GN::Logger::ERROR_:
                attrib = FOREGROUND_RED;
                break;

            case GN::Logger::WARN:
                attrib = FOREGROUND_RED | FOREGROUND_GREEN;
                break;

            case GN::Logger::INFO:
            case GN::Logger::VERBOSE:
            case GN::Logger::VVERBOSE:
            default:
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

#if GN_POSIX
///
/// local mutex class on X11 platform (to avoid referencing GN::LocalMutex)
///
class LocalMutexX11
{
    pthread_mutex_t mMutex;

public:

    /// ctor
    LocalMutexX11()
    {
        // initiialize a recursive mutex (same behavior as mutex on MSWIN)
        pthread_mutexattr_t mta;
        pthread_mutexattr_init(&mta);
        pthread_mutexattr_settype( &mta, PTHREAD_MUTEX_RECURSIVE );
        pthread_mutex_init( &mMutex, &mta );
        pthread_mutexattr_destroy(&mta);
    }

    /// dtor
    ~LocalMutexX11()
    {
        pthread_mutex_destroy( &mMutex );
    }

    /// acquire the lock
    void Lock()
    {
        pthread_mutex_lock( &mMutex );
    }

    /// release the lock
    void Unlock()
    {
        pthread_mutex_unlock( &mMutex );
    }
};
typedef LocalMutexX11 LocalMutex;
#elif GN_MSWIN
typedef GN::Mutex LocalMutex;
#else
#error Unsupported platform
#endif

//
//
// -----------------------------------------------------------------------------
static inline GN::StrA sLevel2Str( int level )
{
    switch( level )
    {
        case GN::Logger::FATAL    : return "FATAL";
        case GN::Logger::ERROR_   : return "ERROR";
        case GN::Logger::WARN     : return "WARN";
        case GN::Logger::INFO     : return "INFO";
        case GN::Logger::VERBOSE  : return "VERBOSE";
        case GN::Logger::VVERBOSE : return "VERY_VERBOSE";
        default                   : return GN::StringFormat( "%d", level );
    }
}

//
//
// -----------------------------------------------------------------------------
static GN::StrA sFormatPath( const char * path )
{
    GN::StrA s;

    if( NULL == path ) return s;

    for( ; *path != 0; ++path )
    {
        char c = *path;

    #if GN_MSWIN
        s.Append( '/' == c ? '\\' : c );
    #else
        s.Append( '\\' == c ? '/' : c );
    #endif
    }

    return s;
}


template<class T>
class LoggerTreeNode
{
    T * mParent;
    T * mFirstChild;
    T * mPrevSibling;
    T * mNextSibling;

public:

    LoggerTreeNode() : mParent(0), mFirstChild(0), mPrevSibling(0), mNextSibling(0) {}

    T * Parent() const { return mParent; }
    T * FirstChild() const { return mFirstChild; }
    T * PrevSibling() const { return mPrevSibling; }
    T * NextSibling() const { return mNextSibling; }

    ///
    /// set Parent
    ///
    void SetParent( T * newParent )
    {
        if( 0 == newParent )
        {
            if( mFirstChild )
            {
                // modify Parent's first child pointer
                if( mParent && this == mParent->mFirstChild )
                {
                    GN_ASSERT( 0 == mPrevSibling );
                    mParent->mFirstChild = mFirstChild;
                }

                // set children's Parent as my Parent
                T * child = mFirstChild;
                while( child )
                {
                    child->mParent = mParent;
                    child = child->mNextSibling;
                }

                // find last child
                T * lastChild = mFirstChild;
                while( lastChild->mNextSibling ) lastChild = lastChild->mNextSibling;

                // connect children to brothers
                if( mPrevSibling )
                {
                    GN_ASSERT( this == mPrevSibling->mNextSibling );
                    GN_ASSERT( 0 == mFirstChild->mPrevSibling );
                    mPrevSibling->mNextSibling = mFirstChild;
                    mFirstChild->mPrevSibling = mPrevSibling;
                }
                if( mNextSibling )
                {
                    GN_ASSERT( this == mNextSibling->mPrevSibling );
                    GN_ASSERT( 0 == lastChild->mNextSibling );
                    mNextSibling->mPrevSibling = lastChild;
                    lastChild->mNextSibling = mNextSibling;
                }
            }
            else
            {
                // modify Parent's first child pointer
                if( mParent && this == mParent->mFirstChild )
                {
                    GN_ASSERT( 0 == mPrevSibling );
                    mParent->mFirstChild = mNextSibling;
                }

                // remove itsel from brother list
                if( mPrevSibling )
                {
                    GN_ASSERT( this == mPrevSibling->mNextSibling );
                    mPrevSibling->mNextSibling = mNextSibling;
                }
                if( mNextSibling )
                {
                    GN_ASSERT( this == mNextSibling );
                    mNextSibling->mPrevSibling = mPrevSibling;
                }
            }

            // clear my Parent
            mParent = 0;
        }
        else
        {
            // detach from old Parent
            SetParent( 0 );

            // attach to new Parent
            mParent = newParent;
            mPrevSibling = NULL;
            mNextSibling = mParent->mFirstChild;
            mParent->mFirstChild = (T*)this;
        }
    }
};

// *********************************************************************
// default logger and receiver implementation
// *********************************************************************

namespace GN
{
    ///
    /// Log to console
    ///
    struct ConsoleReceiver : public Logger::Receiver
    {
        virtual void OnLog( Logger & logger, const Logger::LogDesc & desc, const char * msg )
        {
            if( GetEnvBoolean( "GN_LOG_QUIET" ) ) return;

            if( NULL == msg ) msg = "";

            ConsoleColor cc(desc.level);
            if( desc.level >= GN::Logger::INFO )
            {
                ::fprintf( stdout, "%s\n", msg );
            }
            else
            {
                ::fprintf(
                    stderr,
                    "%s(%d)\n"
                    "\tname(%s), level(%s)\n"
                    "\t%s\n\n",
                    sFormatPath(desc.file).ToRawPtr(),
                    desc.line,
                    logger.GetName(),
                    sLevel2Str(desc.level).ToRawPtr(),
                    msg );
            }
        };
        virtual void OnLog( Logger & logger, const Logger::LogDesc & desc, const wchar_t * msg )
        {
            if( GetEnvBoolean( "GN_LOG_QUIET" ) ) return;

            if( NULL == msg ) msg = L"";

            ConsoleColor cc(desc.level);
            if( desc.level >= GN::Logger::INFO )
            {
                ::fprintf( stdout, "%S\n", msg );
            }
            else
            {
                ::fprintf(
                    stderr,
                    "%s(%d)\n"
                    "\tname(%s), level(%s)\n"
                    "\t%S\n\n",
                    sFormatPath(desc.file).ToRawPtr(),
                    desc.line,
                    logger.GetName(),
                    sLevel2Str(desc.level).ToRawPtr(),
                    msg );
            }
        };
    };

    ///
    /// Log to disk file
    ///
    struct FileReceiver : public Logger::Receiver
    {
        StrA mFileName;

        struct AutoFile
        {
            FILE * fp;
            AutoFile( const StrA & name, const char * mode = "at" ) : fp(0)
            {
                if( name.Empty() ) return;
#if GN_MSVC8
                if( 0 != ::fopen_s( &fp, name.ToRawPtr(), mode ) ) fp = 0;
#else
                fp = ::fopen( name.ToRawPtr(), mode );
#endif
            }
            ~AutoFile()
            {
                if( fp ) ::fclose( fp );
            }
        };

        FileReceiver()
#if GN_XENON
            : mFileName( "game:\\garnet3d.log.xml" )
#else
            : mFileName( GetEnv("GN_LOG_FILENAME") )
#endif
        {
            AutoFile af( mFileName, "wt" );
            if( !af.fp ) return;
            // TODO: convert log message to UTF-8
            ::fprintf( af.fp, "<?xml version=\"1.0\" encoding=\"GB18030\" standalone=\"yes\"?><srlog>\n" );
        }

        ~FileReceiver()
        {
            AutoFile af( mFileName );
            if( !af.fp ) return;
            ::fprintf( af.fp, "</srlog>\n" );
        }

        virtual void OnLog( Logger & logger, const Logger::LogDesc & desc, const char * msg )
        {
            AutoFile af( mFileName );
            if( !af.fp ) return;

            if( NULL == msg ) msg = "";

            ::fprintf( af.fp,
                "<log file=\"%s\" line=\"%d\" name=\"%s\" level=\"%s\"><![CDATA[%s]]></log>\n",
                sFormatPath(desc.file).ToRawPtr(),
                desc.line,
                logger.GetName(),
                sLevel2Str(desc.level).ToRawPtr(),
                msg );
        }
        virtual void OnLog( Logger & logger, const Logger::LogDesc & desc, const wchar_t * msg )
        {
            AutoFile af( mFileName );
            if( !af.fp ) return;

            if( NULL == msg ) msg = L"";

            ::fprintf( af.fp,
                "<log file=\"%s\" line=\"%d\" name=\"%s\" level=\"%s\"><![CDATA[%S]]></log>\n",
                sFormatPath(desc.file).ToRawPtr(),
                desc.line,
                logger.GetName(),
                sLevel2Str(desc.level).ToRawPtr(),
                msg );
        }
    };

    ///
    /// Log to debugger
    ///
    class DebugReceiver : public Logger::Receiver
    {
        virtual void OnLog( Logger & logger, const Logger::LogDesc & desc, const char * msg )
        {
#if GN_MSWIN
            char buf[16384];
            StringPrintf(
                buf,
                16384,
                "%s(%d) : name(%s), level(%s) : %s\n",
                sFormatPath(desc.file).ToRawPtr(),
                desc.line,
                logger.GetName(),
                sLevel2Str(desc.level).ToRawPtr(),
                msg );
            ::OutputDebugStringA( buf );
#endif
        }
        virtual void OnLog( Logger & logger, const Logger::LogDesc & desc, const wchar_t * msg )
        {
#if GN_MSWIN

            if( NULL == msg ) msg = L"";

            wchar_t buf[16384];
            StringPrintf(
                buf,
                16384,
                L"%S(%d) : name(%S), level(%S) : %s\n",
                sFormatPath(desc.file).ToRawPtr(),
                desc.line,
                logger.GetName(),
                sLevel2Str(desc.level).ToRawPtr(),
                msg );
            ::OutputDebugStringW( buf );
#endif
        }
    };

    ///
    /// Logger implementation class
    ///
    class LoggerImpl : public Logger, public LoggerTreeNode<LoggerImpl>
    {
    public:

        LoggerImpl( const char * name, LocalMutex & mutex )
            : Logger(name?name:"")
            , mGlobalMutex( mutex )
            , mInheritLevel(true)
            , mInheritEnabled(true) {}

        void ReapplyAttributes()
        {
            RecursiveUpdateLevel( GetLevel() );
            RecursiveUpdateEnabled( isEnabled() );
        }

    public:

        virtual void SetLevel( int level )
        {
            ScopeMutex<LocalMutex> m(mGlobalMutex);
            RecursiveUpdateLevel( level );
            mInheritLevel = false;
        }

        virtual void SetEnabled( bool enabled )
        {
            ScopeMutex<LocalMutex> m(mGlobalMutex);
            RecursiveUpdateEnabled( enabled );
            mInheritEnabled = false;
        }

        virtual void DoLog( const LogDesc & desc, const char * msg )
        {
            ScopeMutex<LocalMutex> m(mGlobalMutex);
            RecursiveLog( *this, desc, msg );
        }

        virtual void DoLog( const LogDesc & desc, const wchar_t * msg )
        {
            ScopeMutex<LocalMutex> m(mGlobalMutex);
            RecursiveLog( *this, desc, msg );
        }

        virtual void AddReceiver( Receiver * r )
        {
            ScopeMutex<LocalMutex> m(mGlobalMutex);
            if( 0 == r ) return;
            mReceivers.insert( r );
        }

        virtual void RemoveReceiver( Receiver * r )
        {
            ScopeMutex<LocalMutex> m(mGlobalMutex);
            if( 0 == r ) return;
            mReceivers.erase( r );
        }

        virtual void RemoveAllReceivers()
        {
            ScopeMutex<LocalMutex> m(mGlobalMutex);
            mReceivers.clear();
        }

    private:

        LocalMutex & mGlobalMutex;

        std::set<Receiver*> mReceivers;
        bool mInheritLevel;
        bool mInheritEnabled;

        template<typename CHAR>
        void RecursiveLog( Logger & logger, const LogDesc & desc, const CHAR * msg )
        {
            // call Parent's logging
            LoggerImpl * p = Parent();
            if( p ) p->RecursiveLog( logger, desc, msg );

            // send msg to receivers
            for( std::set<Receiver*>::const_iterator i = mReceivers.begin(); i != mReceivers.end(); ++i )
            {
                (*i)->OnLog( logger, desc, msg );
            }
        }

        void RecursiveUpdateLevel( int level )
        {
            mLevel = level;
            LoggerImpl * child = FirstChild();
            while( child )
            {
                if( child->mInheritLevel ) child->RecursiveUpdateLevel( level );
                child = child->NextSibling();
            }
        }

        void RecursiveUpdateEnabled( bool enabled )
        {
            mEnabled = enabled;
            LoggerImpl * child = FirstChild();
            while( child )
            {
                if( child->mInheritEnabled ) child->RecursiveUpdateEnabled( enabled );
                child = child->NextSibling();
            }
        }
    };

    ///
    /// Log container
    ///
    class LoggerContainer
    {
        // Note: Logger map is case "insensitive"
        typedef GN::StringMap<char,LoggerImpl*,GN::StringCompareCase::INSENSITIVE> LoggerMap;

        ConsoleReceiver mCr;
        FileReceiver    mFr;
        DebugReceiver   mDr;
        LoggerImpl      mRootLogger;
        LocalMutex      mMutex;
        LoggerMap       mLoggers;

        LoggerImpl * FindParent( const StrA & name )
        {
            // get parent name
            size_t n = name.FindLastOf( "." );
            if( StrA::NOT_FOUND == n ) return &mRootLogger; // shortcut for root logger
            GN_ASSERT( n > 0 );
            StrA parent = name.SubString( 0, n );

            return GetLogger( parent.ToRawPtr() );
        }

        void PrintLoggerTree( StrA & str, int level, LoggerImpl & logger )
        {
            // print itself
            for( int i = 0; i < level; ++i ) str.Append( "  " );
            str.Append( StringFormat( "%s\n", logger.GetName() ) );

            // print children
            LoggerImpl * c = logger.FirstChild();
            while( c )
            {
                PrintLoggerTree( str, level + 1, *c );
                c = c->NextSibling();
            }
        }

    public:

        LoggerContainer() : mRootLogger("ROOT",mMutex)
        {
            // config root logger
            mRootLogger.SetLevel( Logger::INFO );
            mRootLogger.SetEnabled( true );
#if !GN_XENON
            mRootLogger.AddReceiver( &mCr );
#endif
            mRootLogger.AddReceiver( &mFr );
            mRootLogger.AddReceiver( &mDr );
        }

        ~LoggerContainer()
        {
            static Logger * sLogger = GetLogger("GN.core.LoggerContainer");
            StrA loggerTree;
            PrintLoggerTree( loggerTree, 0, mRootLogger );
            GN_VERBOSE(sLogger)( "\n%s", loggerTree.ToRawPtr() );
            for( LoggerMap::KeyValuePair * p = mLoggers.First(); NULL != p; p = mLoggers.Next( p ) )
            {
                delete p->value;
            }
        }

        LoggerImpl * GetLogger( const char * name )
        {
            ScopeMutex<LocalMutex> m( mMutex );

            // trip leading and trailing dots
            StrA n(name);
            n.Trim( '.' );

            // shortcut for root logger
            if( n.Empty() || 0 == StringCompareI( "ROOT", n.ToRawPtr() ) ) return &mRootLogger;

            // find for existing logger
            LoggerImpl ** pplogger = mLoggers.Find( n );
            if( NULL != pplogger ) { GN_ASSERT( *pplogger ); return *pplogger; }

            // not found. create new one.
            AutoObjPtr<LoggerImpl> newLogger( new LoggerImpl(n,mMutex) );
            mLoggers[n] = newLogger;

            // update logger tree
            LoggerImpl * parent = FindParent( n );
            GN_ASSERT( parent );
            newLogger->SetParent( parent );
            parent->ReapplyAttributes();

            // sucess
            return newLogger.Detach();
        }
    };

    LoggerContainer * msInstancePtr = 0;

    //
    // Implement global log function.
    // -------------------------------------------------------------------------
    static LoggerContainer & sGetLoggerContainer()
    {
        // WARNING: this function is not thread-safe!
        static LoggerContainer lc;
        return lc;
    }

    //
    // Implement global log function.
    // -------------------------------------------------------------------------
    GN_PUBLIC Logger * GetLogger( const char * name )
    {
        LoggerContainer & lc = sGetLoggerContainer();
        return lc.GetLogger( name );
    }
}
