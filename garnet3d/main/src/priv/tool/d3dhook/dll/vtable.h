#pragma once

#include <garnet/GNbase.h>

#define HOOK_ERROR_LOG(...) GN_ERROR(GN::getLogger("GN.d3dhook"))(__VA_ARGS__)
#define HOOK_INFO_LOG(...)  GN_INFO(GN::getLogger("GN.d3dhook"))(__VA_ARGS__)
#define HOOK_WARN_LOG(...)  GN_WARN(GN::getLogger("GN.d3dhook"))(__VA_ARGS__)
#define HOOK_ASSERT(x)      GN_ASSERT(x)
#define HOOK_RIP()          GN_UNEXPECTED()

// -----------------------------------------------------------------------------
// common call trace utilities
// -----------------------------------------------------------------------------

namespace calltrace
{
    /// Global switch to turn on/off call tracking
    extern bool g_callTraceEnabled;

    /// return call level (including current call)
    int enter(const wchar_t * text);

    /// return call level (including current call)
    int enter(const char * text);

    void leave();

    /// auotmatic call trace helper
    class AutoTrace
    {
        bool _enabled;

    public:

        AutoTrace(const wchar_t * text) : _enabled(g_callTraceEnabled)
        {
            if (_enabled)
            {
                enter(text);
            }
        }

        AutoTrace(const char * text) : _enabled(g_callTraceEnabled)
        {
            if (_enabled)
            {
                enter(text);
            }
        }

        ~AutoTrace()
        {
            if (_enabled)
            {
                leave();
            }
        }
    };
};
