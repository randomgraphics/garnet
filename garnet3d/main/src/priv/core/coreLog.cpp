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
    void lock()
    {
        pthread_mutex_lock( &mMutex );
    }

    /// release the lock
    void unlock()
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
        default                   : return GN::strFormat( "%d", level );
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
        s.append( '/' == c ? '\\' : c );
    #else
        s.append( '\\' == c ? '/' : c );
    #endif
    }

    return s;
}


template<class T>
class LoggerTreeNode
{
    T * mParent;
    T * mFirstChild;
    T * mPrevBrother;
    T * mNextBrother;

public:

    LoggerTreeNode() : mParent(0), mFirstChild(0), mPrevBrother(0), mNextBrother(0) {}

    T * parent() const { return mParent; }
    T * firstChild() const { return mFirstChild; }
    T * prevBrother() const { return mPrevBrother; }
    T * nextBrother() const { return mNextBrother; }

    ///
    /// set parent
    ///
    void setParent( T * newParent )
    {
        if( 0 == newParent )
        {
            if( mFirstChild )
            {
                // modify parent's first child pointer
                if( mParent && this == mParent->mFirstChild )
                {
                    GN_ASSERT( 0 == mPrevBrother );
                    mParent->mFirstChild = mFirstChild;
                }

                // set children's parent as my parent
                T * child = mFirstChild;
                while( child )
                {
                    child->mParent = mParent;
                    child = child->mNextBrother;
                }

                // find last child
                T * lastChild = mFirstChild;
                while( lastChild->mNextBrother ) lastChild = lastChild->mNextBrother;

                // connect children to brothers
                if( mPrevBrother )
                {
                    GN_ASSERT( this == mPrevBrother->mNextBrother );
                    GN_ASSERT( 0 == mFirstChild->mPrevBrother );
                    mPrevBrother->mNextBrother = mFirstChild;
                    mFirstChild->mPrevBrother = mPrevBrother;
                }
                if( mNextBrother )
                {
                    GN_ASSERT( this == mNextBrother->mPrevBrother );
                    GN_ASSERT( 0 == lastChild->mNextBrother );
                    mNextBrother->mPrevBrother = lastChild;
                    lastChild->mNextBrother = mNextBrother;
                }
            }
            else
            {
                // modify parent's first child pointer
                if( mParent && this == mParent->mFirstChild )
                {
                    GN_ASSERT( 0 == mPrevBrother );
                    mParent->mFirstChild = mNextBrother;
                }

                // remove itsel from brother list
                if( mPrevBrother )
                {
                    GN_ASSERT( this == mPrevBrother->mNextBrother );
                    mPrevBrother->mNextBrother = mNextBrother;
                }
                if( mNextBrother )
                {
                    GN_ASSERT( this == mNextBrother );
                    mNextBrother->mPrevBrother = mPrevBrother;
                }
            }

            // clear my parent
            mParent = 0;
        }
        else
        {
            // detach from old parent
            setParent( 0 );

            // attach to new parent
            mParent = newParent;
            mPrevBrother = NULL;
            mNextBrother = mParent->mFirstChild;
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
        virtual void onLog( Logger & logger, const Logger::LogDesc & desc, const StrA & msg )
        {
            if( getEnvBoolean( "GN_LOG_QUIET" ) ) return;
            ConsoleColor cc(desc.level);
            if( desc.level >= GN::Logger::INFO )
            {
                ::fprintf( stdout, "%s\n", msg.cptr() );
            }
            else
            {
                ::fprintf(
                    stderr,
                    "%s(%d)\n"
                    "\tname(%s), level(%s)\n"
                    "\t%s\n\n",
                    sFormatPath(desc.file).cptr(),
                    desc.line,
                    logger.getName().cptr(),
                    sLevel2Str(desc.level).cptr(),
                    msg.cptr() );
            }
        };
        virtual void onLog( Logger & logger, const Logger::LogDesc & desc, const StrW & msg )
        {
            if( getEnvBoolean( "GN_LOG_QUIET" ) ) return;
            ConsoleColor cc(desc.level);
            if( desc.level >= GN::Logger::INFO )
            {
                ::fprintf( stdout, "%S\n", msg.cptr() );
            }
            else
            {
                ::fprintf(
                    stderr,
                    "%s(%d)\n"
                    "\tname(%s), level(%s)\n"
                    "\t%S\n\n",
                    sFormatPath(desc.file).cptr(),
                    desc.line,
                    logger.getName().cptr(),
                    sLevel2Str(desc.level).cptr(),
                    msg.cptr() );
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
                if( name.empty() ) return;
#if GN_MSVC8
                if( 0 != ::fopen_s( &fp, name.cptr(), mode ) ) fp = 0;
#else
                fp = ::fopen( name.cptr(), mode );
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
            : mFileName( getEnv("GN_LOG_FILENAME") )
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

        virtual void onLog( Logger & logger, const Logger::LogDesc & desc, const StrA & msg )
        {
            AutoFile af( mFileName );
            if( !af.fp ) return;

            ::fprintf( af.fp,
                "<log file=\"%s\" line=\"%d\" name=\"%s\" level=\"%s\"><![CDATA[%s]]></log>\n",
                sFormatPath(desc.file).cptr(),
                desc.line,
                logger.getName().cptr(),
                sLevel2Str(desc.level).cptr(),
                msg.cptr() );
        }
        virtual void onLog( Logger & logger, const Logger::LogDesc & desc, const StrW & msg )
        {
            AutoFile af( mFileName );
            if( !af.fp ) return;

            ::fprintf( af.fp,
                "<log file=\"%s\" line=\"%d\" name=\"%s\" level=\"%s\"><![CDATA[%S]]></log>\n",
                sFormatPath(desc.file).cptr(),
                desc.line,
                logger.getName().cptr(),
                sLevel2Str(desc.level).cptr(),
                msg.cptr() );
        }
    };

    ///
    /// Log to debugger
    ///
    class DebugReceiver : public Logger::Receiver
    {
        virtual void onLog( Logger & logger, const Logger::LogDesc & desc, const StrA & msg )
        {
#if GN_MSWIN
            char buf[16384];
            strPrintf(
                buf,
                16384,
                "%s(%d) : name(%s), level(%s) : %s\n",
                sFormatPath(desc.file).cptr(),
                desc.line,
                logger.getName().cptr(),
                sLevel2Str(desc.level).cptr(),
                msg.cptr() );
            ::OutputDebugStringA( buf );
#endif
        }
        virtual void onLog( Logger & logger, const Logger::LogDesc & desc, const StrW & msg )
        {
#if GN_MSWIN
            wchar_t buf[16384];
            strPrintf(
                buf,
                16384,
                L"%S(%d) : name(%S), level(%S) : %s\n",
                sFormatPath(desc.file).cptr(),
                desc.line,
                logger.getName().cptr(),
                sLevel2Str(desc.level).cptr(),
                msg.cptr() );
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

        LoggerImpl( const StrA & name, LocalMutex & mutex )
            : Logger(name)
            , mGlobalMutex( mutex )
            , mInheritLevel(true)
            , mInheritEnabled(true) {}

        void reapplyAttributes()
        {
            recursiveUpdateLevel( getLevel() );
            recursiveUpdateEnabled( isEnabled() );
        }

    public:

        virtual void setLevel( int level )
        {
            ScopeMutex<LocalMutex> m(mGlobalMutex);
            recursiveUpdateLevel( level );
            mInheritLevel = false;
        }

        virtual void setEnabled( bool enabled )
        {
            ScopeMutex<LocalMutex> m(mGlobalMutex);
            recursiveUpdateEnabled( enabled );
            mInheritEnabled = false;
        }

        virtual void doLog( const LogDesc & desc, const StrA & msg )
        {
            ScopeMutex<LocalMutex> m(mGlobalMutex);
            recursiveLog( *this, desc, msg );
        }

        virtual void doLog( const LogDesc & desc, const StrW & msg )
        {
            ScopeMutex<LocalMutex> m(mGlobalMutex);
            recursiveLog( *this, desc, msg );
        }

        virtual void addReceiver( Receiver * r )
        {
            ScopeMutex<LocalMutex> m(mGlobalMutex);
            if( 0 == r ) return;
            mReceivers.insert( r );
        }

        virtual void removeReceiver( Receiver * r )
        {
            ScopeMutex<LocalMutex> m(mGlobalMutex);
            if( 0 == r ) return;
            mReceivers.erase( r );
        }

        virtual void removeAllReceivers()
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
        void recursiveLog( Logger & logger, const LogDesc & desc, const Str<CHAR> & msg )
        {
            // call parent's logging
            LoggerImpl * p = parent();
            if( p ) p->recursiveLog( logger, desc, msg );

            // send msg to receivers
            for( std::set<Receiver*>::const_iterator i = mReceivers.begin(); i != mReceivers.end(); ++i )
            {
                (*i)->onLog( logger, desc, msg );
            }
        }

        void recursiveUpdateLevel( int level )
        {
            mLevel = level;
            LoggerImpl * child = firstChild();
            while( child )
            {
                if( child->mInheritLevel ) child->recursiveUpdateLevel( level );
                child = child->nextBrother();
            }
        }

        void recursiveUpdateEnabled( bool enabled )
        {
            mEnabled = enabled;
            LoggerImpl * child = firstChild();
            while( child )
            {
                if( child->mInheritEnabled ) child->recursiveUpdateEnabled( enabled );
                child = child->nextBrother();
            }
        }
    };

    ///
    /// Log container
    ///
    class LoggerContainer
    {
        ConsoleReceiver mCr;
        FileReceiver    mFr;
        DebugReceiver   mDr;
        LoggerImpl      mRootLogger;
        LocalMutex      mMutex;

        std::map<StrA,LoggerImpl*> mLoggers;

        static void sDeleteLogger( std::map<StrA,LoggerImpl*>::value_type & i ) { delete i.second; }

        LoggerImpl * findParent( const StrA & name )
        {
            // get parent name
            size_t n = name.findLastOf( "." );
            if( StrA::NOT_FOUND == n ) return &mRootLogger; // shortcut for root logger
            GN_ASSERT( n > 0 );
            StrA parent = name.subString( 0, n );

            return getLogger( parent.cptr() );
        }

        void printLoggerTree( StrA & str, int level, LoggerImpl & logger )
        {
            // print itself
            for( int i = 0; i < level; ++i ) str.append( "  " );
            str.append( strFormat( "%s\n", logger.getName().cptr() ) );

            // print children
            LoggerImpl * c = logger.firstChild();
            while( c )
            {
                printLoggerTree( str, level + 1, *c );
                c = c->nextBrother();
            }
        }

		struct CaseInsensitiveMatch
		{
			const StrA & mName;

			CaseInsensitiveMatch( const StrA & name ) : mName(name)
			{
			}

			inline bool operator()( const std::map<StrA,LoggerImpl*>::value_type & i )
			{
				return 0 == strCmpI( mName.cptr(), i.first.cptr() );
			}
		};

    public:

        LoggerContainer() : mRootLogger("ROOT",mMutex)
        {
            // config root logger
            mRootLogger.setLevel( Logger::INFO );
            mRootLogger.setEnabled( true );
#if !GN_XENON
            mRootLogger.addReceiver( &mCr );
#endif
            mRootLogger.addReceiver( &mFr );
            mRootLogger.addReceiver( &mDr );
        }

        ~LoggerContainer()
        {
            static Logger * sLogger = getLogger("GN.core.LoggerContainer");
            StrA loggerTree;
            printLoggerTree( loggerTree, 0, mRootLogger );
            GN_VERBOSE(sLogger)( "\n%s", loggerTree.cptr() );
            std::for_each( mLoggers.begin(), mLoggers.end(), &sDeleteLogger );
        }

        LoggerImpl * getLogger( const char * name )
        {
            ScopeMutex<LocalMutex> m( mMutex );

            // trip leading and trailing dots
            StrA n(name);
            n.trim( '.' );

            // shortcut for root logger
            if( n.empty() || 0 == strCmpI( "ROOT", n.cptr() ) ) return &mRootLogger;

            // find for existing logger
            std::map<StrA,LoggerImpl*>::const_iterator i = std::find_if(
				mLoggers.begin(),
				mLoggers.end(),
				CaseInsensitiveMatch(n) );
            if( mLoggers.end() != i ) { GN_ASSERT( i->second ); return i->second; }

            // not found. create new one.
            AutoObjPtr<LoggerImpl> newLogger( new LoggerImpl(n,mMutex) );
            mLoggers[n] = newLogger;

            // update logger tree
            LoggerImpl * parent = findParent( n );
            GN_ASSERT( parent );
            newLogger->setParent( parent );
            parent->reapplyAttributes();

            // sucess
            return newLogger.detach();
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
    GN_PUBLIC Logger * getLogger( const char * name )
    {
        LoggerContainer & lc = sGetLoggerContainer();
        return lc.getLogger( name );
    }
}
