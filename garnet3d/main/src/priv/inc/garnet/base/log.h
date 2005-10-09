#ifndef __GN_BASE_LOG_H__
#define __GN_BASE_LOG_H__
// *****************************************************************************
//! \file    log.h
//! \brief   log functions and macros
//! \author  chenlee (2005.4.17)
// *****************************************************************************

//!
//! output fatal error message
//!
#define GN_FATAL  ::GN::detail::LogHelper( __FILE__, __LINE__, GN::LOGLEVEL_FATAL, NULL ).log

//!
//! output error message
//!
#define GN_ERROR  ::GN::detail::LogHelper( GN_FUNCTION, __FILE__, __LINE__, GN::LOGLEVEL_ERROR, NULL ).log

//!
//! output warning message
//!
#define GN_WARN  ::GN::detail::LogHelper( GN_FUNCTION, __FILE__, __LINE__, GN::LOGLEVEL_WARN, NULL ).log

//!
//! output informational message
//!
#define GN_INFO  ::GN::detail::LogHelper( GN_FUNCTION, __FILE__, __LINE__, GN::LOGLEVEL_INFO, NULL ).log

//!
//! output trace message
//!
#if GN_DEBUG
#define GN_TRACE  ::GN::detail::LogHelper( GN_FUNCTION, __FILE__, __LINE__, GN::LOGLEVEL_TRACE, NULL ).log
#else
#define GN_TRACE  ::GN::detail::LogHelper::fake
#endif

//!
//! Implement default log
//!
#define GN_IMPLEMENT_DEFAULT_LOG() \
    GN_PUBLIC void ::GN::doLog( \
        GN::LogLevel level, const char * category, const char * msg, \
        const char * func, const char * file, int line ) \
    { ::GN::detail::defaultLogImpl(level,category,msg,func,file,line); }

namespace GN
{
    //!
    //! logging level
    //!
    enum LogLevel
    {
        LOGLEVEL_FATAL,
        LOGLEVEL_ERROR,
        LOGLEVEL_WARN,
        LOGLEVEL_INFO,
        LOGLEVEL_TRACE,
        NUM_LOGLEVELS,
    };

    //!
    //! log function (general version)
    //!
    //! \param level
    //!     log level.
    //! \param category
    //!     log category.
    //! \param msg
    //!     log message.
    //! \param func, file, line
    //!     source position. NULL file string indicates lacking of information
    //!     about source position.
    //!
    //! \note It is client's responsibility to implement this function
    //!
    // ------------------------------------------------------------------------
    GN_PUBLIC void
    doLog(
        LogLevel     level,
        const char * category,
        const char * msg,
        const char * func,
        const char * file,
        int          line );

    namespace detail
    {
        //!
        //! Log helper class
        //!
        //! This class will finally call doLog() to do the actual logging. 
        //!
        class LogHelper
        {
            const char * mFunc;
            const char * mFile;
            int          mLine;
            LogLevel     mLevel;
            const char * mCate;

        public:

            //!
            //! constructor
            //!
            LogHelper(
                const char * func,
                const char * file,
                int          line,
                LogLevel     level,
                const char * category )
                : mFunc(func)
                , mFile(file)
                , mLine(line)
                , mLevel(level)
                , mCate(category)
            {}

            //!
            //! fake log functions
            //!
            static void fake(...) {}

            //!
            //! do log, with predefined level and category
            //!
            void log( const char * fmt, ... );

            //!
            //! do log, with predefined level
            //!
            void logc( const char * category, const char * fmt, ... );

            //!
            //! do log, with specifc level and category
            //!
            void loglc( LogLevel level, const char * category, const char * fmt, ... );
        };

        //!
        //! Default implementation of log function.
        //!
        void
        defaultLogImpl(
            LogLevel     level,
            const char * category,
            const char * msg,
            const char * func,
            const char * file,
            int          line );
    }
} // end of namespace GN


// *****************************************************************************
//                           End of log.h
// *****************************************************************************
#endif // __GN_BASE_LOG_H__
