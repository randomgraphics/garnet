#ifndef __GN_BASE_LOG_H__
#define __GN_BASE_LOG_H__
// *****************************************************************************
/// \file
/// \brief   log functions and macros
/// \author  chenlee (2005.4.17)
// *****************************************************************************

#include <chrono>
#include <sstream>

/// General log macros, with user specified source code location
//@{
#if GN_ENABLE_LOG
#define GN_LOG_EX( logger, level, func, file, line ) if( logger->isOff( level ) ) {} else GN::Logger::LogHelper( logger, level, func, file, line )
#define GN_LOG_BEGIN( logger, level )                if( logger->isOn ( level ) ) {
#define GN_LOG_END()                                 }
#else
#define GN_LOG_EX( logger, level, func, file, line ) if( 1 ) {} else GN::Logger::sFakeLog
#define GN_LOG_BEGIN( logger, level )                if( 0 ) {
#define GN_LOG_END()                                 }
#endif
//@}

///
/// General log macro, with automatic source code location
///
#define GN_LOG( logger, level ) GN_LOG_EX( logger, level, GN_FUNCTION, __FILE__, __LINE__ )

///
/// output fatal error message
///
#define GN_FATAL( logger )  GN_LOG( logger, GN::Logger::FATAL )

///
/// output error message
///
#define GN_ERROR( logger )  GN_LOG( logger, GN::Logger::ERROR_ )

///
/// output warning message
///
#define GN_WARN( logger )  GN_LOG( logger, GN::Logger::WARN )

///
/// output informational message
///
#define GN_INFO( logger )  GN_LOG( logger, GN::Logger::INFO )

///
/// output verbose message
///
#define GN_VERBOSE( logger ) GN_LOG( logger, GN::Logger::VERBOSE )

///
/// output very-verbose message
///
#define GN_VVERBOSE( logger ) GN_LOG( logger, GN::Logger::VVERBOSE )

///
/// Debug only log macros (no effect to non-debug build)
///
//@{
#if GN_BUILD_DEBUG_ENABLED
#define GN_TRACE( logger )   GN_INFO( logger )
#define GN_VTRACE( logger )  GN_VERBOSE( logger )
#define GN_VVTRACE( logger ) GN_VVERBOSE( logger )
#else
#define GN_TRACE( logger )   if( 1 ) {} else ::GN::Logger::sFakeLog
#define GN_VTRACE( logger )  if( 1 ) {} else ::GN::Logger::sFakeLog
#define GN_VVTRACE( logger ) if( 1 ) {} else ::GN::Logger::sFakeLog
#endif
//@}

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
            FATAL    = 10,  ///< fatal error message
            ERROR_   = 20,  ///< error message (Note: ERROR is define as a macro)
            WARN     = 30,  ///< warning message
            INFO     = 40,  ///< informational message
            VERBOSE  = 50,  ///< verbose message
            VVERBOSE = 60,  ///< very verbose message
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
        struct GN_API LogHelper
        {
            Logger * mLogger; ///< Logger instance pointer
            LogDesc  mDesc;   ///< Logging descriptor
            uint8_t  mStreamBuffer[sizeof(std::stringstream)];
            bool     mStreamConstructed = false;

            std::stringstream * ss() {
                if( !mStreamConstructed ) {
                    new (mStreamBuffer) std::stringstream();
                    mStreamConstructed = true;
                }
                return (std::stringstream*)mStreamBuffer;
            }

            template<class T>
            static inline void dtor(T * p) { p->~T(); }

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
            /// destructor
            ///
            ~LogHelper() {
                if (mStreamConstructed) {
                    operator()( ss()->str().c_str() );
                    dtor(ss());
                }
            }

            ///
            /// stream style log operator
            ///
            template <typename T>
            inline LogHelper & operator<<( T && t ) {
                *ss() << std::forward<T>( t );
                return *this;
            }

            ///
            /// printf style log
            ///
            void operator()( const char * fmt, ... );

            ///
            /// printf style log (wide char)
            ///
            void operator()( const wchar_t * fmt, ... );
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
            virtual void onLog( Logger &, const LogDesc &, const char * ) = 0;

            ///
            /// deal with incoming UNICODE log message
            ///
            virtual void onLog( Logger &, const LogDesc &, const wchar_t * ) = 0;
        };

        ///
        /// virtual destructor
        ///
        virtual ~Logger() {}

        ///
        /// Do log
        ///
        virtual void doLog( const LogDesc & desc, const char * msg ) = 0;

        ///
        /// Do log (UNICODE)
        ///
        virtual void doLog( const LogDesc & desc, const wchar_t * msg ) = 0;

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
        const char * getName() const { return mName; }

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
                ( level > mLevel && level != -mLevel );
        }

        ///
        /// Fake logging. Do nothing.
        ///
        static inline void sFakeLog(...) {}

    protected:

        ///
        /// protective constructor
        ///
        Logger( const char * name ) : mName(name) {}

        int  mLevel;   ///< doLog level
        bool mEnabled; ///< logger enabled or not.

    private:
        const char * mName;
    };

    /// \name Global doLog functions
    //@{

    ///
    /// Get logger by name. Set name to NULL or empty string to get root logger.
    ///
    /// \param name
    ///     Logger name (case insensitive)
    ///
    GN_API Logger * getLogger( const char * name );

    ///
    /// Get root logger
    ///
    inline Logger * getRootLogger() { return getLogger( 0 ); }

    //@}
} // end of namespace GN


// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_LOG_H__
