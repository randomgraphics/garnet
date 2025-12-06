#ifndef __GN_BASE_LOG_H__
#define __GN_BASE_LOG_H__
// *****************************************************************************
/// \file
/// \brief   log functions and macros
/// \author  chenlee (2005.4.17)
// *****************************************************************************

#include <chrono>
#include <sstream>

#ifndef FMT_HEADER_ONLY
    #define FMT_HEADER_ONLY
#endif
#include <fmt/format.h>
#include <fmt/xchar.h>
#include <fmt/printf.h>

///
/// General log macros, with user specified source code location
//@{
#if GN_ENABLE_LOG
    #define GN_LOG_EX(logger, level, func, file, line) \
        if (logger->isOff(level)) {                    \
        } else                                         \
            GN::Logger::LogHelper(logger, level, func, file, line)
#else
    #define GN_LOG_EX(logger, level, func, file, line) \
        if (1) {                                       \
        } else                                         \
            GN::Logger::sFakeLog
#endif
//@}

///
/// General log macro, with automatic source code location
///
#define GN_LOG(logger, level) GN_LOG_EX(logger, level, GN_FUNCTION, __FILE__, __LINE__)

///
/// output fatal error message
///
#define GN_FATAL(logger) GN_LOG(logger, GN::Logger::FATAL)

///
/// output error message
///
#define GN_ERROR(logger) GN_LOG(logger, GN::Logger::ERROR_)

///
/// output warning message
///
#define GN_WARN(logger) GN_LOG(logger, GN::Logger::WARN)

///
/// output informational message
///
#define GN_INFO(logger) GN_LOG(logger, GN::Logger::INFO)

///
/// output verbose message
///
#define GN_VERBOSE(logger) GN_LOG(logger, GN::Logger::VERBOSE)

///
/// output very-verbose message
///
#define GN_BABBLE(logger) GN_LOG(logger, GN::Logger::BABBLE)

///
/// Debug only log macros (no effect to non-debug build)
///
//@{
#if GN_BUILD_DEBUG_ENABLED
    #define GN_TRACE(logger)   GN_INFO(logger)
    #define GN_VTRACE(logger)  GN_VERBOSE(logger)
    #define GN_VVTRACE(logger) GN_BABBLE(logger)
#else
    #define GN_TRACE(logger) \
        if (1) {             \
        } else               \
            ::GN::Logger::sFakeLog
    #define GN_VTRACE(logger) \
        if (1) {              \
        } else                \
            ::GN::Logger::sFakeLog
    #define GN_VVTRACE(logger) \
        if (1) {               \
        } else                 \
            ::GN::Logger::sFakeLog
#endif
//@}

namespace GN {

namespace internal {

struct GN_API WideString {
    const wchar_t * wstr         = nullptr;
    bool            needDeletion = false;
    WideString(const char *);
    ~WideString();
};

///
/// String format utility class. Reserved for internal use only.
///
template<typename CHAR>
class StringFormatter {
#if GN_BUILD_DEBUG_ENABLED
    static bool lookForPrintfSpecifiers(const CHAR * fmt) {
        if (!fmt) return false;

        // Helper function to check if a character is a valid printf conversion specifier
        auto isPrintfSpecifier = [](CHAR c) -> bool {
            // Valid printf conversion specifiers: d, i, o, u, x, X, f, F, e, E, g, G, a, A, c, s, p, n
            return (c == 'd' || c == 'i' || c == 'o' || c == 'u' || c == 'x' || c == 'X' || c == 'f' || c == 'F' || c == 'e' || c == 'E' || c == 'g' ||
                    c == 'G' || c == 'a' || c == 'A' || c == 'c' || c == 's' || c == 'p' || c == 'n');
        };

        const CHAR * p = fmt;
        while (*p) {
            if (*p == '%') {
                ++p;
                // Handle escaped percent sign
                if (*p == '%') {
                    ++p;
                    continue;
                }

                // Skip optional flags: -+ #0 space
                while (*p == '-' || *p == '+' || *p == ' ' || *p == '#' || *p == '0') { ++p; }

                // Skip optional width: digits or *
                if (*p == '*') {
                    ++p;
                } else {
                    while (*p >= '0' && *p <= '9') { ++p; }
                }

                // Skip optional precision: . followed by digits or *
                if (*p == '.') {
                    ++p;
                    if (*p == '*') {
                        ++p;
                    } else {
                        while (*p >= '0' && *p <= '9') { ++p; }
                    }
                }

                // Skip optional length modifiers: h, hh, l, ll, j, z, t, L
                if (*p == 'h') {
                    ++p;
                    if (*p == 'h') ++p; // hh
                } else if (*p == 'l') {
                    ++p;
                    if (*p == 'l') ++p; // ll
                } else if (*p == 'j' || *p == 'z' || *p == 't' || *p == 'L') {
                    ++p;
                }

                // Check for conversion specifier
                if (*p && isPrintfSpecifier(*p)) {
                    return true; // Found printf-style format specifier
                }
            } else {
                ++p;
            }
        }

        return false; // No printf-style format specifiers found
    }

    static void checkForPrintf(const CHAR * fmt) {
        if (!fmt || !*fmt) return;
        if constexpr (std::is_same_v<CHAR, char>) {
            auto s = fmt::format("Printf syntax is deprecated: {}", fmt);
            GN_ASSERT_EX(!lookForPrintfSpecifiers(fmt), s.c_str());
        } else if constexpr (std::is_same_v<CHAR, wchar_t>) {
            auto s = fmt::format(L"Printf syntax is deprecated: {}", fmt);
            GN_ASSERT_EX(!lookForPrintfSpecifiers(fmt), s.c_str());
        } else {
            GN_ASSERT_EX(!lookForPrintfSpecifiers(fmt), "Printf syntax is deprecated");
        }
    }
#else
    // do nothing in release build
    static void checkForPrintf(const CHAR *) {}
#endif

    static void printInvalidFormatSyntax([[maybe_unused]] const CHAR * fmt, [[maybe_unused]] const char * what) {
        if constexpr (std::is_same_v<CHAR, char>) {
            GN_ASSERT_EX(false, fmt::format("{}: {}", what, fmt).c_str());
        } else if constexpr (std::is_same_v<CHAR, wchar_t>) {
            GN_ASSERT_EX(false, fmt::format(L"{}: {}", WideString(what).wstr, fmt).c_str());
        }
    }

    std::basic_string<CHAR>         mResult;
    bool                            mIsPreallocated = true;
    static inline thread_local CHAR mPreAllocatedBuffer[1024];

public:
    /// @brief Format the string to the output buffer. The output string is guaranteed to be null terminated.
    /// @tparam ...Args types of the arguments to the format string.
    /// @param outputBuffer The output buffer.
    /// @param outputBufferSize The size of the output buffer, including the null terminator.
    /// @param fmt The format string.
    /// @param ...args The arguments to the format string.
    template<typename... Args, std::enable_if_t<(std::is_convertible<CHAR, char>::value), bool> = true>
    constexpr static void formatToBuffer(CHAR * outputBuffer, size_t outputBufferSize, fmt::format_string<Args...> fmt, Args &&...) {
        // handle empty input and output buffer
        if (!outputBuffer || 0 == outputBufferSize) return;
        checkForPrintf(fmt.str.data());
        try {
            // auto result       = fmt::format_to_n(outputBuffer, outputBufferSize - 1, fmt, std::forward<Args>(args)...);
            // auto len          = std::min(result.size, outputBufferSize - 1);
            // outputBuffer[len] = 0;
        } catch (std::exception & e) { printInvalidFormatSyntax(fmt.str.data(), e.what()); } catch (...) {
            printInvalidFormatSyntax(fmt.str.data(), "Unknown exception when formatting string");
        }
    }

    /// @brief Format the string to the output buffer. The output string is guaranteed to be null terminated.
    /// @tparam ...Args types of the arguments to the format string.
    /// @param outputBuffer The output buffer.
    /// @param outputBufferSize The size of the output buffer, including the null terminator.
    /// @param fmt The format string.
    /// @param ...args The arguments to the format string.
    template<typename... Args, std::enable_if_t<(std::is_convertible<CHAR, wchar_t>::value), bool> = true>
    constexpr static void formatToBuffer(CHAR * outputBuffer, size_t outputBufferSize, fmt::wformat_string<Args...> fmt, Args &&...) {
        // handle empty input and output buffer
        if (!outputBuffer || 0 == outputBufferSize) return;
        checkForPrintf(fmt.get().data());
        try {
            // auto result       = fmt::format_to_n(outputBuffer, outputBufferSize - 1, fmt, std::forward<Args>(args)...);
            // auto len          = std::min(result.size, outputBufferSize - 1);
            // outputBuffer[len] = 0;
        } catch (std::exception & e) { printInvalidFormatSyntax(fmt.get().data(), e.what()); } catch (...) {
            printInvalidFormatSyntax(fmt.get().data(), "Unknown exception when formatting string");
        }
    }

    /// Return size of the formatted string, not including the null terminator.
    template<typename... Args>
    constexpr static size_t formattedSize(const CHAR * fmt, Args &&... args) {
        if (!fmt || !*fmt) return 0;
        checkForPrintf(fmt);
        try {
            return fmt::formatted_size(fmt, std::forward<Args>(args)...);
        } catch (std::exception & e) {
            printInvalidFormatSyntax(fmt, e.what());
            return 0;
        } catch (...) {
            printInvalidFormatSyntax(fmt, "Unknown exception when formatting string");
            return 0;
        }
    }

    template<typename... Args>
    constexpr StringFormatter(const CHAR * fmt, Args &&... args) {
        if (!fmt || !*fmt) {
            mIsPreallocated        = true;
            mPreAllocatedBuffer[0] = 0;
            return;
        }

        checkForPrintf(fmt);

        try {
            // get size of the formatted string
            auto r = fmt::formatted_size(fmt, std::forward<Args>(args)...);

            constexpr size_t maxCharacters = sizeof(mPreAllocatedBuffer) / sizeof(CHAR) - 1; // needs one additional space for the null terminator

            if (r > maxCharacters) {
                mIsPreallocated = false;
                mResult         = fmt::format(fmt, std::forward<Args>(args)...);
            } else {
                mIsPreallocated = true;
                fmt::format_to_n(mPreAllocatedBuffer, maxCharacters, fmt, std::forward<Args>(args)...);
                mPreAllocatedBuffer[std::min(r, maxCharacters)] = 0;
            }
        } catch (const std::exception & e) {
            if constexpr (std::is_same_v<CHAR, char>) {
                mResult = fmt::format("{}: {}", e.what(), fmt);
            } else if constexpr (std::is_same_v<CHAR, wchar_t>) {
                mResult = fmt::format(L"{}: {}", WideString(e.what()).wstr, fmt);
            }
            mIsPreallocated = false;
        }
    }

    const CHAR * result() const { return mIsPreallocated ? mPreAllocatedBuffer : mResult.c_str(); }
};

///
/// String formatter class using the old school printf syntax.
///
template<typename CHAR>
class StringPrinter {
    std::basic_string<CHAR> mResult;

public:
    template<typename... Args>
    StringPrinter(const CHAR * fmt, Args &&... args) {
        if (!fmt || !*fmt) { return; }
        mResult = fmt::vsprintf<CHAR>(fmt::basic_string_view<CHAR>(fmt), fmt::make_printf_args<CHAR>(args...));
    }
    const CHAR * result() const { return mResult.c_str(); }
    size_t       size() const { return mResult.size(); }
};

} // end of namespace internal

///
/// Logger class
///
class Logger {
public:
    ///
    /// logging level
    ///
    enum LogLevel {
        FATAL   = 10, ///< fatal error message
        ERROR_  = 20, ///< error message (Note: ERROR is define as a macro)
        WARN    = 30, ///< warning message
        INFO    = 40, ///< informational message
        VERBOSE = 50, ///< verbose message
        BABBLE  = 60, ///< very verbose message
    };

    ///
    /// Log location in source code
    ///
    struct LogLocation {
        int          level; ///< Log level/severity (required)
        const char * func;  ///< Log location: function name (optional). Set to NULL if you don't need it.
        const char * file;  ///< Log location: file name (optional). Set to NULL if you don't need it.
        int          line;  ///< Log location: line number (optional). Set to NULL if you don't need it.

        ///
        /// Default constructor. Do nothing.
        ///
        LogLocation() {}

        ///
        /// Construct doLog descriptor
        ///
        LogLocation(int lvl_, const char * func_, const char * file_, int line_): level(lvl_), func(func_), file(file_), line(line_) {}
    };

    ///
    /// doLog helper
    ///
    struct GN_API LogHelper {
        Logger *    mLogger; ///< Logger instance pointer
        LogLocation mDesc;   ///< Logging descriptor
        uint8_t     mStreamBuffer[sizeof(std::stringstream)];
        bool        mStreamConstructed = false;

        std::stringstream * ss() {
            if (!mStreamConstructed) {
                new (mStreamBuffer) std::stringstream();
                mStreamConstructed = true;
            }
            return (std::stringstream *) mStreamBuffer;
        }

        template<class T>
        static inline void dtor(T * p) {
            p->~T();
        }

    public:
        ///
        /// Construct doLog helper
        ///
        LogHelper(Logger * logger, int level, const char * func, const char * file, int line): mLogger(logger), mDesc(level, func, file, line) {
            GN_ASSERT(mLogger);
        }

        ///
        /// destructor
        ///
        ~LogHelper() {
            if (mStreamConstructed) {
                if (mLogger) GN_LIKELY mLogger->doLog(mDesc, ss()->str().c_str());
                dtor(ss());
            }
        }

        ///
        /// stream style log operator
        ///
        template<typename T>
        inline LogHelper & operator<<(T && t) {
            *ss() << std::forward<T>(t);
            return *this;
        }

        template<typename... Args>
        void operator()(fmt::format_string<Args...> format_, Args &&... args) const {
            // if (mLogger->isPrintfSyntax()) GN_UNLIKELY {
            //         return mLogger->doLog(mDesc, internal::StringPrinter<char>(format_, std::forward<Args>(args_)...).result());
            //     }
            // else { return mLogger->doLog(mDesc, internal::StringFormatter<char>(format_, std::forward<Args>(args_)...).result()); }
            return mLogger->doLog(mDesc, fmt::format(format_, std::forward<Args>(args)...).c_str());
        }

        template<typename... Args>
        void operator()(fmt::wformat_string<Args...> format_, Args &&... args_) {
            // if (mLogger->isPrintfSyntax()) GN_UNLIKELY {
            //         return mLogger->doLog(mDesc, internal::StringPrinter<wchar_t>(format_, std::forward<Args>(args_)...).result());
            //     }
            // else { return mLogger->doLog(mDesc, internal::StringFormatter<wchar_t>(format_, std::forward<Args>(args_)...).result()); }
            return mLogger->doLog(mDesc, fmt::format(format_, std::forward<Args>(args_)...).c_str());
        }
    };

    ///
    /// Log message receiver
    ///
    struct Receiver {
        ///
        /// virtual destructor
        ///
        virtual ~Receiver() {}

        ///
        /// deal with incoming log message
        ///
        virtual void onLog(Logger &, const LogLocation &, const char *) = 0;

        ///
        /// deal with incoming UNICODE log message
        ///
        virtual void onLog(Logger &, const LogLocation &, const wchar_t *) = 0;
    };

    ///
    /// virtual destructor
    ///
    virtual ~Logger() {}

    ///
    /// Do log
    ///
    virtual void doLog(const LogLocation & desc, const char * msg) = 0;

    ///
    /// Do log (UNICODE)
    ///
    virtual void doLog(const LogLocation & desc, const wchar_t * msg) = 0;

    ///
    /// change logger level.
    ///
    /// \param level    Log level.
    ///     - level>0 : output log message with level <= this value.
    ///     - level=0 : muted
    ///     - level<0 : output log messge with level == -(this value)
    ///
    virtual void setLevel(int level) = 0;

    ///
    /// enable or disable this logger
    ///
    virtual void setEnabled(bool) = 0;

    /// \name receiver management
    //@{
    virtual void addReceiver(Receiver * r)    = 0;
    virtual void removeReceiver(Receiver * r) = 0;
    virtual void removeAllReceivers()         = 0;
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
    bool isOn(int level) const {
        GN_ASSERT(level > 0);
        return mEnabled && 0 != mLevel && (level <= mLevel || level == -mLevel);
    }

    ///
    /// is logging muted?
    ///
    bool isOff(int level) const {
        GN_ASSERT(level > 0);
        return !mEnabled || 0 == mLevel || (level > mLevel && level != -mLevel);
    }

    ///
    /// is using printf syntax?
    ///
    bool isPrintfSyntax() const { return mUsePrintfSyntax; }

    ///
    /// Fake logging. Do nothing.
    ///
    static inline void sFakeLog(...) {}

protected:
    ///
    /// protective constructor
    ///
    Logger(const char * name, bool usePrintfSyntax): mName(name), mUsePrintfSyntax(usePrintfSyntax) {}

    int  mLevel;   ///< doLog level
    bool mEnabled; ///< logger enabled or not.

private:
    const char * mName;
    bool         mUsePrintfSyntax;
};

/// \name Global doLog functions
//@{

///
/// Get logger by name. Set name to NULL or empty string to get root logger.
///
/// \param name
///     Logger name (case insensitive)
///
GN_API Logger * getLogger(const char * name, bool usePrintfSyntax = false);

///
/// Get root logger
///
inline Logger * getRootLogger() { return getLogger(0); }

//@}
} // end of namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_LOG_H__
