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
/// General log macro, with user specified source code location.
///
/// The logger comes first, followed by the source location, followed by an optional format argument
/// list in either fmt brace syntax or printf syntax. Because the expansion ends in an expression of
/// type GN::Logger::LogHelper &, a streaming expression may be appended to any of these forms, and
/// the two syntaxes can be combined in a single statement:
///
/// \code
/// GN_LOG_EX(logger, level, func, file, line);                                    // nothing logged
/// GN_LOG_EX(logger, level, func, file, line, "a = {}", a);                       // format only
/// GN_LOG_EX(logger, level, func, file, line) << "a = " << a;                     // streaming only
/// GN_LOG_EX(logger, level, func, file, line, "a = {}", a) << ", b = " << b;      // both -> "a = 1, b = 2"
/// \endcode
///
/// Cost when the level is off is a single level test: the whole expansion sits in the else branch,
/// so neither the helper nor any format or streamed argument is constructed or evaluated.
///
/// The `switch (0) case 0: default:` guard is what makes the inner if/else a complete statement.
/// Without it, `if (cond) GN_LOG_EX(...);` would nest an if/else inside an if and trip
/// -Wdangling-else, and a following `else` would bind to the wrong if.
///
#define GN_LOG_EX(logger, level, func, file, line, ...)         \
    switch (0)                                                  \
    case 0:                                                     \
    default:                                                    \
        if (auto gnLogger_ = (logger); gnLogger_->isOff(level)) \
            GN_LIKELY;                                          \
        else                                                    \
            ::GN::Logger::LogHelper(gnLogger_, level, func, file, line).emit(__VA_ARGS__)

///
/// Log using printf syntax unconditionally, with user specified source code location. Useful for
/// redirecting third party library logging, which is printf based. GN_LOG_EX already accepts printf
/// syntax; reach for this only when a format string must never be interpreted as fmt.
///
#define GN_PRINTF_EX(logger, level, func, file, line, ...)      \
    switch (0)                                                  \
    case 0:                                                     \
    default:                                                    \
        if (auto gnLogger_ = (logger); gnLogger_->isOff(level)) \
            GN_LIKELY;                                          \
        else                                                    \
            ::GN::Logger::LogHelper(gnLogger_, level, func, file, line).emitPrintf(__VA_ARGS__)

///
/// General log macro, with automatic source code location
///
#define GN_LOG(logger, level, ...) GN_LOG_EX(logger, level, GN_FUNCTION, __FILE__, __LINE__, ##__VA_ARGS__)

///
/// output fatal error message
///
#define GN_FATAL(logger, ...) GN_LOG(logger, ::GN::Logger::FATAL, ##__VA_ARGS__)

///
/// output error message
///
#define GN_ERROR(logger, ...) GN_LOG(logger, ::GN::Logger::ERROR_, ##__VA_ARGS__)

///
/// output warning message
///
#define GN_WARN(logger, ...) GN_LOG(logger, ::GN::Logger::WARN, ##__VA_ARGS__)

///
/// output informational message
///
#define GN_INFO(logger, ...) GN_LOG(logger, ::GN::Logger::INFO, ##__VA_ARGS__)

///
/// output verbose message
///
#define GN_VERBOSE(logger, ...) GN_LOG(logger, ::GN::Logger::VERBOSE, ##__VA_ARGS__)

///
/// output very-verbose message
///
#define GN_BABBLE(logger, ...) GN_LOG(logger, ::GN::Logger::BABBLE, ##__VA_ARGS__)

///
/// Debug only log macros (no effect to non-debug build)
///
//@{
#if GN_BUILD_DEBUG_ENABLED
    #define GN_TRACE(logger, ...)   GN_INFO(logger, ##__VA_ARGS__)
    #define GN_VTRACE(logger, ...)  GN_VERBOSE(logger, ##__VA_ARGS__)
    #define GN_VVTRACE(logger, ...) GN_BABBLE(logger, ##__VA_ARGS__)
#else
    /// Discards the statement without generating code, while still type-checking the arguments so
    /// that disabled logs cannot rot and log-only locals do not warn as unused. Shaped like
    /// GN_LOG_EX so that an appended streaming expression still compiles.
    #define GN_LOG_DISCARD_(logger, ...) \
        switch (0)                       \
        case 0:                          \
        default:                         \
            if (true)                    \
                GN_LIKELY;               \
            else                         \
                ::GN::Logger::LogHelper(logger, ::GN::Logger::BABBLE, "", "", 0).emit(__VA_ARGS__)
    #define GN_TRACE(logger, ...)   GN_LOG_DISCARD_(logger, ##__VA_ARGS__)
    #define GN_VTRACE(logger, ...)  GN_LOG_DISCARD_(logger, ##__VA_ARGS__)
    #define GN_VVTRACE(logger, ...) GN_LOG_DISCARD_(logger, ##__VA_ARGS__)
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
/// Does the format string contain at least one printf conversion specifier? Used to route
/// between printf and fmt formatting. Reserved for internal use only.
///
template<typename CHAR>
inline bool hasPrintfSpecifier(const CHAR * formatString) {
    if (!formatString) return false;

    for (const CHAR * p = formatString; *p; ++p) {
        if (*p != '%') continue;
        ++p;
        if (*p == '%') return true; // "%%" is a printf escape, so this is printf syntax

        // optional flags
        while (*p == '-' || *p == '+' || *p == ' ' || *p == '#' || *p == '0') ++p;

        // optional width
        if (*p == '*') {
            ++p;
        } else {
            while (*p >= '0' && *p <= '9') ++p;
        }

        // optional precision
        if (*p == '.') {
            ++p;
            if (*p == '*') {
                ++p;
            } else {
                while (*p >= '0' && *p <= '9') ++p;
            }
        }

        // optional length modifier
        if (*p == 'h' || *p == 'l') {
            ++p;
            if (*p == 'h' || *p == 'l') ++p; // "hh" and "ll"
        } else if (*p == 'j' || *p == 'z' || *p == 't' || *p == 'L') {
            ++p;
        }

        switch (*p) {
        case 'd':
        case 'i':
        case 'o':
        case 'u':
        case 'x':
        case 'X':
        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
        case 'a':
        case 'A':
        case 'c':
        case 's':
        case 'p':
        case 'n':
            return true;
        default:
            break;
        }

        if (!*p) break; // the trailing '%' ate the null terminator
    }

    return false;
}

///
/// Does the format string contain at least one fmt replacement field? "{{" is an escaped brace
/// and does not count. Reserved for internal use only.
///
template<typename CHAR>
inline bool hasFmtField(const CHAR * formatString) {
    if (!formatString) return false;

    for (const CHAR * p = formatString; *p; ++p) {
        if (*p != '{') continue;
        if (p[1] == '{') {
            ++p;
            continue;
        }
        return true;
    }

    return false;
}

///
/// String format utility class. Reserved for internal use only.
///
template<typename CHAR, size_t PREALLOCATED_CHARACTERS = 1024>
class StringFormatter {
#if GN_BUILD_DEBUG_ENABLED
    /// StringFormatter only understands fmt brace syntax. Callers that want printf syntax must
    /// route through fmt::sprintf instead; GN::Logger::LogHelper::emit() does that automatically.
    /// A string carrying a printf specifier alongside a brace field is intentional and allowed.
    static void checkForPrintf(const CHAR * formatString) {
        if (!formatString || !*formatString) return;
        if (!hasPrintfSpecifier(formatString) || hasFmtField(formatString)) return;
        if constexpr (std::is_same_v<CHAR, char>) {
            auto s = fmt::format("Printf syntax is not supported here, use GN_PRINTF_EX or a log macro: {}", formatString);
            GN_ASSERT(false, s.c_str());
        } else if constexpr (std::is_same_v<CHAR, wchar_t>) {
            auto s = fmt::format(L"Printf syntax is not supported here, use GN_PRINTF_EX or a log macro: {}", formatString);
            GN_ASSERT(false, s.c_str());
        } else {
            GN_ASSERT(false, "Printf syntax is not supported here");
        }
    }
#else
    // do nothing in release build
    static void checkForPrintf(const CHAR *) {}
#endif

    static void printInvalidFormatSyntax([[maybe_unused]] const CHAR * formatString, [[maybe_unused]] const char * what) {
        if constexpr (std::is_same_v<CHAR, char>) {
            GN_ASSERT(false, fmt::format("{}: {}", what, formatString));
        } else if constexpr (std::is_same_v<CHAR, wchar_t>) {
            GN_ASSERT(false, fmt::format(L"{}: {}", WideString(what).wstr, formatString));
        }
    }

    std::basic_string<CHAR>         mResult;
    bool                            mIsPreallocated = true;
    static inline thread_local CHAR mPreAllocatedBuffer[PREALLOCATED_CHARACTERS + 1]; // needs one additional space for the null terminator

public:
    /// @brief Format the string to the output buffer. The output string is guaranteed to be null terminated.
    /// @tparam ...Args types of the arguments to the format string.
    /// @param outputBuffer The output buffer.
    /// @param outputBufferSize The size of the output buffer, including the null terminator.
    /// @param fmt The format string.
    /// @param ...args The arguments to the format string.
    template<typename... Args, std::enable_if_t<(std::is_convertible<CHAR, char>::value), bool> = true>
    constexpr static void formatToBuffer(CHAR * outputBuffer, size_t outputBufferSize, fmt::format_string<Args...> formatString, Args &&... args) {
        // handle empty input and output buffer
        if (!outputBuffer || 0 == outputBufferSize) return;
        checkForPrintf(formatString.get().data());
        try {
            auto result       = fmt::format_to_n(outputBuffer, outputBufferSize - 1, formatString, std::forward<Args>(args)...);
            auto len          = std::min(result.size, outputBufferSize - 1);
            outputBuffer[len] = 0;
        } catch (std::exception & e) { printInvalidFormatSyntax(formatString.get().data(), e.what()); } catch (...) {
            printInvalidFormatSyntax(formatString.get().data(), "Unknown exception when formatting string");
        }
    }

    /// @brief Format the string to the output buffer. The output string is guaranteed to be null terminated.
    /// @tparam ...Args types of the arguments to the format string.
    /// @param outputBuffer The output buffer.
    /// @param outputBufferSize The size of the output buffer, including the null terminator.
    /// @param fmt The format string.
    /// @param ...args The arguments to the format string.
    template<typename... Args, std::enable_if_t<(std::is_convertible<CHAR, wchar_t>::value), bool> = true>
    constexpr static void formatToBuffer(CHAR * outputBuffer, size_t outputBufferSize, fmt::wformat_string<Args...> formatString, Args &&... args) {
        // handle empty input and output buffer
        if (!outputBuffer || 0 == outputBufferSize) return;
        checkForPrintf(formatString.get().data());
        try {
            auto result       = fmt::format_to_n(outputBuffer, outputBufferSize - 1, formatString.get(), std::forward<Args>(args)...);
            auto len          = std::min(result.size, outputBufferSize - 1);
            outputBuffer[len] = 0;
        } catch (std::exception & e) { printInvalidFormatSyntax(formatString.get().data(), e.what()); } catch (...) {
            printInvalidFormatSyntax(formatString.get().data(), "Unknown exception when formatting string");
        }
    }

    /// Return size of the formatted string, not including the null terminator.
    template<typename... Args, std::enable_if_t<std::is_convertible<CHAR, char>::value, bool> = true>
    constexpr static size_t formattedSize(fmt::format_string<Args...> formatString, Args &&... args) {
        checkForPrintf(formatString.get().data());
        try {
            return fmt::formatted_size(formatString, std::forward<Args>(args)...);
        } catch (std::exception & e) {
            printInvalidFormatSyntax(formatString.get().data(), e.what());
            return 0;
        } catch (...) {
            printInvalidFormatSyntax(formatString.get().data(), "Unknown exception when formatting string");
            return 0;
        }
    }

    /// Return size of the formatted string, not including the null terminator.
    template<typename... Args, std::enable_if_t<std::is_convertible<CHAR, wchar_t>::value, bool> = true>
    constexpr static size_t formattedSize(fmt::wformat_string<Args...> formatString, Args &&... args) {
        checkForPrintf(formatString.get().data());
        try {
            return fmt::formatted_size(formatString.get(), std::forward<Args>(args)...);
        } catch (std::exception & e) {
            printInvalidFormatSyntax(formatString.get().data(), e.what());
            return 0;
        } catch (...) {
            printInvalidFormatSyntax(formatString.get().data(), "Unknown exception when formatting string");
            return 0;
        }
    }

    template<typename... Args, std::enable_if_t<std::is_convertible<CHAR, char>::value, bool> = true>
    StringFormatter(fmt::format_string<Args...> formatString, Args &&... args) {
        checkForPrintf(formatString.get().data());
        try {
            // get size of the formatted string
            auto r = fmt::formatted_size(formatString, std::forward<Args>(args)...);

            constexpr size_t maxCharacters = sizeof(mPreAllocatedBuffer) / sizeof(CHAR) - 1; // needs one additional space for the null terminator

            if (r > maxCharacters) {
                mIsPreallocated = false;
                mResult         = fmt::format(formatString, std::forward<Args>(args)...);
            } else {
                mIsPreallocated = true;
                fmt::format_to_n(mPreAllocatedBuffer, maxCharacters, formatString, std::forward<Args>(args)...);
                mPreAllocatedBuffer[std::min(r, maxCharacters)] = 0;
            }
        } catch (const std::exception & e) {
            mResult         = fmt::format("{}: {}", e.what(), formatString.get());
            mIsPreallocated = false;
        }
    }

    template<typename... Args, std::enable_if_t<std::is_convertible<CHAR, wchar_t>::value, bool> = true>
    StringFormatter(fmt::wformat_string<Args...> formatString, Args &&... args) {
        checkForPrintf(formatString.get().data());
        try {
            // get size of the formatted string
            auto r = fmt::formatted_size(formatString.get(), std::forward<Args>(args)...);

            constexpr size_t maxCharacters = sizeof(mPreAllocatedBuffer) / sizeof(CHAR) - 1; // needs one additional space for the null terminator

            if (r > maxCharacters) {
                mIsPreallocated = false;
                mResult         = fmt::format(formatString, std::forward<Args>(args)...);
            } else {
                mIsPreallocated = true;
                fmt::format_to_n(mPreAllocatedBuffer, maxCharacters, formatString.get(), std::forward<Args>(args)...);
                mPreAllocatedBuffer[std::min(r, maxCharacters)] = 0;
            }
        } catch (const std::exception & e) {
            mIsPreallocated = false;
            mResult         = fmt::format(L"{}: {}", WideString(e.what()).wstr, formatString.get());
        }
    }

    const CHAR * result() const { return mIsPreallocated ? mPreAllocatedBuffer : mResult.c_str(); }

    bool isPreallocated() const { return mIsPreallocated; }
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
        Logger *    mLogger; ///< Logger instance pointer. Never null: the macro tests the level first.
        LogLocation mDesc;   ///< Logging descriptor

        /// Narrow message accumulated so far. A log must not be reported until the whole statement has
        /// run, because a streaming tail may still follow a format argument list
        /// (`GN_ERROR(logger, "a = {}", a) << ", b = " << b` is one message, not two). Reporting from
        /// emit() instead would split it.
        std::string mMessage;

        /// Created by the first streamed operand only, so a format-only log never pays for it. Seeded
        /// from mMessage so that a format prefix keeps its place ahead of the streamed text.
        std::stringstream * mStream = nullptr;

        std::stringstream & ss() {
            if (!mStream) GN_UNLIKELY {
                    mStream = new std::stringstream();
                    if (!mMessage.empty()) {
                        *mStream << mMessage;
                        mMessage.clear();
                    }
                }
            return *mStream;
        }

        /// Should this format string be handed to printf rather than fmt? A brace replacement field
        /// resolves the ambiguity in favor of fmt, which also keeps such strings on the
        /// StringFormatter fast path instead of tripping its brace-syntax-only assert.
        template<typename CHAR>
        static bool isPrintfSyntax(const CHAR * formatString) {
            return internal::hasPrintfSpecifier(formatString) && !internal::hasFmtField(formatString);
        }

        /// Append formatted narrow text to whichever accumulator is in use.
        void append(const char * text) {
            if (mStream) GN_UNLIKELY {
                    *mStream << text;
                    return;
                }
            mMessage += text;
        }

    public:
        ///
        /// Construct a log helper. The caller has already applied the level test, so the logger is
        /// known to be live.
        ///
        LogHelper(Logger * logger, int level, const char * func, const char * file, int line): mLogger(logger), mDesc(level, func, file, line) {
            GN_ASSERT(mLogger);
        }

        ///
        /// Reports the accumulated message, as a single log entry. This is the only place a narrow
        /// message is reported, which is what keeps a mixed format-plus-streaming statement together.
        ///
        ~LogHelper() {
            if (mStream) GN_UNLIKELY {
                    mLogger->doLog(mDesc, mStream->str().c_str());
                    delete mStream;
                }
            else if (!mMessage.empty()) { mLogger->doLog(mDesc, mMessage.c_str()); }
        }

        ///
        /// Stream style log operator. Appends to the message; the destructor reports the result.
        ///
        template<typename T>
        inline LogHelper & operator<<(T && t) {
            ss() << std::forward<T>(t);
            return *this;
        }

        ///
        /// No format arguments. Returns *this so that a streaming expression can be appended, which is
        /// how the streaming-only form of the log macros works.
        ///
        LogHelper & emit() { return *this; }

        ///
        /// Format and append the message. Accepts both fmt brace syntax and printf syntax: the string
        /// is routed to fmt::sprintf when it carries a printf conversion specifier and no brace
        /// replacement field, and to fmt::format otherwise.
        ///
        /// A string holding both, such as "{} %d", is ambiguous and resolves to fmt: the brace field
        /// is substituted and the %d stays literal. A lone "%%" escape counts as printf syntax, so
        /// "100%% done" is unescaped to "100% done"; combine it with a brace field and fmt wins
        /// instead, leaving the percent doubled.
        ///
        template<typename... Args>
        LogHelper & emit(fmt::format_string<Args...> formatString, Args &&... args) {
            if (isPrintfSyntax(formatString.get().data())) GN_UNLIKELY {
                    append(fmt::sprintf(formatString.get(), std::forward<Args>(args)...).c_str());
                    return *this;
                }
            append(internal::StringFormatter<char>(formatString, std::forward<Args>(args)...).result());
            return *this;
        }

        ///
        /// Wide overload. Returns void rather than LogHelper &, which makes appending a streaming tail
        /// to a wide format string a compile error instead of silently producing something wrong: the
        /// accumulators above are narrow, and folding a wide message into them would need a
        /// lossy conversion. No call site mixes the two. Report the wide message directly.
        ///
        template<typename... Args>
        void emit(fmt::wformat_string<Args...> formatString, Args &&... args) {
            GN_ASSERT(!mStream && mMessage.empty()); // a wide format is always the whole message
            if (isPrintfSyntax(formatString.get().data())) GN_UNLIKELY {
                    return mLogger->doLog(mDesc, fmt::vsprintf(formatString.get(), fmt::make_printf_args<wchar_t>(args...)).c_str());
                }
            mLogger->doLog(mDesc, internal::StringFormatter<wchar_t>(formatString, std::forward<Args>(args)...).result());
        }

        ///
        /// Format and append the message using printf syntax unconditionally.
        ///
        LogHelper & emitPrintf() { return *this; }

        template<typename... Args>
        LogHelper & emitPrintf(fmt::format_string<Args...> formatString, Args &&... args) {
            append(fmt::sprintf(formatString.get(), std::forward<Args>(args)...).c_str());
            return *this;
        }

        /// Wide overload. Returns void for the same reason as emit() above.
        template<typename... Args>
        void emitPrintf(fmt::wformat_string<Args...> formatString, Args &&... args) {
            GN_ASSERT(!mStream && mMessage.empty());
            mLogger->doLog(mDesc, fmt::vsprintf(formatString.get(), fmt::make_printf_args<wchar_t>(args...)).c_str());
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
    /// enable or disable this logger and all sub-loggers
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

protected:
    ///
    /// protective constructor
    ///
    Logger(const char * name): mName(name) {}

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
GN_API Logger * getLogger(const char * name);

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
