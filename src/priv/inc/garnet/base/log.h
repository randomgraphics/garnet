#ifndef __GN_BASE_LOG_H__
#define __GN_BASE_LOG_H__
// *****************************************************************************
/// \file    log.h
/// \brief   log functions and macros
/// \author  chenlee (2005.4.17)
// *****************************************************************************

///
/// General doLog macro, with user specified source code location
///
#if GN_ENABLE_LOG
#define GN_LOG_EX( logger, level, func, file, line ) if( logger->isOff( level ) ) {} else GN::Logger::LogHelper( logger, level, func, file, line ).doLog
#else
#define GN_LOG_EX( logger, level, func, file, line ) if( 1 ) {} else GN::Logger::sFakeLog
#endif

///
/// General doLog macro, with automatic source code location
///
#define GN_LOG( logger, level ) GN_LOG_EX( logger, level, GN_FUNCTION, __FILE__, __LINE__ )

///
/// output fatal error message
///
#define GN_FATAL( logger )  GN_LOG( logger, GN::Logger::LL_FATAL )

///
/// output error message
///
#define GN_ERROR( logger )  GN_LOG( logger, GN::Logger::LL_ERROR )

///
/// output warning message
///
#define GN_WARN( logger )  GN_LOG( logger, GN::Logger::LL_WARN )

///
/// output informational message
///
#define GN_INFO( logger )  GN_LOG( logger, GN::Logger::LL_INFO )

///
/// output detail message
///
#define GN_DETAIL( logger ) GN_LOG( logger, GN::Logger::LL_DETAIL )

///
/// output debug message (only effective in debug build)
///
#if GN_DEBUG_BUILD
#define GN_TRACE( logger ) GN_LOG( logger, GN::Logger::LL_TRACE )
#else
#define GN_TRACE( logger ) if( 1 ) {} else ::GN::Logger::sFakeLog
#endif

namespace GN
{
    ///
    /// Logger class
    ///
    class Logger
    {
    public:

        ///
        /// logging level
        ///
        enum LogLevel
        {
            LL_FATAL  = 10,  ///< fatal error
            LL_ERROR  = 20,  ///< error
            LL_WARN   = 30,  ///< warnning
            LL_INFO   = 40,  ///< information
            LL_DETAIL = 50,  ///< detailed log that you want to disabled normally
            LL_TRACE  = 200, ///< debugging (only available in debug build)
        };

        ///
        /// Log description structure
        ///
        struct LogDesc
        {
            int          level; ///< Log level/severity (required)
            const char * func;  ///< Log location: function name (optional). Set to NULL if you don't need it.
            const char * file;  ///< Log location: file name (optional). Set to NULL if you don't need it.
            int          line;  ///< Log location: line number (optional). Set to NULL if you don't need it.

            ///
            /// Default constructor. Do nothing.
            ///
            LogDesc() {}

            ///
            /// Construct doLog descriptor
            ///
            LogDesc(
                int          lvl_,
                const char * func_,
                const char * file_,
                int          line_ )
                : level(lvl_)
                , func(func_)
                , file(file_)
                , line(line_)
            {}
        };

        ///
        /// doLog helper
        ///
        struct LogHelper
        {
            Logger * mLogger; ///< Logger instance pointer
            LogDesc  mDesc;   ///< Logging descriptor

        public:

            ///
            /// Construct doLog helper
            ///
            LogHelper( Logger * logger, int level, const char * func, const char * file, int line )
                : mLogger(logger), mDesc(level,func,file,line)
            {
                GN_ASSERT( mLogger );
            }

            ///
            /// Do doLog
            ///
            void doLog( const char * fmt, ... );

            ///
            /// Do doLog (UNICODE)
            ///
            void doLog( const wchar_t * fmt, ... );
        };

        ///
        /// Log message receiver
        ///
        struct Receiver
        {
            ///
            /// virtual destructor
            ///
            virtual ~Receiver() {}

            ///
            /// deal with incoming log message
            ///
            virtual void onLog( Logger &, const LogDesc &, const StrA & ) = 0;

            ///
            /// deal with incoming UNICODE log message
            ///
            virtual void onLog( Logger &, const LogDesc &, const StrW & ) = 0;
        };

        ///
        /// virtual destructor
        ///
        virtual ~Logger() {}

        ///
        /// Do log
        ///
        virtual void doLog( const LogDesc & desc, const StrA & msg ) = 0;

        ///
        /// Do log (UNICODE)
        ///
        virtual void doLog( const LogDesc & desc, const StrW & msg ) = 0;

        ///
        /// change logger level.
        ///
        /// \param level    Log level.
        ///     - level>0 : output log message with level <= this value.
        ///     - level=0 : muted
        ///     - level<0 : output log messge with level == -(this value)
        ///
        virtual void setLevel( int level ) = 0;

        ///
        /// enable or disable this logger
        ///
        virtual void setEnabled( bool ) = 0;

        /// \name receiver management
        //@{
        virtual void addReceiver( Receiver * r ) = 0;
        virtual void removeReceiver( Receiver * r ) = 0;
        virtual void removeAllReceivers() = 0;
        //@}

        ///
        /// get logger name
        ///
        const StrA & getName() const { return mName; }

        ///
        /// get logger level
        ///
        int getLevel() const { return mLevel; }

        ///
        /// get logger enable/disable status
        ///
        bool isEnabled() const { return mEnabled; }

        ///
        /// is logging in effective?
        ///
        bool isOn( int level ) const
        {
            GN_ASSERT( level > 0 );
            return
                mEnabled &&
                0 != mLevel &&
                ( level <= mLevel || level == -mLevel );
        }

        ///
        /// is logging muted?
        ///
        bool isOff( int level ) const
        {
            GN_ASSERT( level > 0 );
            return
                !mEnabled ||
                0 == mLevel ||
                level > mLevel && level != -mLevel;
        }

        ///
        /// Fake logging. Do nothing.
        ///
        static inline void sFakeLog(...) {}

    protected:

        ///
        /// protective constructor
        ///
        Logger( const StrA & name ) : mName(name) {}

        int  mLevel;   ///< doLog level
        bool mEnabled; ///< logger enabled or not.

    private:
        StrA mName;
    };

    /// \name Global doLog functions
    //@{

    ///
    /// Get logger by name. Set name to NULL or empty string to get root logger.
    ///
    /// \param name
    ///     Logger name (case insensitive)
    ///
    GN_PUBLIC Logger * getLogger( const char * name );

    ///
    /// Get root logger
    ///
    inline Logger * getRootLogger() { return getLogger( 0 ); }

    //@}
} // end of namespace GN


// *****************************************************************************
//                           End of doLog.h
// *****************************************************************************
#endif // __GN_BASE_LOG_H__
