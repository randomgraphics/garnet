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
class ConsoleColor {
#if GN_WINPC
    HANDLE mConsole;
    WORD   mAttrib;

public:
    ConsoleColor(int level) {
        // store console attributes
        mConsole = GetStdHandle((level >= GN::Logger::INFO) ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbf;
        GetConsoleScreenBufferInfo(mConsole, &csbf);
        mAttrib = csbf.wAttributes;

        // change console color
        WORD attrib;
        switch (level) {
        case GN::Logger::FATAL:
        case GN::Logger::ERROR_:
            attrib = FOREGROUND_RED;
            break;

        case GN::Logger::WARN:
            attrib = FOREGROUND_RED | FOREGROUND_GREEN;
            break;

        case GN::Logger::INFO:
            attrib = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;

        case GN::Logger::VERBOSE:
        case GN::Logger::BABBLE:
        default:
            attrib = FOREGROUND_GREEN;
            break;
        }
        SetConsoleTextAttribute(mConsole, attrib);
    }

    ~ConsoleColor() {
        // restore console attributes
        SetConsoleTextAttribute(mConsole, mAttrib);
    }

#else
public:
    ConsoleColor(int) {}
    ~ConsoleColor() {}
#endif
};

#if GN_POSIX
///
/// local mutex class on X11 platform (to avoid referencing GN::LocalMutex)
///
class LocalMutex {
    pthread_mutex_t mMutex;

public:
    /// ctor
    LocalMutex() {
        // initiialize a recursive mutex (same behavior as mutex on MSWIN)
        pthread_mutexattr_t mta;
        pthread_mutexattr_init(&mta);
        pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&mMutex, &mta);
        pthread_mutexattr_destroy(&mta);
    }

    /// dtor
    ~LocalMutex() { pthread_mutex_destroy(&mMutex); }

    /// acquire the lock
    void lock() { pthread_mutex_lock(&mMutex); }

    /// release the lock
    void unlock() { pthread_mutex_unlock(&mMutex); }
};
#else
typedef std::recursive_mutex LocalMutex;
#endif

//
//
// -----------------------------------------------------------------------------
static inline GN::StrA sLevel2Str(int level) {
    switch (level) {
    case GN::Logger::FATAL:
        return "FATAL";
    case GN::Logger::ERROR_:
        return "ERROR";
    case GN::Logger::WARN:
        return "WARN";
    case GN::Logger::INFO:
        return "INFO";
    case GN::Logger::VERBOSE:
        return "VERBOSE";
    case GN::Logger::BABBLE:
        return "VERY_VERBOSE";
    default:
        return GN::str::format("%d", level);
    }
}

//
//
// -----------------------------------------------------------------------------
static GN::StrA sFormatPath(const char * path) {
    GN::StrA s;

    if (NULL == path) return s;

    for (; *path != 0; ++path) {
        char c = *path;

#if GN_MSWIN
        s.append('/' == c ? '\\' : c);
#else
        s.append('\\' == c ? '/' : c);
#endif
    }

    return s;
}

template<class T>
class LoggerTreeNode {
    T * mParent;
    T * mFirstChild;
    T * mPrevSibling;
    T * mNextSibling;

public:
    LoggerTreeNode(): mParent(0), mFirstChild(0), mPrevSibling(0), mNextSibling(0) {}

    T * parent() const { return mParent; }
    T * firstChild() const { return mFirstChild; }
    T * prevSibling() const { return mPrevSibling; }
    T * nextSibling() const { return mNextSibling; }

    ///
    /// set parent
    ///
    void setParent(T * newParent) {
        if (0 == newParent) {
            if (mFirstChild) {
                // modify parent's first child pointer
                if (mParent && this == mParent->mFirstChild) {
                    GN_ASSERT(0 == mPrevSibling);
                    mParent->mFirstChild = mFirstChild;
                }

                // set children's parent as my parent
                T * child = mFirstChild;
                while (child) {
                    child->mParent = mParent;
                    child          = child->mNextSibling;
                }

                // find last child
                T * lastChild = mFirstChild;
                while (lastChild->mNextSibling) lastChild = lastChild->mNextSibling;

                // connect children to brothers
                if (mPrevSibling) {
                    GN_ASSERT(this == mPrevSibling->mNextSibling);
                    GN_ASSERT(0 == mFirstChild->mPrevSibling);
                    mPrevSibling->mNextSibling = mFirstChild;
                    mFirstChild->mPrevSibling  = mPrevSibling;
                }
                if (mNextSibling) {
                    GN_ASSERT(this == mNextSibling->mPrevSibling);
                    GN_ASSERT(0 == lastChild->mNextSibling);
                    mNextSibling->mPrevSibling = lastChild;
                    lastChild->mNextSibling    = mNextSibling;
                }
            } else {
                // modify parent's first child pointer
                if (mParent && this == mParent->mFirstChild) {
                    GN_ASSERT(0 == mPrevSibling);
                    mParent->mFirstChild = mNextSibling;
                }

                // remove itsel from brother list
                if (mPrevSibling) {
                    GN_ASSERT(this == mPrevSibling->mNextSibling);
                    mPrevSibling->mNextSibling = mNextSibling;
                }
                if (mNextSibling) {
                    GN_ASSERT(this == mNextSibling);
                    mNextSibling->mPrevSibling = mPrevSibling;
                }
            }

            // clear my parent
            mParent = 0;
        } else {
            // detach from old parent
            setParent(0);

            // attach to new parent
            mParent              = newParent;
            mPrevSibling         = NULL;
            mNextSibling         = mParent->mFirstChild;
            mParent->mFirstChild = (T *) this;
        }
    }
};

// *********************************************************************
// default logger and receiver implementation
// *********************************************************************

namespace GN {
///
/// Log to console
///
struct ConsoleReceiver : public Logger::Receiver {
    virtual void onLog(Logger & logger, const Logger::LogDesc & desc, const char * msg) {
        if (getEnvBoolean("GN_LOG_QUIET")) return;

        if (NULL == msg) msg = "";

        ConsoleColor cc(desc.level);
        if (desc.level >= GN::Logger::INFO) {
            ::fprintf(stdout, "%s\n", msg);
        } else {
            ::fprintf(stderr,
                      "%s(%d)\n"
                      "\tname=%s, level=%s\n"
                      "\t%s\n\n",
                      sFormatPath(desc.file).rawptr(), desc.line, logger.getName(), sLevel2Str(desc.level).rawptr(), msg);
        }
    };
    virtual void onLog(Logger & logger, const Logger::LogDesc & desc, const wchar_t * msg) {
        if (getEnvBoolean("GN_LOG_QUIET")) return;

        if (NULL == msg) msg = L"";

        ConsoleColor cc(desc.level);
        if (desc.level >= GN::Logger::INFO) {
            ::fprintf(stdout, "%S\n", msg);
        } else {
            ::fprintf(stderr,
                      "%s(%d)\n"
                      "\tname=%s, level=%s\n"
                      "\t%S\n\n",
                      sFormatPath(desc.file).rawptr(), desc.line, logger.getName(), sLevel2Str(desc.level).rawptr(), msg);
        }
    };
};

///
/// Log to disk file
///
struct FileReceiver : public Logger::Receiver {
    StrA mFileName;

    struct AutoFile {
        FILE * fp;
        AutoFile(const StrA & name, const char * mode = "at"): fp(0) {
            if (name.empty()) return;
#if GN_MSVC8
            if (0 != ::fopen_s(&fp, name.rawptr(), mode)) fp = 0;
#else
            fp = ::fopen(name.rawptr(), mode);
#endif
        }
        ~AutoFile() {
            if (fp) ::fclose(fp);
        }
    };

    FileReceiver()
#if GN_XBOX2
        : mFileName("game:\\garnet3d.log.xml")
#else
        : mFileName(getEnv("GN_LOG_FILENAME"))
#endif
    {
        AutoFile af(mFileName, "wt");
        if (!af.fp) return;
        // TODO: convert log message to UTF-8
        ::fprintf(af.fp, "<?xml version=\"1.0\" encoding=\"GB18030\" standalone=\"yes\"?><srlog>\n");
    }

    ~FileReceiver() {
        AutoFile af(mFileName);
        if (!af.fp) return;
        ::fprintf(af.fp, "</srlog>\n");
    }

    virtual void onLog(Logger & logger, const Logger::LogDesc & desc, const char * msg) {
        AutoFile af(mFileName);
        if (!af.fp) return;

        if (NULL == msg) msg = "";

        ::fprintf(af.fp, "<log file=\"%s\" line=\"%d\" name=\"%s\" level=\"%s\"><![CDATA[%s]]></log>\n", sFormatPath(desc.file).rawptr(), desc.line,
                  logger.getName(), sLevel2Str(desc.level).rawptr(), msg);
    }
    virtual void onLog(Logger & logger, const Logger::LogDesc & desc, const wchar_t * msg) {
        AutoFile af(mFileName);
        if (!af.fp) return;

        if (NULL == msg) msg = L"";

        ::fprintf(af.fp, "<log file=\"%s\" line=\"%d\" name=\"%s\" level=\"%s\"><![CDATA[%S]]></log>\n", sFormatPath(desc.file).rawptr(), desc.line,
                  logger.getName(), sLevel2Str(desc.level).rawptr(), msg);
    }
};

///
/// Log to debugger
///
class DebugReceiver : public Logger::Receiver {
    virtual void onLog(Logger & logger, const Logger::LogDesc & desc, const char * msg) {
#if GN_MSWIN
        char buf[16384];
        if (desc.level >= GN::Logger::INFO) {
            str::formatTo(buf, 16384, "%s\n", msg);
        } else {
            str::formatTo(buf, 16384, "%s(%d) : name(%s), level(%s) : %s\n", sFormatPath(desc.file).rawptr(), desc.line, logger.getName(),
                          sLevel2Str(desc.level).rawptr(), msg);
        }
        ::OutputDebugStringA(buf);
#else
        GN_UNUSED_PARAM(logger);
        GN_UNUSED_PARAM(desc);
        GN_UNUSED_PARAM(msg);
#endif
    }
    virtual void onLog(Logger & logger, const Logger::LogDesc & desc, const wchar_t * msg) {
#if GN_MSWIN
        if (NULL == msg) msg = L"";

        wchar_t buf[16384];
        if (desc.level >= GN::Logger::INFO) {
            str::formatTo(buf, 16384, L"%S\n", msg);
        } else {
            str::formatTo(buf, 16384, L"%S(%d) : name(%S), level(%S) : %s\n", sFormatPath(desc.file).rawptr(), desc.line, logger.getName(),
                          sLevel2Str(desc.level).rawptr(), msg);
        }
        ::OutputDebugStringW(buf);
#else
        GN_UNUSED_PARAM(logger);
        GN_UNUSED_PARAM(desc);
        GN_UNUSED_PARAM(msg);
#endif
    }
};

///
/// Logger implementation class
///
class LoggerImpl : public Logger, public LoggerTreeNode<LoggerImpl> {
public:
    LoggerImpl(const char * name, LocalMutex & mutex): Logger(sDuplicateName(name)), mGlobalMutex(mutex), mInheritLevel(true), mInheritEnabled(true) {}

    ~LoggerImpl() {
        const char * name = getName();

        if (NULL != name && 0 != *name) { GN::HeapMemory::dealloc((void *) name); }
    }

    void reapplyAttributes() {
        recursiveUpdateLevel(getLevel());
        recursiveUpdateEnabled(isEnabled());
    }

public:
    virtual void setLevel(int level) {
        std::lock_guard<LocalMutex> m(mGlobalMutex);
        recursiveUpdateLevel(level);
        mInheritLevel = false;
    }

    virtual void setEnabled(bool enabled) {
        std::lock_guard<LocalMutex> m(mGlobalMutex);
        recursiveUpdateEnabled(enabled);
        mInheritEnabled = false;
    }

    virtual void doLog(const LogDesc & desc, const char * msg) {
        std::lock_guard<LocalMutex> m(mGlobalMutex);
        recursiveLog(*this, desc, msg);
    }

    virtual void doLog(const LogDesc & desc, const wchar_t * msg) {
        std::lock_guard<LocalMutex> m(mGlobalMutex);
        recursiveLog(*this, desc, msg);
    }

    virtual void addReceiver(Receiver * r) {
        std::lock_guard<LocalMutex> m(mGlobalMutex);
        if (0 == r) return;
        mReceivers.insert(r);
    }

    virtual void removeReceiver(Receiver * r) {
        std::lock_guard<LocalMutex> m(mGlobalMutex);
        if (0 == r) return;
        mReceivers.erase(r);
    }

    virtual void removeAllReceivers() {
        std::lock_guard<LocalMutex> m(mGlobalMutex);
        mReceivers.clear();
    }

private:
    LocalMutex & mGlobalMutex;

    std::set<Receiver *> mReceivers;
    bool                 mInheritLevel;
    bool                 mInheritEnabled;

    static const char * sDuplicateName(const char * name) {
        if (NULL == name || 0 == *name) {
            return "";
        } else {
            size_t n = strlen(name);

            char * p = (char *) HeapMemory::alloc(n + 1);

            if (p) {
                memcpy(p, name, n + 1);

                return p;
            } else {
                return "";
            }
        }
    }

    template<typename CHAR>
    void recursiveLog(Logger & logger, const LogDesc & desc, const CHAR * msg) {
        // call parent's logging
        LoggerImpl * p = parent();
        if (p) p->recursiveLog(logger, desc, msg);

        // send msg to receivers
        for (std::set<Receiver *>::const_iterator i = mReceivers.begin(); i != mReceivers.end(); ++i) { (*i)->onLog(logger, desc, msg); }
    }

    void recursiveUpdateLevel(int level) {
        mLevel             = level;
        LoggerImpl * child = firstChild();
        while (child) {
            if (child->mInheritLevel) child->recursiveUpdateLevel(level);
            child = child->nextSibling();
        }
    }

    void recursiveUpdateEnabled(bool enabled) {
        mEnabled           = enabled;
        LoggerImpl * child = firstChild();
        while (child) {
            if (child->mInheritEnabled) child->recursiveUpdateEnabled(enabled);
            child = child->nextSibling();
        }
    }
};

///
/// Log container
///
class LoggerContainer {
    // Note: Logger map is case "insensitive"
    typedef GN::StringMap<char, LoggerImpl *, GN::str::INSENSITIVE> LoggerMap;

    ConsoleReceiver mCr;
    FileReceiver    mFr;
    DebugReceiver   mDr;
    LoggerImpl      mRootLogger;
    LocalMutex      mMutex;
    LoggerMap       mLoggers;

    LoggerImpl * findParent(const StrA & name) {
        // get parent name
        size_t n = name.findLastOf(".");
        if (StrA::NOT_FOUND == n) return &mRootLogger; // shortcut for root logger
        GN_ASSERT(n > 0);
        StrA parent = name.subString(0, n);

        return getLogger(parent.rawptr());
    }

    void printLoggerTree(StrA & str, int level, LoggerImpl & logger) {
        // print itself
        for (int i = 0; i < level; ++i) str.append("  ");
        str.append(str::format("%s\n", logger.getName()));

        // print children
        LoggerImpl * c = logger.firstChild();
        while (c) {
            printLoggerTree(str, level + 1, *c);
            c = c->nextSibling();
        }
    }

public:
    LoggerContainer(): mRootLogger("ROOT", mMutex) {
        // config root logger
        mRootLogger.setLevel(Logger::INFO);
        mRootLogger.setEnabled(true);
#if !GN_XBOX2
        mRootLogger.addReceiver(&mCr);
#endif
        mRootLogger.addReceiver(&mFr);
        mRootLogger.addReceiver(&mDr);
    }

    ~LoggerContainer() {
        static Logger * sLogger = getLogger("GN.core.LoggerContainer");
        StrA            loggerTree;
        GN_VERBOSE(sLogger)
        ("\n"
         "===================\n"
         "    Logger Tree\n"
         "===================");
        printLoggerTree(loggerTree, 0, mRootLogger);
        GN_VERBOSE(sLogger)("\n%s", loggerTree.rawptr());
        for (LoggerMap::KeyValuePair * p = mLoggers.first(); NULL != p; p = mLoggers.next(p)) { delete p->value; }
    }

    LoggerImpl * getLogger(const char * name) {
        std::lock_guard<LocalMutex> m(mMutex);

        // trip leading and trailing dots
        StrA n(name);
        n.trim('.');

        // shortcut for root logger
        if (n.empty() || 0 == str::compareI("ROOT", n.rawptr())) return &mRootLogger;

        // find for existing logger
        LoggerImpl ** pplogger = mLoggers.find(n);
        if (NULL != pplogger) {
            GN_ASSERT(*pplogger);
            return *pplogger;
        }

        // not found. create new one.
        AutoObjPtr<LoggerImpl> newLogger(new LoggerImpl(n, mMutex));
        mLoggers[n] = newLogger;

        // update logger tree
        LoggerImpl * parent = findParent(n);
        GN_ASSERT(parent);
        newLogger->setParent(parent);
        parent->reapplyAttributes();

        // sucess
        return newLogger.detach();
    }
};

LoggerContainer * msInstancePtr = 0;

//
// Implement global log function.
// -------------------------------------------------------------------------
static LoggerContainer & sGetLoggerContainer() {
    // WARNING: this function is not thread-safe!
    static LoggerContainer lc;
    return lc;
}

//
// Implement global log function.
// -------------------------------------------------------------------------
GN_API Logger * getLogger(const char * name) {
    LoggerContainer & lc = sGetLoggerContainer();
    return lc.getLogger(name);
}
} // namespace GN
