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
#define GN_FATAL  ::GN::detail::LogHelper( GN::LOGLEVEL_FATAL, NULL, GN_FUNCTION, __FILE__, __LINE__ ).log

//!
//! output error message
//!
#define GN_ERROR  ::GN::detail::LogHelper( GN::LOGLEVEL_ERROR, NULL, GN_FUNCTION, __FILE__, __LINE__ ).log

//!
//! output warning message
//!
#define GN_WARN  ::GN::detail::LogHelper( GN::LOGLEVEL_WARN, NULL, GN_FUNCTION, __FILE__, __LINE__ ).log

//!
//! output informational message
//!
#define GN_INFO  ::GN::detail::LogHelper( GN::LOGLEVEL_INFO, NULL, GN_FUNCTION, __FILE__, __LINE__ ).log

//!
//! output trace message
//!
#if GN_DEBUG
#define GN_TRACE  ::GN::detail::LogHelper( GN::LOGLEVEL_TRACE_0, NULL, GN_FUNCTION, __FILE__, __LINE__ ).log
#else
#define GN_TRACE  ::GN::detail::LogHelper::fake
#endif

//!
//! Implement default log
//!
#define GN_IMPLEMENT_DEFAULT_LOG() \
    GN_PUBLIC void ::GN::doLog( const LogDesc & desc, const char * msg ) \
    { ::GN::detail::defaultLogImpl(desc,msg); }

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
        LOGLEVEL_TRACE_0,
    };

    //!
    //! Log description structure
    //!
    struct LogDesc
    {
        int          level; //!< Log level/severity (required)
        const char * cate;  //!< Log category (optional). Set to NULL if you don't need it.
        const char * func;  //!< Log location: function name (optional). Set to NULL if you don't need it.
        const char * file;  //!< Log location: file name (optional). Set to NULL if you don't need it.
        int          line;  //!< Log location: line number (optional). Set to NULL if you don't need it.

        //!
        //! Default constructor. Do nothing.
        //!
        LogDesc() {}

        //!
        //! Construct log descriptor
        //!
        LogDesc(
            int          lvl_,
            const char * cate_,
            const char * func_,
            const char * file_,
            int          line_ )
            : level(lvl_)
            , cate(cate_)
            , func(func_)
            , file(file_)
            , line(line_)
        {}
    };

    //!
    //! log function (general version)
    //!
    //! \param desc Log descriptor.
    //! \param msg  Log message.
    //!
    //! \note It is client's responsibility to implement this function
    //!
    // ------------------------------------------------------------------------
    GN_PUBLIC void doLog( const LogDesc & desc, const char * msg );

    namespace detail
    {
        //!
        //! Log helper class
        //!
        //! This class will finally call doLog() to do the actual logging.
        //!
        class LogHelper
        {
            LogDesc mDesc;

        public:

            //!
            //! constructor
            //!
            LogHelper(
                int          level,
                const char * cate,
                const char * func,
                const char * file,
                int          line )
                : mDesc(level,cate,func,file,line)
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
            //! do log, with predefined level and user-specified category
            //!
            void logc( const char * cate, const char * fmt, ... );

            //!
            //! do log, with user-specified level and category
            //!
            void loglc( int level, const char * cate, const char * fmt, ... );
        };

        //!
        //! Default implementation of log function.
        //!
        void defaultLogImpl( const LogDesc & desc, const char * msg );
    }
} // end of namespace GN


// *****************************************************************************
//                           End of log.h
// *****************************************************************************
#endif // __GN_BASE_LOG_H__
