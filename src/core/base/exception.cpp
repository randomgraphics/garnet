#include "pch.h"

// headers to generate backtrace
#if GN_ANDROID
    #include <unwind.h>
    #include <cxxabi.h>
    #include <dlfcn.h>
    #include <sys/system_properties.h>
    #include <android/trace.h>
#elif GN_POSIX
    #include <backward.hpp>
#elif GN_MSWIN
    #include <StackWalker.h>
#endif

namespace GN {

static Logger * sLogger = getLogger("GN.base.exception");

// ---------------------------------------------------------------------------------------------------------------------
//
GN_API StrA backtrace(bool includeSourceSnippet) {
    (void) includeSourceSnippet; // this is to avoid unreferenced variable warning.
#if GN_ANDROID
    struct android_backtrace_state {
        void ** current;
        void ** end;

        static _Unwind_Reason_Code android_unwind_callback(struct _Unwind_Context * context, void * arg) {
            android_backtrace_state * state = (android_backtrace_state *) arg;
            uintptr_t                 pc    = _Unwind_GetIP(context);
            if (pc) {
                if (state->current == state->end)
                    return _URC_END_OF_STACK;
                else
                    *state->current++ = reinterpret_cast<void *>(pc);
            }
            return _URC_NO_REASON;
        }

        static std::string addr2symbol(const void * addr) {
            // convert address to symbol
            Dl_info info;
            if (!dladdr(addr, &info) || !info.dli_sname) return {};
            std::string result = info.dli_sname;

            // demangle c++ syntax
            int    status = 0;
            char * symbol = abi::__cxa_demangle(info.dli_sname, 0, 0, &status);
            if (!symbol) return result;
            if (0 == status) result = symbol;
            free(symbol);

            // done
            return result;
        }
    };

    const int indent = 0;

    std::string prefix;
    for (int i = 0; i < indent; ++i) prefix += ' ';

    std::stringstream ss;
    ss << prefix << "android stack dump\n";

    const int max = 100;
    void *    buffer[max];

    android_backtrace_state state;
    state.current = buffer;
    state.end     = buffer + max;

    _Unwind_Backtrace(android_backtrace_state::android_unwind_callback, &state);

    int count = (int) (state.current - buffer);

    for (int idx = 0; idx < count; idx++) {
        auto addr   = buffer[idx];
        auto symbol = android_backtrace_state::addr2symbol(addr);
        if (symbol.empty()) symbol = "<no symbol>";
        ss << prefix << str::format("%03d: 0x%p %s\n", idx, addr, symbol.c_str());
    }

    ss << prefix << "android stack dump done\n";

    return ss.str();
#elif GN_POSIX
    using namespace backward;
    StackTrace st;
    st.load_here(32);
    std::stringstream ss;
    Printer           p;
    p.snippet = includeSourceSnippet; // print code snippet in debug build only.
    p.print(st, ss);
    return ss.str();
#elif GN_MSWIN
    class MyStackWalker : public StackWalker {
    protected:
        void OnLoadModule(LPCSTR, LPCSTR, DWORD64, DWORD, DWORD, LPCSTR, LPCSTR, ULONGLONG) override {}
        void OnDbgHelpErr(LPCSTR, DWORD, DWORD64) override {}
        void OnSymInit(LPCSTR, DWORD, LPCSTR) override {}
        void OnOutput(LPCSTR szText) override { ss << szText; }

    public:
        using StackWalker::StackWalker;
        std::stringstream ss;
    };
    MyStackWalker sw(StackWalker::RetrieveLine | StackWalker::RetrieveSymbol);
    return sw.ShowCallstack() ? sw.ss.str() : std::string {};
#else
    return {};
#endif
}

GN_API void throwException(const char * func, const char * file, int line, StrA && msg) {
    // TODO: print callstack.
    auto bt = backtrace();
    GN_LOG_EX(sLogger, GN::Logger::ERROR_, func, file, line)("%s\n%s", msg.data(), bt.data());
    throw Exception(func, file, line, std::move(msg));
}

///
/// exception handler
///
GN_API void exceptionHandler(const char * msg, const char * func, const char * file, int line) { GN_LOG_EX(sLogger, Logger::FATAL, func, file, line)(msg); }

} // namespace GN