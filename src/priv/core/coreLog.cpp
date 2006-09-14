#include "pch.h"
#include "garnet/GNcore.h"
#include <set>

// *****************************************************************************
// local classes and utils
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
            GN::Logger::LL_INFO <= level ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE );
        CONSOLE_SCREEN_BUFFER_INFO csbf;
        GetConsoleScreenBufferInfo( mConsole, &csbf );
        mAttrib = csbf.wAttributes;

        // change console color
        WORD attrib;
        switch( level )
        {
            case GN::Logger::LL_FATAL:
            case GN::Logger::LL_ERROR:
                attrib = FOREGROUND_RED;
                break;

            case GN::Logger::LL_WARN:
                attrib = FOREGROUND_RED | FOREGROUND_GREEN;
                break;

            case GN::Logger::LL_INFO:

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
static inline GN::StrA sLevel2Str( int level )
{
    switch( level )
    {
        case GN::Logger::LL_FATAL : return "FATAL";
        case GN::Logger::LL_ERROR : return "ERROR";
        case GN::Logger::LL_WARN  : return "WARN";
        case GN::Logger::LL_INFO  : return "INFO";
        case GN::Logger::LL_TRACE : return "TRACE";
        default                   : return GN::strFormat( "%d", level );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sPrintToFile( FILE * fp, GN::Logger & logger, const GN::Logger::LogDesc & desc, const GN::StrA & msg )
{
    if( GN::Logger::LL_INFO == desc.level )
    {
        ::fprintf( fp, "%s\n", msg );
    }
    else 
    {
        ::fprintf(
            fp,
            "%s(%d) : name(%s), level(%s) : %s\n",
            desc.file, desc.line,
            logger.getName().cptr(),
            sLevel2Str(desc.level).cptr(),
            msg.cptr() );
    }
}

template<class T>
class TreeNode
{
    T * mParent;
    T * mFirstChild;
    T * mPrevBrother;
    T * mNextBrother;

public:

    TreeNode() : mParent(0), mFirstChild(0), mPrevBrother(0), mNextBrother(0) {}

    T * parent() const { return mParent; }
    T * firstChild() const { return mFirstChild; }
    T * prevBrother() const { return mPrevBrother; }
    T * nextBrother() const { return mNextBrother; }

    //!
    //! set parent
    //!
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
    //!
    //! Log to console
    //!
    struct ConsoleReceiver : public Logger::Receiver
    {
        virtual void onLog( Logger & logger, const Logger::LogDesc & desc, const StrA & msg )
        {
            if( getEnvBoolean( "GN_LOG_QUIET" ) ) return;
            ConsoleColor cc(desc.level);
            sPrintToFile( desc.level < Logger::LL_INFO ? stderr : stdout, logger, desc, msg );
        };
    };

    //!
    //! Log to disk file
    //!
    struct FileReceiver : public Logger::Receiver
    {
        virtual void onLog( Logger & logger, const Logger::LogDesc & desc, const StrA & msg )
        {
#if GN_XENON
            StrA fileName = "game:\\garnet3d.log";
#else
            StrA fileName = getEnv( "GN_LOG_FILENAME" );
            if( fileName.empty() ) return;
#endif
            
#if GN_MSVC8
            FILE * fp;
            if( 0 == fopen_s( &fp, fileName.cptr(), "at" ) )
#else
            FILE * fp = fopen( fileName.cptr(), "at" );
            if( fp )
#endif
            {
                sPrintToFile( fp, logger, desc, msg );
                fclose( fp );
            }
        };
    };

    //!
    //! Log to debugger
    //!
    class DebugReceiver : public Logger::Receiver
    {
        virtual void onLog( Logger & logger, const Logger::LogDesc & desc, const StrA & msg )
        {
#if GN_MSWIN
            char buf[16384];
            strPrintf(
                buf,
                16384,
                "%s(%d) : %s : %s\n",
                desc.file,
                desc.line,
                logger.getName().cptr(),
                sLevel2Str(desc.level).cptr(),
                msg );
            ::OutputDebugStringA( buf );
#endif
        }
    };

    //!
    //! Logger implementation class
    //!
    class LoggerImpl : public Logger, public TreeNode<LoggerImpl>
    {
    public:

        LoggerImpl( const StrA & name ) : Logger(name), mInheritLevel(true), mInheritEnabled(true) {}

        void reapplyAttributes()
        {
            recursiveUpdateLevel( getLevel() );
            recursiveUpdateEnabled( isEnabled() );
        }

    public:

        virtual void setLevel( int level )
        {
            recursiveUpdateLevel( level );
            mInheritLevel = false;
        }

        virtual void setEnabled( bool enabled )
        {
            recursiveUpdateEnabled( enabled );
            mInheritEnabled = false;
        }

        virtual void doLog( const LogDesc & desc, const StrA & msg )
        {
            recursiveLog( *this, desc, msg );
        }

        virtual void addReceiver( Receiver * r )
        {
            if( 0 == r ) return;
            mReceivers.insert( r );
        }

        virtual void removeReceiver( Receiver * r )
        {
            if( 0 == r ) return;
            mReceivers.erase( r );
        }

        virtual void removeAllReceivers() { mReceivers.clear(); }

    private:

        std::set<Receiver*> mReceivers;
        bool mInheritLevel;
        bool mInheritEnabled;

        void recursiveLog( Logger & logger, const LogDesc & desc, const StrA & msg )
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

    //!
    //! Log container
    //!
    class LoggerContainer
    {
        ConsoleReceiver mCr;
        FileReceiver    mFr;
        DebugReceiver   mDr;
        LoggerImpl mRootLogger;

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

    public:

        LoggerContainer() : mRootLogger("ROOT")
        {
            // config root logger
#if GN_DEBUG_BUILD
            mRootLogger.setLevel( Logger::LL_TRACE );
#else
            mRootLogger.setLevel( Logger::LL_INFO );
#endif
            mRootLogger.setEnabled( true );
            mRootLogger.addReceiver( &mCr );
            mRootLogger.addReceiver( &mFr );
            mRootLogger.addReceiver( &mDr );
        }

        ~LoggerContainer()
        {
#if GN_DEBUG_BUILD
            static Logger * sLogger = getLogger("GN.core.LoggerContainer");
            StrA loggerTree;
            printLoggerTree( loggerTree, 0, mRootLogger );
            GN_TRACE(sLogger)( "\n%s", loggerTree.cptr() );
#endif
            std::for_each( mLoggers.begin(), mLoggers.end(), &sDeleteLogger );
        }

        LoggerImpl * getLogger( const char * name )
        {
            // trip leading and trailing dots
            StrA n(name);
            n.trim( '.' );

            // shortcut for root logger
            if( n.empty() ) return &mRootLogger;

            // find for existing logger
            std::map<StrA,LoggerImpl*>::const_iterator i = mLoggers.find( n );
            if( mLoggers.end() != i ) { GN_ASSERT( i->second ); return i->second; }

            // not found. create new one.
            AutoObjPtr<LoggerImpl> newLogger( new LoggerImpl(n) );
            mLoggers[n] = newLogger.get();

            // update logger tree
            LoggerImpl * parent = findParent( n );
            GN_ASSERT( parent );
            newLogger->setParent( parent );
            parent->reapplyAttributes();

            // sucess
            return newLogger.detach();
        }
    };

    //
    // Implement global log function.
    // -------------------------------------------------------------------------
    GN_PUBLIC Logger * getLogger( const char * name )
    {
        static LoggerContainer lc;
        return lc.getLogger( name );
    }
};
